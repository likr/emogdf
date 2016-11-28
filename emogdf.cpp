#include <emscripten/bind.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/graph_generators.h>

EMSCRIPTEN_BINDINGS(ogdf) {
  emscripten::function("randomSimpleGraph", &ogdf::randomSimpleGraph);

  emscripten::class_<ogdf::Graph>("Graph")
    .constructor()
    .function("empty", &ogdf::Graph::empty)
    .function("numberOfNodes", &ogdf::Graph::numberOfNodes)
    .function("numberOfEdges", &ogdf::Graph::numberOfEdges)
    ;
}
