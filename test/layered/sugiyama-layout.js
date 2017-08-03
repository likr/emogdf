/* eslint-env mocha */

const {run} = require('../util')

run((ogdf) => {
  const {
    Graph,
    GraphAttributes,
    NodeArrayInt,
    SugiyamaLayout
  } = ogdf
  describe('SugiyamaLayout', () => {
    describe('call(GA)', () => {
      it('computes layout', () => {
        const graph = new Graph()
        const u = graph.newNode()
        const v = graph.newNode()
        const w = graph.newNode()
        graph.newEdge(u, v)
        graph.newEdge(u, w)
        graph.newEdge(v, w)

        const {
          nodeGraphics,
          edgeGraphics,
          nodeStyle,
          edgeStyle
        } = GraphAttributes
        const attributes = new GraphAttributes(graph, nodeGraphics | edgeGraphics | nodeStyle | edgeStyle)
        const layout = new SugiyamaLayout()
        layout.call(attributes)
      })
    })

    describe('callWithRank(GA, rank)', () => {
      it('computes layout', () => {
        const graph = new Graph()
        const u = graph.newNode()
        const v = graph.newNode()
        const w = graph.newNode()
        graph.newEdge(u, v)
        graph.newEdge(u, w)
        graph.newEdge(v, w)

        const rank = new NodeArrayInt(graph, 0)
        rank.set(u, 0)
        rank.set(v, 1)
        rank.set(w, 2)

        const {
          nodeGraphics,
          edgeGraphics,
          nodeStyle,
          edgeStyle
        } = GraphAttributes
        const attributes = new GraphAttributes(graph, nodeGraphics | edgeGraphics | nodeStyle | edgeStyle)
        const layout = new SugiyamaLayout()
        layout.call(attributes)
      })
    })
  })
})
