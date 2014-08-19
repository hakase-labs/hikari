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
