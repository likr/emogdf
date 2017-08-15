/* eslint-env mocha */

const assert = require('power-assert')
const {run} = require('../util')

run((ogdf) => {
  const {
    Graph,
    EdgeList
  } = ogdf

  describe('EdgeList', () => {
    it('works', () => {
      const graph = new Graph()
      const u = graph.newNode()
      const v = graph.newNode()
      const e = graph.newEdge(u, v)
      const edges = new EdgeList()
      graph.allEdges(edges)
      assert.equal(edges.get(0).index(), e.index())
      assert.equal(edges.get(0).source().index(), u.index())
      assert.equal(edges.get(0).target().index(), v.index())
    })
  })
})
