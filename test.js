const m = require('./emogdf')

const graph = new m.Graph()
console.log('empty ?: ', graph.empty())
m.randomSimpleGraph(graph, 10, 10)
console.log(graph.numberOfNodes(), graph.numberOfEdges())
