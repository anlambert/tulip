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

#include <stack>

#include <talipot/StableIterator.h>
#include <talipot/BooleanProperty.h>
#include <talipot/Graph.h>
#include <talipot/GraphIterators.h>
#include <talipot/GraphView.h>
#include <talipot/PropertyManager.h>
#include <talipot/ConversionIterator.h>

using namespace std;
namespace tlp {
//----------------------------------------------------------------
GraphView::GraphView(Graph *supergraph, BooleanProperty *filter, unsigned int sgId)
    : GraphAbstract(supergraph, sgId) {
  _nodeData.setAll(nullptr);

  if (filter == nullptr) {
    return;
  }

  if ((filter->getGraph() == supergraph) && (filter->getNodeDefaultValue() == true) &&
      (filter->numberOfNonDefaultValuatedNodes() == 0)) {
    // clone all supergraph nodes
    _nodes.clone(supergraph->nodes());
    unsigned int nbNodes = _nodes.size();

    for (unsigned int i = 0; i < nbNodes; ++i) {
      _nodeData.set(_nodes[i], new SGraphNodeData());
    }
  } else {
    Iterator<unsigned int> *it = nullptr;
    it = filter->nodeProperties.findAll(true);

    Iterator<node> *iteN = nullptr;

    if (it == nullptr) {
      Graph *graphToFilter = filter->getGraph();

      if (graphToFilter == nullptr) {
        graphToFilter = supergraph;
      }

      iteN = graphToFilter->getNodes();
    } else {
      iteN = conversionIterator<node>(it, idToNode);
    }

    while (iteN->hasNext()) {
      auto n = iteN->next();
      if (filter->getNodeValue(n)) {
        addNode(n);
      }
    }
    delete iteN;
  }

  if ((filter->getGraph() == supergraph) && (filter->getEdgeDefaultValue() == true) &&
      (filter->numberOfNonDefaultValuatedEdges() == 0)) {
    // clone all supergraph edges
    _edges.clone(supergraph->edges());
    // and degrees of nodes
    for (auto n : _nodes) {
      SGraphNodeData *nData = _nodeData.get(n.id);
      nData->outDegree = supergraph->outdeg(n);
      nData->inDegree = supergraph->indeg(n);
    }
  } else {
    Iterator<unsigned int> *it = nullptr;
    it = filter->edgeProperties.findAll(true);

    Iterator<edge> *itE = nullptr;

    if (it == nullptr) {
      Graph *graphToFilter = filter->getGraph();

      if (graphToFilter == nullptr) {
        graphToFilter = supergraph;
      }

      itE = graphToFilter->getEdges();
    } else {
      itE = conversionIterator<edge>(it, idToEdge);
    }

    while (itE->hasNext()) {
      auto e = itE->next();
      if (filter->getEdgeValue(e)) {
        addEdge(e);
      }
    }
    delete itE;
  }
}
//----------------------------------------------------------------
GraphView::~GraphView() {
  // notify destruction
  observableDeleted();
}
//----------------------------------------------------------------
edge GraphView::existEdge(const node src, const node tgt, bool directed) const {
  if (!isElement(src) || !isElement(tgt)) {
    return edge();
  }

  std::vector<edge> ee = getRootImpl()->getEdges(src, tgt, directed, this);
  return !ee.empty() ? ee[0] : edge();
}
//----------------------------------------------------------------
void GraphView::reverseInternal(const edge e, const node src, const node tgt) {
  if (isElement(e)) {
    SGraphNodeData *srcData = _nodeData.get(src.id);
    SGraphNodeData *tgtData = _nodeData.get(tgt.id);
    srcData->outDegreeAdd(-1);
    srcData->inDegreeAdd(1);
    tgtData->inDegreeAdd(-1);
    tgtData->outDegreeAdd(1);

    notifyReverseEdge(e);

    // propagate edge reversal on subgraphs
    for (Graph *sg : subGraphs()) {
      static_cast<GraphView *>(sg)->reverseInternal(e, src, tgt);
    }
  }
}
//----------------------------------------------------------------
void GraphView::setEndsInternal(const edge e, node src, node tgt, const node newSrc,
                                const node newTgt) {
  if (isElement(e)) {
    if (isElement(newSrc) && isElement(newTgt)) {
      notifyBeforeSetEnds(e);

      if (src != newSrc) {
        _nodeData.get(newSrc.id)->outDegreeAdd(1);

        if (src.isValid() && isElement(src)) {
          _nodeData.get(src.id)->outDegreeAdd(-1);
        } else {
          // as src may no longer exist (pop case)
          // set src as invalid for subgraphs loop
          src = node();
        }
      }

      if (tgt != newTgt) {
        _nodeData.get(newTgt.id)->inDegreeAdd(1);

        if (tgt.isValid() && isElement(tgt)) {
          _nodeData.get(tgt.id)->inDegreeAdd(-1);
        } else {
          // as tgt may no longer exist (pop case)
          // set tgt as invalid for subgraphs loop
          tgt = node();
        }
      }

      // notification
      notifyAfterSetEnds(e);

      // propagate edge ends update on subgraphs
      for (Graph *sg : subGraphs()) {
        static_cast<GraphView *>(sg)->setEndsInternal(e, src, tgt, newSrc, newTgt);
      }
    } else {
      // delete e if its new ends do no belong to the graph
      // propagate edge ends update on subgraphs
      for (Graph *sg : subGraphs()) {
        static_cast<GraphView *>(sg)->setEndsInternal(e, src, tgt, newSrc, newTgt);
      }
      notifyDelEdge(e);

      _edges.remove(e);
      propertyContainer->erase(e);
      _nodeData.get(src.id)->outDegreeAdd(-1);
      _nodeData.get(tgt.id)->inDegreeAdd(-1);
    }
  }
}
//----------------------------------------------------------------
node GraphView::addNode() {
  node tmp = getSuperGraph()->addNode();
  restoreNode(tmp);
  return tmp;
}
//----------------------------------------------------------------
void GraphView::addNodes(unsigned int nb) {
  getSuperGraph()->addNodes(nb);
  addNodesInternal(nb, nullptr);
}
//----------------------------------------------------------------
void GraphView::addNodes(unsigned int nb, std::vector<node> &addedNodes) {
  getSuperGraph()->addNodes(nb, addedNodes);
  addNodesInternal(nb, &addedNodes);
}
//----------------------------------------------------------------
void GraphView::restoreNode(node n) {
  _nodeData.set(n.id, new SGraphNodeData());
  _nodes.add(n);
  notifyAddNode(n);
}
//----------------------------------------------------------------
void GraphView::addNodesInternal(unsigned int nbAdded, const std::vector<node> *nodes) {
  _nodes.reserve(_nodes.size() + nbAdded);

  std::vector<node>::const_iterator it;

  if (nodes) {
    it = nodes->begin();
  } else {
    nodes = &getSuperGraph()->nodes();
    it = nodes->begin() + nodes->size() - nbAdded;
  }

  std::vector<node>::const_iterator ite = nodes->end();

  for (; it != ite; ++it) {
    node n(*it);
    assert(getRootImpl()->isElement(n));
    _nodeData.set(n.id, new SGraphNodeData());
    _nodes.add(n);
  }

  if (hasOnlookers()) {
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODES, nbAdded));
  }
}
//----------------------------------------------------------------
void GraphView::addNode(const node n) {
  assert(getRoot()->isElement(n));

  if (!isElement(n)) {
    if (!getSuperGraph()->isElement(n)) {
      getSuperGraph()->addNode(n);
    }

    restoreNode(n);
  }
}
//----------------------------------------------------------------
void GraphView::addNodes(Iterator<node> *addedNodes) {
  std::vector<node> nodes;
  std::vector<node> superNodes;
  Graph *super = getSuperGraph();
  bool superIsRoot = (super == getRoot());

  for (auto n : addedNodes) {
    if (!isElement(n)) {
      nodes.push_back(n);

      if (!superIsRoot && !super->isElement(n)) {
        superNodes.push_back(n);
      }
    }
  }

  if (!superNodes.empty()) {
    super->addNodes(stlIterator(superNodes));
  }

  if (!nodes.empty()) {
    addNodesInternal(nodes.size(), &nodes);
  }
}
//----------------------------------------------------------------
edge GraphView::addEdgeInternal(edge e) {
  _edges.add(e);
  auto eEnds = ends(e);
  node src = eEnds.first;
  node tgt = eEnds.second;
  _nodeData.get(src.id)->outDegreeAdd(1);
  _nodeData.get(tgt.id)->inDegreeAdd(1);
  notifyAddEdge(e);
  return e;
}
//----------------------------------------------------------------
void GraphView::restoreEdge(edge e, const node, const node) {
  addEdgeInternal(e);
}
//----------------------------------------------------------------
void GraphView::addEdgesInternal(unsigned int nbAdded, const std::vector<edge> *ee,
                                 const std::vector<std::pair<node, node>> &ends) {
  _edges.reserve(_edges.size() + nbAdded);

  bool hasEnds = !ends.empty();

  unsigned int i = 0;
  std::vector<edge>::const_iterator it;

  if (ee) {
    it = ee->begin();
  } else {
    ee = &getSuperGraph()->edges();
    it = ee->begin() + ee->size() - nbAdded;
  }

  std::vector<edge>::const_iterator ite = ee->end();

  for (; it != ite; ++it, ++i) {
    edge e = *it;
    assert(getRootImpl()->isElement(e));
    _edges.add(e);
    auto eEnds = hasEnds ? ends[i] : this->ends(e);
    node src = eEnds.first;
    node tgt = eEnds.second;
    _nodeData.get(src.id)->outDegreeAdd(1);
    _nodeData.get(tgt.id)->inDegreeAdd(1);
  }

  if (hasOnlookers()) {
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGES, nbAdded));
  }
}
//----------------------------------------------------------------
edge GraphView::addEdge(const node n1, const node n2) {
  assert(isElement(n1));
  assert(isElement(n2));
  return addEdgeInternal(getSuperGraph()->addEdge(n1, n2));
}
//----------------------------------------------------------------
void GraphView::addEdge(const edge e) {
  assert(getRootImpl()->isElement(e));
  assert(isElement(source(e)));
  assert(isElement(target(e)));

  if (!isElement(e)) {
    if (!getSuperGraph()->isElement(e)) {
      getSuperGraph()->addEdge(e);
    }

    addEdgeInternal(e);
  }
}
//----------------------------------------------------------------
void GraphView::addEdges(const std::vector<std::pair<node, node>> &ends) {
  getSuperGraph()->addEdges(ends);
  addEdgesInternal(ends.size(), nullptr, ends);
}
//----------------------------------------------------------------
void GraphView::addEdges(const std::vector<std::pair<node, node>> &ends,
                         std::vector<edge> &addedEdges) {
  getSuperGraph()->addEdges(ends, addedEdges);
  addEdgesInternal(ends.size(), &addedEdges, ends);
}
//----------------------------------------------------------------
void GraphView::addEdges(Iterator<edge> *addedEdges) {
  std::vector<edge> ee;
  std::vector<edge> superEdges;
  Graph *super = getSuperGraph();
  bool superIsRoot = (super == getRoot());

  for (auto e : addedEdges) {
    assert(getRootImpl()->isElement(e));
    assert(isElement(source(e)));
    assert(isElement(target(e)));

    if (!isElement(e)) {
      ee.push_back(e);

      if (!superIsRoot && !super->isElement(e)) {
        superEdges.push_back(e);
      }
    }
  }

  if (!superEdges.empty()) {
    super->addEdges(stlIterator(superEdges));
  }

  if (!ee.empty()) {
    addEdgesInternal(ee.size(), &ee, std::vector<pair<node, node>>());
  }
}
//----------------------------------------------------------------
void GraphView::removeNode(const node n) {
  assert(isElement(n));
  notifyDelNode(n);
  _nodeData.set(n.id, nullptr);
  _nodes.remove(n);
  propertyContainer->erase(n);
}
//----------------------------------------------------------------
void GraphView::removeNode(const node n, const std::vector<edge> &ee) {
  removeEdges(ee);
  removeNode(n);
}
//----------------------------------------------------------------
void GraphView::delNode(const node n, bool deleteInAllGraphs) {
  if (deleteInAllGraphs) {
    getRootImpl()->delNode(n, true);
  } else {
    assert(isElement(n));

    // get edges vector
    std::vector<edge> ee(this->allEdges(n));

    // use a stack for a dfs subgraphs propagation
    std::stack<Graph *> sgq;

    for (Graph *sg : subGraphs()) {

      if (sg->isElement(n)) {
        sgq.push(sg);
      }
    }

    // subgraphs loop
    while (!sgq.empty()) {
      Graph *sg = sgq.top();

      for (Graph *ssg : sg->subGraphs()) {
        if (ssg->isElement(n)) {
          sgq.push(ssg);
        }
      }

      if (sg == sgq.top()) {
        static_cast<GraphView *>(sg)->removeNode(n, ee);
        sgq.pop();
      }
    }

    removeNode(n, ee);
  }
}
//----------------------------------------------------------------
void GraphView::removeEdge(const edge e) {
  assert(isElement(e));
  notifyDelEdge(e);

  _edges.remove(e);
  propertyContainer->erase(e);
  const std::pair<node, node> &eEnds = ends(e);
  node src = eEnds.first;
  node tgt = eEnds.second;
  _nodeData.get(src.id)->outDegreeAdd(-1);
  _nodeData.get(tgt.id)->inDegreeAdd(-1);
}
//----------------------------------------------------------------
void GraphView::removeEdges(const std::vector<edge> &ee) {
  for (auto e : ee) {
    if (isElement(e)) {
      removeEdge(e);
    }
  }
}
//----------------------------------------------------------------
void GraphView::delEdge(const edge e, bool deleteInAllGraphs) {
  if (deleteInAllGraphs) {
    getRootImpl()->delEdge(e, true);
  } else {
    assert(isElement(e));
    // propagate to subgraphs
    for (Graph *subGraph : subGraphs()) {
      if (subGraph->isElement(e)) {
        subGraph->delEdge(e);
      }
    }

    removeEdge(e);
  }
}
//----------------------------------------------------------------
Iterator<node> *GraphView::getNodes() const {
  return stlIterator(_nodes);
}
//----------------------------------------------------------------
Iterator<node> *GraphView::getInNodes(const node n) const {
  return new InNodesIterator(this, n);
}
//----------------------------------------------------------------
Iterator<node> *GraphView::getOutNodes(const node n) const {
  return new OutNodesIterator(this, n);
}
//----------------------------------------------------------------
Iterator<node> *GraphView::getInOutNodes(const node n) const {
  return new InOutNodesIterator(this, n);
}
//----------------------------------------------------------------
Iterator<edge> *GraphView::getEdges() const {
  return stlIterator(_edges);
}
//----------------------------------------------------------------
Iterator<edge> *GraphView::getInEdges(const node n) const {
  return new InEdgesIterator(this, n);
}
//----------------------------------------------------------------
Iterator<edge> *GraphView::getOutEdges(const node n) const {
  return new OutEdgesIterator(this, n);
}
//----------------------------------------------------------------
Iterator<edge> *GraphView::getInOutEdges(const node n) const {
  return new InOutEdgesIterator(this, n);
}
//----------------------------------------------------------------
std::vector<edge> GraphView::getEdges(const node src, const node tgt, bool directed) const {
  std::vector<edge> ee;

  if (isElement(src) && isElement(tgt)) {
    ee = getRootImpl()->getEdges(src, tgt, directed, this);
  }

  return ee;
}
//----------------------------------------------------------------
void GraphView::reserveNodes(unsigned int) {
#ifndef NDEBUG
  tlp::warning() << "Warning: " << __PRETTY_FUNCTION__ << " ... Impossible operation on a subgraph"
                 << std::endl;
#endif
}
//----------------------------------------------------------------
void GraphView::reserveEdges(unsigned int) {
#ifndef NDEBUG
  tlp::warning() << "Warning: " << __PRETTY_FUNCTION__ << " ... Impossible operation on a subgraph"
                 << std::endl;
#endif
}
//----------------------------------------------------------------
bool GraphView::canPop() {
  return getRootImpl()->canPop();
}
//----------------------------------------------------------------
bool GraphView::canUnpop() {
  return getRootImpl()->canUnpop();
}
//----------------------------------------------------------------
bool GraphView::canPopThenUnpop() {
  return getRootImpl()->canPopThenUnpop();
}
//----------------------------------------------------------------
void GraphView::push(bool unpopAllowed,
                     std::vector<PropertyInterface *> *propertiesToPreserveOnPop) {
  getRootImpl()->push(unpopAllowed, propertiesToPreserveOnPop);
}
//----------------------------------------------------------------
void GraphView::pop(bool unpopAllowed) {
  getRootImpl()->pop(unpopAllowed);
}
//----------------------------------------------------------------
void GraphView::popIfNoUpdates() {
  getRootImpl()->popIfNoUpdates();
}
//----------------------------------------------------------------
void GraphView::unpop() {
  getRootImpl()->unpop();
}
}
