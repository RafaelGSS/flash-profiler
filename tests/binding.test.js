const assert = require('assert')
const { startProfiling } = require('../lib/binding')

assert.ok(startProfiling)
assert.strictEqual(typeof startProfiling, 'function')
assert.ok(startProfiling())
