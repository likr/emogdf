#include <emscripten/bind.h>
#include <ogdf/energybased/FMMMLayout.h>

using namespace emscripten;

void defineEnergybased () {
  class_<ogdf::FMMMLayout>("FMMMLayout")
    .constructor()
    .function("call", select_overload<void(ogdf::GraphAttributes&)>(&ogdf::FMMMLayout::call))
    .function("useHighLevelOptions", select_overload<void(bool)>(&ogdf::FMMMLayout::useHighLevelOptions))
    .function("unitEdgeLength", select_overload<void(double)>(&ogdf::FMMMLayout::unitEdgeLength))
    .function("newInitialPlacement", select_overload<void(bool)>(&ogdf::FMMMLayout::newInitialPlacement))
    ;
}
