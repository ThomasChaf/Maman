/*global phantom */
'use strict';

var page = require('webpage').create();

page.onConsoleMessage = function(msg) {
    console.log(msg);
};

page.open('http://www.gcgd.lu', function(status) {

    setTimeout(function() {
        if ( status === 'success' ) {
            console.log(page.content);
            phantom.exit();
        }
    }, 5000);
});
