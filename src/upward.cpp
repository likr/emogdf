#include <emscripten/bind.h>
#include <ogdf/upward/DominanceLayout.h>
#include <ogdf/upward/UpwardPlanarizationLayout.h>
#include <ogdf/upward/VisibilityLayout.h>

using namespace emscripten;

void defineUpward () {
  class_<ogdf::DominanceLayout>("DominanceLayout")
    .constructor()
    .function("call", &ogdf::DominanceLayout::call)
    ;

  class_<ogdf::UpwardPlanarizationLayout>("UpwardPlanarizationLayout")
    .constructor()
    .function("call", &ogdf::UpwardPlanarizationLayout::call)
    ;

  // class_<ogdf::VisibilityLayout>("VisibilityLayout")
  //   .constructor()
  //   .function("call", &ogdf::VisibilityLayout::call)
  //   ;
}
