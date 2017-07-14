#include <emscripten/bind.h>
#include <ogdf/basic/basic.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>

using namespace emscripten;

typedef ogdf::List<ogdf::node> NodeList;

void setX (ogdf::GraphAttributes& GA, ogdf::node n, double val) {
  GA.x(n) = val;
}

void setY (ogdf::GraphAttributes& GA, ogdf::node n, double val) {
  GA.y(n) = val;
}

void setZ (ogdf::GraphAttributes& GA, ogdf::node n, double val) {
  GA.z(n) = val;
}

void setWidth (ogdf::GraphAttributes& GA, ogdf::node n, double val) {
  GA.width(n) = val;
}

void setHeight (ogdf::GraphAttributes& GA, ogdf::node n, double val) {
  GA.height(n) = val;
}

void setShape (ogdf::GraphAttributes& GA, ogdf::node n, ogdf::Shape val) {
	GA.shape(n) = val;
}

// void setStrokeType (ogdf::GraphAttributes& GA, ogdf::node n, ogdf::StrokeType val) {
//   GA.setStrokeType(n, val);
// }

void setStrokeColor (ogdf::GraphAttributes& GA, ogdf::node n, ogdf::Color val) {
  GA.strokeColor(n) = val;
}

void setStrokeWidth (ogdf::GraphAttributes& GA, ogdf::node n, float val) {
  GA.strokeWidth(n) = val;
}

// void setFillPattern (ogdf::GraphAttributes& GA, ogdf::node n, ogdf::FillPattern val) {
//   GA.setFillPattern(n, val);
// }

void setFillColor (ogdf::GraphAttributes& GA, ogdf::node n, ogdf::Color val) {
  GA.fillColor(n) = val;
}

void setFillBgColor (ogdf::GraphAttributes& GA, ogdf::node n, ogdf::Color val) {
  GA.fillBgColor(n) = val;
}

void setLabel (ogdf::GraphAttributes& GA, ogdf::node n, std::string val) {
  GA.label(n) = val;
}

void setTemplateNode (ogdf::GraphAttributes& GA, ogdf::node n, std::string val) {
  GA.templateNode(n) = val;
}

ogdf::node nodeListAt (NodeList& list, int pos) {
  return *list.get(pos);
}

void defineGraph () {
  class_<ogdf::Graph>("Graph")
    .constructor()
    .function("numberOfNodes", &ogdf::Graph::numberOfNodes)
    .function("numberOfEdges", &ogdf::Graph::numberOfEdges)
    .function("maxNodeIndex", &ogdf::Graph::maxNodeIndex)
    .function("maxEdgeIndex", &ogdf::Graph::maxEdgeIndex)
    .function("firstNode", &ogdf::Graph::firstNode, allow_raw_pointers())
    .function("lastNode", &ogdf::Graph::lastNode, allow_raw_pointers())
    .function("firstEdge", &ogdf::Graph::firstEdge, allow_raw_pointers())
    .function("lastEdge", &ogdf::Graph::lastEdge, allow_raw_pointers())
    .function("chooseNode", &ogdf::Graph::chooseNode, allow_raw_pointers())
    .function("chooseEdge", &ogdf::Graph::chooseEdge, allow_raw_pointers())
    .function("empty", &ogdf::Graph::empty)
    .function("newNode", select_overload<ogdf::node()>(&ogdf::Graph::newNode), allow_raw_pointers())
    .function("newNode", select_overload<ogdf::node(int)>(&ogdf::Graph::newNode), allow_raw_pointers())
    .function("newEdge", select_overload<ogdf::edge(ogdf::node, ogdf::node)>(&ogdf::Graph::newEdge), allow_raw_pointers())
    .function("allNodes", &ogdf::Graph::allNodes<NodeList>)
    ;

  class_<NodeList>("NodeList")
    .constructor()
    .function("at", &nodeListAt, allow_raw_pointers())
    .function("size", &NodeList::size)
    ;

  class_<ogdf::NodeElement>("NodeElement")
    .function("degree", &ogdf::NodeElement::degree)
    .function("index", &ogdf::NodeElement::index)
#ifdef OGDB_DEBUG
    .function("graphOf", &ogdf::NodeElement::graphOf, allow_raw_pointers())
#endif
    ;

  class_<ogdf::EdgeElement>("EdgeElement")
    ;
}

