/** \file
 * \brief Implementation of GraphML string conversion functions.
 *
 * \author Łukasz Hanuszczak
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * \par
 * Copyright (C)<br>
 * See README.md in the OGDF root directory for details.
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation;
 * see the file LICENSE.txt included in the packaging of this file
 * for details.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, see
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <ogdf/fileformats/Utils.h>
#include <ogdf/fileformats/GraphML.h>


namespace ogdf {

namespace graphml {


std::string toString(const Attribute &attr)
{
	switch(attr) {
	// (moved down to default case)
#if 0
	case a_unknown: return "unknown";
#endif

	case Attribute::NodeLabel: return "label";
	case Attribute::EdgeLabel: return "edgelabel";

	case Attribute::X: return "x";
	case Attribute::Y: return "y";
	case Attribute::Z: return "z";
	case Attribute::Width: return "width";
	case Attribute::Height:	return "height";
	case Attribute::Size: return "size";

	case Attribute::Shape: return "shape";

	case Attribute::NodeStroke: return "nodestroke";
	case Attribute::EdgeStroke: return "edgestroke";
	case Attribute::ClusterStroke: return "clusterstroke";
	case Attribute::NodeFill: return "nodefill";
	case Attribute::R: return "r";
	case Attribute::G: return "g";
	case Attribute::B: return "b";

	case Attribute::NodeWeight: return "nodeweight";
	case Attribute::EdgeWeight: return "weight";

	case Attribute::NodeType: return "nodetype";
	case Attribute::EdgeType: return "edgetype";

	case Attribute::Template: return "template";

	case Attribute::EdgeArrow: return "arrow";
	case Attribute::EdgeSubGraph: return "avaliable-for";
	case Attribute::EdgeBends: return "bends";

	// default case (to avoid compiler warnings)
	default: return "unknown";
	}
}


std::string toString(const Shape &shape)
{
	switch(shape) {
		case Shape::Rect:             return "rect";
		case Shape::RoundedRect:      return "rounded-rect";
		case Shape::Ellipse:          return "ellipse";
		case Shape::Triangle:         return "triangle";
		case Shape::Pentagon:         return "pentagon";
		case Shape::Hexagon:          return "hexagon";
		case Shape::Octagon:          return "octagon";
		case Shape::Rhomb:            return "rhomb";
		case Shape::Trapeze:          return "trapeze";
		case Shape::Parallelogram:    return "parallelogram";
		case Shape::InvTriangle:      return "inv-triangle";
		case Shape::InvTrapeze:       return "inv-trapeze";
		case Shape::InvParallelogram: return "inv-parallelogram";
		case Shape::Image:            return "image";
	}
	OGDF_ASSERT(false);
	return "UNKNOWN";
}


std::string toString(const EdgeArrow &arrow)
{
	switch(arrow) {
		case EdgeArrow::None:      return "none";
		case EdgeArrow::Last:      return "last";
		case EdgeArrow::First:     return "first";
		case EdgeArrow::Both:      return "both";
		case EdgeArrow::Undefined: return "undefined";
	}
	OGDF_ASSERT(false);
	return "UNKNOWN";
}


std::string toString(const Graph::NodeType &type)
{
	switch(type) {
	case Graph::NodeType::vertex: return "vertex";
	case Graph::NodeType::dummy: return "dummy";
	case Graph::NodeType::generalizationMerger: return "generalization-merger";
	case Graph::NodeType::generalizationExpander: return "generalization-expander";
	case Graph::NodeType::highDegreeExpander: return "high-degree-expander";
	case Graph::NodeType::lowDegreeExpander: return "low-degree-expander";
	case Graph::NodeType::associationClass: return "association-class";
	default: return "vertex";
	}
}


std::string toString(const Graph::EdgeType &type)
{
	switch(type) {
		case Graph::EdgeType::association:    return "association";
		case Graph::EdgeType::generalization: return "generalization";
		case Graph::EdgeType::dependency:     return "dependency";
	}
	OGDF_ASSERT(false);
	return "UNKNOWN";
}

// Map is lazily-evaluated (this could be avoided with C++11 constexpr).
static std::map<std::string, Attribute> attrMap;

Attribute toAttribute(const std::string &str)
{
	return toEnum(
		str, attrMap, toString,
		static_cast<Attribute>(0), Attribute::Unknown, Attribute::Unknown);
}


// Same as attrMap but with shapes.
static std::map<std::string, Shape> shapeMap;

Shape toShape(const std::string &str)
{
	return toEnum(str, shapeMap, toString, Shape::Rect, Shape::Image, Shape::Rect);
}


// Same as attrMap but with arrows.
static std::map<std::string, EdgeArrow> arrowMap;

EdgeArrow toArrow(const std::string &str)
{
	return toEnum(str, arrowMap, toString, EdgeArrow::None, EdgeArrow::Undefined, EdgeArrow::Undefined);
}


// Same as attrMap but with node types.
static std::map<std::string, Graph::NodeType> nodeTypeMap;

Graph::NodeType toNodeType(const std::string &str)
{
	return toEnum(
		str, nodeTypeMap, toString,
		static_cast<Graph::NodeType>(0), Graph::NodeType::vertex, Graph::NodeType::vertex);
}

// Same as attrMap but with edge types.
static std::map<std::string, Graph::EdgeType> edgeTypeMap;

Graph::EdgeType toEdgeType(const std::string &str)
{
	return toEnum(
		str, edgeTypeMap, toString,
		static_cast<Graph::EdgeType>(0), Graph::EdgeType::dependency, Graph::EdgeType::association);
}


} // end namespace graphml

} // end namespace ogdf