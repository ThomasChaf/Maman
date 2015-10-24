/*global phantom */
'use strict';

var step = 30000;

var fs = require('fs'),
    argv = require('system').args,
    stream = fs.open(argv[1], 'r'),
    page = require('webpage').create(),
    i = 1,
    url,
    filename,
    httpStatus;

page.onConsoleMessage = function(msg) {
    console.log(msg);
};

page.onResourceReceived = function(resource) {
    httpStatus = resource.status;
};

function finishCrawler()
{
    stream.close();
    phantom.exit(0);
    console.log('Sucess exit');
}

function crawlWebsites()
{
    if (stream.atEnd() || i >= step)
    {
        finishCrawler();
        return ;
    }

    httpStatus = null;
    url = stream.readLine();
    if (url.indexOf('http') !== 0)
        url = 'http://' + url;
    filename = 'sites/fichier' + i;
    i = i + 1;

    console.log('Craw website [' + filename + ']: ' + url);
    if (fs.isFile(filename))
    {
        console.log('file ' + filename + ' already exist');
        crawlWebsites();
        return ;
    }

    page.open(url, function(status) {

        setTimeout(function() {
            if (status === 'success') {
                fs.write(filename, page.content, 'w');
            }
            else {
                if (httpStatus === null)
                    fs.write(filename, 'Error: Website is no more online', 'w');
                if (httpStatus === 200)
                    fs.write(filename, page.content, 'w');
            }
            crawlWebsites();
        }, 5000);
    });
}

crawlWebsites();
