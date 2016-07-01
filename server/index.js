//Import modules
var express = require('express');
var morgan = require('morgan');
var bodyParser = require('body-parser');
var app = express();
var config = require('./config');
var socket = require('./socket/notificationSocket');
//Initializing DB
var db = require('./db/db');

//Logging with morgan
app.use(morgan('dev'));

//Enabling Body Parser to POST request
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

//Enabling CORS
app.use(function(req, res, next) {
	res.setHeader('Access-Control-Allow-Origin', '*');
	res.setHeader('Access-Control-Allow-Methods', 'GET, POST');
	res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type, Authorization');
	next();
});


//Starting server
var server = app.listen(config.PORT, function(){
  console.log('Server Running on: ' + config.PORT);
});

//Using socket.io
var io = require('socket.io')(server);
io.sockets.on('connection', socket);
