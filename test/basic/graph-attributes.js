/* eslint-env mocha */

const assert = require('power-assert')
const {run} = require('../util')

run((ogdf) => {
  const {
    Graph,
    GraphAttributes
  } = ogdf

  describe('GraphAttributes', () => {
    const properties = [
      ['nodeGraphics', 0x1],
      ['edgeGraphics', 0x2],
      ['edgeIntWeight', 0x4],
      ['edgeDoubleWeight', 0x8],
      ['edgeLabel', 0x10],
      ['nodeLabel', 0x20],
      ['edgeType', 0x40],
      ['nodeType', 0x80],
      ['nodeId', 0x100],
      ['edgeArrow', 0x200],
      ['edgeStyle', 0x400],
      ['nodeStyle', 0x800],
      ['nodeTemplate', 0x1000],
      ['edgeSubGraphs', 0x2000],
      ['nodeWeight', 0x4000],
      ['threeD', 0x8000]
    ]
    for (const [name, value] of properties) {
      it(`has a constant property ${name}`, () => {
        assert.equal(GraphAttributes[name], value)
      })
    }

    const nodeAttributes = [
      'x',
      'y',
      'z',
      'width',
      'height'
    ]
    for (const name of nodeAttributes) {
      it(`has a property ${name}`, () => {
        const graph = new Graph()
        const v = graph.newNode()
        const attributes = new GraphAttributes(graph)
        const val = attributes[name](v)
        attributes[name](v, val)
      })
    }
  })
})
