#include <emscripten/bind.h>
#include <ogdf/packing/ComponentSplitterLayout.h>

using namespace emscripten;

void definePacking () {
  class_<ogdf::ComponentSplitterLayout>("ComponentSplitterLayout")
    .constructor()
    .function("call", &ogdf::ComponentSplitterLayout::call)
    ;
}
