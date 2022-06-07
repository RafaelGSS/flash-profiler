const assert = require('assert')
const { startProfiling, stopProfiling } = require('../lib/binding')

assert.ok(startProfiling)
assert.strictEqual(typeof startProfiling, 'function')
assert.ok(startProfiling())

assert.ok(stopProfiling)
assert.strictEqual(typeof stopProfiling, 'function')
assert.ok(stopProfiling())
