/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "tulip2ogdf/OGDFLayoutPluginBase.h"

#include <vector>
#include <tulip/DrawingTools.h>

using namespace std;

OGDFLayoutPluginBase::OGDFLayoutPluginBase(const tlp::PluginContext *context,
                                           ogdf::LayoutModule *ogdfLayoutAlgo)
    : tlp::LayoutAlgorithm(context), tlpToOGDF(nullptr), ogdfLayoutAlgo(ogdfLayoutAlgo) {
  // convert Tulip Graph to OGDF Graph including attributes
  if (graph)
    tlpToOGDF = new TulipToOGDF(graph, false);
}

OGDFLayoutPluginBase::~OGDFLayoutPluginBase() {
  delete tlpToOGDF;
  delete ogdfLayoutAlgo;
}

bool OGDFLayoutPluginBase::run() {

  ogdf::GraphAttributes &gAttributes = tlpToOGDF->getOGDFGraphAttr();

  beforeCall();

  try {
    // run the algorithm on the OGDF Graph with attributes
    callOGDFLayoutAlgorithm(gAttributes);
  } catch (PreconditionViolatedException &pvce) {
    std::string msg;

    switch (pvce.exceptionCode()) {
    case pvcSelfLoop:
      msg = "graph contains a self-loop";
      break;

    case pvcTreeHierarchies:
      msg = "hierarchies are not only trees";
      break;

    case pvcAcyclicHierarchies:
      msg = "hierarchies are not acyclic";
      break;

    case pvcSingleSource:
      msg = "graph has not a single source";
      break;

    case pvcUpwardPlanar:
      msg = "graph is not upward planar";
      break;

    case pvcTree:
      msg = "graph is not a rooted tree";
      break;

    case pvcForest:
      msg = "graph is not a rooted forest";
      break;

    case pvcOrthogonal:
      msg = "layout is not orthogonal";
      break;

    case pvcPlanar:
      msg = "graph is not planar";
      break;

    case pvcClusterPlanar:
      msg = "graph is not c-planar";
      break;

    case pvcNoCopy:
      msg = "graph is not a copy of the corresponding graph";
      break;

    case pvcConnected:
      msg = "graph is not connected";
      break;

    case pvcBiconnected:
      msg = "graph is not twoconnected";
      break;

    default:
      msg = "unknown error";
    }

    pluginProgress->setError(std::string("Error\n") + msg);
    return false;
  } catch (AlgorithmFailureException &afce) {
    std::string msg;

    switch (afce.exceptionCode()) {
    case afcIllegalParameter:
      msg = "function parameter is illegal";
      break;

    case afcNoFlow:
      msg = "min-cost flow could not find a legal flow";
      break;

    case afcSort:
      msg = "sequence not sorted";
      break;

    case afcLabel:
      msg = "labelling failed";
      break;

    case afcExternalFace:
      msg = "external face not correct";
      break;

    case afcForbiddenCrossing:
      msg = "crossing forbidden but necessary";
      break;

    case afcTimelimitExceeded:
      msg = "it took too long";
      break;

    case afcNoSolutionFound:
      msg = "couldn't solve the problem";
      break;

    default:
      msg = "unknown error";
    }

    pluginProgress->setError(std::string("Error\n") + msg);
    return false;
  }

  // retrieve nodes coordinates computed by the OGDF Layout Algorithm
  // and store them in the Tulip Layout Property
  for (const tlp::node &n : graph->nodes()) {
    tlp::Coord nodeCoord = tlpToOGDF->getNodeCoordFromOGDFGraphAttr(n);
    result->setNodeValue(n, nodeCoord);
  }

  // same operation as above but with edges
  for (const tlp::edge &e : graph->edges()) {
    vector<tlp::Coord> edgeCoord = tlpToOGDF->getEdgeCoordFromOGDFGraphAttr(e);
    result->setEdgeValue(e, edgeCoord);
  }

  afterCall();

  return true;
}

void OGDFLayoutPluginBase::callOGDFLayoutAlgorithm(ogdf::GraphAttributes &gAttributes) {
  ogdfLayoutAlgo->call(gAttributes);
}

void OGDFLayoutPluginBase::transposeLayoutVertically() {

  const vector<tlp::node> &nodes = graph->nodes();
  const vector<tlp::edge> &edges = graph->edges();

  tlp::BoundingBox graphBB =
      tlp::computeBoundingBox(nodes, edges, result, graph->getProperty<SizeProperty>("viewSize"),
                              graph->getProperty<DoubleProperty>("viewRotation"));
  float midY = (graphBB[0][1] + graphBB[1][1]) / 2.f;

  for (const tlp::node &n : nodes) {
    tlp::Coord nodeCoord = result->getNodeValue(n);
    nodeCoord[1] = midY - (nodeCoord[1] - midY);
    result->setNodeValue(n, nodeCoord);
  }

  for (const tlp::edge &e : edges) {
    std::vector<tlp::Coord> bends = result->getEdgeValue(e);

    if (bends.size()) {
      for (size_t i = 0; i < bends.size(); ++i) {
        bends[i][1] = midY - (bends[i][1] - midY);
      }

      result->setEdgeValue(e, bends);
    }
  }
}
