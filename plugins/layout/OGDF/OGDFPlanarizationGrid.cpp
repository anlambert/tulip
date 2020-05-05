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

#include <ogdf/planarity/PlanarizationGridLayout.h>

#include <talipot/OGDFLayoutPluginBase.h>
#include <talipot/StringCollection.h>

static const char *paramHelp[] = {
    // page ratio
    "Sets the option pageRatio."};

class OGDFPlanarizationGrid : public tlp::OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Planarization Grid (OGDF)", "Carsten Gutwenger", "12/11/2007",
                    "The planarization grid layout algorithm applies the planarization approach "
                    "for crossing minimization, combined with the topology-shape-metrics approach "
                    "for orthogonal planar graph drawing. It produces drawings with few crossings "
                    "and is suited for small to medium sized sparse graphs. It uses a planar grid "
                    "layout algorithm to produce a drawing on a grid.",
                    "1.0", "Planar")
  OGDFPlanarizationGrid(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::PlanarizationGridLayout()) {
    addInParameter<double>("page ratio", paramHelp[0], "1.1");
  }

  void beforeCall() override {
    ogdf::PlanarizationGridLayout *pgl =
        static_cast<ogdf::PlanarizationGridLayout *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
      double dval = 0;

      if (dataSet->get("page ratio", dval)) {
        pgl->pageRatio(dval);
      }
    }
  }
};

PLUGIN(OGDFPlanarizationGrid)
