const assert = require('assert')
const util = require('util')
const { startProfiling, stopProfiling } = require('../lib/binding')

let testArr = [];
let startTime = Date.now()

function busyLoop(durationSeconds) {
  for (let i = 0; i < testArr.length; i++) {
    for (let j = 0; j < testArr[i].length; j++) {
      testArr[i][j] = Math.sqrt(j * testArr[i][j]);
    }
  }
  if (Date.now() - startTime < 1000 * durationSeconds) {
    setTimeout(() => busyLoop(durationSeconds), 5);
  }
}

function benchmark(durationSeconds) {
  // Allocate 16 MiB in 64 KiB chunks.
  for (let i = 0; i < 16 * 16; i++) {
    testArr[i] = new Array(64 * 1024);
  }
  busyLoop(durationSeconds);
}

function logProfilingData(data) {
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
