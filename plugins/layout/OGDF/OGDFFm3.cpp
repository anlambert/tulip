/**
 *
 * Copyright (C) 2019-2020  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include <talipot/OGDFLayoutPluginBase.h>
#include <talipot/NumericProperty.h>
#include <talipot/StringCollection.h>
#include <talipot/SizeProperty.h>

#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/energybased/fmmm/FMMMOptions.h>

using namespace std;

#define ELT_PAGEFORMAT "Page Format"
#define ELT_PAGEFORMATLIST "Square;Portrait;Landscape"
#define ELT_SQUARE 0
#define ELT_PORTRAIT 1
#define ELT_LANDSCAPE 2

#define ELT_QUALITYVSSPEED "Quality vs Speed"
#define ELT_QUALITYVSSPEEDLIST "BeautifulAndFast;NiceAndIncredibleSpeed;GorgeousAndEfficient"
#define ELT_BEAUTIFULANDFAST 0
#define ELT_NICEANDINCREDIBLESPEED 1
#define ELT_GORGEOUSANDEFFICIENT 2

#define ELT_EDGELENGTHMEASUREMENT "Edge Length Measurement"
#define ELT_EDGELENGTHMEASUREMENTLIST "BoundingCircle;Midpoint"
#define ELT_BOUNDINGCIRCLE 0
#define ELT_MIDPOINT 1

#define ELT_ALLOWEDPOSITIONS "Allowed Positions"
#define ELT_ALLOWEDPOSITIONSLIST "Integer;Exponent;All"
#define ELT_INTEGER 0
#define ELT_EXPONENT 1
#define ELT_ALL 2

#define ELT_TIPOVER "Tip Over"
#define ELT_TIPOVERLIST "NoGrowingRow;Always;None"
#define ELT_NOGROWINGROW 0
#define ELT_ALWAYS 1
#define ELT_NONE 2

#define ELT_PRESORT "Pre Sort"
#define ELT_PRESORTLIST "DecreasingHeight;DecreasingWidth;None;"
#define ELT_DECREASINGHEIGHT 0
#define ELT_DECREASINGWDTH 1

#define ELT_GALAXYCHOICE "Galaxy Choice"
#define ELT_GALAXYCHOICELIST "NonUniformProbLowerMass;NonUniformProbHigherMass;UniformProb"
#define ELT_NONUNIFORMPROBLOWERMASS 0
#define ELT_NONUNIFORMPROBHIGHERMASS 1
#define ELT_UNIFORMPROB 2

#define ELT_MAXITERCHANGE "Max Iter Change"
#define ELT_MAXITERCHANGELIST "LinearlyDecreasing;RapidlyDecreasing;Constant"
#define ELT_LINEARLYDECREASING 0
#define ELT_RAPIDLYDECREASING 1
#define ELT_CONSTANT 2

#define ELT_INITIALPLACEMENTMULT "Initial Placement Mult"
#define ELT_INITIALPLACEMENTMULTLIST "Advanced;Simple"
#define ELT_ADVANCED 0
#define ELT_SIMPLE 1

#define ELT_FORCEMODEL "Force Model"
#define ELT_FORCEMODELLIST "New;FruchtermanReingold;Eades"
#define ELT_NEW 0
#define ELT_FRUCHTERMANNREINGOLD 1
#define ELT_EADES 2

#define ELT_REPULSIVEFORCEMETHOD "Repulsive Force Method"
#define ELT_REPULSIVEFORCEMETHODLIST "NMM;Exact;GridApproximation"
#define ELT_NMM 0
#define ELT_EXACT 1
#define ELT_GRIDAPPROXIMATION 2

#define ELT_INITIALPLACEMENTFORCES "Initial Placement Forces"
#define ELT_INITIALPLACEMENTFORCESLIST "RandomRandIterNr;RandomTime;UniformGrid;KeepPositions"
#define ELT_RANDOMRANDITERNR 0
#define ELT_RANDOMTIME 1
#define ELT_UNIFORMGRID 2
#define ELT_KEEPPOSITIONS 3

#define ELT_REDUCEDTREECONSTRCUCTION "Reduced Tree Construction"
#define ELT_REDUCEDTREECONSTRCUCTIONLIST "SubtreeBySubtree;PathByPath"
#define ELT_SUBTREEBYSUBTREE 0
#define ELT_PATHBYPATH 1

#define ELT_SMALLESTCELLFINDING "Smallest Cell Finding"
#define ELT_SMALLESTCELLFINDINGLIST "Iteratively;Aluru"
#define ELT_ITERATIVELY 0
#define ELT_ALURU 1

static const char *paramHelp[] = {

    // Edge Length Property
    "A numeric property containing unit edge length to use.",

    // Node Size
    "The node sizes.",

    // Unit edge length
    "The unit edge length.",

    // New initial placement
    "Indicates the initial placement before running algorithm.",

    // Fixed iterations
    "The fixed number of iterations for the stop criterion.",

    // Threshold
    "The threshold for the stop criterion.",

    // Page Format
    "Possible page formats.",

    // Quality vs Speed
    "Trade-off between run-time and quality.",

    // Edge Length Measurement
    "Specifies how the length of an edge is measured.",

    // Allowed Positions
    "Specifies which positions for a node are allowed.",

    // Tip Over
    "Specifies in which case it is allowed to tip over drawings of connected components.",

    // Pre Sort
    "Specifies how connected components are sorted before the packing algorithm is applied.",

    // Galaxy Choice
    "Specifies how sun nodes of galaxies are selected.",

    // Max Iter Change
    "Specifies how MaxIterations is changed in subsequent multilevels.",

    // Initial Placement
    "Specifies how the initial placement is generated.",

    // Force Model
    "Specifies the force-model.",

    // Repulsive Force Model
    "Specifies how to calculate repulsive forces.",

    // Initial Placement Forces
    "Specifies how the initial placement is done.",

    // Reduced Tree Construction
    "Specifies how the reduced bucket quadtree is constructed.",

    // Smallest Cell Finding
    "Specifies how to calculate the smallest quadratic cell surrounding particles of a node in the "
    "reduced bucket quadtree."};

static const char *pageFormatValuesDescription = "Portrait <i>(A4 portrait page)</i><br>"
                                                 "Landscape <i>(A4 landscape page)</i><br>"
                                                 "Square <i>(Square format)</i>";

static const char *qualityVsSpeedValuesDescription =
    "GorgeousAndEfficient <i>(Best quality)</i><br>"
    "BeautifulAndFast <i>(Medium quality and speed)</i><br>"
    "NiceAndIncredibleSpeed <i>(Best speed</i>";

static const char *edgeLengthMeasurementValuesDescription =
    "Midpoint <i>(Measure from center point of edge end points)</i><br>"
    "BoundingCircle <i>(Measure from border of circle surrounding edge end points)</i>";

static const char *presortValuesDescription =
    "None <i>(Do not presort)</i><br>"
    "DecreasingHeight <i>(Presort by decreasing height of components)</i><br>"
    "DecreasingWidth <i>(Presort by decreasing width of components)</i>";

static const char *galaxyChoiceValuesDescription = "UniformProb<br>"
                                                   "NonUniformProbLowerMass<br>"
                                                   "NonUniformProbHigherMass";

static const char *maxIterChangeValuesDescription = "Constant <br>"
                                                    "LinearlyDecreasing <br>"
                                                    "RapidlyDecreasing";

static const char *forceModelValuesDescription =
    "FruchtermanReingold <i>(The force-model by Fruchterman, Reingold)</i><br>"
    "Eades <i>(The force-model by Eades)</i><br>"
    "New <i>(The new force-model)</i>";

static const char *repulsiveForceValuesDescription =
    "Exact <i>(Exact calculation)</i><br>"
    "GridApproximation <i>(Grid approximation)</i><br>"
    "NMM <i>(Calculation as for new multipole method)</i>";

static const char *initialPlacementValuesDescription =
    "UniformGrid <i>(Uniform placement on a grid)</i><br>"
    "RandomTime <i>(Random placement, based on current time)</i><br>"
    "RandomRandIterNr <i>(Random placement, based on randIterNr())</i><br>"
    "KeepPositions <i>(No change in placement)</i>";

static const char *smallestCellFindingValuesDescription =
    "Iteratively <i>(Iteratively, in constant time)</i><br>"
    "Aluru <i>(According to formula by Aluru et al., in constant time)</i>";

class OGDFFm3 : public tlp::OGDFLayoutPluginBase {

  tlp::StringCollection stringCollection;

public:
  PLUGININFORMATION("FM^3 (OGDF)", "Stephan Hachul", "09/11/2007",
                    "Implements the FM³ layout algorithm by Hachul and Jünger. It is a multilevel, "
                    "force-directed layout algorithm that can be applied to very large graphs.",
                    "1.2", "Force Directed")
  OGDFFm3(const tlp::PluginContext *context);
  void beforeCall() override;
  void callOGDFLayoutAlgorithm(ogdf::GraphAttributes &gAttributes) override;
};

/*Nom de la classe, Nom du plugins, nom de l'auteur,date de
 creation,information, release, groupe*/
