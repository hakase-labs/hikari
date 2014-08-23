// tmx2json.js

var fs = require('fs');
var _ = require('lodash');
var rsvp = require('rsvp');
var xml2js = require('xml2js');

var parser = new xml2js.Parser({
  trim: true
});

fs.readdir(__dirname + '/dev-map', function(err, data) {
  console.log(JSON.stringify(data, undefined, 1));

  var outputFilename = __dirname + '/dev-map.json';

  // Map the file names to full paths.
  var files = _.map(data, function(file) {
    return __dirname + '/dev-map/' + file;
  });

  // Map each file path to a async read -> promise.
  var fileReadPromises = _.map(files, function(filePath) {
    return new rsvp.Promise(function(resolve, reject) {
      fs.readFile(filePath, 'utf8', function(err, data) {
        if(err) {
          reject(err);
        } else {
          resolve(data);
        }
      });
    });
  });

  // Wait for all the files to be read and then kick-start the transformation.
  rsvp.all(fileReadPromises)
  .then(convertFilesToJson)
  .then(function(jsonMaps) {
    console.log('I converted all the files!');

    var mapMetaData = extractMapMetaData(jsonMaps[0]);
    var roomData = _.map(jsonMaps, _.partialRight(extractRoom, mapMetaData));
    var assembledMap = _.extend({}, mapMetaData, {
      rooms: roomData
    });

    // Clean up any data params we don't need anymore.
    delete assembledMap._tilesetGid;

    fs.writeFile(outputFilename, JSON.stringify(assembledMap, null, 2), function(err) {
      if(err) {
        console.log(err);
      } else {
        console.log("JSON saved to " + outputFilename);
      }
    });
  })
  .catch(function(reason) {
    console.log('Error reading all files!', reason);
  });
});

/**
 * Converts an array of XML files strings (TMX file data) to JSON and returns
 * a promise that resolves when all have been converted.
 */
function convertFilesToJson(fileContents) {
  var jsonTmxObjects = _.map(fileContents, function parseXmlStringToJson(xmlString) {
    return new rsvp.Promise(function(resolve, reject) {
      parser.parseString(xmlString, function(err, data) {
        if(err) {
          reject(err);
        } else {
          resolve(data);
        }
      });
    });
  });

  return rsvp.all(jsonTmxObjects);
}

/**
 * Extracts metadata for an entire map.
 * @param  {Object} tmxJson - a JSON version of a TMX file
 * @return {Object} map metadata
 */
function extractMapMetaData(tmxJson) {
  var result = {},
    tmxMap,
    tmxMeta,
    tmxProperties,
    tmxTileset,
    tmxTilesetFirstGid,
    maybeMusicId,
    maybeBossEntity;

  if(_.has(tmxJson, 'map')) {
    tmxMap = tmxJson.map;

    if(_.has(tmxMap, '$')) {
      tmxMeta = tmxMap.$;

      result.version = tmxMeta.version;
      result.gridsize = parseInt(tmxMeta.tilewidth);
    }

    if(_.has(tmxMap, 'properties')) {
      tmxProperties = tmxMap.properties;
      tmxProperties = _.flatten(_.pluck(tmxProperties, 'property'));

      maybeMusicId = unwrapProperty(tmxProperties, 'musicId');
      result.musicId = maybeMusicId ? parseInt(maybeMusicId, 10) : null;

      maybeBossEntity = unwrapProperty(tmxProperties, 'bossEntity');
      result.bossEntity = maybeBossEntity;
    }

    if(_.has(tmxMap, 'tileset')) {
      tmxTileset = _.flatten(tmxMap.tileset);
      tmxTileset = _.find(tmxTileset, function (tileset) {
        return tileset.$.name !== 'attribute';
      });
      tmxTilesetFirstGid = parseInt(tmxTileset.$.firstgid, 10);

      result.tileset = tmxTileset.$.name;
      result._tilesetGid = tmxTilesetFirstGid;
    }
  }

  return result;
}

function unwrapProperty(obj, propName) {
  var maybeProp;

  if(obj && propName) {
    maybeProp = _.find(obj, function(property) {
      return property.$.name === propName;
    });

    if(maybeProp) {
      return maybeProp.$.value;
    }
  }

  return undefined;
}

