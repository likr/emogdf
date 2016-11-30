#include <emscripten/bind.h>
#include <ogdf/planarity/PlanarizationLayout.h>

using namespace emscripten;

void definePlanarity () {
  class_<ogdf::PlanarizationLayout>("PlanarizationLayout")
    .constructor()
    .function("call", select_overload<void(ogdf::GraphAttributes&)>(&ogdf::PlanarizationLayout::call))
    ;
}
