/** \file
 * \brief Declaration of class ELabelPosSimple which represents
 *        a simple labeling algorithm.
 *
 * \author Joachim Kupke
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

#include <ogdf/labeling/ELabelInterface.h>


namespace ogdf {

class OGDF_EXPORT ELabelPosSimple {

public:

	ELabelPosSimple();
	~ELabelPosSimple();

	void call(GraphAttributes& ug, ELabelInterface<double>& eli); //double

	bool   m_absolut;
	double m_marginDistance;
	double m_edgeDistance;

	bool   m_midOnEdge;

private:


};


}