'use strict';

let mysql = require('mysql');

let conn = mysql.createConnection(require('./config').mysql);

/**
 * MySQL will close the connection if there is no activity for some time.
 * See: http://stackoverflow.com/questions/20210522/nodejs-mysql-error-connection-lost-the-server-closed-the-connection
 */
let disconnectHandler = function() {
    conn.on('error', (err) => {
        if (!err.fatal) {
            return;
        }
        if (err.code !== 'PROTOCOL_CONNECTION_LOST') {
            throw err;
        }
        conn.destroy();
        conn = mysql.createConnection(require('./config').mysql);
        disconnectHandler();
    });
}
disconnectHandler();

module.exports = {
    getConn: function() {
        return conn;
    }
}