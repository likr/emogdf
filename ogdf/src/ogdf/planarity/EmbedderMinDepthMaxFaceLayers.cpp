/** \file
 * \brief Computes an embedding of a graph with minimum depth and
 * maximum external face.
 *
 * See the paper "Graph Embedding with Minimum
 * Depth and Maximum External Face" by C. Gutwenger and P. Mutzel
 * (2004) for details.
 * The algorithm for minimum depth and maximum external face is
 * combined with the algorithm for maximum external layers which
 * defines how to embed blocks into inner faces. See diploma thesis
 * "Algorithmen zur Bestimmung von guten Graph-Einbettungen für
 * orthogonale Zeichnungen" (in german) by Thorsten Kerkhof (2007)
 * for details.
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

#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/planarity/embedder/ConnectedSubgraph.h>
#include <ogdf/planarity/embedder/EmbedderMaxFaceBiconnectedGraphsLayers.h>

namespace ogdf {

void EmbedderMinDepthMaxFaceLayers::doCall(Graph& G, adjEntry& adjExternal)
{
	int maxint = 0xFFFFFF;

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
		EmbedderMaxFaceBiconnectedGraphsLayers<int>::embed(
			G, m_adjExternal, m_nodeLength, m_edgeLength);
		adjExternal = m_adjExternal->twin();

		delete pBCTree;
		return;
	}


	// First step: calculate min depth and node lengths

	//Find root Block (only node with out-degree of 0):
	node rootBlockNode = nullptr;
	for(node n : pBCTree->bcTree().nodes)
	{
		if (n->outdeg() == 0) {
			rootBlockNode = n;
			break;
		}
	}
	OGDF_ASSERT(rootBlockNode != nullptr);

	/* MIN DEPTH                                                                */

	//Node lengths of block graph:
	md_nodeLength.init(pBCTree->auxiliaryGraph(), 0);

	//Edge lengths of BC-tree, values m_{c, B} for all (c, B) \in bcTree:
	md_m_cB.init(pBCTree->bcTree(), 0);

	//Bottom-up traversal: (set m_cB for all {c, B} \in bcTree)
	for(adjEntry adj : rootBlockNode->adjEntries) {
		edge e = adj->theEdge();
		node cT = e->source();
		//node cH = pBCTree->cutVertex(cT, rootBlockNode);

		//set length of c in block graph of root block node:
		for(adjEntry adjCT : cT->adjEntries) {
			edge e2 = adjCT->theEdge();
			if (e2->target() != cT)
				continue;

			node blockNode = e2->source();
			node cutVertex = pBCTree->cutVertex(cT, blockNode);

			//Start recursion:
			md_m_cB[e2] = md_bottomUpTraversal(blockNode, cutVertex);
		}
	}

	//Top-down traversal: (set m_cB for all {B, c} \in bcTree and get min depth
	//for each block)
	md_nodeLength.fill(0);
	md_minDepth.init(pBCTree->bcTree(), maxint);
	md_M_B.init(pBCTree->bcTree());
	md_M2.init(pBCTree->bcTree());
	md_topDownTraversal(rootBlockNode);

	/* MAX FACE                                                                 */

	mf_cstrLength.init(pBCTree->auxiliaryGraph(), 0);
	mf_nodeLength.init(pBCTree->auxiliaryGraph(), 0);
	mf_maxFaceSize.init(pBCTree->bcTree(), 0);

	//Bottom-Up-Traversal:
	for(adjEntry adj : rootBlockNode->adjEntries) {
		edge e = adj->theEdge();
		node cT = e->source();
		node cH = pBCTree->cutVertex(cT, rootBlockNode);

		//set length of v in block graph of root block node:
		int length_v_in_rootBlock = 0;
		for(adjEntry adjCT : cT->adjEntries) {
			edge e2 = adjCT->theEdge();
			//check if edge is an incoming edge:
			if (e2->target() != cT)
				continue;

			node blockNode = e2->source();
			node cutVertex = pBCTree->cutVertex(cT, blockNode);
			length_v_in_rootBlock += mf_constraintMaxFace(blockNode, cutVertex);
		}
		mf_nodeLength[cH] = length_v_in_rootBlock;
	}

	node mf_bT_opt = G.chooseNode(); //= G.chooseNode() only to get rid of warning
	int mf_ell_opt = 0;
	mf_maximumFaceRec(rootBlockNode, mf_bT_opt, mf_ell_opt);

	/* MIN DEPTH + MAX FACE                                                     */

	//compute bT_opt:
	mdmf_edgeLength.init(pBCTree->auxiliaryGraph(), MDMFLengthAttribute(0, 1));
	mdmf_nodeLength.init(pBCTree->auxiliaryGraph(), MDMFLengthAttribute(0, 0));
	int d_opt = maxint;
	int ell_opt = -1;
	node bT_opt;
	for(node bT : pBCTree->bcTree().nodes)
	{
		if (pBCTree->typeOfBNode(bT) != BCTree::BNodeType::BComp)
			continue;
		if (md_minDepth[bT] < d_opt || (md_minDepth[bT] == d_opt && mf_maxFaceSize[bT] > ell_opt))
		{
			d_opt = md_minDepth[bT];
			ell_opt = mf_maxFaceSize[bT];
			bT_opt = bT;
		}
	}

	// Second step: Embed G by expanding a maximum face in bT_opt

	newOrder.init(G);
	treeNodeTreated.init(pBCTree->bcTree(), false);
	//reset md_nodeLength and set them during embedBlock call, because they are
	//calculated for starting embedding with rootBlockNode, which is not
	//guarenteed
	md_nodeLength.fill(0);
	embedBlock(bT_opt);

	for(node n : G.nodes)
		G.sort(n, newOrder[n]);

	delete pBCTree;
}


