/** \file
 * \brief Computes an embedding of a graph with maximum external face.
 * See paper "Graph Embedding with Minimum Depth and Maximum External
 * Face" by C. Gutwenger and P. Mutzel (2004) for details.
 *
 * \author Thorsten Kerkhof
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

#include <ogdf/planarity/EmbedderMaxFace.h>
#include <ogdf/planarity/embedder/ConnectedSubgraph.h>
#include <ogdf/planarity/embedder/EmbedderMaxFaceBiconnectedGraphs.h>

namespace ogdf {

void EmbedderMaxFace::doCall(Graph& G, adjEntry& adjExternal)
{
	adjExternal = nullptr;
	pAdjExternal = &adjExternal;

	//simple base cases:
	if (G.numberOfNodes() <= 1)
		return;
	if (G.numberOfEdges() == 1)
	{
		edge e = G.firstEdge();
		adjExternal = e->adjSource();
		return;
	}

	//HINT: Edges are directed from child to parent in BC-trees
	pBCTree = new BCTree(G);

	//base case of biconnected graph:
	if (pBCTree->bcTree().numberOfNodes() == 1)
	{
		NodeArray<int> m_nodeLength(G, 0);
		EdgeArray<int> m_edgeLength(G, 1);
		adjEntry m_adjExternal;
		EmbedderMaxFaceBiconnectedGraphs<int>::embed(G, m_adjExternal, m_nodeLength, m_edgeLength);
		adjExternal = m_adjExternal->twin();

		delete pBCTree;
		return;
	}

	//First step: calculate maximum face and node lengths

	//Find root Block (root node is only node with out-degree of 0):
	node rootBlockNode = nullptr;
	for(node n : pBCTree->bcTree().nodes)
	{
		if (n->outdeg() == 0) {
			rootBlockNode = n;
			break;
		}
	}
	OGDF_ASSERT(rootBlockNode != nullptr);

	//compute block graphs and SPQR trees:
	blockG.init(pBCTree->bcTree());
	nBlockEmbedding_to_nH.init(pBCTree->bcTree());
	eBlockEmbedding_to_eH.init(pBCTree->bcTree());
	nH_to_nBlockEmbedding.init(pBCTree->bcTree());
	eH_to_eBlockEmbedding.init(pBCTree->bcTree());
	nodeLength.init(pBCTree->bcTree());
	cstrLength.init(pBCTree->bcTree());
	spqrTrees.init(pBCTree->bcTree(),nullptr);
	computeBlockGraphs(rootBlockNode, nullptr);

	//Bottom-Up-Traversal:
	for(adjEntry adj : rootBlockNode->adjEntries) {
		edge e = adj->theEdge();
		node cT = e->source();
		node cH = pBCTree->cutVertex(cT, rootBlockNode);
		node cB = nH_to_nBlockEmbedding[rootBlockNode][cH];

		//set length of v in block graph of root block node:
		int length_v_in_rootBlock = 0;
		for(adjEntry adjCT : cT->adjEntries) {
			edge e2 = adjCT->theEdge();
			//check if edge is an incoming edge:
			if (e2->target() != cT)
				continue;

			node blockNode = e2->source();
			node cutVertex = pBCTree->cutVertex(cT, blockNode);
			length_v_in_rootBlock += constraintMaxFace(blockNode, cutVertex);
		}
		nodeLength[rootBlockNode][cB] = length_v_in_rootBlock;
	}

	node bT_opt = G.chooseNode(); //= G.chooseNode() only to get rid of warning
	int ell_opt = 0;
	maximumFaceRec(rootBlockNode, bT_opt, ell_opt);


	//Second step: Embed G by expanding a maximum face in bT_opt
	newOrder.init(G);
	treeNodeTreated.init(pBCTree->bcTree(), false);
	embedBlock(bT_opt);

	for(node v : G.nodes)
		G.sort(v, newOrder[v]);

	for(node v : pBCTree->bcTree().nodes)
		delete spqrTrees[v];

	delete pBCTree;
}


void EmbedderMaxFace::computeBlockGraphs(const node& bT, const node& cH)
{
	//recursion:
	for(adjEntry adj : bT->adjEntries) {
		edge e = adj->theEdge();
		if (e->source() == bT)
			continue;

		node cT = e->source();
		for(adjEntry adjCT : cT->adjEntries) {
			edge e2 = adjCT->theEdge();
			if (e2->source() == cT)
				continue;
			node cH2 = pBCTree->cutVertex(cT, e2->source());
			computeBlockGraphs(e2->source(), cH2);
		}
	}

	//embed block bT:
	node m_cH = cH;
	if (m_cH == nullptr)
		m_cH = pBCTree->cutVertex(bT->firstAdj()->twinNode(), bT);
	embedder::ConnectedSubgraph<int>::call(pBCTree->auxiliaryGraph(), blockG[bT], m_cH,
		nBlockEmbedding_to_nH[bT], eBlockEmbedding_to_eH[bT],
		nH_to_nBlockEmbedding[bT], eH_to_eBlockEmbedding[bT]);
	nodeLength[bT].init(blockG[bT], 0);
	cstrLength[bT].init(blockG[bT], 0);
	if (   !blockG[bT].empty()
		&& blockG[bT].numberOfNodes() != 1
		&& blockG[bT].numberOfEdges() > 2)
	{
		spqrTrees[bT] = new StaticSPQRTree(blockG[bT]);
	}
}


int EmbedderMaxFace::constraintMaxFace(const node& bT, const node& cH)
{
	//forall (v \in B, v \neq c) do:
	//  length_B(v) := \sum_{(v, B') \in B} ConstraintMaxFace(B', v);
	for(adjEntry adj : bT->adjEntries) {
		edge e = adj->theEdge();
		if (e->target() != bT)
			continue;
		node vT = e->source();
		node vH = pBCTree->cutVertex(vT, bT);

		//set length of vertex v in block graph of bT:
		int length_v_in_block = 0;
		for(adjEntry adjVT : vT->adjEntries) {
			edge e2 = adjVT->theEdge();
			//check if edge is an incoming edge:
			if (e2->target() != vT)
				continue;

			node bT2 = e2->source();
			node cutVertex = pBCTree->cutVertex(vT, bT2);
			length_v_in_block += constraintMaxFace(bT2, cutVertex);
		}
		nodeLength[bT][nH_to_nBlockEmbedding[bT][vH]] = length_v_in_block;
	}

	EdgeArray<int> edgeLength(blockG[bT], 1);
	int cstrLengthBc = EmbedderMaxFaceBiconnectedGraphs<int>::computeSize(
		blockG[bT],
		nH_to_nBlockEmbedding[bT][cH],
		nodeLength[bT],
		edgeLength,
		spqrTrees[bT]);
	cstrLength[bT][nH_to_nBlockEmbedding[bT][cH]] = cstrLengthBc;
	return cstrLengthBc;
}


void EmbedderMaxFace::maximumFaceRec(const node& bT, node& bT_opt, int& ell_opt)
{
	//(B*, \ell*) := (B, size of a maximum face in B):
	node m_bT_opt = bT;
	EdgeArray<int> edgeLengthForEllOpt(blockG[bT], 1);
	NodeArray< EdgeArray<int> > edgeLengthSkel;
	int m_ell_opt = EmbedderMaxFaceBiconnectedGraphs<int>::computeSize(
		blockG[bT], nodeLength[bT], edgeLengthForEllOpt, spqrTrees[bT], edgeLengthSkel);

	for(adjEntry adj : bT->adjEntries) {
		edge e = adj->theEdge();
		if (e->target() != bT)
			continue;
		node cT = e->source();
		node cH = pBCTree->cutVertex(cT, bT);

		EdgeArray<int> edgeLength(blockG[bT], 1);
		cstrLength[bT][nH_to_nBlockEmbedding[bT][cH]]
			= EmbedderMaxFaceBiconnectedGraphs<int>::computeSize(blockG[bT],
				nH_to_nBlockEmbedding[bT][cH],
				nodeLength[bT],
				edgeLength,
				spqrTrees[bT],
				edgeLengthSkel);

		//L := \sum_{(B', c) \in bcTree} cstrLength(B', c)
		int L = 0;
		for(adjEntry adjCT : cT->adjEntries) {
			edge e2 = adjCT->theEdge();
			//check if edge is an incoming edge:
			if (e2->source() != cT)
				continue;

			//get partner vertex of c in the block graph of B'=e->target() and add
			//cstrLength(B', c) to L:
			node bT2 = e2->target();
			L += cstrLength[bT2][nH_to_nBlockEmbedding[bT2][pBCTree->cutVertex(cT, bT2)]];
		}

		for(adjEntry adjCT : cT->adjEntries) {
			edge e2 = adjCT->theEdge();
			//check if edge is an outgoing edge or the edge from bT to cT:
			if (e2->target() != cT || e2->source() == bT)
				continue;

			//get partner vertex of c in the block graph of B'=e->source():
			node pT = e2->source();
			node partnerV = pBCTree->cutVertex(cT, pT);
			node pB = nH_to_nBlockEmbedding[pT][partnerV];
			nodeLength[pT][pB] = L - cstrLength[pT][pB];

			//pBCTree->originalGraph().chooseNode() just to get rid of warning:
			node thisbT_opt = pBCTree->originalGraph().chooseNode();
			int thisell_opt = 0;
			maximumFaceRec(pT, thisbT_opt, thisell_opt);
			if (thisell_opt > m_ell_opt)
			{
				m_bT_opt = thisbT_opt;
				m_ell_opt = thisell_opt;
			}
		}
	}

	//return (B*, \ell*):
	bT_opt = m_bT_opt;
	ell_opt = m_ell_opt;
}


void EmbedderMaxFace::embedBlock(const node& bT)
{
	ListIterator<adjEntry> after;
	node cT = nullptr;
	embedBlock(bT, cT, after);
}


void EmbedderMaxFace::embedBlock(
	const node& bT,
	const node& cT,
	ListIterator<adjEntry>& after)
{
	treeNodeTreated[bT] = true;
	node cH = nullptr;
	if (cT != nullptr)
		cH = pBCTree->cutVertex(cT, bT);

	// 1. Compute embedding of block
	EdgeArray<int> edgeLength(blockG[bT], 1);
	adjEntry m_adjExternal = nullptr;
	if (cH == nullptr)
		EmbedderMaxFaceBiconnectedGraphs<int>::embed(blockG[bT], m_adjExternal,
			nodeLength[bT], edgeLength);
	else
		EmbedderMaxFaceBiconnectedGraphs<int>::embed(blockG[bT], m_adjExternal,
			nodeLength[bT], edgeLength, nH_to_nBlockEmbedding[bT][cH]);

	// 2. Copy block embedding into graph embedding and call recursively
	//    embedBlock for all cut vertices in bT
	CombinatorialEmbedding CE(blockG[bT]);
	face f = CE.leftFace(m_adjExternal);

	if (*pAdjExternal == nullptr)
	{
		node on = pBCTree->original(nBlockEmbedding_to_nH[bT][m_adjExternal->theNode()]);
		adjEntry ae1 = on->firstAdj();
		for (adjEntry ae = ae1; ae; ae = ae->succ())
		{
			if (ae->theEdge() == pBCTree->original(eBlockEmbedding_to_eH[bT][m_adjExternal->theEdge()]))
			{
				*pAdjExternal = ae->twin();
				break;
			}
		}
	}

	for(node nSG : blockG[bT].nodes)
	{
		node nH = nBlockEmbedding_to_nH[bT][nSG];
		node nG = pBCTree->original(nH);
		adjEntry ae = nSG->firstAdj();
		ListIterator<adjEntry>* pAfter;
		if (pBCTree->bcproper(nG) == cT)
			pAfter = &after;
		else
			pAfter = new ListIterator<adjEntry>();

		if (pBCTree->typeOfGNode(nG) == BCTree::GNodeType::CutVertex)
		{
			node cT2 = pBCTree->bcproper(nG);
			bool no_recursion = false;
			if (cT2 == cT)
			{
				node parent_bT_of_cT2 = nullptr;
				for(adjEntry adj : cT2->adjEntries) {
					edge e_cT2_to_bT2 = adj->theEdge();
					if (e_cT2_to_bT2->source() == cT2)
					{
						parent_bT_of_cT2 = e_cT2_to_bT2->target();
						break;
					}
				}
				OGDF_ASSERT(parent_bT_of_cT2 != nullptr);
				if (treeNodeTreated[parent_bT_of_cT2])
					no_recursion = true;
			}

			if (no_recursion)
			{
				//find adjacency entry of nSG which lies on external face f:
				adjEntry aeFace = f->firstAdj();
				do
				{
					if (aeFace->theNode() == nSG)
					{
						if (aeFace->succ())
							ae = aeFace->succ();
						else
							ae = nSG->firstAdj();
						break;
					}
					aeFace = aeFace->faceCycleSucc();
				} while(aeFace != f->firstAdj());
			}
			else //!no_recursion
			{
				//(if exists) find adjacency entry of nSG which lies on external face f:
				adjEntry aeFace = f->firstAdj();
				do
				{
					if (aeFace->theNode() == nSG)
					{
						if (aeFace->succ())
							ae = aeFace->succ();
						else
							ae = nSG->firstAdj();
						break;
					}
					aeFace = aeFace->faceCycleSucc();
				} while(aeFace != f->firstAdj());

				for(adjEntry adj : cT2->adjEntries) {
					edge e_cT2_to_bT2 = adj->theEdge();
					node bT2;
					if (e_cT2_to_bT2->source() == cT2)
						bT2 = e_cT2_to_bT2->target();
					else
						bT2 = e_cT2_to_bT2->source();
					if (!treeNodeTreated[bT2])
						embedBlock(bT2, cT2, *pAfter);
				}
			}
		}

		//embed all edges of block bT:
		bool after_ae = true;
		for (adjEntry aeNode = ae;
			after_ae || aeNode != ae;
			after_ae = after_ae && aeNode->succ(),
			aeNode = aeNode->succ() ? aeNode->succ() : nSG->firstAdj())
		{
			edge eG = pBCTree->original(eBlockEmbedding_to_eH[bT][aeNode->theEdge()]);
			if (nG == eG->source())
			{
				if (!pAfter->valid())
					*pAfter = newOrder[nG].pushBack(eG->adjSource());
				else
					*pAfter = newOrder[nG].insertAfter(eG->adjSource(), *pAfter);
			}
			else //!(nG == eG->source())
			{
				if (!pAfter->valid())
					*pAfter = newOrder[nG].pushBack(eG->adjTarget());
				else
					*pAfter = newOrder[nG].insertAfter(eG->adjTarget(), *pAfter);
			}
		} //for (adjEntry aeNode = ae; aeNode; aeNode = aeNode->succ())

		if (!(*pAfter == after))
			delete pAfter;
	}
}

} // end namespace ogdf
