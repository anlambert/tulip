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

#ifndef TALIPOT_STATIC_PROPERTY_H
#define TALIPOT_STATIC_PROPERTY_H

#include <talipot/Graph.h>
#include <talipot/GraphParallelTools.h>
#include <talipot/NumericProperty.h>

namespace tlp {
template <typename TYPE>
class NodeVectorProperty : public std::vector<TYPE> {
  const Graph *graph;

public:
  // constructor
  NodeVectorProperty(const Graph *g) : graph(g) {
    assert(g);
    // set the vector size to the number of graph nodes
    this->resize(graph->numberOfNodes());
  }

  typename std::vector<TYPE>::const_reference operator[](unsigned int i) const {
    return std::vector<TYPE>::operator[](i);
  }

  typename std::vector<TYPE>::reference operator[](unsigned int i) {
    return std::vector<TYPE>::operator[](i);
  }

  typename std::vector<TYPE>::const_reference operator[](node n) const {
    return (*this)[graph->nodePos(n)];
  }

  typename std::vector<TYPE>::reference operator[](node n) {
    return (*this)[graph->nodePos(n)];
  }

  // get the stored value of a node
  typename std::vector<TYPE>::const_reference getNodeValue(node n) const {
    return (*this)[n];
  }

  // set the stored value of a node
  void setNodeValue(node n, TYPE val) {
    (*this)[n] = val;
  }

  // set all to same values
  void setAll(const TYPE &val) {
    TLP_PARALLEL_MAP_INDICES(graph->numberOfNodes(), [&](unsigned int i) { (*this)[i] = val; });
  }

  // add a value for a newly created node
  void addNodeValue(node n, TYPE val) {
    unsigned int nPos = graph->nodePos(n);

    if (nPos + 1 > this->size()) {
      this->resize(nPos + 1);
    }

    (*this)[nPos] = val;
  }

  // get values from a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyFromProperty(PROP_PTR prop) {
    TLP_PARALLEL_MAP_NODES_AND_INDICES(
        graph, [&](const node n, unsigned int i) { (*this)[i] = prop->getNodeValue(n); });
  }

  // get values from a NumericProperty
  void copyFromNumericProperty(const NumericProperty *prop) {
    TLP_PARALLEL_MAP_NODES_AND_INDICES(
        graph, [&](const node n, unsigned int i) { (*this)[i] = prop->getNodeDoubleValue(n); });
  }

  // copy values into a typed typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyToProperty(PROP_PTR prop) {
    const std::vector<node> &nodes = graph->nodes();
    unsigned int nbNodes = nodes.size();

    for (unsigned int i = 0; i < nbNodes; ++i) {
      prop->setNodeValue(nodes[i], (*this)[i]);
    }
  }
};

template <>
class NodeVectorProperty<bool> : public std::vector<unsigned char> {
  const Graph *graph;

public:
  // constructor
  NodeVectorProperty(const Graph *g) : graph(g) {
    assert(g);
    // set the vector size to the number of graph nodes
    this->resize(graph->numberOfNodes());
  }

  const Graph *getGraph() const {
    return graph;
  }

  bool operator[](unsigned int i) const {
    return static_cast<bool>(std::vector<unsigned char>::operator[](i));
  }

  std::vector<unsigned char>::reference operator[](unsigned int i) {
    return std::vector<unsigned char>::operator[](i);
  }

  bool operator[](node n) const {
    return (*this)[graph->nodePos(n)];
  }

  std::vector<unsigned char>::reference operator[](node n) {
    return (*this)[graph->nodePos(n)];
  }

  // get the stored value of a node
  bool getNodeValue(node n) const {
    return (*this)[graph->nodePos(n)];
  }

  // set the stored value of a node
  void setNodeValue(node n, bool val) {
    (*this)[graph->nodePos(n)] = val;
  }

  // set all to same values
  void setAll(const bool &val) {
    TLP_PARALLEL_MAP_INDICES(graph->numberOfNodes(), [&](unsigned int i) { (*this)[i] = val; });
  }

  // add a value for a newly created node
  void addNodeValue(node n, bool val) {
    unsigned int nPos = graph->nodePos(n);

    if (nPos + 1 > this->size()) {
      this->resize(nPos + 1);
    }

    (*this)[nPos] = val;
  }

  // get values from a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyFromProperty(PROP_PTR prop) {
    TLP_PARALLEL_MAP_NODES_AND_INDICES(
        graph, [&](const node n, unsigned int i) { (*this)[i] = prop->getNodeValue(n); });
  }

  // copy values into a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyToProperty(PROP_PTR prop) {
    const std::vector<node> &nodes = graph->nodes();
    unsigned int nbNodes = nodes.size();

    for (unsigned int i = 0; i < nbNodes; ++i) {
      prop->setNodeValue(nodes[i], (*this)[i]);
    }
  }
};

