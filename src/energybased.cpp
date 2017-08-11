#include <emscripten/bind.h>
#include <ogdf/energybased/DTreeMultilevelEmbedder.h>
#include <ogdf/energybased/DavidsonHarelLayout.h>
#include <ogdf/energybased/FastMultipoleEmbedder.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/energybased/GEMLayout.h>
#include <ogdf/energybased/MultilevelLayout.h>
#include <ogdf/energybased/TutteLayout.h>

using namespace emscripten;

void defineDavidsonHarelLayout () {
  class_<ogdf::DavidsonHarelLayout>("DavidsonHarelLayout")
    .constructor()
    .function("call", &ogdf::DavidsonHarelLayout::call)
    ;
}

void defineFMMMLayout () {
  class_<ogdf::FMMMLayout>("FMMMLayout")
    .constructor()
    .function("call", select_overload<void(ogdf::GraphAttributes&)>(&ogdf::FMMMLayout::call))
    .function("callEdgeLength", select_overload<void(ogdf::GraphAttributes&, const ogdf::EdgeArray<double>&)>(&ogdf::FMMMLayout::call))
    .function("setSingleLevel", &ogdf::FMMMLayout::setSingleLevel)
    .property("useHighLevelOptions",
        select_overload<bool()const>(&ogdf::FMMMLayout::useHighLevelOptions),
        select_overload<void(bool)>(&ogdf::FMMMLayout::useHighLevelOptions))
    .property("pageFormat",
        select_overload<ogdf::FMMMOptions::PageFormatType()const>(&ogdf::FMMMLayout::pageFormat),
        select_overload<void(ogdf::FMMMOptions::PageFormatType)>(&ogdf::FMMMLayout::pageFormat))
    .property("unitEdgeLength",
        select_overload<double()const>(&ogdf::FMMMLayout::unitEdgeLength),
        select_overload<void(double)>(&ogdf::FMMMLayout::unitEdgeLength))
    .property("newInitialPlacement",
        select_overload<bool()const>(&ogdf::FMMMLayout::newInitialPlacement),
        select_overload<void(bool)>(&ogdf::FMMMLayout::newInitialPlacement))
    .property("qualityVersusSpeed",
        select_overload<ogdf::FMMMOptions::QualityVsSpeed()const>(&ogdf::FMMMLayout::qualityVersusSpeed),
        select_overload<void(ogdf::FMMMOptions::QualityVsSpeed)>(&ogdf::FMMMLayout::qualityVersusSpeed))
    .property("randSeed",
        select_overload<int()const>(&ogdf::FMMMLayout::randSeed),
        select_overload<void(int)>(&ogdf::FMMMLayout::randSeed))
    .property("edgeLengthMeasurement",
        select_overload<ogdf::FMMMOptions::EdgeLengthMeasurement()const>(&ogdf::FMMMLayout::edgeLengthMeasurement),
        select_overload<void(ogdf::FMMMOptions::EdgeLengthMeasurement)>(&ogdf::FMMMLayout::edgeLengthMeasurement))
    .property("allowedPositions",
        select_overload<ogdf::FMMMOptions::AllowedPositions()const>(&ogdf::FMMMLayout::allowedPositions),
        select_overload<void(ogdf::FMMMOptions::AllowedPositions)>(&ogdf::FMMMLayout::allowedPositions))
    .property("maxIntPosExponent",
        select_overload<int()const>(&ogdf::FMMMLayout::maxIntPosExponent),
        select_overload<void(int)>(&ogdf::FMMMLayout::maxIntPosExponent))
    .property("initialPlacementMult",
        select_overload<ogdf::FMMMOptions::InitialPlacementMult()const>(&ogdf::FMMMLayout::initialPlacementMult),
        select_overload<void(ogdf::FMMMOptions::InitialPlacementMult)>(&ogdf::FMMMLayout::initialPlacementMult))
    .property("initialPlacementForces",
        select_overload<ogdf::FMMMOptions::InitialPlacementForces()const>(&ogdf::FMMMLayout::initialPlacementForces),
        select_overload<void(ogdf::FMMMOptions::InitialPlacementForces)>(&ogdf::FMMMLayout::initialPlacementForces))
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
}