/**
 * Maps weird TMX properties into a normal JS Object.
 * @param  {Object} tmxJson - weird TMX JSON
 * @return {Object} a POJO of key/values from the properties input
 */
function mapProperties(tmxJson) {
  return _.assign.apply(this, _.map(tmxJson, function(prop) {
    var result = {};

    result[prop.$.name] = prop.$.value;

    return result;
  }));
}

function extractTiles(tmxJsonTileLayer) {
  var csvTileData = tmxJsonTileLayer.data[0]._,
    cleanedTileData = csvTileData.replace(new RegExp('\\n', 'g'), '');

  return _.map(cleanedTileData.split(','), function(num) {
    return parseInt(num, 10);
  });
}

function extractTileSets(tmxJsonMap) {
  return _.map(tmxJsonMap, extractTileSet);
}

function extractTileSet(tmxJsonTileset) {
  return {};
}

function extractRoom(tmxJson, roomIndex, allRooms, mapMetaData) {
  var result,
    layers = tmxJson.map.layer,
    objectGroups = tmxJson.map.objectgroup,
    properties,
    tileLayer,
    attrLayer,
    cameraLayer,
    enemyLayer,
    itemLayer,
    transitionLayer,
    doorLayer,
    cameraBounds,
    TILE_LAYER = 0,
    ATTR_LAYER = 1,
    CAMERA_LAYER = 0,
    ENEMY_LAYER = 1,
    ITEM_LAYER = 2,
    TRANSITION_LAYER = 3,
    DOOR_LAYER = 4;

  result = {};

  if(layers) {
    tileLayer = layers[TILE_LAYER];
    attrLayer = layers[ATTR_LAYER];
    cameraLayer = objectGroups[CAMERA_LAYER];
    enemyLayer = objectGroups[ENEMY_LAYER];
    itemLayer = objectGroups[ITEM_LAYER];
    transitionLayer = objectGroups[TRANSITION_LAYER];
    doorLayer = objectGroups[DOOR_LAYER];
    properties = mapProperties(tileLayer.properties[0].property);

    result.id = roomIndex;
    result.width = parseInt(tileLayer.$.width, 10);
    result.height = parseInt(tileLayer.$.height, 10);
    result.x = parseInt(properties.x, 10);
    result.y = parseInt(properties.y, 10);

    if(_.has(properties, 'heroSpawnX')) {
      result.heroSpawnX = parseInt(properties.heroSpawnX, 10);
    }

    if(_.has(properties, 'heroSpawnY')) {
      result.heroSpawnY = parseInt(properties.heroSpawnY, 10);
    }

    result.tile = _.map(extractTiles(tileLayer), function(num) {
      return num - mapMetaData._tilesetGid;
    });
    result.attr = extractTiles(attrLayer);

    // Grab the camera bounds.
    cameraBounds = _.omit(cameraLayer.object[0].$, 'name');

    result.cameraBounds = {
      x: parseInt(cameraBounds.x, 10) / mapMetaData.gridsize,
      y: parseInt(cameraBounds.y, 10) / mapMetaData.gridsize,
      width: parseInt(cameraBounds.width, 10) / mapMetaData.gridsize,
      height: parseInt(cameraBounds.height, 10) / mapMetaData.gridsize
    };

    result.enemies = extractEnemies(enemyLayer);
    result.items = extractItems(itemLayer);
    result.transitions = extractTransitions(transitionLayer);
    result.doors = extractDoors(doorLayer);
  }

  return result;
}

function extractEnemies(tmxJsonEnemiesLayer) {
  return _.map(tmxJsonEnemiesLayer.object, function(enemy) {
    var properties = enemy.properties ? mapProperties(enemy.properties[0].property) : {},
      mappedEnemy = _.extend({
        type: enemy.$.name,
        x: parseInt(enemy.$.x, 10),
        y: parseInt(enemy.$.y, 10)
      }, properties);

    return mappedEnemy;
  });
}

function extractItems(tmxJsonItemsLayer) {
  return [];
}

function extractTransitions(tmxJsonTransitionLayer) {
  return [];
}

function extractDoors(tmxJsonDoorLayer) {
  return [];
}
