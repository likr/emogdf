/* eslint-env mocha */

const {run} = require('../util')

run((ogdf) => {
  const {
    Graph
  } = ogdf

  describe('Graph', () => {
    describe('newNode()', () => {
      it('works', () => {
        const graph = new Graph()
        graph.newNode()
      })
    })

    describe('newEdge()', () => {
      it('works', () => {
        const graph = new Graph()
        const u = graph.newNode()
        const v = graph.newNode()
        graph.newEdge(u, v)
      })
    })
  })
})
