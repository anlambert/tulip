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

#include <cstdlib>

#include <talipot/BasicIterators.h>
#include <talipot/ConversionIterator.h>

template <class NodeType, class EdgeType, class PropType>
tlp::AbstractProperty<NodeType, EdgeType, PropType>::AbstractProperty(tlp::Graph *sg,
                                                                      const std::string &n) {
  PropType::graph = sg;
  PropType::name = n;
  nodeDefaultValue = NodeType::defaultValue();
  edgeDefaultValue = EdgeType::defaultValue();
  nodeProperties.setAll(NodeType::defaultValue());
  edgeProperties.setAll(EdgeType::defaultValue());
  PropType::metaValueCalculator = nullptr;
}
//=============================================================
template <class NodeType, class EdgeType, class PropType>
typename NodeType::RealType
tlp::AbstractProperty<NodeType, EdgeType, PropType>::getNodeDefaultValue() const {
  return nodeDefaultValue;
}
//=============================================================
template <class NodeType, class EdgeType, class PropType>
typename EdgeType::RealType
tlp::AbstractProperty<NodeType, EdgeType, PropType>::getEdgeDefaultValue() const {
  return edgeDefaultValue;
}
//=============================================================
template <class NodeType, class EdgeType, class PropType>
typename tlp::StoredType<typename NodeType::RealType>::ReturnedConstValue
tlp::AbstractProperty<NodeType, EdgeType, PropType>::getNodeValue(const tlp::node n) const {
  assert(n.isValid());
  return nodeProperties.get(n.id);
}
//=============================================================
template <class NodeType, class EdgeType, class PropType>
typename tlp::StoredType<typename EdgeType::RealType>::ReturnedConstValue
tlp::AbstractProperty<NodeType, EdgeType, PropType>::getEdgeValue(const tlp::edge e) const {
  assert(e.isValid());
  return edgeProperties.get(e.id);
}
//=================================================================================
template <class NodeType, class EdgeType, class PropType>
tlp::Iterator<tlp::node> *tlp::AbstractProperty<NodeType, EdgeType, PropType>::getNodesEqualTo(
    typename tlp::StoredType<typename NodeType::RealType>::ReturnedConstValue val,
    const Graph *sg) const {
  if (sg == nullptr) {
    sg = this->graph;
  }

  tlp::Iterator<unsigned int> *it = nullptr;

  if (sg == this->graph) {
    it = nodeProperties.findAll(val);
  }

  if (it == nullptr) {
    return new tlp::SGraphNodeIterator<typename NodeType::RealType>(sg, nodeProperties, val);
  }

  return tlp::conversionIterator<tlp::node>(it, idToNode);
}
//=================================================================================
template <class NodeType, class EdgeType, class PropType>
tlp::Iterator<tlp::edge> *tlp::AbstractProperty<NodeType, EdgeType, PropType>::getEdgesEqualTo(
    typename tlp::StoredType<typename EdgeType::RealType>::ReturnedConstValue val,
    const Graph *sg) const {
  if (sg == nullptr) {
    sg = this->graph;
  }

  tlp::Iterator<unsigned int> *it = nullptr;

  if (sg == this->graph) {
    it = edgeProperties.findAll(val);
  }

  if (it == nullptr) {
    return new tlp::SGraphEdgeIterator<typename EdgeType::RealType>(sg, edgeProperties, val);
  }

  return tlp::conversionIterator<tlp::edge>(it, idToEdge);
}
//=============================================================
template <class NodeType, class EdgeType, class PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::setNodeValue(
    const tlp::node n,
    typename tlp::StoredType<typename NodeType::RealType>::ReturnedConstValue v) {
  assert(n.isValid());
  PropType::notifyBeforeSetNodeValue(n);
  nodeProperties.set(n.id, v);
  PropType::notifyAfterSetNodeValue(n);
}
//=============================================================
template <class NodeType, class EdgeType, class PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::setEdgeValue(
    const tlp::edge e,
    typename tlp::StoredType<typename EdgeType::RealType>::ReturnedConstValue v) {
  assert(e.isValid());
  PropType::notifyBeforeSetEdgeValue(e);
  edgeProperties.set(e.id, v);
  PropType::notifyAfterSetEdgeValue(e);
}
//=============================================================
template <class NodeType, class EdgeType, class PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::setNodeDefaultValue(
    typename tlp::StoredType<typename NodeType::RealType>::ReturnedConstValue v) {
  if (nodeDefaultValue == v) {
    return;
  }

  // backup old default value
  auto oldDefaultValue = nodeDefaultValue;
  // we need to get the list of nodes whose value equals the current default one first
  std::vector<tlp::node> nodesOldDefaultToUpdate;
  std::vector<tlp::node> nodesDefaultToUpdate;

  for (auto n : this->getGraph()->nodes()) {
    auto val = nodeProperties.get(n.id);

    if (val == oldDefaultValue) {
      nodesOldDefaultToUpdate.push_back(n);
    } else if (val == v) {
      nodesDefaultToUpdate.push_back(n);
    }
  }

  // set new default value that will be associated to future added nodes
  nodeDefaultValue = v;
  nodeProperties.setDefault(v);

  // reset the backup nodes to the old default value as there is a new one in the
  // underlying MutableContainer
  for (auto n : nodesOldDefaultToUpdate) {
    nodeProperties.set(n.id, oldDefaultValue);
  }

  // reset the backup nodes to their current value in order
  // to synchronize the underlying MutableContainer state
  for (auto n : nodesDefaultToUpdate) {
    nodeProperties.set(n.id, v, true);
  }
}
//=============================================================
template <class NodeType, class EdgeType, class PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::setAllNodeValue(
    typename tlp::StoredType<typename NodeType::RealType>::ReturnedConstValue v,
    const Graph *graph) {
  if (graph && this->getGraph()->isDescendantGraph(graph)) {
    for (auto n : graph->nodes()) {
      setNodeValue(n, v);
    }
  } else {
    PropType::notifyBeforeSetAllNodeValue();
    nodeDefaultValue = v;
    nodeProperties.setAll(v);
    PropType::notifyAfterSetAllNodeValue();
  }
}
//=============================================================
template <class NodeType, class EdgeType, class PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::setEdgeDefaultValue(
    typename tlp::StoredType<typename EdgeType::RealType>::ReturnedConstValue v) {
  if (edgeDefaultValue == v) {
    return;
  }

  // backup old default value
  auto oldDefaultValue = edgeDefaultValue;
  // backup list of edges whose value equals the current default one
  std::vector<tlp::edge> edgesOldDefaultToUpdate;
  // backup list of edges whose value equals the new default one
  std::vector<tlp::edge> edgesDefaultToUpdate;

  for (auto e : this->getGraph()->edges()) {
    auto val = edgeProperties.get(e.id);

    if (val == oldDefaultValue) {
      edgesOldDefaultToUpdate.push_back(e);
    } else if (val == v) {
      edgesDefaultToUpdate.push_back(e);
    }
  }

  // set new default value that will be associated to future added edges
  edgeDefaultValue = v;
  edgeProperties.setDefault(v);

  // reset the backup edges to the old default value as there is a new one in the
  // underlying MutableContainer
  for (auto e : edgesOldDefaultToUpdate) {
    edgeProperties.set(e.id, oldDefaultValue);
  }

  // reset the backup edges to their current value in order
  // to synchronize the underlying MutableContainer state
  for (auto e : edgesDefaultToUpdate) {
    edgeProperties.set(e.id, v, true);
  }
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::setAllEdgeValue(
    typename tlp::StoredType<typename EdgeType::RealType>::ReturnedConstValue v,
    const Graph *graph) {
  if (graph && this->getGraph()->isDescendantGraph(graph)) {
    for (auto e : graph->edges()) {
      setEdgeValue(e, v);
    }
  } else {
    PropType::notifyBeforeSetAllEdgeValue();
    edgeDefaultValue = v;
    edgeProperties.setAll(v);
    PropType::notifyAfterSetAllEdgeValue();
  }
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
int tlp::AbstractProperty<NodeType, EdgeType, PropType>::compare(const node n1,
                                                                 const node n2) const {
  const typename NodeType::RealType &n1Value = getNodeValue(n1);
  const typename NodeType::RealType &n2Value = getNodeValue(n2);
  return (n1Value < n2Value) ? -1 : ((n1Value == n2Value) ? 0 : 1);
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
int tlp::AbstractProperty<NodeType, EdgeType, PropType>::compare(const edge e1,
                                                                 const edge e2) const {
  const typename EdgeType::RealType &e1Value = getEdgeValue(e1);
  const typename EdgeType::RealType &e2Value = getEdgeValue(e2);
  return (e1Value < e2Value) ? -1 : ((e1Value == e2Value) ? 0 : 1);
}
//============================================================
// define template iterator class to iterate over graph elts
// belonging to a given graph instance
// used by the two methods below
template <typename ELT_TYPE>
class GraphEltIterator : public tlp::Iterator<ELT_TYPE> {
public:
  ELT_TYPE next() {
    ELT_TYPE tmp = curElt;

    if ((_hasnext = it->hasNext())) {
      curElt = it->next();

      while (!(_hasnext = (!graph || graph->isElement(curElt)))) {
        if (!it->hasNext()) {
          break;
        }

        curElt = it->next();
      }
    }

    return tmp;
  }
  GraphEltIterator(const tlp::Graph *g, tlp::Iterator<ELT_TYPE> *itN)
      : it(itN), graph(g), curElt(ELT_TYPE()), _hasnext(false) {
    next();
  }

  bool hasNext() {
    return (_hasnext);
  }
  ~GraphEltIterator() {
    delete it;
  }

private:
  tlp::Iterator<ELT_TYPE> *it;
  const tlp::Graph *graph;
  ELT_TYPE curElt;
  bool _hasnext;
};

//============================================================
template <class NodeType, class EdgeType, class PropType>
tlp::Iterator<tlp::node> *
tlp::AbstractProperty<NodeType, EdgeType, PropType>::getNonDefaultValuatedNodes(
    const Graph *g) const {
  tlp::Iterator<tlp::node> *it =
      conversionIterator<tlp::node>(nodeProperties.findAll(nodeDefaultValue, false), idToNode);

  if (PropType::name.empty()) {
    // we always need to check that nodes belong to graph
    // for non registered properties, because deleted nodes are not erased
    // from them
    return new GraphEltIterator<tlp::node>(g != nullptr ? g : PropType::graph, it);
  }

  return ((g == nullptr) || (g == PropType::graph)) ? it : new GraphEltIterator<tlp::node>(g, it);
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::hasNonDefaultValuatedNodes(
    const Graph *g) const {
  if (g == nullptr) {
    return nodeProperties.hasNonDefaultValues();
  } else {
    return !tlp::iteratorEmpty(getNonDefaultValuatedNodes(g));
  }
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
unsigned int tlp::AbstractProperty<NodeType, EdgeType, PropType>::numberOfNonDefaultValuatedNodes(
    const Graph *g) const {
  if (g == nullptr) {
    return nodeProperties.numberOfNonDefaultValues();
  } else {
    return tlp::iteratorCount(getNonDefaultValuatedNodes(g));
  }
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
unsigned int tlp::AbstractProperty<NodeType, EdgeType, PropType>::nodeValueSize() const {
  return NodeType::valueSize();
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::writeNodeDefaultValue(
    std::ostream &oss) const {
  NodeType::writeb(oss, nodeDefaultValue);
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::writeNodeValue(std::ostream &oss,
                                                                         node n) const {
  assert(n.isValid());
  NodeType::writeb(oss, nodeProperties.get(n.id));
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::readNodeDefaultValue(std::istream &iss) {
  if (NodeType::readb(iss, nodeDefaultValue)) {
    nodeProperties.setAll(nodeDefaultValue);
    return true;
  }

  return false;
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::readNodeValue(std::istream &iss, node n) {
  typename NodeType::RealType val;

  if (NodeType::readb(iss, val)) {
    nodeProperties.set(n.id, val);
    return true;
  }

  return false;
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
tlp::Iterator<tlp::edge> *
tlp::AbstractProperty<NodeType, EdgeType, PropType>::getNonDefaultValuatedEdges(
    const Graph *g) const {
  tlp::Iterator<tlp::edge> *it =
      conversionIterator<tlp::edge>(edgeProperties.findAll(edgeDefaultValue, false), idToEdge);

  if (PropType::name.empty()) {
    // we always need to check that edges belong to graph
    // for non registered properties, because deleted edges are not erased
    // from them
    return new GraphEltIterator<tlp::edge>(g != nullptr ? g : PropType::graph, it);
  }

  return ((g == nullptr) || (g == PropType::graph)) ? it : new GraphEltIterator<tlp::edge>(g, it);
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::hasNonDefaultValuatedEdges(
    const Graph *g) const {
  if (g == nullptr) {
    return edgeProperties.hasNonDefaultValues();
  } else {
    return !tlp::iteratorEmpty(getNonDefaultValuatedEdges(g));
  }
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
unsigned int tlp::AbstractProperty<NodeType, EdgeType, PropType>::numberOfNonDefaultValuatedEdges(
    const Graph *g) const {
  if (g == nullptr) {
    return edgeProperties.numberOfNonDefaultValues();
  } else {
    return tlp::iteratorCount(getNonDefaultValuatedEdges(g));
  }
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
unsigned int tlp::AbstractProperty<NodeType, EdgeType, PropType>::edgeValueSize() const {
  return EdgeType::valueSize();
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::writeEdgeDefaultValue(
    std::ostream &oss) const {
  EdgeType::writeb(oss, edgeDefaultValue);
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::writeEdgeValue(std::ostream &oss,
                                                                         edge e) const {
  assert(e.isValid());
  EdgeType::writeb(oss, edgeProperties.get(e.id));
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::readEdgeDefaultValue(std::istream &iss) {
  if (EdgeType::readb(iss, edgeDefaultValue)) {
    edgeProperties.setAll(edgeDefaultValue);
    return true;
  }

  return false;
}
//============================================================
template <class NodeType, class EdgeType, class PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::readEdgeValue(std::istream &iss, edge e) {
  typename EdgeType::RealType val;

  if (EdgeType::readb(iss, val)) {
    edgeProperties.set(e.id, val);
    return true;
  }

  return false;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
tlp::AbstractProperty<NodeType, EdgeType, PropType> &
tlp::AbstractProperty<NodeType, EdgeType, PropType>::operator=(
    tlp::AbstractProperty<NodeType, EdgeType, PropType> &prop) {
  if (this != &prop) {
    if (PropType::graph == nullptr) {
      PropType::graph = prop.PropType::graph;
    }

    if (PropType::graph == prop.PropType::graph) {
      setAllNodeValue(prop.getNodeDefaultValue());
      setAllEdgeValue(prop.getEdgeDefaultValue());

      for (auto itn : prop.getNonDefaultValuatedNodes()) {
        setNodeValue(itn, prop.getNodeValue(itn));
      }

      for (auto ite : prop.getNonDefaultValuatedEdges()) {
        setEdgeValue(ite, prop.getEdgeValue(ite));
      }

    } else {
      //==============================================================*
      for (auto n : PropType::graph->nodes()) {
        if (prop.PropType::graph->isElement(n)) {
          setNodeValue(n, prop.getNodeValue(n));
        }
      }

      for (auto e : PropType::graph->edges()) {
        if (prop.PropType::graph->isElement(e)) {
          setEdgeValue(e, prop.getEdgeValue(e));
        }
      }
    }

    clone_handler(prop);
  }

  return *this;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::erase(const tlp::node n) {
  setNodeValue(n, nodeDefaultValue);
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::erase(const tlp::edge e) {
  setEdgeValue(e, edgeDefaultValue);
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
std::string tlp::AbstractProperty<NodeType, EdgeType, PropType>::getNodeDefaultStringValue() const {
  typename NodeType::RealType v = getNodeDefaultValue();
  return NodeType::toString(v);
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
std::string tlp::AbstractProperty<NodeType, EdgeType, PropType>::getEdgeDefaultStringValue() const {
  typename EdgeType::RealType v = getEdgeDefaultValue();
  return EdgeType::toString(v);
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
std::string
tlp::AbstractProperty<NodeType, EdgeType, PropType>::getNodeStringValue(const tlp::node n) const {
  typename NodeType::RealType v = getNodeValue(n);
  return NodeType::toString(v);
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
std::string
tlp::AbstractProperty<NodeType, EdgeType, PropType>::getEdgeStringValue(const tlp::edge e) const {
  typename EdgeType::RealType v = getEdgeValue(e);
  return EdgeType::toString(v);
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::setNodeStringValue(
    const tlp::node inN, const std::string &inV) {
  typename NodeType::RealType v;

  if (!NodeType::fromString(v, inV)) {
    return false;
  }

  setNodeValue(inN, v);
  return true;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::setEdgeStringValue(
    const tlp::edge inE, const std::string &inV) {
  typename EdgeType::RealType v;

  if (!EdgeType::fromString(v, inV)) {
    return false;
  }

  setEdgeValue(inE, v);
  return true;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::setNodeDefaultStringValue(
    const std::string &inV) {
  typename NodeType::RealType v;

  if (!NodeType::fromString(v, inV)) {
    return false;
  }

  setNodeDefaultValue(v);
  return true;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::setAllNodeStringValue(
    const std::string &inV, const tlp::Graph *graph) {
  typename NodeType::RealType v;

  if (!NodeType::fromString(v, inV)) {
    return false;
  }

  setAllNodeValue(v, graph);
  return true;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::setEdgeDefaultStringValue(
    const std::string &inV) {
  typename EdgeType::RealType v;

  if (!EdgeType::fromString(v, inV)) {
    return false;
  }

  setEdgeDefaultValue(v);
  return true;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::setAllEdgeStringValue(
    const std::string &inV, const tlp::Graph *graph) {
  typename EdgeType::RealType v;

  if (!EdgeType::fromString(v, inV)) {
    return false;
  }

  setAllEdgeValue(v, graph);
  return true;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::copy(const tlp::node destination,
                                                               const tlp::node source,
                                                               tlp::PropertyInterface *property,
                                                               bool ifNotDefault) {
  if (property == nullptr) {
    return false;
  }

  auto *tp = dynamic_cast<tlp::AbstractProperty<NodeType, EdgeType, PropType> *>(property);
  assert(tp);
  bool notDefault;
  typename tlp::StoredType<typename NodeType::RealType>::ReturnedValue value =
      tp->nodeProperties.get(source.id, notDefault);

  if (ifNotDefault && !notDefault) {
    return false;
  }

  setNodeValue(destination, value);
  return true;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
bool tlp::AbstractProperty<NodeType, EdgeType, PropType>::copy(const tlp::edge destination,
                                                               const tlp::edge source,
                                                               tlp::PropertyInterface *property,
                                                               bool ifNotDefault) {
  if (property == nullptr) {
    return false;
  }

  auto *tp = dynamic_cast<tlp::AbstractProperty<NodeType, EdgeType, PropType> *>(property);
  assert(tp);
  bool notDefault;
  typename tlp::StoredType<typename EdgeType::RealType>::ReturnedValue value =
      tp->edgeProperties.get(source.id, notDefault);

  if (ifNotDefault && !notDefault) {
    return false;
  }

  setEdgeValue(destination, value);
  return true;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::copy(tlp::PropertyInterface *property) {
  auto *prop =
      dynamic_cast<typename tlp::AbstractProperty<NodeType, EdgeType, PropType> *>(property);
  assert(prop != nullptr);
  *this = *prop;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
tlp::DataMem *
tlp::AbstractProperty<NodeType, EdgeType, PropType>::getNodeDefaultDataMemValue() const {
  return new tlp::TypedValueContainer<typename NodeType::RealType>(getNodeDefaultValue());
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
tlp::DataMem *
tlp::AbstractProperty<NodeType, EdgeType, PropType>::getEdgeDefaultDataMemValue() const {
  return new tlp::TypedValueContainer<typename EdgeType::RealType>(getEdgeDefaultValue());
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
tlp::DataMem *
tlp::AbstractProperty<NodeType, EdgeType, PropType>::getNodeDataMemValue(const tlp::node n) const {
  return new tlp::TypedValueContainer<typename NodeType::RealType>(getNodeValue(n));
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
tlp::DataMem *
tlp::AbstractProperty<NodeType, EdgeType, PropType>::getEdgeDataMemValue(const tlp::edge e) const {
  return new tlp::TypedValueContainer<typename EdgeType::RealType>(getEdgeValue(e));
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
tlp::DataMem *tlp::AbstractProperty<NodeType, EdgeType, PropType>::getNonDefaultDataMemValue(
    const tlp::node n) const {
  bool notDefault;
  typename tlp::StoredType<typename NodeType::RealType>::ReturnedValue value =
      nodeProperties.get(n.id, notDefault);

  if (notDefault) {
    return new tlp::TypedValueContainer<typename NodeType::RealType>(value);
  }

  return nullptr;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
tlp::DataMem *tlp::AbstractProperty<NodeType, EdgeType, PropType>::getNonDefaultDataMemValue(
    const tlp::edge e) const {
  bool notDefault;
  typename tlp::StoredType<typename EdgeType::RealType>::ReturnedValue value =
      edgeProperties.get(e.id, notDefault);

  if (notDefault) {
    return new tlp::TypedValueContainer<typename EdgeType::RealType>(value);
  }

  return nullptr;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::setNodeDataMemValue(
    const tlp::node n, const tlp::DataMem *v) {
  setNodeValue(
      n, static_cast<const tlp::TypedValueContainer<typename NodeType::RealType> *>(v)->value);
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::setEdgeDataMemValue(
    const tlp::edge e, const tlp::DataMem *v) {
  setEdgeValue(
      e, static_cast<const tlp::TypedValueContainer<typename EdgeType::RealType> *>(v)->value);
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::setAllNodeDataMemValue(
    const tlp::DataMem *v) {
  setAllNodeValue(
      static_cast<const tlp::TypedValueContainer<typename NodeType::RealType> *>(v)->value);
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::setAllEdgeDataMemValue(
    const tlp::DataMem *v) {
  setAllEdgeValue(
      static_cast<const tlp::TypedValueContainer<typename EdgeType::RealType> *>(v)->value);
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::MetaValueCalculator::computeMetaValue(
    AbstractProperty<NodeType, EdgeType, PropType> *, tlp::node, tlp::Graph *, tlp::Graph *) {}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::MetaValueCalculator::computeMetaValue(
    AbstractProperty<NodeType, EdgeType, PropType> *, tlp::edge, tlp::Iterator<tlp::edge> *itE,
    tlp::Graph *) {
  delete itE;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::computeMetaValue(tlp::node n,
                                                                           tlp::Graph *sg,
                                                                           tlp::Graph *mg) {
  if (PropType::metaValueCalculator) {
    static_cast<
        typename tlp::AbstractProperty<NodeType, EdgeType, PropType>::MetaValueCalculator *>(
        PropType::metaValueCalculator)
        ->computeMetaValue(this, n, sg, mg);
  }
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::computeMetaValue(
    tlp::edge e, tlp::Iterator<tlp::edge> *itE, tlp::Graph *mg) {
  if (PropType::metaValueCalculator) {
    static_cast<
        typename tlp::AbstractProperty<NodeType, EdgeType, PropType>::MetaValueCalculator *>(
        PropType::metaValueCalculator)
        ->computeMetaValue(this, e, itE, mg);
  } else {
    delete itE;
  }
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::setMetaValueCalculator(
    tlp::PropertyInterface::MetaValueCalculator *mvCalc) {
  if (mvCalc &&
      !dynamic_cast<
          typename tlp::AbstractProperty<NodeType, EdgeType, PropType>::MetaValueCalculator *>(
          mvCalc)) {
    tlp::warning()
        << "Warning : " << __PRETTY_FUNCTION__ << " ... invalid conversion of "
        << typeid(mvCalc).name() << "into "
        << typeid(
               typename tlp::AbstractProperty<NodeType, EdgeType, PropType>::MetaValueCalculator *)
               .name()
        << std::endl;
    abort();
  }

  PropType::metaValueCalculator = mvCalc;
}
//============================================================
template <typename NodeType, typename EdgeType, typename PropType>
void tlp::AbstractProperty<NodeType, EdgeType, PropType>::clone_handler(
    tlp::AbstractProperty<NodeType, EdgeType, PropType> &) {}
//============================================================

//============================================================
template <typename VecType, typename EltType, typename PropType>
tlp::AbstractVectorProperty<VecType, EltType, PropType>::AbstractVectorProperty(
    tlp::Graph *g, const std::string &name)
    : AbstractProperty<VecType, VecType, PropType>(g, name) {}
//============================================================
template <typename VecType, typename EltType, typename PropType>
bool tlp::AbstractVectorProperty<VecType, EltType, PropType>::tokenize(
    const std::string &s, std::vector<std::string> &vect, char openChar, char sepChar,
    char closeChar) {
  return VecType::tokenize(s, vect, openChar, sepChar, closeChar);
}
//============================================================
template <typename VecType, typename EltType, typename PropType>
bool tlp::AbstractVectorProperty<VecType, EltType, PropType>::setNodeStringValueAsVector(
    const node n, const std::vector<std::string> &vs) {
  typename VecType::RealType v;
  if (!VecType::read(vs, v)) {
    return false;
  }

  this->setNodeValue(n, v);
  return true;
}
//============================================================
template <typename VecType, typename EltType, typename PropType>
bool tlp::AbstractVectorProperty<VecType, EltType, PropType>::setNodeStringValueAsVector(
    const node n, const std::string &s, char openChar, char sepChar, char closeChar) {
  typename VecType::RealType v;
  std::istringstream iss(s);

  if (!VecType::read(iss, v, openChar, sepChar, closeChar)) {
    return false;
  }

  this->setNodeValue(n, v);
  return true;
}
//============================================================
template <typename VecType, typename EltType, typename PropType>
bool tlp::AbstractVectorProperty<VecType, EltType, PropType>::setEdgeStringValueAsVector(
    const edge e, const std::vector<std::string> &vs) {
  typename VecType::RealType v;
  if (!VecType::read(vs, v)) {
    return false;
  }

  this->setEdgeValue(e, v);
  return true;
}
//============================================================
template <typename VecType, typename EltType, typename PropType>
bool tlp::AbstractVectorProperty<VecType, EltType, PropType>::setEdgeStringValueAsVector(
    const edge e, const std::string &s, char openChar, char sepChar, char closeChar) {
  typename VecType::RealType v;
  std::istringstream iss(s);

  if (!VecType::read(iss, v, openChar, sepChar, closeChar)) {
    return false;
  }

  this->setEdgeValue(e, v);
  return true;
}
//============================================================
template <typename VecType, typename EltType, typename PropType>
void tlp::AbstractVectorProperty<VecType, EltType, PropType>::setNodeEltValue(
    const node n, unsigned int i,
    typename tlp::StoredType<typename EltType::RealType>::ReturnedConstValue v) {
  assert(n.isValid());
  bool isNotDefault;
  typename tlp::StoredType<typename VecType::RealType>::ReturnedValue vect =
      AbstractProperty<VecType, VecType, PropType>::nodeProperties.get(n, isNotDefault);
  assert(vect.size() > i);
  this->PropType::notifyBeforeSetNodeValue(n);

  if (isNotDefault) {
    vect[i] = v;
  } else {
    typename VecType::RealType tmp(vect);
    tmp[i] = v;
    AbstractProperty<VecType, VecType, PropType>::nodeProperties.set(n.id, tmp);
  }

  this->PropType::notifyAfterSetNodeValue(n);
}
//============================================================
template <typename VecType, typename EltType, typename PropType>
typename tlp::StoredType<typename EltType::RealType>::ReturnedConstValue
tlp::AbstractVectorProperty<VecType, EltType, PropType>::getNodeEltValue(const node n,
                                                                         unsigned int i) const {
  assert(n.isValid());
  typename tlp::StoredType<typename VecType::RealType>::ReturnedConstValue vect =
      AbstractProperty<VecType, VecType, PropType>::nodeProperties.get(n);
  assert(vect.size() > i);
  return vect[i];
}
//============================================================
template <typename VecType, typename EltType, typename PropType>
void tlp::AbstractVectorProperty<VecType, EltType, PropType>::pushBackNodeEltValue(
    const node n, typename tlp::StoredType<typename EltType::RealType>::ReturnedConstValue v) {
  assert(n.isValid());
  bool isNotDefault;
  typename tlp::StoredType<typename VecType::RealType>::ReturnedValue vect =
      AbstractProperty<VecType, VecType, PropType>::nodeProperties.get(n, isNotDefault);
  this->PropType::notifyBeforeSetNodeValue(n);

  if (isNotDefault) {
    vect.push_back(v);
  } else {
    typename VecType::RealType tmp(vect);
    tmp.push_back(v);
    AbstractProperty<VecType, VecType, PropType>::nodeProperties.set(n, tmp);
  }

  this->PropType::notifyAfterSetNodeValue(n);
}
//============================================================
template <typename VecType, typename EltType, typename PropType>
void tlp::AbstractVectorProperty<VecType, EltType, PropType>::popBackNodeEltValue(const node n) {
  assert(n.isValid());
  bool isNotDefault;
  typename tlp::StoredType<typename VecType::RealType>::ReturnedValue vect =
      AbstractProperty<VecType, VecType, PropType>::nodeProperties.get(n, isNotDefault);
  this->PropType::notifyBeforeSetNodeValue(n);
  assert(isNotDefault);
  vect.pop_back();
  this->PropType::notifyAfterSetNodeValue(n);
}
//============================================================
template <typename VecType, typename EltType, typename PropType>
void tlp::AbstractVectorProperty<VecType, EltType, PropType>::resizeNodeValue(
    const node n, size_t size, typename EltType::RealType elt) {
  assert(n.isValid());
  bool isNotDefault;
  typename tlp::StoredType<typename VecType::RealType>::ReturnedValue vect =
      AbstractProperty<VecType, VecType, PropType>::nodeProperties.get(n, isNotDefault);
  assert(isNotDefault);
  this->PropType::notifyBeforeSetNodeValue(n);
  vect.resize(size, elt);
  this->PropType::notifyAfterSetNodeValue(n);
}
//============================================================
template <typename VecType, typename EltType, typename PropType>
void tlp::AbstractVectorProperty<VecType, EltType, PropType>::setEdgeEltValue(
    const edge e, unsigned int i,
    typename tlp::StoredType<typename EltType::RealType>::ReturnedConstValue v) {
  assert(e.isValid());
  bool isNotDefault;
  typename tlp::StoredType<typename VecType::RealType>::ReturnedValue vect =
      AbstractProperty<VecType, VecType, PropType>::edgeProperties.get(e, isNotDefault);
  assert(vect.size() > i);
  this->PropType::notifyBeforeSetEdgeValue(e);

  if (isNotDefault) {
    vect[i] = v;
  } else {
    typename VecType::RealType tmp(vect);
    tmp[i] = v;
    AbstractProperty<VecType, VecType, PropType>::edgeProperties.set(e, tmp);
  }

  this->PropType::notifyAfterSetEdgeValue(e);
}
//============================================================
template <typename VecType, typename EltType, typename PropType>
typename tlp::StoredType<typename EltType::RealType>::ReturnedConstValue
tlp::AbstractVectorProperty<VecType, EltType, PropType>::getEdgeEltValue(const edge e,
                                                                         unsigned int i) const {
  assert(e.isValid());
  typename tlp::StoredType<typename VecType::RealType>::ReturnedConstValue vect =
      AbstractProperty<VecType, VecType, PropType>::edgeProperties.get(e);
  assert(vect.size() > i);
  return vect[i];
} //============================================================
template <typename VecType, typename EltType, typename PropType>
void tlp::AbstractVectorProperty<VecType, EltType, PropType>::pushBackEdgeEltValue(
    const edge e, typename tlp::StoredType<typename EltType::RealType>::ReturnedConstValue v) {
  assert(e.isValid());
  bool isNotDefault;
  typename tlp::StoredType<typename VecType::RealType>::ReturnedValue vect =
      AbstractProperty<VecType, VecType, PropType>::edgeProperties.get(e, isNotDefault);
  this->PropType::notifyBeforeSetEdgeValue(e);

  if (isNotDefault) {
    vect.push_back(v);
  } else {
    typename VecType::RealType tmp(vect);
    tmp.push_back(v);
    AbstractProperty<VecType, VecType, PropType>::edgeProperties.set(e, tmp);
  }

  this->PropType::notifyAfterSetEdgeValue(e);
}
//============================================================
template <typename VecType, typename EltType, typename PropType>
void tlp::AbstractVectorProperty<VecType, EltType, PropType>::popBackEdgeEltValue(const edge e) {
  assert(e.isValid());
  bool isNotDefault;
  typename tlp::StoredType<typename VecType::RealType>::ReturnedValue vect =
      AbstractProperty<VecType, VecType, PropType>::edgeProperties.get(e, isNotDefault);
  this->PropType::notifyBeforeSetEdgeValue(e);
  assert(isNotDefault);
  vect.pop_back();
  this->PropType::notifyAfterSetEdgeValue(e);
}
//============================================================
template <typename VecType, typename EltType, typename PropType>
void tlp::AbstractVectorProperty<VecType, EltType, PropType>::resizeEdgeValue(
    const edge e, size_t size, typename EltType::RealType elt) {
  assert(e.isValid());
  bool isNotDefault;
  typename tlp::StoredType<typename VecType::RealType>::ReturnedValue vect =
      AbstractProperty<VecType, VecType, PropType>::edgeProperties.get(e, isNotDefault);
  assert(isNotDefault);
  this->PropType::notifyBeforeSetEdgeValue(e);
  vect.resize(size, elt);
  this->PropType::notifyAfterSetEdgeValue(e);
}
