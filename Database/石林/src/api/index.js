'use strict';

const express = require('express');
const config = require('../config');

let router = express.Router();
let dictHandler = require('./dict');
let userHandler = require('./user');
let officerHandler = require('./officer')
let newsHandler = require('./news')

router.use(require('./auth'));

router.get('/', (req, res) => {
    res.send({
        status: 'ok',
    })
});

router.get('/portal', require('./portal'));

router.get('/status', require('./status'));

router.get('/dict', dictHandler.get);
router.put('/dict', dictHandler.put);
router.post('/dict', dictHandler.post);
router.delete('/dict', dictHandler.delete);

router.get('/user', userHandler.get);
router.put('/user', userHandler.put);
router.post('/user', userHandler.post);
router.delete('/user', userHandler.delete);

router.get('/officer', officerHandler.get);
router.put('/officer', officerHandler.put);
router.post('/officer', officerHandler.post);
router.delete('/officer', officerHandler.delete);

router.get('/news', newsHandler.get);
router.put('/news', newsHandler.put);
router.post('/news', newsHandler.post);
router.delete('/news', newsHandler.delete);

module.exports = router;
