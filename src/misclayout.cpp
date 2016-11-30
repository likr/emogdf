#include <emscripten/bind.h>
#include <ogdf/misclayout/BalloonLayout.h>
#include <ogdf/misclayout/BertaultLayout.h>
#include <ogdf/misclayout/CircularLayout.h>

using namespace emscripten;

void defineMisclayout () {
  class_<ogdf::BalloonLayout>("BalloonLayout")
    .constructor()
    .function("call", &ogdf::BalloonLayout::call)
    ;

  class_<ogdf::BertaultLayout>("BertaultLayout")
    .constructor()
    .function("call", &ogdf::BertaultLayout::call)
    ;

  class_<ogdf::CircularLayout>("CircularLayout")
    .constructor()
    .function("call", &ogdf::CircularLayout::call)
    ;
}
