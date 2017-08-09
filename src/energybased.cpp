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
    .function("callEdgeLength", select_overload<void(ogdf::GraphAttributes&, const ogdf::EdgeArray<double>&)>(&ogdf::FMMMLayout::call))
    .function("useHighLevelOptions", select_overload<void(bool)>(&ogdf::FMMMLayout::useHighLevelOptions))
    .function("setSingleLevel", &ogdf::FMMMLayout::setSingleLevel)
    .function("pageFormat", select_overload<void(ogdf::FMMMOptions::PageFormatType)>(&ogdf::FMMMLayout::pageFormat))
    .function("unitEdgeLength", select_overload<void(double)>(&ogdf::FMMMLayout::unitEdgeLength))
    .function("newInitialPlacement", select_overload<void(bool)>(&ogdf::FMMMLayout::newInitialPlacement))
    .function("qualityVersusSpeed", select_overload<void(ogdf::FMMMOptions::QualityVsSpeed)>(&ogdf::FMMMLayout::qualityVersusSpeed))
    .function("randSeed", select_overload<void(int)>(&ogdf::FMMMLayout::randSeed))
    .function("edgeLengthMeasurement", select_overload<void(ogdf::FMMMOptions::EdgeLengthMeasurement)>(&ogdf::FMMMLayout::edgeLengthMeasurement))
    .function("allowedPositions", select_overload<void(ogdf::FMMMOptions::AllowedPositions)>(&ogdf::FMMMLayout::allowedPositions))
    .function("maxIntPosExponent", select_overload<void(int)>(&ogdf::FMMMLayout::maxIntPosExponent))
    .function("initialPlacementMult", select_overload<void(ogdf::FMMMOptions::InitialPlacementMult)>(&ogdf::FMMMLayout::initialPlacementMult))
    .function("initialPlacementForces", select_overload<void(ogdf::FMMMOptions::InitialPlacementForces)>(&ogdf::FMMMLayout::initialPlacementForces))
    ;

  enum_<ogdf::FMMMOptions::PageFormatType>("FMMMOptionsPageFormatType")
    .value("Portrait", ogdf::FMMMOptions::PageFormatType::Portrait)
    .value("Landscape", ogdf::FMMMOptions::PageFormatType::Landscape)
    .value("Square", ogdf::FMMMOptions::PageFormatType::Square)
    ;

  enum_<ogdf::FMMMOptions::QualityVsSpeed>("FMMMOptionsQualityVsSpeed")
    .value("GorgeousAndEfficient", ogdf::FMMMOptions::QualityVsSpeed::GorgeousAndEfficient)
    .value("BeautifulAndFast", ogdf::FMMMOptions::QualityVsSpeed::BeautifulAndFast)
    .value("NiceAndIncredibleSpeed", ogdf::FMMMOptions::QualityVsSpeed::NiceAndIncredibleSpeed)
    ;

  enum_<ogdf::FMMMOptions::EdgeLengthMeasurement>("FMMMOptionsEdgeLengthMeasurement")
    .value("Midpoint", ogdf::FMMMOptions::EdgeLengthMeasurement::Midpoint)
    .value("BoundingCircle", ogdf::FMMMOptions::EdgeLengthMeasurement::BoundingCircle)
    ;

  enum_<ogdf::FMMMOptions::AllowedPositions>("FMMMOptionsEdgeLengthAllowedPositions")
    .value("All", ogdf::FMMMOptions::AllowedPositions::All)
    .value("Integer", ogdf::FMMMOptions::AllowedPositions::Integer)
    .value("Exponent", ogdf::FMMMOptions::AllowedPositions::Exponent)
    ;

  enum_<ogdf::FMMMOptions::InitialPlacementMult>("FMMMOptionsInitialPlacementMult")
    .value("Simple", ogdf::FMMMOptions::InitialPlacementMult::Simple)
    .value("Advanced", ogdf::FMMMOptions::InitialPlacementMult::Advanced)
    ;

  enum_<ogdf::FMMMOptions::InitialPlacementForces>("FMMMOptionsInitialPlacementForces")
    .value("UniformGrid", ogdf::FMMMOptions::InitialPlacementForces::UniformGrid)
    .value("RandomTime", ogdf::FMMMOptions::InitialPlacementForces::RandomTime)
    .value("RandomRandIterNr", ogdf::FMMMOptions::InitialPlacementForces::RandomRandIterNr)
    .value("KeepPositions", ogdf::FMMMOptions::InitialPlacementForces::KeepPositions)
    ;

  class_<ogdf::TutteLayout>("TutteLayout")
    .constructor()
    .function("call", select_overload<void(ogdf::GraphAttributes&)>(&ogdf::TutteLayout::call))
    ;
}
