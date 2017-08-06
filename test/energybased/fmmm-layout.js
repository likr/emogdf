/* eslint-env mocha */

const assert = require('power-assert')
const {run} = require('../util')

run((ogdf) => {
  const {
    Graph,
    GraphAttributes,
    FMMMLayout
  } = ogdf
  describe('FMMMLayout', () => {
    describe('useHighLevelOptions(value)', () => {
      it('sets parameter', () => {
        const layout = new FMMMLayout()
        assert.equal(layout.useHighLevelOptions, false)
        layout.useHighLevelOptions = true
        assert.equal(layout.useHighLevelOptions, true)
      })
    })

    describe('unitEdgeLength(value)', () => {
      it('sets parameter', () => {
        const layout = new FMMMLayout()
        layout.unitEdgeLength = 1
      })
    })

    describe('newInitialPlacement(value)', () => {
      it('sets parameter', () => {
        const layout = new FMMMLayout()
        layout.newInitialPlacement = true
      })
    })

    describe('randSeed(value)', () => {
      it('sets parameter', () => {
        const layout = new FMMMLayout()
        layout.randSeed = 0
      })
    })

    describe('call(GA)', () => {
      it('computes layout', () => {
        const graph = new Graph()
        const u = graph.newNode()
        const v = graph.newNode()
        graph.newEdge(u, v)
        const {
          nodeGraphics,
          edgeGraphics,
          nodeStyle,
          edgeStyle
        } = GraphAttributes
        const attributes = new GraphAttributes(graph, nodeGraphics | edgeGraphics | nodeStyle | edgeStyle)

        const layout = new FMMMLayout()
        layout.call(attributes)
      })
    })
  })
})
