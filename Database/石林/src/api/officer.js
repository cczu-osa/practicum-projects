'use strict';

let db = require('../db');
let common = require('../common');

let formatter = (table) => {
    return table.map(row => {
        return {
            id: row.officer_id,
            uid: row.user_id,
            name: row.officer_name,
            gender: row.officer_gender,
            major: row.officer_major,
            job: row.officer_job,
            license: row.officer_license_id,
            locale_id: row.officer_locale_id,
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
    conn.query({
        sql: 'SELECT * FROM officer',
        values: [],
    }, (err, table) => sendBack(err, table, res));
}

/**
 * Upgrade value, by id, all column data required.
 *
 */
let postHandler = (req, res) => {
    if (req.role !== common.ROLE.ADMINISTRATOR) {
        res.send({
            status: 'err',
            message: 'permission denied.'
        });
        return;
    }
    let data = {};
    data.id = req.body.id || req.query.id;
    data.uid = req.body.uid || req.query.uid;
    data.job = req.body.job || req.query.job;
    data.name = req.body.name || req.query.name;
    data.major = req.body.major || req.query.major;
    data.gender = req.body.gender || req.query.gender;
    data.license = req.body.license || req.query.license;
    data.locale_id = req.body.locale_id || req.query.locale_id;
    
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
            'UPDATE officer SET officer_name = ?, officer_gender = ?,',
            '       officer_major = ?, officer_job = ?, officer_license_id = ?,',
            '       officer_locale_id = ?, user_id = ?',
            'WHERE officer_id = ?',
        ].join(' '),
        values: [
            data.name, data.gender, data.major, data.job,
            data.license, data.locale_id, data.uid, data.id
        ],
    }, (err, table) => {
        if (err) {
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
    let data = {};
    data.uid = req.body.uid || req.query.uid;
    data.job = req.body.job || req.query.job;
    data.name = req.body.name || req.query.name;
    data.major = req.body.major || req.query.major;
    data.gender = req.body.gender || req.query.gender;
    data.license = req.body.license || req.query.license;
    data.locale_id = req.body.locale_id || req.query.locale_id;
    
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
            'INSERT INTO officer(officer_name, officer_gender,',
            '            officer_major, officer_job, ',
            '            officer_license_id, officer_locale_id, ',
            '            user_id)',
            'VALUES(?, ?, ?, ?, ?, ?, ?)',
        ].join(' '),
        values: [
            data.name, data.gender, data.major, data.job,
            data.license, data.locale_id, data.uid
        ],
    }, (err, table) => {
        if (err) {
            console.log(err);
            res.send({
                status: 'err',
                message: 'server-side database error or data mismatch.'
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
        sql: 'DELETE FROM officer WHERE officer_id = ?',
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