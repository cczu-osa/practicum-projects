'use strict';

let db = require('../db');
let common = require('../common');

let formatter = (table) => {
    return table.map(row => {
        return {
            id: row.user_id,
            username: row.username,
            password: row.password,
            role: common.roleToString(row.role),
        }
    })
} 

let sendBack = (err, table, res) => {
    if (err) {
        res.send({
            status: 'err',
            message: 'server-side database error',
        });
    }
    else {
        res.send({
            status: 'ok',
            dataset: formatter(table),
        });
    }
}

/**
 * Get value, by id or category or all.
 */
let getHandler = (req, res) => {
    let conn = db.getConn();
    if (req.query.id) {
        conn.query({
            sql: 'SELECT * FROM user WHERE user_id = ?',
            values: [req.query.id]
        }, (err, table) => sendBack(err, table, res))
    }
    else if (req.query.username) {
        conn.query({
            sql: 'SELECT * FROM user WHERE username = ?',
            values: [req.query.username]
        }, (err, table) => sendBack(err, table, res))
    }
    else if (req.query.role) {
        let roleId = common.ROLE[req.query.role];
        if (typeof roleId === 'undefined') {
            res.send({
                status: 'err',
                message: 'invalid role id',
            });
            return;
        }
        conn.query({
            sql: 'SELECT * FROM user WHERE role = ?',
            values: [roleId],
        }, (err, table) => sendBack(err, table, res));
    }
    else {
        conn.query({
            sql: 'SELECT * FROM user',
        }, (err, table) => sendBack(err, table, res))
    }
}

/**
 * Upgrade value, by id, all column data required.
 */
let postHandler = (req, res) => {
    if (req.role !== common.ROLE.ADMINISTRATOR) {
        res.send({
            status: 'err',
            message: 'permission denied.'
        });
        return;
    }
    let id = req.query.id || req.body.id;
    let username = req.query.username || req.body.username;
    let password = req.query.password || req.body.password;
    let role = req.query.role || req.body.role;
    if (typeof id === 'undefined' || typeof username === 'undefined' || typeof password === 'undefined' || typeof role === 'undefined') {
        res.send({
            status: 'err',
            message: 'required filed(s) empty!',
        });
        return;
    }
    let conn = db.getConn();
    let roleId = common.ROLE[role];
    conn.query({
        sql: 'UPDATE user SET username = ?, password = ?, role = ? WHERE user_id = ?',
        values: [username, password, roleId, id],
    }, (err, table) => {
        if (err) {
            res.send({
                status: 'err',
                message: 'server-side database error.'
            })
            return;
        }
        else if (table.affectedRows === 0) {
            res.send({
                status: 'err',
                message: 'no such id.'
            });
            return;
        }
        else {
            res.send({
                status: 'ok',
            });
        }
    })
}

/**
 * New value
 */

let putHandler = (req, res) => {
    if (req.role !== common.ROLE.ADMINISTRATOR) {
        res.send({
            status: 'err',
            message: 'permission denied.'
        });
        return;
    }
    let username = req.query.username || req.body.username;
    let password = req.query.password || req.body.password;
    let role = req.query.role || req.body.role;

    if (typeof username === 'undefined' || typeof password === 'undefined' || typeof role === 'undefined') {
        res.send({
            status: 'err',
            message: 'required filed(s) empty!',
        });
        return;
    }
    let conn = db.getConn();
    let roleId = common.ROLE[role];
    conn.query({
        sql: 'INSERT INTO user(username, password, role) VALUES(?, ?, ?)',
        values: [username, password, roleId],
    }, (err, table) => {
        if (err) {
            res.send({
                status: 'err',
                message: 'server-side database error.'
            })
            return;
        }
        else {
            res.send({
                status: 'ok',
            });
        }
    })
}

/**
 * Delete Handler, by id
 */
let deleteHandler = (req, res) => {
    if (req.role !== common.ROLE.ADMINISTRATOR) {
        res.send({
            status: 'err',
            message: 'permission denied.'
        });
        return;
    }
    let id = req.query.id || req.body.id;
    if (typeof id === 'undefined') {
        res.send({
            status: 'err',
            message: 'required filed(s) empty!',
        });
        return;
    }
    let conn = db.getConn();
    conn.query({
        sql: 'DELETE FROM user WHERE user_id = ?',
        values: [id],
    }, (err, table) => {
        if (err) {
            res.send({
                status: 'err',
                message: 'server-side database error.'
            })
            return;
        }
        else if (table.affectedRows === 0) {
            res.send({
                status: 'err',
                message: 'no such id.'
            });
            return;
        }
        else {
            res.send({
                status: 'ok',
            });
        }
    })
}


module.exports = {
    get: getHandler,
    put: putHandler,
    post: postHandler,
    delete: deleteHandler,
}