template <typename TYPE>
class EdgeVectorProperty : public std::vector<TYPE> {
  const Graph *graph;

public:
  // constructor
  EdgeVectorProperty(const Graph *g) : graph(g) {
    assert(g);
    // set the vector size to the number of graph edges
    this->resize(graph->numberOfEdges());
  }

  const Graph *getGraph() const {
    return graph;
  }

  typename std::vector<TYPE>::const_reference operator[](unsigned int i) const {
    return std::vector<TYPE>::operator[](i);
  }

  typename std::vector<TYPE>::reference operator[](unsigned int i) {
    return std::vector<TYPE>::operator[](i);
  }

  typename std::vector<TYPE>::const_reference operator[](edge e) const {
    return (*this)[graph->edgePos(e)];
  }

  typename std::vector<TYPE>::reference operator[](edge e) {
    return (*this)[graph->edgePos(e)];
  }

  // get the stored value of a edge
  typename std::vector<TYPE>::const_reference getEdgeValue(edge e) const {
    return (*this)[e];
  }

  // set the stored value of a edge
  void setEdgeValue(edge e, TYPE val) {
    (*this)[e] = val;
  }

  void setAll(const TYPE &val) {
    TLP_PARALLEL_MAP_INDICES(graph->numberOfEdges(), [&](unsigned int i) { (*this)[i] = val; });
  }

  // add a value for a newly created edge
  void addEdgeValue(edge e, TYPE val) {
    unsigned int ePos = graph->edgePos(e);

    if (ePos + 1 > this->size()) {
      this->resize(ePos + 1);
    }

    (*this)[ePos] = val;
  }

  // get values from a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyFromProperty(PROP_PTR prop) {
    TLP_PARALLEL_MAP_EDGES_AND_INDICES(
        graph, [&](const edge e, unsigned int i) { (*this)[i] = prop->getEdgeValue(e); });
  }

  // get values from a NumericProperty
  void copyFromNumericProperty(const NumericProperty *prop) {
    TLP_PARALLEL_MAP_EDGES_AND_INDICES(
        graph, [&](const edge e, unsigned int i) { (*this)[i] = prop->getEdgeDoubleValue(e); });
  }

  // copy values into a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyToProperty(PROP_PTR prop) {
    const std::vector<edge> &edges = graph->edges();
    unsigned int nbEdges = edges.size();

    for (unsigned int i = 0; i < nbEdges; ++i) {
      prop->setEdgeValue(edges[i], (*this)[i]);
    }
  }
};

template <>
class EdgeVectorProperty<bool> : public std::vector<unsigned char> {
  const Graph *graph;

public:
  // constructor
  EdgeVectorProperty(const Graph *g) : graph(g) {
    assert(g);
    // set the vector size to the number of graph edges
    this->resize(graph->numberOfEdges());
  }

  bool operator[](unsigned int i) const {
    return static_cast<bool>(std::vector<unsigned char>::operator[](i));
  }

  std::vector<unsigned char>::reference operator[](unsigned int i) {
    return std::vector<unsigned char>::operator[](i);
  }

  bool operator[](edge e) const {
    return (*this)[graph->edgePos(e)];
  }

  std::vector<unsigned char>::reference operator[](edge e) {
    return (*this)[graph->edgePos(e)];
  }

  // get the stored value of a edge
  bool getEdgeValue(edge e) const {
    return (*this)[graph->edgePos(e)];
  }

  // set the stored value of a edge
  void setEdgeValue(edge e, bool val) {
    (*this)[graph->edgePos(e)] = val;
  }

  // set all to same values
  void setAll(const bool &val) {
    TLP_PARALLEL_MAP_INDICES(graph->numberOfEdges(), [&](unsigned int i) { (*this)[i] = val; });
  }

  // add a value for a newly created edge
  void addEdgeValue(edge e, bool val) {
    unsigned int ePos = graph->edgePos(e);

    if (ePos + 1 > this->size()) {
      this->resize(ePos + 1);
    }

    (*this)[ePos] = val;
  }

  // get values from a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyFromProperty(PROP_PTR prop) {
    TLP_PARALLEL_MAP_EDGES_AND_INDICES(
        graph, [&](const edge e, unsigned int i) { (*this)[i] = prop->getEdgeValue(e); });
  }

  // copy values into a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyToProperty(PROP_PTR prop) {
    const std::vector<edge> &edges = graph->edges();
    unsigned int nbEdges = edges.size();

    for (unsigned int i = 0; i < nbEdges; ++i) {
      prop->setEdgeValue(edges[i], (*this)[i]);
    }
  }
};
}

#endif // TALIPOT_STATIC_PROPERTY_H