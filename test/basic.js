/* eslint-env mocha */

const assert = require('power-assert')
const {run} = require('./util')

run((ogdf) => {
  const {
    Graph,
    NodeList,
    EdgeList,
    GraphAttributes,
    Shape
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

  describe('NodeList', () => {
    it('works', () => {
      const graph = new Graph()
      const u = graph.newNode()
      const nodes = new NodeList()
      graph.allNodes(nodes)
      assert.equal(nodes.at(0).index(), u.index())
    })
  })

  describe('EdgeList', () => {
    it('works', () => {
      const graph = new Graph()
      const u = graph.newNode()
      const v = graph.newNode()
      const e = graph.newEdge(u, v)
      const edges = new EdgeList()
      graph.allEdges(edges)
      assert.equal(edges.at(0).index(), e.index())
    })
  })

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
  })

  describe('Shape', () => {
    const properties = [
      'Rect',
      'RoundedRect',
      'Ellipse',
      'Triangle',
      'Pentagon',
      'Hexagon',
      'Octagon',
      'Rhomb',
      'Trapeze',
      'Parallelogram',
      'InvTriangle',
      'InvTrapeze',
      'InvParallelogram',
      'Image'
    ]
    for (const name of properties) {
      it(`has enum ${name}`, () => {
        assert.notEqual(Shape[name].value, undefined)
      })
    }
  })
})