int EmbedderMinDepthMaxFaceLayers::md_bottomUpTraversal(const node& bT, const node& cH)
{
	int m_B = 0; //max_{c \in B} m_B(c)
	List<node> M_B; //{c \in B | m_B(c) = m_B}

	//Recursion:
	for(adjEntry adj : bT->adjEntries) {
		edge e = adj->theEdge();
		if (e->target() != bT)
			continue;
		node cT = e->source();
		//node c_in_bT = pBCTree->cutVertex(cT, bT);

		//set length of c in block graph of root block node:
		for(adjEntry adjCT : cT->adjEntries) {
			edge e_cT_bT2 = adjCT->theEdge();
			if (e == e_cT_bT2)
				continue;

			node bT2 = e_cT_bT2->source();
			node c_in_bT2 = pBCTree->cutVertex(cT, bT2);
			md_m_cB[e_cT_bT2] = md_bottomUpTraversal(bT2, c_in_bT2);

			//update m_B and M_B:
			if (m_B < md_m_cB[e_cT_bT2])
			{
				node cV_in_bT = pBCTree->cutVertex(cT, bT);
				m_B = md_m_cB[e_cT_bT2];
				M_B.clear();
				M_B.pushBack(cV_in_bT);
			}
			else if (m_B == md_m_cB[e_cT_bT2] && !M_B.search(pBCTree->cutVertex(cT, bT)).valid())
			{
				node cV_in_bT = pBCTree->cutVertex(cT, bT);
				M_B.pushBack(cV_in_bT);
			}
		}
	}

	//set vertex length for all vertices in bH to 1 if vertex is in M_B:
	for (ListIterator<node> iterator = M_B.begin(); iterator.valid(); ++iterator)
		md_nodeLength[*iterator] = 1;

	//generate block graph of bT:
	Graph blockGraph_bT;
	node cInBlockGraph_bT;
	NodeArray<int> nodeLengthSG(blockGraph_bT);
	embedder::ConnectedSubgraph<int>::call(pBCTree->auxiliaryGraph(), blockGraph_bT, cH,
		cInBlockGraph_bT, md_nodeLength, nodeLengthSG);

	//leafs of BC-tree:
	if (M_B.size() == 0)
		return 1;

	//set edge length for all edges in block graph to 0:
	EdgeArray<int> edgeLength(blockGraph_bT, 0);

	//compute maximum external face of block graph and get its size:
	int cstrLength_B_c = EmbedderMaxFaceBiconnectedGraphsLayers<int>::computeSize(
		blockGraph_bT,
		cInBlockGraph_bT,
		nodeLengthSG,
		edgeLength);

	if (cstrLength_B_c == M_B.size())
		return m_B;
	//else:
	return m_B + 2;
}


