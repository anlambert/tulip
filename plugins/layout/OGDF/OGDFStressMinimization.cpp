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

#include <ogdf/energybased/StressMinimization.h>

#include <talipot/OGDFLayoutPluginBase.h>
#include <talipot/StringCollection.h>
#include <talipot/NumericProperty.h>
#include <talipot/DoubleProperty.h>

static const char *paramHelp[] = {
    // terminationCriterion
    "Tells which TERMINATION_CRITERIA should be used.",

    // fixXCoordinates
    "Tells whether the x coordinates are allowed to be modified or not.",

    // fixYCoordinates
    "Tells whether the y coordinates are allowed to be modified or not.",

    // hasInitialLayout
    "Tells whether the current layout should be used or the initial layout needs to be computed.",

    // layoutComponentsSeparately
    "Sets whether the graph components should be layouted separately or a dummy distance should be "
    "used for nodes within different components.",

    // numberOfIterations
    "Sets a fixed number of iterations for stress minimization. If the new value is smaller or "
    "equal 0 the default value (200) is used.",

    // edgeCosts
    "Sets the desired distance between adjacent nodes. If the new value is smaller or equal 0 the "
    "default value (100) is used.",

    // useEdgeCostsProperty
    "Tells whether the edge costs are uniform or defined in an edge costs property.",

    // edgeCostsProperty
    "The numeric property that holds the desired cost for each edge."

};

class OGDFStressMinimization : public tlp::OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Stress Minimization (OGDF)", "Karsten Klein", "12/11/2007",
                    "Implements an alternative to force-directed layout which is a distance-based "
                    "layout realized by the stress minimization approach. ",
                    "2.0", "Force Directed")
  OGDFStressMinimization(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, tlp::getOGDFLayoutModule<ogdf::StressMinimization>(context)) {
    addInParameter<tlp::StringCollection>("terminationCriterion", paramHelp[0],
                                          "None;PositionDifference;Stress", true,
                                          "None <br> PositionDifference <br> Stress");
    addInParameter<bool>("fixXCoordinates", paramHelp[1], "false");
    addInParameter<bool>("fixYCoordinates", paramHelp[2], "false");
    addInParameter<bool>("hasInitialLayout", paramHelp[3], "false");
    addInParameter<bool>("layoutComponentsSeparately", paramHelp[4], "false");
    addInParameter<int>("numberOfIterations", paramHelp[5], "200");
    addInParameter<double>("edgeCosts", paramHelp[6], "100");
    addInParameter<bool>("useEdgeCostsProperty", paramHelp[7], "false");
    addInParameter<tlp::NumericProperty *>("edgeCostsProperty", paramHelp[8], "viewMetric");
  }

  void beforeCall() override {
    ogdf::StressMinimization *stressm = static_cast<ogdf::StressMinimization *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
      double dval = 0;
      int ival = 0;
      bool bval = false;
      tlp::StringCollection sc;
      tlp::NumericProperty *edgeCosts = graph->getDoubleProperty("viewMetric");

      if (dataSet->get("terminationCriterion", sc)) {
        if (sc.getCurrentString() == "PositionDifference") {
          stressm->convergenceCriterion(
              ogdf::StressMinimization::TerminationCriterion::PositionDifference);
        } else if (sc.getCurrentString() == "Stress") {
          stressm->convergenceCriterion(ogdf::StressMinimization::TerminationCriterion::Stress);
        } else {
          stressm->convergenceCriterion(ogdf::StressMinimization::TerminationCriterion::None);
        }
      }

      if (dataSet->get("fixXCoordinates", bval)) {
        stressm->fixXCoordinates(bval);
      }

      if (dataSet->get("fixYCoordinates", bval)) {
        stressm->fixXCoordinates(bval);
      }

      if (dataSet->get("hasInitialLayout", bval)) {
        stressm->hasInitialLayout(bval);
      }

      if (dataSet->get("layoutComponentsSeparately", bval)) {
        stressm->layoutComponentsSeparately(bval);
      }

      if (dataSet->get("numberOfIterations", ival)) {
        stressm->setIterations(ival);
      }

      if (dataSet->get("edgeCosts", dval)) {
        stressm->setEdgeCosts(dval);
      }

      if (dataSet->get("useEdgeCostsProperty", bval)) {
        stressm->useEdgeCostsAttribute(bval);

        if (bval) {
          dataSet->get("edgeCostsProperty", edgeCosts);
          tlpToOGDF->copyTlpNumericPropertyToOGDFEdgeLength(edgeCosts);
        }
      }
    }
  }
};

PLUGIN(OGDFStressMinimization)