PLUGIN(OGDFFm3)

OGDFFm3::OGDFFm3(const tlp::PluginContext *context)
    : OGDFLayoutPluginBase(context, new ogdf::FMMMLayout()) {
  addInParameter<tlp::NumericProperty *>("Edge Length Property", paramHelp[0], "viewMetric", false);
  addInParameter<tlp::SizeProperty>("Node Size", paramHelp[1], "viewSize", false);
  addInParameter<double>("Unit edge length", paramHelp[2], "10.0", false);
  addInParameter<bool>("New initial placement", paramHelp[3], "true");
  addInParameter<int>("Fixed iterations", paramHelp[4], "30");
  addInParameter<double>("Threshold", paramHelp[5], "0.01");
  addInParameter<tlp::StringCollection>(ELT_PAGEFORMAT, paramHelp[6], ELT_PAGEFORMATLIST, true,
                                        pageFormatValuesDescription);
  addInParameter<tlp::StringCollection>(ELT_QUALITYVSSPEED, paramHelp[7], ELT_QUALITYVSSPEEDLIST,
                                        true, qualityVsSpeedValuesDescription);
  addInParameter<tlp::StringCollection>(ELT_EDGELENGTHMEASUREMENT, paramHelp[8],
                                        ELT_EDGELENGTHMEASUREMENTLIST, true,
                                        edgeLengthMeasurementValuesDescription);
  addInParameter<tlp::StringCollection>(ELT_ALLOWEDPOSITIONS, paramHelp[9],
                                        ELT_ALLOWEDPOSITIONSLIST, true,
                                        "All<br> Integer <br> Exponent");
  addInParameter<tlp::StringCollection>(ELT_TIPOVER, paramHelp[10], ELT_TIPOVERLIST, true,
                                        "None<br> NoGrowingRow<br> Always");
  addInParameter<tlp::StringCollection>(ELT_PRESORT, paramHelp[11], ELT_PRESORTLIST, true,
                                        presortValuesDescription);
  addInParameter<tlp::StringCollection>(ELT_GALAXYCHOICE, paramHelp[12], ELT_GALAXYCHOICELIST, true,
                                        galaxyChoiceValuesDescription);
  addInParameter<tlp::StringCollection>(ELT_MAXITERCHANGE, paramHelp[13], ELT_MAXITERCHANGELIST,
                                        true, maxIterChangeValuesDescription);
  addInParameter<tlp::StringCollection>(ELT_INITIALPLACEMENTMULT, paramHelp[14],
                                        ELT_INITIALPLACEMENTMULTLIST, true, "Simple <br> Advanced");
  addInParameter<tlp::StringCollection>(ELT_FORCEMODEL, paramHelp[15], ELT_FORCEMODELLIST, true,
                                        forceModelValuesDescription);
  addInParameter<tlp::StringCollection>(ELT_REPULSIVEFORCEMETHOD, paramHelp[16],
                                        ELT_REPULSIVEFORCEMETHODLIST, true,
                                        repulsiveForceValuesDescription);
  addInParameter<tlp::StringCollection>(ELT_INITIALPLACEMENTFORCES, paramHelp[17],
                                        ELT_INITIALPLACEMENTFORCESLIST, true,
                                        initialPlacementValuesDescription);
  addInParameter<tlp::StringCollection>(ELT_REDUCEDTREECONSTRCUCTION, paramHelp[18],
                                        ELT_REDUCEDTREECONSTRCUCTIONLIST, true,
                                        "PathByPath <br> SubtreeBySubtree");
  addInParameter<tlp::StringCollection>(ELT_SMALLESTCELLFINDING, paramHelp[19],
                                        ELT_SMALLESTCELLFINDINGLIST, true,
                                        smallestCellFindingValuesDescription);
}

