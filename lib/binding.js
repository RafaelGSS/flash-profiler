const addon = require('../build/Release/profiler');

module.exports = {
  startProfiling: addon.StartProfiling,
  stopProfiling: addon.StopProfiling
}
