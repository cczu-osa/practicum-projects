'use strict';

let db = require('../db');

let handler = (req, res) => {
    res.send({
        status: 'ok',
        user: req.name,
        server: [
            { name: '运行时间', value: process.uptime() },
            { name: '运行平台', value: process.platform },
            { name: '服务端软件', value: process.title },
            { name: '服务端软件版本', value: process.version },
        ],
    });
}

module.exports = handler;