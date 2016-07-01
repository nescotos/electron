//Import modules
var couchbase = require('couchbase');
var ottoman = require('ottoman');
var config = require('../config');

// Build my cluster object and open a new cluster
var myCluster = new couchbase.Cluster(config.DATABASE.HOST + ':' + config.DATABASE.PORT);
var myBucket = myCluster.openBucket('Electron');

ottoman.bucket = myBucket;

//Building the whole schema
require('./models/hic');
require('./models/humidity');
require('./models/ligth');
require('./models/sound');
require('./models/temperature');

//Build indexes
ottoman.ensureIndices(function(){});
