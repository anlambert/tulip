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

#ifndef TALIPOT_MIN_MAX_PROPERTY_H
#define TALIPOT_MIN_MAX_PROPERTY_H

#include <unordered_map>

#include <talipot/Observable.h>
#include <talipot/AbstractProperty.h>

#define MINMAX_PAIR(TYPE) std::pair<typename TYPE::RealType, typename TYPE::RealType>
#define MINMAX_MAP(TYPE) typename std::unordered_map<unsigned int, MINMAX_PAIR(TYPE)>

namespace tlp {

/**
 * @brief Abstracts the computation of minimal and maximal values on node and edge values of
 *properties.
 *
 * The value is lazily computed on first request.
 * The value is cached, and the cache is invalidated whenever it cannot be simply updated.
 **/
template <typename NodeType, typename EdgeType, typename PropType = PropertyInterface>
class MinMaxProperty : public tlp::AbstractProperty<NodeType, EdgeType, PropType> {
public:
  /**
   * @brief Constructs a MinMaxProperty.
   *
   * @param graph The graph to attach the property to.
   * @param name The name of the property.
   * @param NodeMin The minimal value the property can take for nodes (e.g. INT_MIN)
   * @param NodeMax The maximal value the property can take for nodes (e.g. INT_MIN)
   * @param EdgeMin The minimal value the property can take for edges (e.g. INT_MIN)
   * @param EdgeMax The maximal value the property can take for edges (e.g. INT_MIN)
   **/
  MinMaxProperty(tlp::Graph *graph, const std::string &name, typename NodeType::RealType NodeMin,
                 typename NodeType::RealType NodeMax, typename EdgeType::RealType EdgeMin,
                 typename EdgeType::RealType EdgeMax);

  void treatEvent(const tlp::Event &ev) override;

  /**
   * @brief Gets the minimum value on the nodes.
   * It is only computed if it has never been retrieved before, or if the cached value could not be
   *updated.
   *
   * @param graph The graph on which to compute.
   * @return The minimal value on this graph for this property.
   **/
  typename NodeType::RealType getNodeMin(const Graph *graph = nullptr);

  /**
   * @brief Computes the maximum value on the nodes.
   * It is only computed if it has never been retrieved before, or if the cached value could not be
   *updated.
   *
   * @param graph The graph on which to compute.
   * @return The maximal value on this graph for this property.
   **/
  typename NodeType::RealType getNodeMax(const Graph *graph = nullptr);

  /**
   * @brief Computes the minimum value on the edges.
   * It is only computed if it has never been retrieved before, or if the cached value could not be
   *updated.
   *
   * @param graph The graph on which to compute.
   * @return The minimal value on this graph for this property.
   **/
  typename EdgeType::RealType getEdgeMin(const Graph *graph = nullptr);

  /**
   * @brief Computes the maximum value on the edges.
   * It is only computed if it has never been retrieved before, or if the cached value could not be
   *updated.
   *
   * @param graph The graph on which to compute.
   * @return The maximal value on this graph for this property.
   **/
  typename EdgeType::RealType getEdgeMax(const Graph *graph = nullptr);

  /**
   * @brief Updates the value on a node, and updates the minimal/maximal cached values if necessary.
   * Should be called by subclasses in order to keep the cache up to date.
   *
   * @param n The node for which the value is updated.
   * @param newValue The new value on this node.
   **/
  void updateNodeValue(tlp::node n, typename NodeType::RealType newValue);

  /**
   * @brief Updates the value on an edge, and updates the minimal/maximal cached values if
   *necessary.
   * Should be called by subclasses in order to keep the cache up to date.
   *
   * @param e The edge for which the value is updated.
   * @param newValue The new value on this edge.
   **/
  virtual void updateEdgeValue(tlp::edge e, typename EdgeType::RealType newValue);

  /**
   * @brief Updates the value of all nodes, setting the maximum and minimum values to this.
   * Should be called by subclasses in order to keep the cache up to date.
   *
   * @param newValue The new maximal and minimal value.
   **/
  void updateAllNodesValues(typename NodeType::RealType newValue);

  /**
   * @brief Updates the value of all edges, setting the maximum and minimum values to this.
   * Should be called by subclasses in order to keep the cache up to date.
   *
   * @param newValue The new maximal and minimal value.
   **/
  void updateAllEdgesValues(typename EdgeType::RealType newValue);

protected:
  MINMAX_MAP(NodeType) minMaxNode;
  MINMAX_MAP(EdgeType) minMaxEdge;

  typename NodeType::RealType _nodeMin;
  typename NodeType::RealType _nodeMax;
  typename EdgeType::RealType _edgeMin;
  typename EdgeType::RealType _edgeMax;

  // this will indicate if we can stop PropType::graph observation
  bool needGraphListener; // default is false

  virtual MINMAX_PAIR(NodeType) computeMinMaxNode(const Graph *graph);
  MINMAX_PAIR(EdgeType) computeMinMaxEdge(const Graph *graph);
  void removeListenersAndClearNodeMap();
  void removeListenersAndClearEdgeMap();
};
}

#ifdef DLL_TALIPOT
#include "cxx/MinMaxProperty.cxx"
#endif

#endif // TALIPOT_MIN_MAX_PROPERTY_H
