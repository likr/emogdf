#include <emscripten/bind.h>
#include <ogdf/energybased/DavidsonHarelLayout.h>
#include <ogdf/energybased/FastMultipoleEmbedder.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/energybased/TutteLayout.h>

using namespace emscripten;

void defineEnergybased () {
  class_<ogdf::DavidsonHarelLayout>("DavidsonHarelLayout")
    .constructor()
    .function("call", &ogdf::DavidsonHarelLayout::call)
    ;

  class_<ogdf::FastMultipoleEmbedder>("FastMultipoleEmbedder")
    .constructor()
    .function("call", select_overload<void(ogdf::GraphAttributes&)>(&ogdf::FastMultipoleEmbedder::call))
    ;

  // class_<ogdf::FastMultipoleMultilevelEmbedder>("FastMultipoleMultilevelEmbedder")
  //   .constructor()
  //   .function("call", &ogdf::FastMultipoleMultilevelEmbedder::call)
  //   ;

  class_<ogdf::FMMMLayout>("FMMMLayout")
    .constructor()
    .function("call", select_overload<void(ogdf::GraphAttributes&)>(&ogdf::FMMMLayout::call))
    .function("useHighLevelOptions", select_overload<void(bool)>(&ogdf::FMMMLayout::useHighLevelOptions))
    .function("unitEdgeLength", select_overload<void(double)>(&ogdf::FMMMLayout::unitEdgeLength))
    .function("newInitialPlacement", select_overload<void(bool)>(&ogdf::FMMMLayout::newInitialPlacement))
    .function("randSeed", select_overload<void(int)>(&ogdf::FMMMLayout::randSeed))
    ;

  class_<ogdf::TutteLayout>("TutteLayout")
    .constructor()
    .function("call", select_overload<void(ogdf::GraphAttributes&)>(&ogdf::TutteLayout::call))
    ;
}
