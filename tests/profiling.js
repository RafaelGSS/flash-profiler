const util = require('util')
const { startProfiling, stopProfiling } = require('../lib/binding')

const testArr = []
const startTime = Date.now()

function busyLoop (durationSeconds) {
  for (let i = 0; i < testArr.length; i++) {
    for (let j = 0; j < testArr[i].length; j++) {
      testArr[i][j] = Math.sqrt(j * testArr[i][j])
    }
  }
  if (Date.now() - startTime < 1000 * durationSeconds) {
    setTimeout(() => busyLoop(durationSeconds), 5)
  }
}

function logProfilingData (data) {
  console.log(
    util.inspect(
      data,
      false,
      null,
      true /* enable colors */
    )
  )
}

{
  startProfiling()
  busyLoop(5)
  const profilingData = stopProfiling()
  logProfilingData(profilingData)
}