void defineGEMLayout () {
  class_<ogdf::GEMLayout>("GEMLayout")
    .constructor()
    .function("call", &ogdf::GEMLayout::call)
    .property("attractionFormula",
        select_overload<int()const>(&ogdf::GEMLayout::attractionFormula),
        select_overload<void(int)>(&ogdf::GEMLayout::attractionFormula))
    .property("desiredLength",
        select_overload<double()const>(&ogdf::GEMLayout::desiredLength),
        select_overload<void(double)>(&ogdf::GEMLayout::desiredLength))
    .property("gravitationalConstant",
        select_overload<double()const>(&ogdf::GEMLayout::gravitationalConstant),
        select_overload<void(double)>(&ogdf::GEMLayout::gravitationalConstant))
    .property("initialTemperature",
        select_overload<double()const>(&ogdf::GEMLayout::initialTemperature),
        select_overload<void(double)>(&ogdf::GEMLayout::initialTemperature))
    .property("maximalDisturbance",
        select_overload<double()const>(&ogdf::GEMLayout::maximalDisturbance),
        select_overload<void(double)>(&ogdf::GEMLayout::maximalDisturbance))
    .property("minDistCC",
        select_overload<double()const>(&ogdf::GEMLayout::minDistCC),
        select_overload<void(double)>(&ogdf::GEMLayout::minDistCC))
    .property("minimalTemperature",
        select_overload<double()const>(&ogdf::GEMLayout::minimalTemperature),
        select_overload<void(double)>(&ogdf::GEMLayout::minimalTemperature))
    .property("numberOfRounds",
        select_overload<int()const>(&ogdf::GEMLayout::numberOfRounds),
        select_overload<void(int)>(&ogdf::GEMLayout::numberOfRounds))
    .property("oscillationAngle",
        select_overload<double()const>(&ogdf::GEMLayout::oscillationAngle),
        select_overload<void(double)>(&ogdf::GEMLayout::oscillationAngle))
    .property("oscillationSensitivity",
        select_overload<double()const>(&ogdf::GEMLayout::oscillationSensitivity),
        select_overload<void(double)>(&ogdf::GEMLayout::oscillationSensitivity))
    .property("pageRatio",
        select_overload<double()const>(&ogdf::GEMLayout::pageRatio),
        select_overload<void(double)>(&ogdf::GEMLayout::pageRatio))
    .property("rotationAngle",
        select_overload<double()const>(&ogdf::GEMLayout::rotationAngle),
        select_overload<void(double)>(&ogdf::GEMLayout::rotationAngle))
    .property("rotationSensitivity",
        select_overload<double()const>(&ogdf::GEMLayout::rotationSensitivity),
        select_overload<void(double)>(&ogdf::GEMLayout::rotationSensitivity))
    ;
}

void defineMultilevelLayout () {
  class_<ogdf::MultilevelLayout>("MultilevelLayout")
    .constructor()
    .function("call", &ogdf::MultilevelLayout::call)
    ;
}

void defineTutteLayout () {
  class_<ogdf::TutteLayout>("TutteLayout")
    .constructor()
    .function("call", select_overload<void(ogdf::GraphAttributes&)>(&ogdf::TutteLayout::call))
    ;
}

void defineEnergybased () {
  class_<ogdf::DTreeMultilevelEmbedder2D>("DTreeMultilevelEmbedder")
    .constructor()
    .function("call", &ogdf::DTreeMultilevelEmbedder2D::call)
    ;

  class_<ogdf::FastMultipoleEmbedder>("FastMultipoleEmbedder")
    .constructor()
    .function("call", select_overload<void(ogdf::GraphAttributes&)>(&ogdf::FastMultipoleEmbedder::call))
    ;

  // class_<ogdf::FastMultipoleMultilevelEmbedder>("FastMultipoleMultilevelEmbedder")
  //   .constructor()
  //   .function("call", &ogdf::FastMultipoleMultilevelEmbedder::call)
  //   ;

  defineDavidsonHarelLayout();
  defineFMMMLayout();
  defineGEMLayout();
  defineMultilevelLayout();
  defineTutteLayout();
}
