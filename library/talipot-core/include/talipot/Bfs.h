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

#ifndef TALIPOT_BFS_H
#define TALIPOT_BFS_H

#include <talipot/MutableContainer.h>
#include <talipot/Node.h>

namespace tlp {
class Graph;
class BooleanProperty;
class TLP_SCOPE Bfs {
public:
  Bfs(Graph *G, BooleanProperty *resultatAlgoSelection);
  ~Bfs();

  Graph *graph;

private:
  void computeBfs(Graph *G, BooleanProperty *resultatAlgoSelection, node root);

  unsigned int nbNodes;
  MutableContainer<bool> selectedNodes;
  MutableContainer<bool> selectedEdges;
};
}
#endif // TALIPOT_BFS_H
