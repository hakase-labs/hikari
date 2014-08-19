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

fs.readFile(__dirname + '/dev-map.xml', function(err, data) {
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
    tmxProperties;

  if(_.has(tmxJson, 'map')) {
    tmxMap = tmxJson.map;

    if(_.has(tmxMap, '$')) {
      tmxMeta = tmxMap.$;

      result.version = tmxMeta.version;
      result.gridsize = parseInt(tmxMeta.tilewidth);
    }

    if(_.has(tmxMap, 'properties')) {
      tmxProperties = tmxMap.properties;
      tmxProperties = _.pluck(tmxProperties, 'property');

      result.musicId = _.find(tmxProperties, function(prop) {
        return prop[0].$.name === 'musicId';
      });

      result.bossEntity = _.findWhere(tmxProperties, function(prop) {
        return prop[0].$.name === 'bossEntity';
      });
    }
  }
      console.log('returning');

  return result;
}
