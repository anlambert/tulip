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

#include <ogdf/misclayout/BalloonLayout.h>
#include <ogdf/packing/ComponentSplitterLayout.h>

#include <talipot/OGDFLayoutPluginBase.h>

static const char *paramHelp[] = {
    // Even angles
    "Subtrees may be assigned even angles or angles depending on their size."};

class OGDFBalloon : public tlp::OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Balloon (OGDF)", "Karsten Klein", "13/11/2007",
                    "Computes a radial (balloon) layout based on a spanning tree.<br/>The "
                    "algorithm is partially based on the paper <b>On Balloon Drawings of Rooted "
                    "Trees</b> by Lin and Yen and on <b>Interacting with Huge Hierarchies: Beyond "
                    "Cone Trees</b> by Carriere and Kazman. ",
                    "1.4", "Hierarchical")
  OGDFBalloon(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context,
                             tlp::getOGDFLayoutModule<ogdf::ComponentSplitterLayout>(context)),
        balloon(tlp::getOGDFLayoutModule<ogdf::BalloonLayout>(context)) {
    addInParameter<bool>("Even angles", paramHelp[0], "false", false);
    if (context) {
      ogdf::ComponentSplitterLayout *csl =
          static_cast<ogdf::ComponentSplitterLayout *>(ogdfLayoutAlgo);
      // ComponentSplitterLayout takes ownership of the BalloonLayout instance
      csl->setLayoutModule(balloon);
    }
  }

  void beforeCall() override {

    if (dataSet != nullptr) {
      bool val = false;

      if (dataSet->get("Even angles", val)) {
        balloon->setEvenAngles(val);
      }
    }
  }

private:
  ogdf::BalloonLayout *balloon;
};

PLUGIN(OGDFBalloon)
