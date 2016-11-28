/** \file
 * \brief Interface for heap implementations.
 *
 * \author Tilo Wiedera
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

#include <stdexcept>

#include <ogdf/basic/basic.h>

namespace ogdf {

/**
 * Common interface for all heap classes.
 *
 * @tparam IMPL The type of heap.
 * @tparam H The type of handle to use.
 *         Such a handle will be given to the user for each pushed value.
 * @tparam T The type of values to be stored.
 * @tparam C The comparator used to order the stored values.
 */
template<
  template<typename T, typename C> class IMPL,
  typename H,
  typename T,
  typename C
>
class HeapBase
{
protected:
	C m_comp;
public:

	/**
	* The type of handle used to identify stored values.
	* The handle type accessible from outside of the heap will always be a pointer.
	*/
	typedef H* Handle;

	HeapBase() {}

	/**
	 * Returns the comparator used to sort the values in the heap.
	 *
	 * @return The comparator for sorting the heaps values
	 */
	virtual const C &comparator() {
		return m_comp;
	}

	/**
	 * Retuns the topmost value in the heap.
	 *
	 * @return the topmost value
	 */
	virtual const T &top() const = 0;

	/**
	 * Inserts a value into the heap.
	 *
	 * @param value The value to be inserted
	 * @return A handle to access and modify the value
	 */
	virtual Handle push(const T &value) = 0;

	/**
	 * Removes the topmost value from the heap.
	 */
	virtual void pop() = 0;

	/**
	 * Decreases a single value.
	 *
	 * @param handle The handle of the value to be decreased
	 * @param value The decreased value. This must be less than the former value
	 */
	virtual void decrease(Handle handle, const T &value) = 0;

	/**
	 * Returns the value of that handle.
	 *
	 * @param handle The handle
	 * @return The value
	 */
	virtual const T &value(const Handle handle) const = 0;

	/**
	 * Merges in values of \a other heap.
	 *
	 * After merge \a other heap becomes empty and is valid for further usage.
	 *
	 * @param other A heap to be merged in.
	 */
	virtual void merge(IMPL<T, C> &other) {
		throw std::runtime_error("Merging two binary heaps is not supported");
	}
};


} // end namespace ogdf