void EmbedderMinDepthMaxFaceLayers::md_topDownTraversal(const node& bT)
{
	//m_B(c) = max {0} \cup {m_{c, B'} | c \in B', B' \neq B}
	int m_B = 0; //max_{c \in B} m_B(c)

	//Compute m_B and M_B:
	node cT_parent = nullptr;
	for(adjEntry adj : bT->adjEntries) {
		edge e_bT_cT = adj->theEdge();
		if (e_bT_cT->source() == bT)
			cT_parent = e_bT_cT->target();
		node cT = (e_bT_cT->source() == bT) ? e_bT_cT->target() : e_bT_cT->source();
		for(adjEntry adjCT : cT->adjEntries) {
			edge e_cT_bT2 = adjCT->theEdge();
			if (e_cT_bT2 == e_bT_cT)
				continue;

			//update m_B and M_B:
			if (m_B < md_m_cB[e_cT_bT2])
			{
				m_B = md_m_cB[e_cT_bT2];
				md_M_B[bT].clear();
				md_M_B[bT].pushBack(pBCTree->cutVertex(cT, bT));
			}
			else if (m_B == md_m_cB[e_cT_bT2] && !md_M_B[bT].search(pBCTree->cutVertex(cT, bT)).valid())
			{
				md_M_B[bT].pushBack(pBCTree->cutVertex(cT, bT));
			}
		}
	}
	//set vertex length for all vertices in bH to 1 if vertex is in M_B:
	NodeArray<int> m_nodeLength(pBCTree->auxiliaryGraph(), 0);
	for (ListIterator<node> iterator = md_M_B[bT].begin(); iterator.valid(); ++iterator) {
		md_nodeLength[*iterator] = 1;
		m_nodeLength[*iterator] = 1;
	}

	//generate block graph of bT:
	Graph blockGraph_bT;
	NodeArray<int> nodeLengthSG(blockGraph_bT);
	NodeArray<node> nG_to_nSG;
	embedder::ConnectedSubgraph<int>::call(pBCTree->auxiliaryGraph(), blockGraph_bT,
	                                       (*(pBCTree->hEdges(bT).begin()))->source(),
	                                       m_nodeLength, nodeLengthSG, nG_to_nSG);

	//set edge length for all edges in block graph to 0:
	EdgeArray<int> edgeLengthBlock(blockGraph_bT, 0);

	//compute size of a maximum external face of block graph:
	StaticSPQRTree* spqrTree = nullptr;
	if (!blockGraph_bT.empty()
		&& blockGraph_bT.numberOfNodes() != 1
		&& blockGraph_bT.numberOfEdges() > 2)
	{
		spqrTree = new StaticSPQRTree(blockGraph_bT);
	}
	NodeArray< EdgeArray<int> > edgeLengthSkel;
	int cstrLength_B_c = EmbedderMaxFaceBiconnectedGraphsLayers<int>::computeSize(
		blockGraph_bT, nodeLengthSG, edgeLengthBlock, spqrTree, edgeLengthSkel);

	//Prepare recursion by setting m_{c, B} for all edges {B, c} \in bcTree:
	if (md_M_B[bT].size() > 0)
	{
		node cT1 = pBCTree->bcproper(pBCTree->original(*(md_M_B[bT].begin())));
		bool calculateNewNodeLengths;
		calculateNewNodeLengths = md_M_B[bT].size() == 1 && cT1 == cT_parent;
		for(adjEntry adj : bT->adjEntries) {
			edge e_bT_cT = adj->theEdge();
			if (e_bT_cT->target() != bT)
				continue;
			node cT = e_bT_cT->source();
			node cH = pBCTree->cutVertex(cT, bT);

			if (md_M_B[bT].size() == 1 && cT1 == cT)
			{
				//Compute new vertex lengths according to
				//m2 = max_{v \in V_B, v != c} m_B(v) and
				//M2 = {c \in V_B \ {v} | m_B(c) = m2}.
				int m2 = 0;

				//Compute m2 and M2:
				for(adjEntry adjBT : bT->adjEntries) {
					edge e_bT_cT2 = adjBT->theEdge();
					node cT2 = (e_bT_cT2->source() == bT) ? e_bT_cT2->target() : e_bT_cT2->source();
					if (cT1 == cT2)
						continue;
					for(adjEntry adjCT2 : cT2->adjEntries) {
						edge e_cT2_bT2 = adjCT2->theEdge();
						if (e_cT2_bT2 == e_bT_cT2)
							continue;

						//update m_B and M_B:
						if (m2 < md_m_cB[e_cT2_bT2])
						{
							m2 = md_m_cB[e_cT2_bT2];
							md_M2[bT].clear();
							md_M2[bT].pushBack(pBCTree->cutVertex(cT2, bT));
						}
						else if (m2 == md_m_cB[e_cT2_bT2] && !md_M2[bT].search(pBCTree->cutVertex(cT2, bT)).valid())
						{
							md_M2[bT].pushBack(pBCTree->cutVertex(cT2, bT));
						}
					}
				}

				//set vertex length for all vertices in bH to 1 if vertex is in M2 and
				//0 otherwise:
				md_nodeLength[*(md_M_B[bT].begin())] = 0;
				for (ListIterator<node> iterator = md_M2[bT].begin(); iterator.valid(); ++iterator)
					md_nodeLength[*iterator] = 1;

				Graph blockGraphBT;
				node cInBlockGraph_bT;
				NodeArray<int> nodeLengthSGBT(blockGraphBT);
				embedder::ConnectedSubgraph<int>::call(pBCTree->auxiliaryGraph(), blockGraphBT, cH,
					cInBlockGraph_bT, md_nodeLength, nodeLengthSGBT);

				//set edge length for all edges in block graph to 0:
				EdgeArray<int> edgeLength(blockGraphBT, 0);

				//compute a maximum external face size of a face containing c in block graph:
				int maxFaceSize = EmbedderMaxFaceBiconnectedGraphsLayers<int>::computeSize(
					blockGraphBT, cInBlockGraph_bT, nodeLengthSGBT, edgeLength);
				if (md_M2[bT].size() == 0)
					md_m_cB[e_bT_cT] = 1;
				else
				{
					if (maxFaceSize == md_M2[bT].size())
						md_m_cB[e_bT_cT] = m2;
					else
						md_m_cB[e_bT_cT] = m2 + 2;
				}

				if (calculateNewNodeLengths)
					calculateNewNodeLengths = false;
				else
				{
					//reset node lengths:
					for (ListIterator<node> iterator = md_M2[bT].begin(); iterator.valid(); ++iterator)
						md_nodeLength[*iterator] = 0;
					md_nodeLength[*(md_M_B[bT].begin())] = 1;
				}
			}
			else //M_B.size() != 1
			{
				//compute a maximum external face size of a face containing c in block graph:
				node cInBlockGraph_bT = nG_to_nSG[cH];
				int maxFaceSize	= EmbedderMaxFaceBiconnectedGraphsLayers<int>::computeSize(
					blockGraph_bT, cInBlockGraph_bT, nodeLengthSG, edgeLengthBlock, spqrTree, edgeLengthSkel);
				if (md_M_B[bT].size() == 0)
					md_m_cB[e_bT_cT] = 1;
				else
				{
					if (maxFaceSize == md_M_B[bT].size())
						md_m_cB[e_bT_cT] = m_B;
					else
						md_m_cB[e_bT_cT] = m_B + 2;
				}
			}
		}

		if (calculateNewNodeLengths)
		{
			//Compute new vertex lengths according to
			//m2 = max_{v \in V_B, v != c} m_B(v) and
			//M2 = {c \in V_B \ {v} | m_B(c) = m2}.
			int m2 = 0;

			//Compute m2 and M2:
			for(adjEntry adj : bT->adjEntries) {
				edge e_bT_cT2 = adj->theEdge();
				node cT2 = (e_bT_cT2->source() == bT) ? e_bT_cT2->target() : e_bT_cT2->source();
				if (cT1 == cT2)
					continue;
				for(adjEntry adjCT2 : cT2->adjEntries) {
					edge e_cT2_bT2 = adjCT2->theEdge();
					if (e_cT2_bT2 == e_bT_cT2)
						continue;

					//update m_B and M_B:
					if (m2 < md_m_cB[e_cT2_bT2])
					{
						m2 = md_m_cB[e_cT2_bT2];
						md_M2[bT].clear();
						md_M2[bT].pushBack(pBCTree->cutVertex(cT2, bT));
					}
					else if (m2 == md_m_cB[e_cT2_bT2] && !md_M2[bT].search(pBCTree->cutVertex(cT2, bT)).valid())
					{
						md_M2[bT].pushBack(pBCTree->cutVertex(cT2, bT));
					}
				}
			}

			//set vertex length for all vertices in bH to 1 if vertex is in M2 and
			//0 otherwise:
			md_nodeLength[*(md_M_B[bT].begin())] = 0;
			for (ListIterator<node> iterator = md_M2[bT].begin(); iterator.valid(); ++iterator)
				md_nodeLength[*iterator] = 1;
		} //if (calculateNewNodeLengths
		else if (md_M_B[bT].size() == 1)
		{
			//Compute M2 = {c \in V_B \ {v} | m_B(c) = m2} with
			//m2 = max_{v \in V_B, v != c} m_B(v).
			int m2 = 0;
			for(adjEntry adj : bT->adjEntries) {
				edge e_bT_cT2 = adj->theEdge();
				node cT2 = (e_bT_cT2->source() == bT) ? e_bT_cT2->target() : e_bT_cT2->source();
				if (cT1 == cT2)
					continue;
				for(adjEntry adjCT2 : cT2->adjEntries) {
					edge e_cT2_bT2 = adjCT2->theEdge();
					if (e_cT2_bT2 == e_bT_cT2)
						continue;

					//update m_B and M_B:
					if (m2 < md_m_cB[e_cT2_bT2])
					{
						m2 = md_m_cB[e_cT2_bT2];
						md_M2[bT].clear();
						md_M2[bT].pushBack(pBCTree->cutVertex(cT2, bT));
					}
					else if (m2 == md_m_cB[e_cT2_bT2] && !md_M2[bT].search(pBCTree->cutVertex(cT2, bT)).valid())
					{
						md_M2[bT].pushBack(pBCTree->cutVertex(cT2, bT));
					}
				}
			}
		}
	}

	//Recursion:
	for(adjEntry adj : bT->adjEntries) {
		edge e_bT_cT = adj->theEdge();
		if (e_bT_cT->target() != bT)
			continue;

		node cT = e_bT_cT->source();
		for(adjEntry adjCT : cT->adjEntries) {
			edge e_cT_bT2 = adjCT->theEdge();
			if (e_cT_bT2 == e_bT_cT)
				continue;

			md_topDownTraversal(e_cT_bT2->source());
		}
	}

	//Compute M_B and M2 for embedBlock-function:
	{
		md_M_B[bT].clear();
		md_M2[bT].clear();
		m_B = 0;
		for(adjEntry adj : bT->adjEntries) {
			edge e_bT_cT = adj->theEdge();
			node cT = (e_bT_cT->source() == bT) ? e_bT_cT->target() : e_bT_cT->source();
			for(adjEntry adjCT : cT->adjEntries) {
				edge e_cT_bT2 = adjCT->theEdge();
				if (e_bT_cT == e_cT_bT2)
					continue;

				//update m_B and M_B:
				if (m_B < md_m_cB[e_cT_bT2])
				{
					m_B = md_m_cB[e_cT_bT2];
					md_M_B[bT].clear();
					md_M_B[bT].pushBack(pBCTree->cutVertex(cT, bT));
				}
				else if (m_B == md_m_cB[e_cT_bT2] && !md_M_B[bT].search(pBCTree->cutVertex(cT, bT)).valid())
				{
					md_M_B[bT].pushBack(pBCTree->cutVertex(cT, bT));
				}
			}
		}

		if (md_M_B[bT].size() == 1)
		{
			int m2 = 0;
			node cT1 = pBCTree->bcproper(pBCTree->original(*(md_M_B[bT].begin())));
			for(adjEntry adj : bT->adjEntries) {
				edge e_bT_cT = adj->theEdge();
				node cT2 = (e_bT_cT->source() == bT) ? e_bT_cT->target() : e_bT_cT->source();
				if (cT1 == cT2)
					continue;
				node cT = (e_bT_cT->source() == bT) ? e_bT_cT->target() : e_bT_cT->source();
				for(adjEntry adjCT : cT->adjEntries) {
					edge e_cT_bT2 = adjCT->theEdge();
					//update m2 and M2:
					if (m2 < md_m_cB[e_cT_bT2])
					{
						m2 = md_m_cB[e_cT_bT2];
						md_M2[bT].clear();
						md_M2[bT].pushBack(pBCTree->cutVertex(cT, bT));
					}
					else if (  m2 == md_m_cB[e_cT_bT2]
									 && !md_M2[bT].search(pBCTree->cutVertex(cT, bT)).valid())
					{
						md_M2[bT].pushBack(pBCTree->cutVertex(cT, bT));
					}
				}
			}
		}
	}

	if (cstrLength_B_c == md_M_B[bT].size())
		md_minDepth[bT] = m_B;
	else
		md_minDepth[bT] = m_B + 2;

	delete spqrTree;
}


