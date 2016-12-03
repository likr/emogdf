#include <emscripten/bind.h>
#include <ogdf/layered/SugiyamaLayout.h>

using namespace emscripten;

void defineLayered () {
  class_<ogdf::SugiyamaLayout>("SugiyamaLayout")
    .constructor()
    .function("call", select_overload<void(ogdf::GraphAttributes&)>(&ogdf::SugiyamaLayout::call))
    ;
}
