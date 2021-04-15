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

#ifndef DFS_H
#define DFS_H

#include <cfloat>
#include <vector>

#include <talipot/Node.h>
#include <talipot/Edge.h>
#include <talipot/VectorProperty.h>

#include "../PathAlgorithm.h"

namespace tlp {
class BooleanProperty;
class DoubleProperty;
class Graph;

/**
 * @brief An implementation of the DFS algorithm to find all the existing paths between two nodes.
 */
class DFS : public PathAlgorithm {
public:
  /**
   * Initiate the DFS algorithm parameters.
   * @param result The boolean property where to store the results.
   * @param dists A working double property where distances between nodes and the target node will
   * be stored.
   * @param tgt the target node.
   * @param weights the edges weights.
   * @param edgesOrientation The edge orientation policy.
   * @param maxDist The maximal distance the algorithm can go from the source node before dropping
   * the search (DBL_MAX by default)
   */
  DFS(tlp::Graph *graph, tlp::BooleanProperty *result, tlp::node tgt,
      const tlp::EdgeVectorProperty<double> &weights, EdgeOrientation edgesOrientation,
      double maxDist = DBL_MAX);

  /**
   * Compute the path between the source node and the target node. Caution ! This method is
   * recursive and could cause a stack overflow on big graphs.
   * @param src The source node.
   * @return true if a path has been found.
   */
  bool searchPaths(tlp::node src);

private:
  tlp::Graph *graph;
  tlp::BooleanProperty *result;
  tlp::node tgt;
  const tlp::EdgeVectorProperty<double> &weights;
  std::vector<tlp::edge> path;
  double currentDist;
  EdgeOrientation edgesOrientation;
  double maxDist;

  bool computeSearchPaths(tlp::node src, tlp::BooleanProperty *visitable,
                          tlp::DoubleProperty *dists);
};
}
#endif // DFS_H
