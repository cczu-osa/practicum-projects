'use strict';

let authorizedKeys = [];
let db = require('../db');

let middleware = (req, res, next) => {
    if (req.url.indexOf('/portal') === 0) {
        next();
        return;
    }

    if (typeof req.query.token === 'undefined') {
        res.send({
            status: 'error',
            message: 'unauthorized access.'
        });
        return;
    }

    let conn = db.getConn();
    conn.query({
        sql: 'SELECT * FROM token INNER JOIN user ON user.user_id = token.user_id WHERE access_token = ?',
        values: [req.query.token]
    }, (err, table) => {
        if (err) {
            console.log(err);
            res.send({
                status: 'error',
                message: 'server-side database error.'
            });
            return;
        }
        if (table.length === 0) {
            res.send({
                status: 'error',
                message: 'unauthorized access.'
            })
        }
        else {
            req.role = table[0].role;
            req.name = table[0].username;
            next();
        }
    })
};

module.exports = middleware;