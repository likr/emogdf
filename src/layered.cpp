#include <emscripten/bind.h>
#include <ogdf/layered/SugiyamaLayout.h>

using namespace emscripten;

void defineLayered () {
  class_<ogdf::SugiyamaLayout>("SugiyamaLayout")
    .constructor()
    .function("call", select_overload<void(ogdf::GraphAttributes&)>(&ogdf::SugiyamaLayout::call))
    .function("callWithRank", select_overload<void(ogdf::GraphAttributes&, ogdf::NodeArray<int>&)>(&ogdf::SugiyamaLayout::call))
    .function("setRanking", &ogdf::SugiyamaLayout::setRanking, allow_raw_pointers())
    .function("setCrossMin", &ogdf::SugiyamaLayout::setCrossMin, allow_raw_pointers())
    .function("setLayout", &ogdf::SugiyamaLayout::setLayout, allow_raw_pointers())
    .function("setClusterLayout", &ogdf::SugiyamaLayout::setClusterLayout, allow_raw_pointers())
    .function("setPacker", &ogdf::SugiyamaLayout::setPacker, allow_raw_pointers())
    .function("numberOfCrossings", &ogdf::SugiyamaLayout::numberOfCrossings)
    .function("numberOfCrossingsCluster", &ogdf::SugiyamaLayout::numberOfCrossingsCluster)
    .function("numberOfLevels", &ogdf::SugiyamaLayout::numberOfLevels)
    .function("maxLevelSize", &ogdf::SugiyamaLayout::maxLevelSize)
    .function("timeReduceCrossings", &ogdf::SugiyamaLayout::timeReduceCrossings)
    .property("fails", select_overload<int() const>(&ogdf::SugiyamaLayout::fails), select_overload<void(int)>(&ogdf::SugiyamaLayout::fails))
    .property("runs", select_overload<int() const>(&ogdf::SugiyamaLayout::runs), select_overload<void(int)>(&ogdf::SugiyamaLayout::runs))
    .property("transpose", select_overload<bool() const>(&ogdf::SugiyamaLayout::transpose), select_overload<void(bool)>(&ogdf::SugiyamaLayout::transpose))
    .property("arrangeCCs", select_overload<bool() const>(&ogdf::SugiyamaLayout::arrangeCCs), select_overload<void(bool)>(&ogdf::SugiyamaLayout::arrangeCCs))
    .property("minDistCC", select_overload<double() const>(&ogdf::SugiyamaLayout::minDistCC), select_overload<void(double)>(&ogdf::SugiyamaLayout::minDistCC))
    .property("pageRatio", select_overload<double() const>(&ogdf::SugiyamaLayout::pageRatio), select_overload<void(double)>(&ogdf::SugiyamaLayout::pageRatio))
    .property("alignBaseClasses", select_overload<bool() const>(&ogdf::SugiyamaLayout::alignBaseClasses), select_overload<void(bool)>(&ogdf::SugiyamaLayout::alignBaseClasses))
    .property("alignSiblings", select_overload<bool() const>(&ogdf::SugiyamaLayout::alignSiblings), select_overload<void(bool)>(&ogdf::SugiyamaLayout::alignSiblings))
    .property("permuteFirst", select_overload<bool() const>(&ogdf::SugiyamaLayout::permuteFirst), select_overload<void(bool)>(&ogdf::SugiyamaLayout::permuteFirst))
    .property("maxThreads", select_overload<unsigned int() const>(&ogdf::SugiyamaLayout::maxThreads), select_overload<void(unsigned int)>(&ogdf::SugiyamaLayout::maxThreads))
    ;
}
