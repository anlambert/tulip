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

#include <talipot/Graph.h>
#include <talipot/Coord.h>

template <typename nodeType, typename edgeType, typename propType>
tlp::MinMaxProperty<nodeType, edgeType, propType>::MinMaxProperty(
    tlp::Graph *graph, const std::string &name, typename nodeType::RealType NodeMin,
    typename nodeType::RealType NodeMax, typename edgeType::RealType EdgeMin,
    typename edgeType::RealType EdgeMax)
    : AbstractProperty<nodeType, edgeType, propType>(graph, name), _nodeMin(NodeMin),
      _nodeMax(NodeMax), _edgeMin(EdgeMin), _edgeMax(EdgeMax), needGraphListener(false) {}

template <typename nodeType, typename edgeType, typename propType>
typename nodeType::RealType
tlp::MinMaxProperty<nodeType, edgeType, propType>::getNodeMin(const tlp::Graph *graph) {
  if (!graph) {
    graph = this->propType::graph;
  }

  unsigned int graphID = graph->getId();

  if (const auto it = minMaxNode.find(graphID); it == minMaxNode.end()) {
    return computeMinMaxNode(graph).first;
  } else {
    return it->second.first;
  }
}

template <typename nodeType, typename edgeType, typename propType>
typename nodeType::RealType
tlp::MinMaxProperty<nodeType, edgeType, propType>::getNodeMax(const tlp::Graph *graph) {
  if (!graph) {
    graph = this->propType::graph;
  }

  unsigned int graphID = graph->getId();

  if (const auto it = minMaxNode.find(graphID); it == minMaxNode.end()) {
    return computeMinMaxNode(graph).second;
  } else {
    return it->second.second;
  }
}

template <typename nodeType, typename edgeType, typename propType>
typename edgeType::RealType
tlp::MinMaxProperty<nodeType, edgeType, propType>::getEdgeMin(const tlp::Graph *graph) {
  if (!graph) {
    graph = this->propType::graph;
  }

  unsigned int graphID = graph->getId();

  if (const auto it = minMaxEdge.find(graphID); it == minMaxEdge.end()) {
    return computeMinMaxEdge(graph).first;
  } else {
    return it->second.first;
  }
}

template <typename nodeType, typename edgeType, typename propType>
typename edgeType::RealType
tlp::MinMaxProperty<nodeType, edgeType, propType>::getEdgeMax(const tlp::Graph *graph) {
  if (!graph) {
    graph = this->propType::graph;
  }

  unsigned int graphID = graph->getId();

  if (const auto it = minMaxEdge.find(graphID); it == minMaxEdge.end()) {
    return computeMinMaxEdge(graph).second;
  } else {
    return it->second.second;
  }
}

template <typename nodeType, typename edgeType, typename propType>
MINMAX_PAIR(nodeType)
tlp::MinMaxProperty<nodeType, edgeType, propType>::computeMinMaxNode(const Graph *graph) {
  if (!graph) {
    graph = this->propType::graph;
  }

  typename nodeType::RealType maxN2 = _nodeMin, minN2 = _nodeMax;

  if (AbstractProperty<nodeType, edgeType, propType>::hasNonDefaultValuatedNodes(graph)) {
    for (auto n : graph->nodes()) {
      typename nodeType::RealType tmp = this->getNodeValue(n);

      if (tmp > maxN2) {
        maxN2 = tmp;
      }

      if (tmp < minN2) {
        minN2 = tmp;
      }
    }
  }

  if (maxN2 < minN2) {
    maxN2 = minN2 = AbstractProperty<nodeType, edgeType, propType>::nodeDefaultValue;
  }

  unsigned int sgi = graph->getId();

  // graph observation is now delayed
  // until we need to do some minmax computation
  // this will minimize the graph loading
  if (minMaxNode.find(sgi) == minMaxNode.end() && minMaxEdge.find(sgi) == minMaxEdge.end()) {
    // launch graph hierarchy observation
    graph->addListener(this);
  }

  MINMAX_PAIR(nodeType) minmax = {minN2, maxN2};
  return minMaxNode[sgi] = minmax;
}