void OGDFFm3::beforeCall() {
  ogdf::FMMMLayout *fmmm = static_cast<ogdf::FMMMLayout *>(ogdfLayoutAlgo);

  if (dataSet != nullptr) {
    fmmm->useHighLevelOptions(true);
    tlp::SizeProperty *size = nullptr;

    if (dataSet->get("Node Size", size)) {
      tlpToOGDF->copyTlpNodeSizeToOGDF(size);
    }

    double edgeLenth = 10;

    if (dataSet->get("Unit edge length", edgeLenth)) {
      fmmm->unitEdgeLength(edgeLenth);
    }

    bool bval = false;

    if (dataSet->get("New initial placement", bval)) {
      fmmm->newInitialPlacement(bval);
    }

    int ival = 0;

    if (dataSet->get("Fixed iterations", ival)) {
      fmmm->fixedIterations(ival);
    }

    double dval = 0;

    if (dataSet->get("Threshold", dval)) {
      fmmm->threshold(dval);
    }

    if (dataSet->get(ELT_PAGEFORMAT, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_PORTRAIT) {
        fmmm->pageFormat(ogdf::FMMMOptions::PageFormatType::Portrait);
      } else if (stringCollection.getCurrent() == ELT_LANDSCAPE) {
        fmmm->pageFormat(ogdf::FMMMOptions::PageFormatType::Landscape);
      } else {
        fmmm->pageFormat(ogdf::FMMMOptions::PageFormatType::Square);
      }
    }

    if (dataSet->get(ELT_QUALITYVSSPEED, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_NICEANDINCREDIBLESPEED) {
        fmmm->qualityVersusSpeed(ogdf::FMMMOptions::QualityVsSpeed::NiceAndIncredibleSpeed);
      } else if (stringCollection.getCurrent() == ELT_GORGEOUSANDEFFICIENT) {
        fmmm->qualityVersusSpeed(ogdf::FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
      } else {
        fmmm->qualityVersusSpeed(ogdf::FMMMOptions::QualityVsSpeed::BeautifulAndFast);
      }
    }

    if (dataSet->get(ELT_EDGELENGTHMEASUREMENT, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_BOUNDINGCIRCLE) {
        fmmm->edgeLengthMeasurement(ogdf::FMMMOptions::EdgeLengthMeasurement::BoundingCircle);
      } else {
        fmmm->edgeLengthMeasurement(ogdf::FMMMOptions::EdgeLengthMeasurement::Midpoint);
      }
    }

    if (dataSet->get(ELT_ALLOWEDPOSITIONS, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_INTEGER) {
        fmmm->allowedPositions(ogdf::FMMMOptions::AllowedPositions::Integer);
      } else if (stringCollection.getCurrent() == ELT_EXPONENT) {
        fmmm->allowedPositions(ogdf::FMMMOptions::AllowedPositions::Exponent);
      } else {
        fmmm->allowedPositions(ogdf::FMMMOptions::AllowedPositions::All);
      }
    }

    if (dataSet->get(ELT_TIPOVER, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_NONE) {
        fmmm->tipOverCCs(ogdf::FMMMOptions::TipOver::None);
      } else if (stringCollection.getCurrent() == ELT_NOGROWINGROW) {
        fmmm->tipOverCCs(ogdf::FMMMOptions::TipOver::NoGrowingRow);
      } else {
        fmmm->tipOverCCs(ogdf::FMMMOptions::TipOver::Always);
      }
    }

    if (dataSet->get(ELT_PRESORT, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_NONE) {
        fmmm->presortCCs(ogdf::FMMMOptions::PreSort::None);
      } else if (stringCollection.getCurrent() == ELT_DECREASINGHEIGHT) {
        fmmm->presortCCs(ogdf::FMMMOptions::PreSort::DecreasingHeight);
      } else {
        fmmm->presortCCs(ogdf::FMMMOptions::PreSort::DecreasingWidth);
      }
    }

    if (dataSet->get(ELT_GALAXYCHOICE, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_UNIFORMPROB) {
        fmmm->galaxyChoice(ogdf::FMMMOptions::GalaxyChoice::UniformProb);
      } else if (stringCollection.getCurrent() == ELT_NONUNIFORMPROBLOWERMASS) {
        fmmm->galaxyChoice(ogdf::FMMMOptions::GalaxyChoice::NonUniformProbLowerMass);
      } else {
        fmmm->galaxyChoice(ogdf::FMMMOptions::GalaxyChoice::NonUniformProbHigherMass);
      }
    }

    if (dataSet->get(ELT_MAXITERCHANGE, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_CONSTANT) {
        fmmm->maxIterChange(ogdf::FMMMOptions::MaxIterChange::Constant);
      } else if (stringCollection.getCurrent() == ELT_LINEARLYDECREASING) {
        fmmm->maxIterChange(ogdf::FMMMOptions::MaxIterChange::LinearlyDecreasing);
      } else {
        fmmm->maxIterChange(ogdf::FMMMOptions::MaxIterChange::RapidlyDecreasing);
      }
    }

    if (dataSet->get(ELT_INITIALPLACEMENTMULT, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_ADVANCED) {
        fmmm->initialPlacementMult(ogdf::FMMMOptions::InitialPlacementMult::Advanced);
      } else {
        fmmm->initialPlacementMult(ogdf::FMMMOptions::InitialPlacementMult::Simple);
      }
    }

    if (dataSet->get(ELT_FORCEMODEL, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_FRUCHTERMANNREINGOLD) {
        fmmm->forceModel(ogdf::FMMMOptions::ForceModel::FruchtermanReingold);
      } else if (stringCollection.getCurrent() == ELT_EADES) {
        fmmm->forceModel(ogdf::FMMMOptions::ForceModel::Eades);
      } else {
        fmmm->forceModel(ogdf::FMMMOptions::ForceModel::New);
      }
    }

    if (dataSet->get(ELT_REPULSIVEFORCEMETHOD, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_EXACT) {
        fmmm->repulsiveForcesCalculation(ogdf::FMMMOptions::RepulsiveForcesMethod::Exact);
      } else if (stringCollection.getCurrent() == ELT_GRIDAPPROXIMATION) {
        fmmm->repulsiveForcesCalculation(
            ogdf::FMMMOptions::RepulsiveForcesMethod::GridApproximation);
      } else {
        fmmm->repulsiveForcesCalculation(ogdf::FMMMOptions::RepulsiveForcesMethod::NMM);
      }
    }

    if (dataSet->get(ELT_INITIALPLACEMENTFORCES, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_UNIFORMGRID) {
        fmmm->initialPlacementForces(ogdf::FMMMOptions::InitialPlacementForces::UniformGrid);
      } else if (stringCollection.getCurrent() == ELT_RANDOMTIME) {
        fmmm->initialPlacementForces(ogdf::FMMMOptions::InitialPlacementForces::RandomTime);
      } else if (stringCollection.getCurrent() == ELT_RANDOMRANDITERNR) {
        fmmm->initialPlacementForces(ogdf::FMMMOptions::InitialPlacementForces::RandomRandIterNr);
      } else {
        fmmm->initialPlacementForces(ogdf::FMMMOptions::InitialPlacementForces::KeepPositions);
      }
    }

    if (dataSet->get(ELT_REDUCEDTREECONSTRCUCTION, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_SUBTREEBYSUBTREE) {
        fmmm->nmTreeConstruction(ogdf::FMMMOptions::ReducedTreeConstruction::SubtreeBySubtree);
      } else {
        fmmm->nmTreeConstruction(ogdf::FMMMOptions::ReducedTreeConstruction::PathByPath);
      }
    }

    if (dataSet->get(ELT_SMALLESTCELLFINDING, stringCollection)) {
      if (stringCollection.getCurrent() == ELT_ITERATIVELY) {
        fmmm->nmSmallCell(ogdf::FMMMOptions::SmallestCellFinding::Iteratively);
      } else {
        fmmm->nmSmallCell(ogdf::FMMMOptions::SmallestCellFinding::Aluru);
      }
    }
  }
}

void OGDFFm3::callOGDFLayoutAlgorithm(ogdf::GraphAttributes &gAttributes) {

  ogdf::FMMMLayout *fmmm = static_cast<ogdf::FMMMLayout *>(ogdfLayoutAlgo);

  tlp::NumericProperty *length = nullptr;

  if (dataSet->get("Edge Length Property", length) && length) {
    ogdf::EdgeArray<double> edgeLength(tlpToOGDF->getOGDFGraph());

    for (auto e : graph->edges()) {
      edgeLength[tlpToOGDF->getOGDFGraphEdge(e)] = length->getEdgeDoubleValue(e);
    }

    fmmm->call(gAttributes, edgeLength);
  } else {
    OGDFLayoutPluginBase::callOGDFLayoutAlgorithm(gAttributes);
  }
}
