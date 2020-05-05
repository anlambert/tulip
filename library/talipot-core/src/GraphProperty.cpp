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

#if defined(__GNUC__) && __GNUC__ >= 4 && \
    ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#include <talipot/GraphProperty.h>

using namespace std;
using namespace tlp;

const string GraphProperty::propertyTypename = "graph";

//==============================
GraphProperty::GraphProperty(Graph *sg, const std::string &n)
    : AbstractProperty<GraphType, EdgeSetType>(sg, n) {
  setAllNodeValue(nullptr);
}
//==============================
GraphProperty::~GraphProperty() {
  if (graph) {

    for (auto n : graph->nodes()) {
      if (getNodeValue(n) != nullptr) {
        getNodeValue(n)->removeListener(this);
      }
    }

    if (getNodeDefaultValue() != nullptr) {
      getNodeDefaultValue()->removeListener(this);
    }
  }
}
//==============================
void GraphProperty::setAllNodeValue(tlp::StoredType<GraphType::RealType>::ReturnedConstValue g,
                                    const Graph *graph) {
  // remove all observed graphs if any
  for (auto n : getNonDefaultValuatedNodes(graph)) {
    getNodeValue(n)->removeListener(this);
  }

  set<node> emptySet;
  referencedGraph.setAll(emptySet);

  if ((!graph || getGraph() == graph) && getNodeDefaultValue() != nullptr) {
    getNodeDefaultValue()->removeListener(this);
  }

  AbstractGraphProperty::setAllNodeValue(g, graph);

  if (g != nullptr) {
    g->addListener(this);
  }
}
//==============================
void GraphProperty::setNodeValue(const node n,
                                 tlp::StoredType<GraphType::RealType>::ReturnedConstValue sg) {
  // gestion désabonnement
  Graph *oldGraph = getNodeValue(n);

  if (oldGraph != nullptr && oldGraph != sg) {
    // gestion du désabonnement
    bool notDefault;
    set<node> &refs = referencedGraph.get(oldGraph->getId(), notDefault);

    if (notDefault) {
      refs.erase(n);

      if (refs.empty()) {
        if (oldGraph != getNodeDefaultValue()) {
          oldGraph->removeListener(this);
        }

        referencedGraph.set(oldGraph->getId(), set<node>());
      }
    } else if (oldGraph != getNodeDefaultValue()) {
      oldGraph->removeListener(this);
    }
  }

  AbstractGraphProperty::setNodeValue(n, sg);

  if (sg == nullptr || oldGraph == sg) {
    return;
  }

  // Gestion de l'abonnement
  sg->addListener(this);

  if (sg != getNodeDefaultValue()) {
    bool notDefault;
    set<node> &refs = referencedGraph.get(sg->getId(), notDefault);

    if (notDefault) {
      refs.insert(n);
    } else {
      set<node> newSet;
      newSet.insert(n);
      referencedGraph.set(sg->getId(), newSet);
    }
  }
}
//============================================================
PropertyInterface *GraphProperty::clonePrototype(Graph *g, const std::string &n) const {
  if (!g) {
    return nullptr;
  }

  // allow to get an unregistered property (empty name)
  GraphProperty *p = n.empty() ? new GraphProperty(g) : g->getLocalGraphProperty(n);

  p->setAllNodeValue(getNodeDefaultValue());
  p->setAllEdgeValue(getEdgeDefaultValue());
  return p;
}
//=============================================================
// disabled, use setNodeValue instead
bool GraphProperty::setNodeStringValue(const node, const std::string &) {
  return false;
}
//=============================================================
// disabled use setAllNodeValue instead
bool GraphProperty::setAllNodeStringValue(const std::string &, const Graph *) {
  return false;
}
//=============================================================
// disabled, use setEdgeValue instead
bool GraphProperty::setEdgeStringValue(const edge, const std::string &) {
  return false;
}
//=============================================================
// disabled use setAllEdgeValue instead
bool GraphProperty::setAllEdgeStringValue(const std::string &, const Graph *) {
  return false;
}
//=============================================================
const set<edge> &GraphProperty::getReferencedEdges(const edge e) const {
  return const_cast<GraphProperty *>(this)->edgeProperties.get(e.id);
}
//=============================================================
void GraphProperty::treatEvent(const Event &evt) {
  if (evt.type() == Event::TLP_DELETE) {

    Graph *sg = static_cast<Graph *>(evt.sender());

#ifndef NDEBUG
    tlp::warning() << "Talipot Warning : A graph pointed by metanode(s) has been deleted, the "
                      "metanode(s) pointer has been set to zero in order to prevent segmentation "
                      "fault"
                   << std::endl;
#endif

    if (getNodeDefaultValue() == sg) {
      // we must backup old value
      MutableContainer<Graph *> backup;
      backup.setAll(nullptr);

      for (auto n : graph->nodes()) {
        if (getNodeValue(n) != sg) {
          backup.set(n.id, getNodeValue(n));
        }
      }

      setAllNodeValue(nullptr);

      // restore values
      for (auto n : graph->nodes()) {
        setNodeValue(n, backup.get(n.id));
      }
    }

    const set<node> &refs = referencedGraph.get(sg->getId());

    if (!refs.empty()) {
      // don't change values if this non longer exists (when undoing)
      if (graph->existProperty(name)) {
        for (auto n : refs) {
          AbstractGraphProperty::setNodeValue(n, nullptr);
        }
      }

      referencedGraph.set(sg->getId(), set<node>());
    }
  }
}

bool GraphProperty::readNodeDefaultValue(std::istream &iss) {
  // must read 0 (see GraphType::writeb)
  unsigned int id = 0;

  if (!bool(iss.read(reinterpret_cast<char *>(&id), sizeof(id)))) {
    return false;
  }

  assert(id == 0);
  return id == 0;
}

bool GraphProperty::readNodeValue(std::istream &iss, node n) {
  // must read the id of a subgraph
  unsigned int id = 0;

  if (!bool(iss.read(reinterpret_cast<char *>(&id), sizeof(id)))) {
    return false;
  }

  Graph *sg = graph->getRoot()->getDescendantGraph(id);
  setNodeValue(n, sg);
  return true;
}
