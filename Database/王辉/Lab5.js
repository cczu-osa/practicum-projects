#!/usr/bin/node

var mysql = require('mysql');
var port = 2333;
var http = require('http');
var url = require('url');

var connection = mysql.createConnection({
    host     : '127.0.0.1',
    user     : 'root',
    password : '',
    database : 'school',
    useTransaction: {
        connectionLimit: 1
    }
});

http.createServer((request, response) => {
    response.setHeader("Access-Control-Allow-Origin", "*");
    
    var urlParsed = url.parse(request.url, true);
    
    if (urlParsed.query['query']) {
        console.log(urlParsed.query['query']);
        connection.query(url.parse(request.url, true).query['query'], (err, res) => {
            response.end(JSON.stringify(err ? err : res));
        });
    }
    else {
        response.end('err');
    }
}).listen(port);
