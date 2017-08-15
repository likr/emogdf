/* eslint-env mocha */

const assert = require('power-assert')
const {run} = require('../util')

run((ogdf) => {
  const {
    Graph,
    NodeList
  } = ogdf

  describe('NodeList', () => {
    it('works', () => {
      const graph = new Graph()
      const u = graph.newNode()
      const nodes = new NodeList()
      graph.allNodes(nodes)
      assert.equal(nodes.get(0).index(), u.index())
    })
  })
})
