/** \file
 * \brief Definition and implementation of a full component generator
 *   based on the Dreyfus-Wagner algorithm
 *
 * \author Stephan Beyer
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

#include <ogdf/basic/Hashing.h>
#include <ogdf/basic/SubsetEnumerator.h>
#include <ogdf/basic/IndexComparer.h>
#include <ogdf/internal/steinertree/EdgeWeightedGraphCopy.h>

namespace ogdf {
namespace steinertree {

//! Sort terminals by index
static void sortTerminals(List<node> &terminals)
{
	IndexComparer<node> idx;
	terminals.quicksort(idx);
}

//! A generator for full components (for Steiner tree approximations) based on the Dreyfus-Wagner algorithm
template<typename T>
class DreyfusWagnerFullComponentGenerator
{
	const EdgeWeightedGraph<T> &m_G; //!< A reference to the graph instance
	const List<node> &m_terminals; //!< A reference to the index-sorted list of terminals
	const NodeArray<NodeArray<T>> &m_distance; //!< A reference to the full distance matrix

	using NodePair = std::pair<node,node>;
	using NodePairs = Array<NodePair>;
	struct DWMData {
		T cost;
		NodePairs nodepairs;
		DWMData(T _cost, NodePairs _nodepairs)
		 : cost(_cost)
		 , nodepairs(_nodepairs)
		{
		}
		DWMData()
		 : cost(numeric_limits<T>::max())
		 , nodepairs()
		{
		}
	};

	class SortedNodeListHashFunc;
	Hashing<List<node>, DWMData, SortedNodeListHashFunc> m_map; //!< A hash array for keys of size > 2

	const DWMData dataOf(const List<node> &key) const
	{
		OGDF_ASSERT(key.size() > 1);
		if (key.size() == 2) {
			NodePairs nodepairs(0, 0, NodePair(key.front(), key.back()));
			return DWMData(
			  m_distance[key.front()][key.back()],
			  nodepairs);
		}
		return m_map.lookup(key)->info(); // copy
	}

	T costOf(const List<node> &key) const
	{
		OGDF_ASSERT(key.size() > 1);
		if (key.size() == 2) {
			return m_distance[key.front()][key.back()];
		}
		OGDF_ASSERT(m_map.member(key));
		return m_map.lookup(key)->info().cost;
	}

	void insertSorted(List<node> &list, node v) const
	{
		for (ListIterator<node> it = list.begin(); it.valid(); ++it) {
			OGDF_ASSERT((*it)->index() != v->index());
			if (v->index() < (*it)->index()) {
				list.insertBefore(v, it);
				return;
			}
		}
		list.pushBack(v);
	}

public:
	/** The constructor
	 * \pre The list of terminals has to be sorted by index (use sortTerminals)
	 */
	DreyfusWagnerFullComponentGenerator(const EdgeWeightedGraph<T> &G, const List<node> &terminals, const NodeArray<NodeArray<T>> &distance)
	  : m_G(G)
	  , m_terminals(terminals)
	  , m_distance(distance)
	  , m_map(1 << 22) // we initially allocate 4MB*sizeof(DWMData) for hashing
	{
	}

	void call(int restricted)
	{
		SubsetEnumerator<node> terminalSubset(m_terminals);
		for (terminalSubset.begin(2, restricted-1); terminalSubset.valid(); terminalSubset.next()) {
			List<node> terminals;
			terminalSubset.list(terminals);
			SubsetEnumerator<node> subset(terminals);
			// populate "split"
			NodeArray<DWMData> split(m_G);
			auto computeSplit = [&](node v) {
				OGDF_ASSERT(!terminalSubset.hasMember(v));
				DWMData best;
				for (subset.begin(1, terminals.size()-1); subset.valid(); subset.next()) {
					List<node> list1, list2;
					subset.list(list1, list2);
					insertSorted(list1, v);
					insertSorted(list2, v);
					T tmp = costOf(list1) + costOf(list2);
					if (tmp < best.cost) {
						best = dataOf(list1);
						DWMData data = dataOf(list2);
						best.cost += data.cost;
						int bsize = best.nodepairs.size();
						int dsize = data.nodepairs.size();
						best.nodepairs.grow(dsize);
						for (int i = 0; i < dsize; ++i) { // copy
							best.nodepairs[bsize + i] = data.nodepairs[i];
						}
					}
				}
				split[v] = best;
			};
			// compute partial solutions
			auto computePartialSolutions = [&](node v) {
				List<node> newTerminals(terminals);
				insertSorted(newTerminals, v);
				if (m_map.member(newTerminals)) { // already defined
					return; // abort
				}
				DWMData best;
				for (node w : m_G.nodes) {
					T dist = m_distance[v][w];
					if (v == w) {
						dist = 0;
					}
					if (terminalSubset.hasMember(w)) {
						// we attach edge vw to tree containing terminal w
						T tmp;
						tmp = costOf(terminals) + dist;
						if (tmp < best.cost) {
							best = dataOf(terminals);
							best.cost += dist;
							best.nodepairs.grow(1, NodePair(v,w));
						}
					} else {
						// we attach edge vw to tree split[w]
						T tmp;
						if (split[w].nodepairs.size() == 0) {
							computeSplit(w);
						}
						tmp = split[w].cost + dist;
						if (tmp < best.cost) {
							best = split[w];
							if (v != w) {
								best.cost += dist;
								best.nodepairs.grow(1, NodePair(v,w));
							}
						}
					}
				}
				m_map.fastInsert(newTerminals, best);
			};
			if (terminalSubset.size() != restricted - 1) {
				for (node v : m_G.nodes) {
					if (!terminalSubset.hasMember(v)) {
						computePartialSolutions(v);
					}
				}
			} else { // maximal terminal subset
				for (node v : m_terminals) { // save time by only adding terminals instead of all nodes
					if (!terminalSubset.hasMember(v)) {
						computePartialSolutions(v);
					}
				}
			}
		}
	}

	//! Construct a Steiner tree for the given set of terminals
	T getSteinerTreeFor(const List<node> &terminals, EdgeWeightedGraphCopy<T> &tree)
	{
		T cost(0);
		DWMData data = dataOf(terminals);
		tree.createEmpty(m_G);
		for (auto nodepair : data.nodepairs) {
			node uO = nodepair.first;
			node vO = nodepair.second;
			node uC = tree.copy(uO);
			node vC = tree.copy(vO);
			if (uC == nullptr) {
				uC = tree.newNode(uO);
			}
			if (vC == nullptr) {
				vC = tree.newNode(vO);
			}
			const T dist = m_distance[uO][vO];
			tree.newEdge(uC, vC, dist);
			cost += dist;
		}
		OGDF_ASSERT(isTree(tree));
		return cost;
	}
};

template<typename T>
class DreyfusWagnerFullComponentGenerator<T>::SortedNodeListHashFunc
{
	static const int c_prime = 0x7fffffff; // mersenne prime 2**31 - 1
	// would be nicer: 0x1fffffffffffffff; // mersenne prime 2**61 - 1
	const int m_random;

public:
	SortedNodeListHashFunc()
	  : m_random(randomNumber(2, c_prime - 1))
	{
	}

	int hash(const List<node> &key) const
	{
		ListConstIterator<node> it = key.begin();
		int hash = (*it)->index();
		for (; it.valid(); ++it) {
			hash = (hash * m_random + (*it)->index()) % c_prime;
		}
		return hash;
	}
};

} // end namespace steinertree
} // end namespace ogdf