void defineGraphAttributes () {
  class_<ogdf::GraphAttributes>("GraphAttributes")
    .constructor()
    .constructor<ogdf::Graph&, long>()
    .function("x", select_overload<double (ogdf::node) const>(&ogdf::GraphAttributes::x), allow_raw_pointers())
    .function("y", select_overload<double (ogdf::node) const>(&ogdf::GraphAttributes::y), allow_raw_pointers())
    .function("z", select_overload<double (ogdf::node) const>(&ogdf::GraphAttributes::z), allow_raw_pointers())
    .function("width", select_overload<double (ogdf::node) const>(&ogdf::GraphAttributes::width), allow_raw_pointers())
    .function("height", select_overload<double (ogdf::node) const>(&ogdf::GraphAttributes::height), allow_raw_pointers())
    .function("shape", select_overload<ogdf::Shape (ogdf::node) const>(&ogdf::GraphAttributes::shape), allow_raw_pointers())
    .function("strokeType", select_overload<ogdf::StrokeType (ogdf::node) const>(&ogdf::GraphAttributes::strokeType), allow_raw_pointers())
    .function("strokeColor", select_overload<const ogdf::Color& (ogdf::node) const>(&ogdf::GraphAttributes::strokeColor), allow_raw_pointers())
    .function("strokeWidth", select_overload<float (ogdf::node) const>(&ogdf::GraphAttributes::strokeWidth), allow_raw_pointers())
    .function("fillPattern", select_overload<ogdf::FillPattern (ogdf::node) const>(&ogdf::GraphAttributes::fillPattern), allow_raw_pointers())
    .function("fillColor", select_overload<const ogdf::Color& (ogdf::node) const>(&ogdf::GraphAttributes::fillColor), allow_raw_pointers())
    .function("fillBgColor", select_overload<const ogdf::Color& (ogdf::node) const>(&ogdf::GraphAttributes::fillBgColor), allow_raw_pointers())
    .function("label", select_overload<const std::string& (ogdf::node) const>(&ogdf::GraphAttributes::label), allow_raw_pointers())
    .function("templateNode", select_overload<const std::string& (ogdf::node) const>(&ogdf::GraphAttributes::templateNode), allow_raw_pointers())
    .function("weight", select_overload<int (ogdf::node) const>(&ogdf::GraphAttributes::weight), allow_raw_pointers())
    .function("type", select_overload<ogdf::Graph::NodeType (ogdf::node) const>(&ogdf::GraphAttributes::type), allow_raw_pointers())
    .function("idNode", select_overload<int (ogdf::node) const>(&ogdf::GraphAttributes::idNode), allow_raw_pointers())
    .function("setX", &setX, allow_raw_pointers())
    .function("setY", &setY, allow_raw_pointers())
    .function("setZ", &setZ, allow_raw_pointers())
    .function("setWidth", &setWidth, allow_raw_pointers())
    .function("setHeight", &setHeight, allow_raw_pointers())
    .function("setShape", &setShape, allow_raw_pointers())
    // .function("setStrokeType", &setStrokeType, allow_raw_pointers())
    .function("setStrokeColor", &setStrokeColor, allow_raw_pointers())
    .function("setStrokeWidth", &setStrokeWidth, allow_raw_pointers())
    // .function("setFillPattern", &setFillPattern, allow_raw_pointers())
    .function("setFillColor", &setFillColor, allow_raw_pointers())
    .function("setFillBgColor", &setFillBgColor, allow_raw_pointers())
    .function("setLabel", &setLabel, allow_raw_pointers())
    .function("setTemplateNode", &setTemplateNode, allow_raw_pointers())
    .function("setWeight", select_overload<int (ogdf::node) const>(&ogdf::GraphAttributes::weight), allow_raw_pointers())
    .function("setType", select_overload<ogdf::Graph::NodeType (ogdf::node) const>(&ogdf::GraphAttributes::type), allow_raw_pointers())
    .function("setIdNode", select_overload<int (ogdf::node) const>(&ogdf::GraphAttributes::idNode), allow_raw_pointers())
    ;

  // enum_<Attributes>("Attributes")
  //   .value("nodeGraphics" ,nodeGraphics)
  //   .value("edgeGraphics" ,edgeGraphics)
  //   .value("edgeIntWeight" ,edgeIntWeight)
  //   .value("edgeDoubleWeight" ,edgeDoubleWeight)
  //   .value("edgeLabel" ,edgeLabel)
  //   .value("nodeLabel" ,nodeLabel)
  //   .value("edgeType" ,edgeType)
  //   .value("nodeType" ,nodeType)
  //   .value("nodeId" ,nodeId)
  //   .value("edgeArrow" ,edgeArrow)
  //   .value("edgeStyle" ,edgeStyle)
  //   .value("nodeStyle" ,nodeStyle)
  //   .value("nodeTemplate" ,nodeTemplate)
  //   .value("edgeSubGraphs" ,edgeSubGraphs)
  //   .value("nodeWeight" ,nodeWeight)
  //   .value("threeD" ,threeD)
  //   ;

  // enum_<ogdf::Shape>("Shape")
  //   .value("shRect", ogdf::Shape::shRect)
  //   .value("shRoundedRect", ogdf::Shape::shRoundedRect)
  //   .value("shEllipse", ogdf::Shape::shEllipse)
  //   .value("shTriangle", ogdf::Shape::shTriangle)
  //   .value("shPentagon", ogdf::Shape::shPentagon)
  //   .value("shHexagon", ogdf::Shape::shHexagon)
  //   .value("shOctagon", ogdf::Shape::shOctagon)
  //   .value("shRhomb", ogdf::Shape::shRhomb)
  //   .value("shTrapeze", ogdf::Shape::shTrapeze)
  //   .value("shParallelogram", ogdf::Shape::shParallelogram)
  //   .value("shInvTriangle", ogdf::Shape::shInvTriangle)
  //   .value("shInvTrapeze", ogdf::Shape::shInvTrapeze)
  //   .value("shInvParallelogram", ogdf::Shape::shInvParallelogram)
  //   .value("shImage", ogdf::Shape::shImage)
  //   ;

  // enum_<ogdf::StrokeType>("StrokeType")
  //   .value("stNone", ogdf::StrokeType::stNone)
  //   .value("stSolid", ogdf::StrokeType::stSolid)
  //   .value("stDash", ogdf::StrokeType::stDash)
  //   .value("stDot", ogdf::StrokeType::stDot)
  //   .value("stDashdot", ogdf::StrokeType::stDashdot)
  //   .value("stDashdotdot", ogdf::StrokeType::stDashdotdot)
  //   ;

  // enum_<ogdf::FillPattern>("FillPattern")
  //   .value("fpNone", ogdf::FillPattern::fpNone)
  //   .value("fpSolid", ogdf::FillPattern::fpSolid)
  //   .value("fpDense1", ogdf::FillPattern::fpDense1)
  //   .value("fpDense2", ogdf::FillPattern::fpDense2)
  //   .value("fpDense3", ogdf::FillPattern::fpDense3)
  //   .value("fpDense4", ogdf::FillPattern::fpDense4)
  //   .value("fpDense5", ogdf::FillPattern::fpDense5)
  //   .value("fpDense6", ogdf::FillPattern::fpDense6)
  //   .value("fpDense7", ogdf::FillPattern::fpDense7)
  //   .value("fpHorizontal", ogdf::FillPattern::fpHorizontal)
  //   .value("fpVertical", ogdf::FillPattern::fpVertical)
  //   .value("fpCross", ogdf::FillPattern::fpCross)
  //   .value("fpBackwardDiagonal", ogdf::FillPattern::fpBackwardDiagonal)
  //   .value("fpForwardDiagonal", ogdf::FillPattern::fpForwardDiagonal)
  //   .value("fpDiagonalCross", ogdf::FillPattern::fpDiagonalCross)
  //   ;

  enum_<ogdf::Graph::NodeType>("NodeType")
    .value("vertex", ogdf::Graph::NodeType::vertex)
    .value("dummy", ogdf::Graph::NodeType::dummy)
    .value("generalizationMerger", ogdf::Graph::NodeType::generalizationMerger)
    .value("generalizationExpander", ogdf::Graph::NodeType::generalizationExpander)
    .value("highDegreeExpander", ogdf::Graph::NodeType::highDegreeExpander)
    .value("lowDegreeExpander", ogdf::Graph::NodeType::lowDegreeExpander)
    .value("associationClass", ogdf::Graph::NodeType::associationClass)
    ;

  class_<ogdf::Color>("Color")
    .constructor()
    .constructor<unsigned char, unsigned char, unsigned char, unsigned char>()
    .function("red", select_overload<unsigned char () const>(&ogdf::Color::red))
    .function("green", select_overload<unsigned char () const>(&ogdf::Color::green))
    .function("blue", select_overload<unsigned char () const>(&ogdf::Color::blue))
    .function("alpha", select_overload<unsigned char () const>(&ogdf::Color::alpha))
    .function("setRed", select_overload<void (unsigned char)>(&ogdf::Color::red))
    .function("setGreen", select_overload<void (unsigned char)>(&ogdf::Color::green))
    .function("setBlue", select_overload<void (unsigned char)>(&ogdf::Color::blue))
    .function("setAlpha", select_overload<void (unsigned char)>(&ogdf::Color::alpha))
    .function("fromString", &ogdf::Color::fromString)
    .function("toString", &ogdf::Color::toString)
    ;
}

