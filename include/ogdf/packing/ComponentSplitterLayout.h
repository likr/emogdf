/** \file
 * \brief Splits and packs the components of a Graph.
 *
 * \author Gereon Bartel
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * \par
 * Copyright (C)<br>
 * See README.txt in the root directory of the OGDF installation for details.
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
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * \see  http://www.gnu.org/copyleft/gpl.html
 ***************************************************************/

#pragma once

#include <ogdf/basic/ModuleOption.h>
#include <ogdf/internal/energybased/MultilevelGraph.h>
#include <ogdf/module/CCLayoutPackModule.h>
#include <ogdf/module/LayoutModule.h>
#include <ogdf/basic/geometry.h>
#include <ogdf/basic/GraphAttributes.h>
#include <vector>


namespace ogdf {

class OGDF_EXPORT ComponentSplitterLayout : public LayoutModule
{
private:
	ModuleOption<LayoutModule> m_secondaryLayout;
	ModuleOption<CCLayoutPackModule> m_packer;

	double m_targetRatio;
	int m_border;

	//! Combines drawings of connected components to
	//! a single drawing by rotating components and packing
	//! the result (optimizes area of axis-parallel rectangle).
	void reassembleDrawings(GraphAttributes &GA, const Array<List<node> > &nodesInCC);

public:
	ComponentSplitterLayout();

	void call(GraphAttributes &GA);

	void setLayoutModule(LayoutModule *layout) {
		m_secondaryLayout.set(layout);
	}

	void setPacker(CCLayoutPackModule *packer) {
		m_packer.set(packer);
	}
};

} // namespace ogdf
