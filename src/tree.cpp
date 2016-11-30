#include <emscripten/bind.h>
#include <ogdf/tree/TreeLayout.h>

using namespace emscripten;

void defineTree () {
  class_<ogdf::TreeLayout>("TreeLayout")
    .constructor()
    .function("call", &ogdf::TreeLayout::call)
    ;
}
