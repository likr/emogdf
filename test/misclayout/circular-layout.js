/* eslint-env mocha */

const assert = require('power-assert')
const {run} = require('../util')

run((ogdf) => {
  const {
    Graph,
    GraphAttributes,
    CircularLayout,
    randomGraph
  } = ogdf
  describe('CircularLayout', () => {
    describe('call(GA)', () => {
      it('computes layout', () => {
        const graph = new Graph()
        randomGraph(graph, 40, 70)

        const {
          nodeGraphics,
          edgeGraphics,
          nodeStyle,
          edgeStyle
        } = GraphAttributes
        const attributes = new GraphAttributes(graph, nodeGraphics | edgeGraphics | nodeStyle | edgeStyle)
        const layout = new CircularLayout()

        layout.call(attributes)
      })
    })

    describe('minDistCircle', () => {
      it('can set and get values', () => {
        const layout = new CircularLayout()
        const value = 20.0
        layout.minDistCircle = value
        assert.equal(layout.minDistCircle, value)
      })
    })

    describe('minDistLevel', () => {
      it('can set and get values', () => {
        const layout = new CircularLayout()
        const value = 20.0
        layout.minDistLevel = value
        assert.equal(layout.minDistLevel, value)
      })
    })

    describe('minDistSibling', () => {
      it('can set and get values', () => {
        const layout = new CircularLayout()
        const value = 10.0
        layout.minDistSibling = value
        assert.equal(layout.minDistSibling, value)
      })
    })

    describe('minDistCC', () => {
      it('can set and get values', () => {
        const layout = new CircularLayout()
        const value = 20.0
        layout.minDistCC = value
        assert.equal(layout.minDistCC, value)
      })
    })

    describe('pageRatio', () => {
      it('can set and get values', () => {
        const layout = new CircularLayout()
        const value = 1.0
        layout.pageRatio = value
        assert.equal(layout.pageRatio, value)
      })
    })
  })
})
