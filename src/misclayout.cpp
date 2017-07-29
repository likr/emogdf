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
    .property("minDistCircle",
        select_overload<double(void)const>(&ogdf::CircularLayout::minDistCircle),
        select_overload<void(double)>(&ogdf::CircularLayout::minDistCircle))
    .property("minDistLevel",
        select_overload<double(void)const>(&ogdf::CircularLayout::minDistLevel),
        select_overload<void(double)>(&ogdf::CircularLayout::minDistLevel))
    .property("minDistSibling",
        select_overload<double(void)const>(&ogdf::CircularLayout::minDistSibling),
        select_overload<void(double)>(&ogdf::CircularLayout::minDistSibling))
    .property("minDistCC",
        select_overload<double(void)const>(&ogdf::CircularLayout::minDistCC),
        select_overload<void(double)>(&ogdf::CircularLayout::minDistCC))
    .property("pageRatio",
        select_overload<double(void)const>(&ogdf::CircularLayout::pageRatio),
        select_overload<void(double)>(&ogdf::CircularLayout::pageRatio))
    .function("call", &ogdf::CircularLayout::call)
    ;
}
