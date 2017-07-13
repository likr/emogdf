/** \file
 * \brief useable example of the Modular Multilevel Mixer
 *
 * \author Gereon Bartel
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

#include <ogdf/energybased/multilevel_mixer/MMMExampleNoTwistLayout.h>
#include <ogdf/basic/PreprocessorLayout.h>
#include <ogdf/packing/ComponentSplitterLayout.h>
#include <ogdf/energybased/multilevel_mixer/ModularMultilevelMixer.h>
#include <ogdf/energybased/multilevel_mixer/ScalingLayout.h>
#include <ogdf/energybased/FastMultipoleEmbedder.h>

#include <ogdf/energybased/multilevel_mixer/LocalBiconnectedMerger.h>
#include <ogdf/energybased/multilevel_mixer/BarycenterPlacer.h>

namespace ogdf {

MMMExampleNoTwistLayout::MMMExampleNoTwistLayout()
{
}


void MMMExampleNoTwistLayout::call(GraphAttributes &GA)
{
	MultilevelGraph MLG(GA);
	call(MLG);
	MLG.exportAttributes(GA);
}


void MMMExampleNoTwistLayout::call(MultilevelGraph &MLG)
{
	// Fast Multipole Embedder
	FastMultipoleEmbedder * FME = new FastMultipoleEmbedder();
	FME->setNumIterations(1000);
	FME->setRandomize(false);

	// Local Biconnected Merger
	LocalBiconnectedMerger * LBCM = new LocalBiconnectedMerger();
	LBCM->setFactor(2.0);
	LBCM->setEdgeLengthAdjustment(0); // BEFORE (but arg is int!): LBCM->setEdgeLengthAdjustment(0.1);

	// Barycenter Placer with weighted Positions
	BarycenterPlacer * BP = new BarycenterPlacer();
	BP->weightedPositionPriority(true);

	// No Scaling
	ScalingLayout *SL = new ScalingLayout();
	SL->setExtraScalingSteps(1);
	SL->setScaling(5.0, 10.0);
	SL->setScalingType(ScalingLayout::ScalingType::RelativeToDesiredLength);
	SL->setSecondaryLayout(FME);
	SL->setLayoutRepeats(1);

	ModularMultilevelMixer *MMM = new ModularMultilevelMixer;
	MMM->setLayoutRepeats(1);
//	MMM->setAllEdgeLenghts(5.0);
//	MMM->setAllNodeSizes(1.0);
	MMM->setLevelLayoutModule(SL);
	MMM->setInitialPlacer(BP);
	MMM->setMultilevelBuilder(LBCM);

	ComponentSplitterLayout *CS = new ComponentSplitterLayout;
	CS->setLayoutModule(MMM);
	PreprocessorLayout PPL;
	PPL.setLayoutModule(CS);
	PPL.setRandomizePositions(true);

	PPL.call(MLG);
}

} // namespace ogdf
