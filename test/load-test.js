var assert = require('assert'),
    async = require('async'),
    fs = require('fs'),
    deferTick = require('..');

describe('nextTick()', function() {
  it('should fire one callback when queued', function(callback) {
    deferTick(callback);
  });

  it('should fire tons of callback when queued', function(callback) {
    async.map(new Array(2000).join('.').split('.'), function(_, callback) {
      deferTick(callback);
    }, callback);
  });

  it('should fire callback after fs action', function(callback) {
    function loop(num, callback) {
      var i = 0;
      fs.stat('/dev/null', function() {
        i++;
      });

      deferTick(function() {
        i++;
        if (i == 2) return callback();
        if (num < 3000) return loop(num + 1, callback);
        assert(num < 3000);
      });
    };

    loop(0, callback);
  });
});