template <typename nodeType, typename edgeType, typename propType>
MINMAX_PAIR(edgeType)
tlp::MinMaxProperty<nodeType, edgeType, propType>::computeMinMaxEdge(const Graph *graph) {
  typename edgeType::RealType maxE2 = _edgeMin, minE2 = _edgeMax;

  if (AbstractProperty<nodeType, edgeType, propType>::hasNonDefaultValuatedEdges(graph)) {
    for (auto ite : graph->edges()) {
      typename edgeType::RealType tmp = this->getEdgeValue(ite);

      if (tmp > maxE2) {
        maxE2 = tmp;
      }

      if (tmp < minE2) {
        minE2 = tmp;
      }
    }
  }

  if (maxE2 < minE2) {
    maxE2 = minE2 = AbstractProperty<nodeType, edgeType, propType>::edgeDefaultValue;
  }

  unsigned int sgi = graph->getId();

  // graph observation is now delayed
  // until we need to do some minmax computation
  // this will minimize the graph loading time
  if (minMaxNode.find(sgi) == minMaxNode.end() && minMaxEdge.find(sgi) == minMaxEdge.end()) {
    // launch graph hierarchy observation
    graph->addListener(this);
  }

  MINMAX_PAIR(edgeType) minmax = {minE2, maxE2};
  return minMaxEdge[sgi] = minmax;
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::removeListenersAndClearNodeMap() {
  // we need to clear one of our map
  // this will invalidate some minmax computations
  // so the graphs corresponding to these cleared minmax computations
  // may not have to be longer observed if they have no validated
  // minmax computation in the other map

  // loop to remove unneeded graph observation
  // it is the case if minmax computation
  //

  for (const auto &[graphId, minMax] : minMaxNode) {

    if (const auto itg = minMaxEdge.find(graphId); itg == minMaxEdge.end()) {
      // no computation in the other map
      // we can stop observing the current graph
      Graph *g = (propType::graph->getId() == graphId)
                     ? (needGraphListener ? nullptr : propType::graph)
                     : propType::graph->getDescendantGraph(graphId);

      if (g) {
        g->removeListener(this);
      }
    }
  }

  // finally clear the map
  minMaxNode.clear();
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::removeListenersAndClearEdgeMap() {
  // we need to clear one of our map
  // this will invalidate some minmax computations
  // so the graphs corresponding to these cleared minmax computations
  // may not have to be longer observed if they have no validated
  // minmax computation in the other map

  // loop to remove unneeded graph observation
  // it is the case if minmax computation

  for (const auto &[graphId, minMax] : minMaxEdge) {

    if (const auto itg = minMaxNode.find(graphId); itg == minMaxNode.end()) {
      // no computation in the other map
      // we can stop observing the current graph
      Graph *g = (propType::graph->getId() == graphId)
                     ? (needGraphListener ? nullptr : propType::graph)
                     : propType::graph->getDescendantGraph(graphId);

      if (g) {
        g->removeListener(this);
      }
    }
  }

  // finally clear the map
  minMaxEdge.clear();
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::updateNodeValue(
    tlp::node n, typename nodeType::RealType newValue) {

  if (!minMaxNode.empty()) {
    typename nodeType::RealType oldV = this->getNodeValue(n);

    if (newValue != oldV) {
      // loop on subgraph min/max
      for (const auto &[graphId, minMax] : minMaxNode) {
        // if min/max is ok for the current subgraph
        // check if min or max has to be updated
        const auto &[minV, maxV] = minMax;

        // check if min or max has to be updated
        if ((newValue < minV) || (newValue > maxV) || (oldV == minV) || (oldV == maxV)) {
          removeListenersAndClearNodeMap();
          break;
        }
      }
    }
  }
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::updateEdgeValue(
    tlp::edge e, typename edgeType::RealType newValue) {

  if (!minMaxEdge.empty()) {
    typename edgeType::RealType oldV = this->getEdgeValue(e);

    if (newValue != oldV) {
      // loop on subgraph min/max
      for (const auto &[graphId, minMax] : minMaxEdge) {
        // if min/max is ok for the current subgraph
        // check if min or max has to be updated
        const auto &[minV, maxV] = minMax;

        // check if min or max has to be updated
        if ((newValue < minV) || (newValue > maxV) || (oldV == minV) || (oldV == maxV)) {
          removeListenersAndClearEdgeMap();
          break;
        }
      }
    }
  }
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::updateAllNodesValues(
    typename nodeType::RealType newValue) {

  // loop on subgraph min/max
  MINMAX_PAIR(nodeType) minmax = {newValue, newValue};

  for (const auto &[graphId, minMax] : minMaxNode) {
    minMaxNode[graphId] = minmax;
  }
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::updateAllEdgesValues(
    typename edgeType::RealType newValue) {
  // loop on subgraph min/max
  MINMAX_PAIR(edgeType) minmax = {newValue, newValue};

  for (const auto &[graphId, minMax] : minMaxEdge) {
    minMaxEdge[graphId] = minmax;
  }
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::treatEvent(const tlp::Event &ev) {
  const auto *graphEvent = dynamic_cast<const tlp::GraphEvent *>(&ev);

  if (graphEvent) {
    tlp::Graph *graph = graphEvent->getGraph();

    switch (graphEvent->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      removeListenersAndClearNodeMap();
      break;

    case GraphEvent::TLP_DEL_NODE: {
      unsigned int sgi = graph->getId();

      if (const auto it = minMaxNode.find(sgi); it != minMaxNode.end()) {
        typename nodeType::RealType oldV = this->getNodeValue(graphEvent->getNode());

        // check if min or max has to be updated
        if ((oldV == it->second.first) || (oldV == it->second.second)) {
          minMaxNode.erase(it);

          if ((minMaxEdge.find(sgi) == minMaxEdge.end()) &&
              (!needGraphListener || (graph != propType::graph))) {
            // graph observation is no longer needed
            graph->removeListener(this);
          }
        }
      }

      break;
    }

    case GraphEvent::TLP_ADD_EDGE:
      removeListenersAndClearEdgeMap();
      break;

    case GraphEvent::TLP_DEL_EDGE: {
      unsigned int sgi = graph->getId();

      if (const auto it = minMaxEdge.find(sgi); it != minMaxEdge.end()) {
        typename edgeType::RealType oldV = this->getEdgeValue(graphEvent->getEdge());

        // check if min or max has to be updated
        if ((oldV == it->second.first) || (oldV == it->second.second)) {
          minMaxEdge.erase(it);

          if ((minMaxNode.find(sgi) == minMaxNode.end()) &&
              (!needGraphListener || (graph != propType::graph))) {
            // graph observation is no longer needed
            graph->removeListener(this);
          }
        }
      }

      break;
    }

    default:
      // we don't care about the rest
      break;
    }
  }
}
