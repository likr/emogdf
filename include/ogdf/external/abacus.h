/** \file
 * \brief Handles Abacus Dependencies.
 *
 * Include this file whenever you want to use Abacus. It does the
 * rest for you. Just be sure to structure your code using the
 * USE_ABACUS Flag. (See AbacusOptimalCrossingMinimizer for an
 * example).
 *
 * \author Markus Chimani
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

#include <ogdf/basic/basic.h>

#ifdef USE_ABACUS

#include <ogdf/lib/abacus/variable.h>
#include <ogdf/lib/abacus/constraint.h>
#include <ogdf/lib/abacus/master.h>
#include <ogdf/lib/abacus/sub.h>
#include <ogdf/lib/abacus/row.h>
#include <ogdf/lib/abacus/nonduplpool.h>
#include <ogdf/lib/abacus/active.h>
#include <ogdf/lib/abacus/branchrule.h>
#include <ogdf/lib/abacus/conbranchrule.h>

#else // USE_ABACUS

#define THROW_NO_ABACUS_EXCEPTION OGDF_THROW_PARAM(LibraryNotSupportedException, lnscAbacus)


#endif // USE_ABACUS
