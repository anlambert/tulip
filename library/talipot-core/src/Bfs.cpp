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

#include <talipot/Bfs.h>
#include <talipot/BooleanProperty.h>

using namespace std;
using namespace tlp;

Bfs::Bfs(Graph *G, BooleanProperty *resultatAlgoSelection)
    : graph(G->addCloneSubGraph()), selectedNodes(), selectedEdges() {
  selectedNodes.setAll(false);
  selectedEdges.setAll(false);
  nbNodes = 0;

  node root;
  bool unselected = false;
  Iterator<node> *itn = resultatAlgoSelection->getNodesEqualTo(true);

  if (itn->hasNext()) {
    root = itn->next();

    if (!G->isElement(root)) {
      unselected = true;
    }
  } else {
    unselected = true;
  }

  delete itn;

  if (unselected) {
    root = graph->getOneNode();
  }

  resultatAlgoSelection->setNodeValue(root, true);
  selectedNodes.set(root.id, true);
  ++nbNodes;
  computeBfs(G, resultatAlgoSelection, root);
}

void Bfs::computeBfs(Graph *G, BooleanProperty *resultatAlgoSelection, node root) {
  unsigned int taille = G->numberOfNodes();
  unsigned int i = 0;
  vector<node> next_roots;
  next_roots.push_back(root);

  while (taille != nbNodes) {
    node r = next_roots[i];

    if (!G->isElement(r)) {
      tlp::error() << __PRETTY_FUNCTION__ << ": ERROR NODE R NOT IN G" << std::endl;
    }

    for (auto e : G->getInOutEdges(r)) {

      if (!selectedEdges.get(e.id)) {
        node tmp = G->opposite(e, r);

        if (!selectedNodes.get(tmp.id)) {
          selectedNodes.set(tmp.id, true);
          selectedEdges.set(e.id, true);
          next_roots.push_back(tmp);
          nbNodes++;
          resultatAlgoSelection->setNodeValue(tmp, true);
          resultatAlgoSelection->setEdgeValue(e, true);
        }
      }
    }

    ++i;
  }
}

Bfs::~Bfs() {}
