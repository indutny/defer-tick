var assert = require('assert'),
    async = require('async'),
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
});
