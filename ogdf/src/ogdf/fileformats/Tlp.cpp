/** \file
 * \brief Implementation of TLP string conversion functions.
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

#include <ogdf/fileformats/Tlp.h>


namespace ogdf {

namespace tlp {


std::string toString(const Attribute &attr)
{
	switch(attr) {
	case Attribute::label: return "viewLabel";
	case Attribute::color: return "viewColor";
	case Attribute::position: return "viewLayout";
	case Attribute::size: return "viewSize";
	case Attribute::shape: return "viewShape";
	default: return "unknown";
	}
}


Attribute toAttribute(const std::string &str)
{
	if(str == "viewLabel") {
		return Attribute::label;
	}
	if(str == "viewColor") {
		return Attribute::color;
	}
	if(str == "viewLayout") {
		return Attribute::position;
	}
	if(str == "viewSize") {
		return Attribute::size;
	}
	if(str == "viewShape") {
		return Attribute::shape;
	}
	return Attribute::unknown;
}


} // end namespace tlp

} // end namespace ogdf
