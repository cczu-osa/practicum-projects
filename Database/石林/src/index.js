'use strict';

const express = require('express');
const bodyParser = require('body-parser');
const cookieParser = require('cookie-parser');
const config = require('./config');
const common = require('./common');

let site = express();

site.use(bodyParser.urlencoded({extended: true}));
site.use(bodyParser.json());
site.use(cookieParser());

site.use('/api/v1', require('./api/index'));
site.use('/', express.static('./front-end/static/'))

site.listen(config.serverPort)
console.log(`Server started on port ${config.serverPort}`);
