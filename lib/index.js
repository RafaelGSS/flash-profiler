'use strict'

const Fastify = require('fastify')
const { allocateBuffer, freeBuffer } = require('./binding')
const server = Fastify()

server.get('/', async (req, res) => {
  const buffer = allocateBigObject()
  process.nextTick(() => freeBuffer(buffer))
  return buffer.length
})

server.get('/simple', async (req, res) => {
  return 'hello world'
})

server.listen(3000)

function allocateBigObject () {
  return allocateBuffer()
}
