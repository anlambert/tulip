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

#include "MultipleEdgeSelection.h"

#include <talipot/SimpleTest.h>

PLUGIN(MultipleEdgeSelection)

using namespace std;
using namespace tlp;

MultipleEdgeSelection::MultipleEdgeSelection(const tlp::PluginContext *context)
    : BooleanAlgorithm(context) {
  addInParameter<bool>("directed",
                       "Indicates if the graph should be considered as directed or not.", "false");
  addOutParameter<unsigned int>("#edges selected", "The number of multiple edges selected");
}

bool MultipleEdgeSelection::run() {
  bool directed = false;
  if (dataSet) {
    dataSet->get("directed", directed);
  }

  vector<edge> multipleEdges;
  SimpleTest::simpleTest(graph, &multipleEdges, nullptr, directed);
  result->setAllNodeValue(false);
  result->setAllEdgeValue(false);

  for (auto e : multipleEdges) {
    result->setEdgeValue(e, true);
  }

  // output some useful information
  if (dataSet != nullptr) {
    dataSet->set("#edges selected", uint(multipleEdges.size()));
  }

  return true;
}