void defineGraphGenerators () {
  function("completeBipartiteGraph", &ogdf::completeBipartiteGraph);
  function("completeGraph", &ogdf::completeGraph);
  function("cubeGraph", &ogdf::cubeGraph);
  function("gridGraph", &ogdf::gridGraph);
  function("petersenGraph", &ogdf::petersenGraph);
  function("planarBiconnectedDiGraph", &ogdf::planarBiconnectedDiGraph);
  function("planarBiconnectedGraph", &ogdf::planarBiconnectedGraph);
  function("planarCNBGraph", &ogdf::planarCNBGraph);
  function("planarConnectedGraph", &ogdf::planarConnectedGraph);
  function("planarTriconnectedGraph", select_overload<void (ogdf::Graph&, int, int)>(&ogdf::planarTriconnectedGraph));
  function("randomBiconnectedGraph", &ogdf::randomBiconnectedGraph);
  function("randomDiGraph", &ogdf::randomDiGraph);
  function("randomGraph", &ogdf::randomGraph);
  function("randomHierarchy", &ogdf::randomHierarchy);
  function("randomSeriesParallelDAG", &ogdf::randomSeriesParallelDAG);
  function("randomSimpleGraph", &ogdf::randomSimpleGraph);
  function("randomTree", select_overload<void (ogdf::Graph&, int)>(&ogdf::randomTree));
  function("randomTriconnectedGraph", &ogdf::randomTriconnectedGraph);
  function("regularTree", &ogdf::regularTree);
  function("suspension", &ogdf::suspension);
  function("upwardPlanarBiconnectedDiGraph", &ogdf::upwardPlanarBiconnectedDiGraph);
  function("wheelGraph", &ogdf::wheelGraph);
}

void defineBasic () {
  defineGraph();
  defineGraphAttributes();
  defineGraphGenerators();
}
