// tmx2json.js

var fs = require('fs');
var _ = require("lodash");
var rsvp = require("rsvp");
var xml2js = require("xml2js");

var parser = new xml2js.Parser({
  trim: true
});

parser.addListener('end', function(result) {
  console.log(JSON.stringify(result, undefined, 1));
});

fs.readFile(__dirname + '/dev-map.xml', function(err, data) {
  parser.parseString(data);
});
