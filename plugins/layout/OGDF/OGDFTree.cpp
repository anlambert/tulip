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

#include <ogdf/tree/TreeLayout.h>

#include <talipot/OGDFLayoutPluginBase.h>
#include <talipot/StringCollection.h>

#define ELT_ORIENTATION "Orientation"
#define ELT_ORIENTATIONLIST "topToBottom;bottomToTop;leftToRight;rightToLeft"
#define ELT_TOPTOBOTTOM 0
#define ELT_BOTTOMTOTOP 1
#define ELT_LEFTTORIGHT 2
#define ELT_RIGHTTOLEFT 3

#define ELT_ROOTSELECTION "Root selection"
#define ELT_ROOTSELECTIONLIST "rootIsSource;rootIsSink;rootByCoord"
#define ELT_ROOTSOURCE 0
#define ELT_ROOTSINK 1
#define ELT_ROOTCOORD 2

static const char *paramHelp[] = {
    // siblings distance
    "The minimal required horizontal distance between siblings.",

    // subtrees distance
    "The minimal required horizontal distance between subtrees.",

    // levels distance
    "The minimal required vertical distance between levels.",

    // trees distance
    "The minimal required horizontal distance between trees in the forest.",

    // orthogonal layout
    "Indicates whether orthogonal edge routing style is used or not.",

    // Orientation
    "This parameter indicates the orientation of the layout.",

    // Root selection
    "This parameter indicates how the root is selected."};

static const char *orientationValuesDescription =
    "topToBottom <i>(Edges are oriented from top to bottom)</i><br>"
    "bottomToTop <i>(Edges are oriented from bottom to top)</i><br>"
    "leftToRight <i>(Edges are oriented from left to right)</i><br>"
    "rightToLeft <i>(Edges are oriented from right to left)</i>";

static const char *rootSelectionValuesDescription =
    "rootIsSource <i>(Select a source in the graph)</i><br>"
    "rootIsSink <i>(Select a sink in the graph)</i><br>"
    "rootByCoord <i>(Use the coordinates, e.g., select the topmost node if orientation is "
    "topToBottom)</i>";

class OGDFTree : public tlp::OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Improved Walker (OGDF)", "Christoph Buchheim", "12/11/2007",
                    "Implements a linear-time tree layout algorithm with straight-line or "
                    "orthogonal edge routing.",
                    "1.5", "Tree")
  OGDFTree(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::TreeLayout()) {
    addInParameter<double>("siblings distance", paramHelp[0], "20");
    addInParameter<double>("subtrees distance", paramHelp[1], "20");
    addInParameter<double>("levels distance", paramHelp[2], "50");
    addInParameter<double>("trees distance", paramHelp[3], "50");
    addInParameter<bool>("orthogonal layout", paramHelp[4], "false");
    addInParameter<tlp::StringCollection>(ELT_ORIENTATION, paramHelp[5], ELT_ORIENTATIONLIST, true,
                                          orientationValuesDescription);
    addInParameter<tlp::StringCollection>(ELT_ROOTSELECTION, paramHelp[6], ELT_ROOTSELECTIONLIST,
                                          true, rootSelectionValuesDescription);
  }

  void beforeCall() override {
    ogdf::TreeLayout *tree = static_cast<ogdf::TreeLayout *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
      double dval = 0;
      bool bval = false;
      tlp::StringCollection sc;

      if (dataSet->get("siblings distance", dval)) {
        tree->siblingDistance(dval);
      }

      if (dataSet->get("subtrees distance", dval)) {
        tree->subtreeDistance(dval);
      }

      if (dataSet->get("levels distance", dval)) {
        tree->levelDistance(dval);
      }

      if (dataSet->get("trees distance", dval)) {
        tree->treeDistance(dval);
      }

      if (dataSet->get("orthogonal layout", bval)) {
        tree->orthogonalLayout(bval);
      }

      if (dataSet->get(ELT_ORIENTATION, sc)) {
        if (sc.getCurrent() == ELT_TOPTOBOTTOM) {
          // because of an ununderstanding fix
          // in thirdparty/OGDF/src/ogdf/tree/TreeLayout.cpp
          tree->orientation(ogdf::Orientation::bottomToTop);
        } else if (sc.getCurrent() == ELT_BOTTOMTOTOP) {
          // same as above
          tree->orientation(ogdf::Orientation::topToBottom);
        } else if (sc.getCurrent() == ELT_LEFTTORIGHT) {
          tree->orientation(ogdf::Orientation::leftToRight);
        } else {
          tree->orientation(ogdf::Orientation::rightToLeft);
        }
      }

      if (dataSet->get(ELT_ROOTSELECTION, sc)) {
        if (sc.getCurrent() == ELT_ROOTSOURCE) {
          tree->rootSelection(ogdf::TreeLayout::RootSelectionType::Source);
        } else if (sc.getCurrent() == ELT_ROOTSINK) {
          tree->rootSelection(ogdf::TreeLayout::RootSelectionType::Sink);
        } else {
          tree->rootSelection(ogdf::TreeLayout::RootSelectionType::ByCoord);
        }
      }
    }
  }
};

PLUGIN(OGDFTree)
