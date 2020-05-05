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

#include "DFS.h"

#include <stack>
#include <talipot/BooleanProperty.h>
#include <talipot/DoubleProperty.h>

using namespace tlp;
using namespace std;

DFS::DFS(Graph *graph, BooleanProperty *result, node tgt,
         const EdgeStaticProperty<double> &eWeights, EdgeOrientation edgesOrientation,
         double maxDist)
    : graph(graph), result(result), tgt(tgt), weights(eWeights), currentDist(0),
      edgesOrientation(edgesOrientation), maxDist(maxDist) {
#ifndef NDEBUG
  assert(graph->getRoot() == result->getGraph()->getRoot());
#endif /* NDEBUG */
}

bool DFS::searchPaths(node src) {
  DoubleProperty dists(result->getGraph());
  dists.setAllNodeValue(DBL_MAX);

  BooleanProperty visitable(graph);
  visitable.setAllNodeValue(true);

  return computeSearchPaths(src, &visitable, &dists);
}

bool DFS::computeSearchPaths(node src, BooleanProperty *visitable, DoubleProperty *dists) {
  if (!visitable->getNodeValue(src)) {
    return false;
  }

  if (dists->getNodeValue(src) != DBL_MAX && currentDist + dists->getNodeValue(src) > maxDist) {
    return false;
  }

  if (currentDist > maxDist) {
    return false;

  } else if (src == tgt || result->getNodeValue(src)) {
    double distLeft(0);

    if (result->getNodeValue(src)) {
      distLeft = dists->getNodeValue(src);
    }

    node nd(src);

    for (auto e : reversed(path)) {
      node opposite(graph->opposite(e, nd));
      result->setEdgeValue(e, true);
      result->setNodeValue(opposite, true);
      result->setNodeValue(nd, true);
      dists->setNodeValue(nd, min<double>(distLeft, dists->getNodeValue(nd)));
      distLeft += weights.getEdgeValue(e);
      nd = opposite;
    }

    dists->setNodeValue(nd, min<double>(distLeft, dists->getNodeValue(nd)));

    return true;
  }

  bool res = false;
  visitable->setNodeValue(src, false);

  Iterator<edge> *edgeIt = nullptr;

  switch (edgesOrientation) {
  case Undirected:
    edgeIt = graph->getInOutEdges(src);
    break;

  case Directed:
    edgeIt = graph->getOutEdges(src);
    break;

  case Reversed:
    edgeIt = graph->getInEdges(src);
    break;
  }

  for (auto e : edgeIt) {
    currentDist += weights.getEdgeValue(e);
    path.push_back(e);
    res |= computeSearchPaths(graph->opposite(e, src), visitable, dists);
    path.pop_back();
    currentDist -= weights.getEdgeValue(e);
  }

  visitable->setNodeValue(src, true);
  return res;
}
