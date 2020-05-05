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

#include <talipot/DrawingTools.h>
#include <talipot/StringCollection.h>

#include "ConnectedComponentPacking.h"
#include "rectanglePackingFonctions.h"
#include "DatasetTools.h"

using namespace std;
using namespace tlp;

PLUGIN(ConnectedComponentPacking)

const float spacing = 8;
#define COMPLEXITY "auto;n5;n4logn;n4;n3logn;n3;n2logn;n2;nlogn;n;"

static const char *paramHelp[] = {
    // coordinates
    "Input layout of nodes and edges.",

    // rotation
    "Input rotation of nodes around the z-axis.",

    // complexity
    "Complexity of the algorithm.<br> n is the number of connected components in the graph."};

//====================================================================
ConnectedComponentPacking::ConnectedComponentPacking(const tlp::PluginContext *context)
    : LayoutAlgorithm(context) {
  addInParameter<LayoutProperty>("coordinates", paramHelp[0], "viewLayout");
  addNodeSizePropertyParameter(this);
  addInParameter<DoubleProperty>("rotation", paramHelp[1], "viewRotation");
  addInParameter<StringCollection>(
      "complexity", paramHelp[2], COMPLEXITY, true,
      "auto <br> n5 <br> n4logn <br> n4 <br> n3logn <br> n3 <br> n2logn <br> n2 <br> nlogn <br> n");
}
//====================================================================
bool ConnectedComponentPacking::run() {

  LayoutProperty *layout = nullptr;
  SizeProperty *size = nullptr;
  DoubleProperty *rotation = nullptr;
  string complexity("auto");

  if (dataSet != nullptr) {
    dataSet->get("coordinates", layout);
    getNodeSizePropertyParameter(dataSet, size);
    dataSet->get("rotation", rotation);
    StringCollection complexityCol;

    if (dataSet->get("complexity", complexityCol)) {
      complexity = complexityCol.getCurrentString();
    }
  }

  if (layout == nullptr) {
    layout = graph->getLayoutProperty("viewLayout");
  }

  if (size == nullptr) {
    size = graph->getSizeProperty("viewSize");
  }

  if (rotation == nullptr) {
    rotation = graph->getDoubleProperty("viewRotation");
  }

  if (complexity == "none") {
    complexity = "auto";
  }

  // compute the connected components
  std::vector<std::vector<node>> ccNodes;
  ConnectedTest::computeConnectedComponents(graph, ccNodes);

  vector<Rectangle<float>> rectangles;
  rectangles.resize(ccNodes.size());
  std::vector<std::vector<edge>> ccEdges;
  ccEdges.resize(ccNodes.size());

  for (unsigned int i = 0; i < ccNodes.size(); ++i) {
    std::vector<edge> &edges = ccEdges[i];
    MutableContainer<bool> visited;
    visited.setAll(false);
    const std::vector<node> &nodes = ccNodes[i];
    unsigned int nbNodes = nodes.size();

    for (unsigned int j = 0; j < nbNodes; ++j) {
      for (auto e : graph->getInOutEdges(nodes[j])) {
        if (!visited.get(e.id)) {
          visited.set(e.id, true);
          edges.push_back(e);
        }
      }
    }

    BoundingBox tmp = tlp::computeBoundingBox(nodes, edges, layout, size, rotation);
    Rectangle<float> &tmpRec = rectangles[i];
    tmpRec[1][0] = tmp[1][0] + spacing;
    tmpRec[1][1] = tmp[1][1] + spacing;
    tmpRec[0][0] = tmp[0][0] + spacing;
    tmpRec[0][1] = tmp[0][1] + spacing;
    assert(tmpRec.isValid());
  }

  if (complexity == "auto") {
    if (rectangles.size() < 25) {
      complexity = "n5";
    } else if (rectangles.size() < 50) {
      complexity = "n4logn";
    } else if (rectangles.size() < 100) {
      complexity = "n4";
    } else if (rectangles.size() < 150) {
      complexity = "n3logn";
    } else if (rectangles.size() < 250) {
      complexity = "n3";
    } else if (rectangles.size() < 500) {
      complexity = "n2logn";
    } else if (rectangles.size() < 1000) {
      complexity = "n2";
    } else if (rectangles.size() < 5000) {
      complexity = "nlogn";
    } else {
      complexity = "n";
    }
  }

  vector<Rectangle<float>> rectanglesBackup(rectangles);
  if (!RectanglePackingLimitRectangles(rectangles, complexity.c_str(), pluginProgress)) {
    return pluginProgress ? pluginProgress->state() != TLP_CANCEL : false;
  }

  for (auto n : graph->nodes()) {
    result->setNodeValue(n, layout->getNodeValue(n));
  }

  for (auto e : graph->edges()) {
    result->setEdgeValue(e, layout->getEdgeValue(e));
  }

  for (unsigned int i = 0; i < ccNodes.size(); ++i) {
    Coord move(rectangles[i][0][0] - rectanglesBackup[i][0][0],
               rectangles[i][0][1] - rectanglesBackup[i][0][1], 0);
    result->translate(move, stlIterator(ccNodes[i]), stlIterator(ccEdges[i]));
  }

  return true;
}
