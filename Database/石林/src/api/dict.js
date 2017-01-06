'use strict';

let db = require('../db');
let common = require('../common');

let formatter = (table) => {
    return table.map(row => {
        return {
            id: row.dict_id,
            category: common.categoryToString(row.dict_category),
            value: row.dict_value,
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
            sql: 'SELECT * FROM dict WHERE dict_id = ?',
            values: [req.query.id]
        }, (err, table) => sendBack(err, table, res))
    }
    else if (req.query.category) {
        let categoryId = common.CATEGORY[req.query.category];
        if (typeof categoryId === 'undefined') {
            res.send({
                status: 'err',
                message: 'invalid category id',
            });
            return;
        }
        conn.query({
            sql: 'SELECT * FROM dict WHERE dict_category = ?',
            values: [categoryId],
        }, (err, table) => sendBack(err, table, res));
    }
    else {
        conn.query({
            sql: 'SELECT * FROM dict',
        }, (err, table) => sendBack(err, table, res))
    }
}

/**
 * Upgrade value, by id, all column data required.
 */
let postHandler = (req, res) => {
    if ([common.ROLE.ADMINISTRATOR, common.ROLE.OFFICER].indexOf(req.role) === -1) {
        res.send({
            status: 'err',
            message: 'permission denied.'
        });
        return;
    }
    let id = req.query.id || req.body.id;
    let category = req.query.category || req.body.category;
    let value = req.query.value || req.body.value;
    if (typeof id === 'undefined' || typeof category === 'undefined' || typeof value === 'undefined') {
        res.send({
            status: 'err',
            message: 'required filed(s) empty!',
        });
        return;
    }
    let conn = db.getConn();
    let categoryId = common.CATEGORY[category];
    conn.query({
        sql: 'UPDATE dict SET dict_category = ?, dict_value = ? WHERE dict_id = ?',
        values: [categoryId, value, id],
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
    if ([common.ROLE.ADMINISTRATOR, common.ROLE.OFFICER].indexOf(req.role) === -1) {
        res.send({
            status: 'err',
            message: 'permission denied.'
        });
        return;
    }
    let category = req.query.category || req.body.category;
    let value = req.query.value || req.body.value;

    if (typeof category === 'undefined' || typeof value === 'undefined') {
        res.send({
            status: 'err',
            message: 'required filed(s) empty!',
        });
        return;
    }
    let conn = db.getConn();
    let categoryId = common.CATEGORY[category];
    conn.query({
        sql: 'INSERT INTO dict(dict_category, dict_value) VALUES(?, ?)',
        values: [categoryId, value],
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
    if ([common.ROLE.ADMINISTRATOR, common.ROLE.OFFICER].indexOf(req.role) === -1) {
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
        sql: 'DELETE FROM dict WHERE dict_id = ?',
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