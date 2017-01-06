'use strict';

let db = require('../db');
let common = require('../common');

let formatter = (table) => {
    return table.map(row => {
        return {
            id: row.news_policy_id,
            date: row.news_date,
            title: row.news_title,
            content: row.news_content,
            type_id: row.news_type_id,
            status_id: row.news_status_id,
            file_type_id: row.file_type_id,
            publish_officer_id: row.publish_officer_id,
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
            sql: 'SELECT * FROM news WHERE news_policy_id = ?',
            values: [req.query.id]
        }, (err, table) => sendBack(err, table, res));
    }
    else {
        conn.query({
            sql: 'SELECT * FROM news;',
            values: []
        }, (err, table) => sendBack(err, table, res));
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
    let data = {
        id: req.body.id || req.query.id,
        date: req.body.date || req.query.date,
        title: req.body.title || req.query.title,
        content: req.body.content || req.query.content,
        type_id: req.body.type_id || req.query.type_id,
        status_id: req.body.status_id || req.query.status_id,
        file_type_id: req.body.file_type_id || req.query.file_type_id,
        publish_officer_id: req.body.publish_officer_id || req.query.publish_officer_id,
    }
    for (let key in data) {
        if (typeof data[key] === 'undefined') {
            res.send({
                status: 'err',
                message: 'required filed(s) empty!',
            });
            return;
        }
    }

    let conn = db.getConn();
    conn.query({
        sql: [
            'UPDATE news SET news_title = ?,',
                            'news_content = ?,',
                            'news_date = ?,',
                            'news_type_id = ?,',
                            'news_status_id = ?,',
                            'file_type_id = ?,',
                            'publish_officer_id = ?',
            'WHERE news_policy_id = ?'
        ].join(' '),
        values: [
            data.title, data.content, data.date, data.type_id, data.status_id, 
            data.file_type_id, data.publish_officer_id, data.id
        ]
    }, (err, table) => {
        if (err) {
            console.log(err);
            res.send({
                status: 'err',
                message: 'server-side database error or data mismatch.'
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
    });
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
    let data = {
        date: req.body.date || req.query.date,
        title: req.body.title || req.query.title,
        content: req.body.content || req.query.content,
        type_id: req.body.type_id || req.query.type_id,
        status_id: req.body.status_id || req.query.status_id,
        file_type_id: req.body.file_type_id || req.query.file_type_id,
        publish_officer_id: req.body.publish_officer_id || req.query.publish_officer_id,
    }
    for (let key in data) {
        if (typeof data[key] === 'undefined') {
            res.send({
                status: 'err',
                message: 'required filed(s) empty!',
            });
            return;
        }
    }

    let conn = db.getConn();
    conn.query({
        sql: [
            'INSERT INTO news(news_title, news_content, news_date, news_type_id,',
                             'news_status_id, file_type_id, publish_officer_id)',
            'VALUES(?, ?, ?, ?, ?, ?, ?)'
        ].join(' '),
        values: [
            data.title, data.content, data.date, data.type_id, data.status_id, 
            data.file_type_id, data.publish_officer_id
        ]
    }, (err, table) => {
        if (err) {
            console.log(err);
            res.send({
                status: 'err',
                message: 'server-side database error or data mismatch.'
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
    });
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
        sql: 'DELETE FROM news WHERE news_policy_id = ?',
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