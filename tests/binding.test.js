const t = require('tap')
const { startProfiling, stopProfiling } = require('../lib/binding')

t.test('startProfiling', (t) => {
  t.plan(3)
  t.ok(startProfiling)
  t.equal(typeof startProfiling, 'function')
  t.ok(startProfiling())
})

t.test('stopProfiling', (t) => {
  t.plan(3)
  t.ok(stopProfiling)
  t.equal(typeof stopProfiling, 'function')
  t.ok(stopProfiling())
})
