/**
 *
 * Copyright (C) 2019-2021  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include <talipot/BooleanProperty.h>
#include <talipot/DoubleProperty.h>
#include <talipot/GraphTools.h>

#include "DFS/DFS.h"

#define SMALLEST_WEIGHT 1.E-6

using namespace tlp;
using namespace std;

double computePathLength(BooleanProperty *result, EdgeVectorProperty<double> &weights) {
  double retVal(0);
  Graph *graph(result->getGraph());
  auto *ite = result->getNonDefaultValuatedEdges(graph);
  while (ite->hasNext()) {
    retVal += weights.getEdgeValue(ite->next());
  }
  delete ite;
  return retVal;
}

bool PathAlgorithm::computePath(Graph *graph, PathType pathType, EdgeOrientation edgesOrientation,
                                node src, node tgt, BooleanProperty *result,
                                DoubleProperty *weights, double tolerance) {
#ifndef NDEBUG
  assert(graph);
  assert(result);

  if (weights)
    assert(result->getGraph() == weights->getGraph());

  assert(graph->isElement(src));
  assert(graph->isElement(tgt));
  assert(src != tgt);
#endif /* NDEBUG */

  bool retVal = false;
  tlp::ShortestPathType spt;

  if (pathType == AllShortest) {
    switch (edgesOrientation) {
    case Directed:
      spt = ShortestPathType::AllDirectedPaths;
      break;
    case Undirected:
      spt = ShortestPathType::AllPaths;
      break;
    case Reversed:
    default:
      spt = ShortestPathType::AllReversedPaths;
    }
  } else {
    switch (edgesOrientation) {
    case Directed:
      spt = ShortestPathType::OneDirectedPath;
      break;
    case Undirected:
      spt = ShortestPathType::OnePath;
      break;
    case Reversed:
    default:
      spt = ShortestPathType::OneReversedPath;
    }
  }
  graph->push();
  retVal = selectShortestPaths(graph, src, tgt, spt, weights, result);
  if (pathType == AllPaths && retVal) {
    EdgeVectorProperty<double> eWeights(graph);

    if (!weights) {
      eWeights.setAll(SMALLEST_WEIGHT);
    } else {
      auto fn = [&](edge e, uint i) {
        double val(weights->getEdgeValue(e));

        eWeights[i] = val ? val : SMALLEST_WEIGHT;
      };
      TLP_PARALLEL_MAP_EDGES_AND_INDICES(graph, fn);
    }

    double pathLength;

    if (tolerance == DBL_MAX) {
      pathLength = DBL_MAX;
    } else {
      pathLength = computePathLength(result, eWeights);
      pathLength *= tolerance;
    }

    if (tolerance > 1) {
      // We only compute the other paths if the tolerance is greater than 1.
      // Meaning that the user doesn't want only the shortest path.
      result->setAllNodeValue(false);
      result->setAllEdgeValue(false);
      DFS d(graph, result, tgt, eWeights, edgesOrientation, pathLength);
      retVal = d.searchPaths(src);
    }
  }
  if (!retVal) {
    graph->pop();
  }
  return retVal;
}
