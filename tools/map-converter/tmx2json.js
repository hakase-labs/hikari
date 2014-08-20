// tmx2json.js

var fs = require('fs');
var _ = require('lodash');
var rsvp = require('rsvp');
var xml2js = require('xml2js');

var parser = new xml2js.Parser({
  trim: true
});

parser.addListener('end', function(result) {
  console.log(JSON.stringify(extractMapMetaData(result), undefined, 1));

  console.log(JSON.stringify(extractRoom(result), undefined, 1));
});

fs.readFile(__dirname + '/dev-map.tmx', function(err, data) {
  parser.parseString(data);
});

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
      tmxTileset = tmxTileset[0].$;

      result.tileset = tmxTileset.name;
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

function extractRoom(tmxJson) {
  var result,
    layers = tmxJson.map.layer,
    properties,
    tileLayer,
    attrLayer,
    TILE_LAYER = 0,
    ATTR_LAYER = 1;

  result = {};

  if(layers) {
    tileLayer = layers[TILE_LAYER];
    attrLayer = layers[ATTR_LAYER];
    properties = mapProperties(tileLayer.properties[0].property);

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

    result.tile = _.map(tileLayer.data[0]._.replace(new RegExp('\\n', 'g'), '').split(','), function(num) {
      return parseInt(num, 10);
    });

    result.attr = _.map(attrLayer.data[0]._.replace(new RegExp('\\n', 'g'), '').split(','), function(num) {
      return parseInt(num, 10);
    });
  }

  console.log(JSON.stringify(layers, undefined, 1));

  return result;
}