int EmbedderMinDepthMaxFaceLayers::mf_constraintMaxFace(const node& bT, const node& cH)
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
			length_v_in_block += mf_constraintMaxFace(bT2, cutVertex);
		}
		mf_nodeLength[vH] = length_v_in_block;
	}

	mf_nodeLength[cH] = 0;
	Graph blockGraph;
	node cInBlockGraph;
	NodeArray<int> nodeLengthSG(blockGraph);
	embedder::ConnectedSubgraph<int>::call(pBCTree->auxiliaryGraph(), blockGraph, cH, cInBlockGraph,
		mf_nodeLength, nodeLengthSG);
	EdgeArray<int> edgeLengthSG(blockGraph, 1);
	int cstrLengthBc = EmbedderMaxFaceBiconnectedGraphsLayers<int>::computeSize(
		blockGraph, cInBlockGraph, nodeLengthSG, edgeLengthSG);
	mf_cstrLength[cH] = cstrLengthBc;
	return cstrLengthBc;
}


void EmbedderMinDepthMaxFaceLayers::mf_maximumFaceRec(const node& bT, node& bT_opt, int& ell_opt)
{
	//(B*, \ell*) := (B, size of a maximum face in B):
	node m_bT_opt = bT;
	Graph blockGraph_bT;
	NodeArray<int> nodeLengthSG(blockGraph_bT);
	NodeArray<node> nG_to_nSG;
	embedder::ConnectedSubgraph<int>::call(
		pBCTree->auxiliaryGraph(), blockGraph_bT,
		(*(pBCTree->hEdges(bT).begin()))->source(),
		mf_nodeLength, nodeLengthSG, nG_to_nSG);
	EdgeArray<int> edgeLengthSG(blockGraph_bT, 1);
	StaticSPQRTree* spqrTree = nullptr;
	if (!blockGraph_bT.empty()
		&& blockGraph_bT.numberOfNodes() != 1
		&& blockGraph_bT.numberOfEdges() > 2)
	{
		spqrTree = new StaticSPQRTree(blockGraph_bT);
	}
	NodeArray< EdgeArray<int> > edgeLengthSkel;
	int m_ell_opt = EmbedderMaxFaceBiconnectedGraphsLayers<int>::computeSize(
		blockGraph_bT, nodeLengthSG, edgeLengthSG, spqrTree, edgeLengthSkel);
	mf_maxFaceSize[bT] = m_ell_opt;

	for(adjEntry adj : bT->adjEntries) {
		edge e = adj->theEdge();
		if (e->target() != bT)
			continue;
		node cT = e->source();
		node cH = pBCTree->cutVertex(cT, bT);

		//cstrLengthBc := size of a maximum face in B containing c:
		node cInBlockGraph_bT = nG_to_nSG[cH];
		mf_cstrLength[cH] = EmbedderMaxFaceBiconnectedGraphsLayers<int>::computeSize(
			blockGraph_bT, cInBlockGraph_bT, nodeLengthSG, edgeLengthSG, spqrTree, edgeLengthSkel);

		//L := \sum_{(B', c) \in bcTree} cstrLength(B', c)
		int L = 0;
		for(adjEntry adjCT : cT->adjEntries) {
			edge e2 = adjCT->theEdge();
			//check if edge is an incoming edge:
			if (e2->source() != cT)
				continue;

			//get partner vertex of c in the block graph of B'=e->target() and add
			//cstrLength(B', c) to L:
			L += mf_cstrLength[pBCTree->cutVertex(cT, e2->target())];
		}

		for(adjEntry adjCT : cT->adjEntries) {

			edge e2 = adjCT->theEdge();
			//check if edge is an outgoing edge or the edge from bT to cT:
			if (e2->target() != cT || e2->source() == bT)
				continue;

			//get partner vertex of c in the block graph of B'=e->source():
			node partnerV = pBCTree->cutVertex(cT, e2->source());
			mf_nodeLength[partnerV] = L - mf_cstrLength[partnerV];

			//pBCTree->originalGraph().chooseNode() just to get rid of warning:
			node thisbT_opt = pBCTree->originalGraph().chooseNode();
			int thisell_opt = 0;
			mf_maximumFaceRec(e2->source(), thisbT_opt, thisell_opt);
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

	delete spqrTree;
}


void EmbedderMinDepthMaxFaceLayers::embedBlock(const node& bT)
{
	ListIterator<adjEntry> after;
	node cT = nullptr;
	embedBlock(bT, cT, after);
}


void EmbedderMinDepthMaxFaceLayers::embedBlock(
	const node& bT,
	const node& cT,
	ListIterator<adjEntry>& after)
{
	treeNodeTreated[bT] = true;
	node cH = nullptr;
	if (cT != nullptr)
		cH = pBCTree->cutVertex(cT, bT);

	// 1. Compute MinDepth node lengths depending on M_B, M2 and cT
	if (cT != nullptr && md_M_B[bT].size() == 1 && *(md_M_B[bT].begin()) == cH)
	{
		//set node length to 1 if node is in M2 and 0 otherwise
		for (ListIterator<node> iterator = md_M2[bT].begin(); iterator.valid(); ++iterator)
			md_nodeLength[*iterator] = 1;
	}
	else
	{
		//set node length to 1 if node is in M_B and 0 otherwise
		for (ListIterator<node> iterator = md_M_B[bT].begin(); iterator.valid(); ++iterator)
			md_nodeLength[*iterator] = 1;
	}

	// 2. Set MinDepthMaxFace node lengths

	//create subgraph (block bT):
	node nodeInBlock = cH;
	if (nodeInBlock == nullptr)
		nodeInBlock = (*(pBCTree->hEdges(bT).begin()))->source();
	Graph SG;
	NodeArray<MDMFLengthAttribute> nodeLengthSG;
	EdgeArray<MDMFLengthAttribute> edgeLengthSG;
	NodeArray<node> nSG_to_nG;
	EdgeArray<edge> eSG_to_eG;
	node nodeInBlockSG;
	embedder::ConnectedSubgraph<MDMFLengthAttribute>::call(
		pBCTree->auxiliaryGraph(), SG,
		nodeInBlock, nodeInBlockSG,
		nSG_to_nG, eSG_to_eG,
		mdmf_nodeLength, nodeLengthSG,
		mdmf_edgeLength, edgeLengthSG);

	//copy (0, 1)-min depth node lengths into nodeLengthSG "a" component and max
	//face sice node lengths into "b" component:
	for(node nSG : SG.nodes)
	{
		nodeLengthSG[nSG].a = md_nodeLength[nSG_to_nG[nSG]];
		nodeLengthSG[nSG].b = mf_nodeLength[nSG_to_nG[nSG]];
	}

	// 3. Compute embedding of block
	adjEntry m_adjExternal = nullptr;
	if (cH == nullptr)
		EmbedderMaxFaceBiconnectedGraphsLayers<MDMFLengthAttribute>::embed(
			SG, m_adjExternal, nodeLengthSG, edgeLengthSG);
	else
		EmbedderMaxFaceBiconnectedGraphsLayers<MDMFLengthAttribute>::embed(
			SG, m_adjExternal, nodeLengthSG, edgeLengthSG, nodeInBlockSG);

	// 4. Copy block embedding into graph embedding and call recursively
	//    embedBlock for all cut vertices in bT
	CombinatorialEmbedding CE(SG);
	face f = CE.leftFace(m_adjExternal);

	if (*pAdjExternal == nullptr)
	{
		node on = pBCTree->original(nSG_to_nG[m_adjExternal->theNode()]);
		adjEntry ae1 = on->firstAdj();
		for (adjEntry ae = ae1; ae; ae = ae->succ())
		{
			if (ae->theEdge() == pBCTree->original(eSG_to_eG[m_adjExternal->theEdge()]))
			{
				*pAdjExternal = ae->twin();
				break;
			}
		}
	}

	bool DGcomputed = false;
	int extFaceID = 0;

	// when the following objects get allocated,
	// the DGcomputed bool is set to true
	Graph* p_DG = nullptr;
	ArrayBuffer<node>* p_fPG_to_nDG = nullptr;
	NodeArray<int>* p_nDG_to_fPG = nullptr;
	NodeArray< List<adjEntry> >* p_adjacencyList = nullptr;
	List< List<adjEntry> >* p_faces = nullptr;
	NodeArray<int>* p_distances = nullptr;


	for(node nSG : SG.nodes)
	{
		node nH = nSG_to_nG[nSG];
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
				bool aeExtExists = false;
				adjEntry aeFace = f->firstAdj();
				do
				{
					if (aeFace->theNode() == nSG)
					{
						if (aeFace->succ())
							ae = aeFace->succ();
						else
							ae = nSG->firstAdj();
						aeExtExists = true;
						break;
					}
					aeFace = aeFace->faceCycleSucc();
				} while(aeFace != f->firstAdj());

				if (!aeExtExists)
				{
					if (!DGcomputed)
					{
						p_DG = new Graph();
						p_fPG_to_nDG = new ArrayBuffer<node>();
						p_nDG_to_fPG = new NodeArray<int>();
						p_adjacencyList = new NodeArray< List<adjEntry> >();
						p_faces = new List< List<adjEntry> >;
						p_distances = new NodeArray<int>;
						DGcomputed = true;

						//compute dual graph of skeleton graph:
						p_adjacencyList->init(SG);
						for(node nBG : SG.nodes)
						{
							for(adjEntry ae_nBG : nBG->adjEntries)
								(*p_adjacencyList)[nBG].pushBack(ae_nBG);
						}

						NodeArray< List<adjEntry> > adjEntryTreated(SG);
						for(node nBG : SG.nodes)
						{
							for(adjEntry adj : nBG->adjEntries)
							{
								if (adjEntryTreated[nBG].search(adj).valid())
									continue;

								List<adjEntry> newFace;
								adjEntry adj2 = adj;
								do
								{
									newFace.pushBack(adj2);
									adjEntryTreated[adj2->theNode()].pushBack(adj2);
									List<adjEntry> &ladj = (*p_adjacencyList)[adj2->twinNode()];
									adj2 = *ladj.cyclicPred(ladj.search(adj2->twin()));
								} while (adj2 != adj);
								p_faces->pushBack(newFace);
							}
						}

						for (ListIterator< List<adjEntry> > it = p_faces->begin(); it.valid(); ++it) {
							p_fPG_to_nDG->push(p_DG->newNode());
						}

						NodeArray< List<node> > adjFaces(*p_DG);
						int i = 0;
						for (ListIterator< List<adjEntry> > it = p_faces->begin(); it.valid(); ++it) {
							int f1_id = i;
							for (ListIterator<adjEntry> it2 = (*it).begin(); it2.valid(); ++it2) {
								int f2_id = 0;
								int j = 0;
								for (ListIterator< List<adjEntry> > it3 = p_faces->begin(); it3.valid(); ++it3) {
									bool do_break = false;
									for (ListIterator<adjEntry> it4 = (*it3).begin(); it4.valid(); ++it4) {
										if ((*it4) == (*it2)->twin()) {
											f2_id = j;
											do_break = true;
											break;
										}
									}
									if (do_break)
										break;
									++j;
								}

								if (f1_id != f2_id
								 && !adjFaces[(*p_fPG_to_nDG)[f1_id]].search((*p_fPG_to_nDG)[f2_id]).valid()
								 && !adjFaces[(*p_fPG_to_nDG)[f2_id]].search((*p_fPG_to_nDG)[f1_id]).valid()) {
									adjFaces[(*p_fPG_to_nDG)[f1_id]].pushBack((*p_fPG_to_nDG)[f2_id]);
									p_DG->newEdge((*p_fPG_to_nDG)[f1_id], (*p_fPG_to_nDG)[f2_id]);
								}

								if (*it2 == f->firstAdj())
									extFaceID = f1_id;
							} //for (ListIterator<adjEntry> it2 = (*it).begin(); it2.valid(); it2++)
							++i;
						} //for (ListIterator< List<adjEntry> > it = faces.begin(); it.valid(); it++)

						//compute shortest path from every face to the external face:
						List<edge> DG_edges;
						p_DG->allEdges(DG_edges);
						for (ListIterator<edge> it_e = DG_edges.begin(); it_e.valid(); ++it_e) {
							node s = (*it_e)->source();
							node t = (*it_e)->target();
							p_DG->newEdge(t, s);
						}
						ShortestPathWithBFM shortestPath;
						node efDG = (*p_fPG_to_nDG)[extFaceID];
						EdgeArray<int> el(*p_DG, 1);
						p_distances->init(*p_DG);
						NodeArray<edge> pi(*p_DG);
						shortestPath.call(*p_DG, efDG, el, *p_distances, pi);
					} //if (!DGcomputed)

					//choose face with minimal shortest path:
					List<adjEntry> optFace;
					int optFaceDist = -1;
					for (int fID = 0; fID < p_faces->size(); ++fID) {
						List<adjEntry> theFace = *(p_faces->get(fID));
						adjEntry ae_nSG;
						bool contains_nSG = false;
						for (ListIterator<adjEntry> it_ae = theFace.begin(); it_ae.valid(); ++it_ae) {
							if ((*it_ae)->theNode() == nSG) {
								contains_nSG = true;
								ae_nSG = *it_ae;
								break;
							}
						}

						if (contains_nSG) {
							int thisDist = (*p_distances)[(*p_fPG_to_nDG)[fID]];
							if (optFaceDist == -1 || optFaceDist > thisDist)
							{
								optFace = theFace;
								optFaceDist = thisDist;
								if (ae_nSG->succ())
									ae = ae_nSG->succ();
								else
									ae = nSG->firstAdj();
							}
						}
					} //for (int fID = 0; fID < faces.size(); fID++)
				} //if (!aeExtExists)

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
			edge eG = pBCTree->original(eSG_to_eG[aeNode->theEdge()]);
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

	if (DGcomputed)
	{
		delete p_DG;
		delete p_fPG_to_nDG;
		delete p_nDG_to_fPG;
		delete p_adjacencyList;
		delete p_faces;
		delete p_distances;
	}
}

} // end namespace ogdf
