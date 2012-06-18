var binding = require('../build/Release/defer_tick');

module.exports = function deferTick(callback) {
  binding.deferTick(callback);
};
