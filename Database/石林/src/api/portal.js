'use strict';

let authorizedKeys = [];
let db = require('../db');

let generateToken = () => {
    var possible = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    var res = '';
    for(let i = 0; i < 30; i++)
        res += possible.charAt(Math.floor(Math.random() * possible.length));
    return res;
}

let handler = (req, res) => {
    if (typeof req.query.username === 'undefined' || typeof req.query.password === 'undefined') {
        res.send({
            status: 'err',
            message: 'require username and password.'
        });
        return;
    }
    
    let conn = db.getConn();

    conn.query({
        sql: 'SELECT * FROM user WHERE username = ? AND password = ?',
        values: [req.query.username, req.query.password]
    }, (err, table) => {
        if (err) {
            res.send({
                status: 'error',
                message: 'server-side database error.'
            });
            return;
        }
        if (table.length === 0) {
            res.send({
                status: 'error',
                message: 'login failed. check your name and password and then try again.'
            });
            return;
        }

        let token = generateToken();
        conn.query({
            sql: 'INSERT INTO token(access_token, user_id) VALUES(?, ?)',
            values: [token, table[0].user_id],
        }, (err, table) => {
            if (err) {
                res.send({
                    status: 'error',
                    message: 'server-side database error.'
                });
                return;
            }
            res.send({
                status: 'ok',
                token
            })
        })
    })
}

module.exports = handler;