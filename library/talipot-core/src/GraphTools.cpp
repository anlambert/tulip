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

#include <talipot/GraphMeasure.h>
#include <talipot/TreeTest.h>
#include <talipot/DoubleProperty.h>
#include <talipot/IntegerProperty.h>
#include <talipot/Ordering.h>
#include <talipot/Dijkstra.h>
#ifndef NDEBUG
#include <talipot/AcyclicTest.h>
#include <talipot/ConnectedTest.h>
#endif

using namespace std;

namespace tlp {
static Iterator<node> *getInNodes(const Graph *graph, const node n) {
  return graph->getInNodes(n);
}
static Iterator<node> *getOutNodes(const Graph *graph, const node n) {
  return graph->getOutNodes(n);
}
static Iterator<node> *getInOutNodes(const Graph *graph, const node n) {
  return graph->getInOutNodes(n);
}

NodesIteratorFn getNodesIterator(EDGE_TYPE direction) {
  switch (direction) {
  case DIRECTED:
    return &getOutNodes;

  case INV_DIRECTED:
    return &getInNodes;

  case UNDIRECTED:
  default:
    return &getInOutNodes;
  }
}

static Iterator<edge> *getInEdges(const Graph *graph, const node n) {
  return graph->getInEdges(n);
}
static Iterator<edge> *getOutEdges(const Graph *graph, const node n) {
  return graph->getOutEdges(n);
}
static Iterator<edge> *getInOutEdges(const Graph *graph, const node n) {
  return graph->getInOutEdges(n);
}

EdgesIteratorFn getEdgesIterator(EDGE_TYPE direction) {
  switch (direction) {
  case DIRECTED:
    return &getOutEdges;

  case INV_DIRECTED:
    return &getInEdges;

  case UNDIRECTED:
  default:
    return &getInOutEdges;
  }
}

//======================================================================
void makeProperDag(Graph *graph, list<node> &addedNodes,
                   std::unordered_map<edge, edge> &replacedEdges, IntegerProperty *edgeLength) {
  if (TreeTest::isTree(graph)) {
    return;
  }

  assert(AcyclicTest::isAcyclic(graph));
  // We compute the dag level metric on resulting sg.
  NodeStaticProperty<unsigned int> dLevel(graph);
  dagLevel(graph, dLevel);

  if (edgeLength) {
    edgeLength->setAllEdgeValue(1);
  }

  // we now transform the dag in a proper Dag, two linked nodes of a proper dag
  // must have a difference of one of dag level metric.
  const vector<edge> &edges = graph->edges();
  unsigned int nbEdges = edges.size();
  for (unsigned int i = 0; i < nbEdges; ++i) {
    edge e = edges[i];
    auto [src, tgt] = graph->ends(e);
    unsigned int fLevel = dLevel.getNodeValue(src);
    unsigned int sLevel = dLevel.getNodeValue(tgt);
    int delta = sLevel - fLevel;

    if (delta > 1) {
      node n1 = graph->addNode();
      replacedEdges[e] = graph->addEdge(src, n1);
      addedNodes.push_back(n1);
      dLevel.addNodeValue(n1, fLevel + 1);

      if (delta > 2) {
        node n2 = graph->addNode();
        addedNodes.push_back(n2);
        edge e = graph->addEdge(n1, n2);

        if (edgeLength) {
          edgeLength->setEdgeValue(e, delta - 2);
        }

        dLevel.addNodeValue(n2, sLevel - 1);
        n1 = n2;
      }

      graph->addEdge(n1, tgt);
    }
  }

  for (const auto &[e, ee] : replacedEdges) {
    graph->delEdge(e);
  }

  assert(AcyclicTest::isAcyclic(graph));
}
//======================================================================
node makeSimpleSource(Graph *graph) {
  assert(AcyclicTest::isAcyclic(graph));

  node startNode = graph->addNode();

  for (auto n : graph->nodes()) {
    if (n != startNode && graph->indeg(n) == 0) {
      graph->addEdge(startNode, n);
    }
  }

  assert(AcyclicTest::isAcyclic(graph));
  return startNode;
}
//======================================================================
vector<vector<node>> computeCanonicalOrdering(PlanarConMap *carte, std::vector<edge> *dummyEdges,
                                              PluginProgress *pluginProgress) {
  Ordering o(carte, pluginProgress, 0, 100, 100); // feedback (0% -> 100%)

  if (dummyEdges != nullptr) {
    *dummyEdges = o.getDummyEdges();
  }

  vector<vector<node>> res;
  unsigned int nbMax = o.size();

  if (nbMax) {
    res.reserve(nbMax);

    while (nbMax) {
      res.push_back(o[--nbMax]);
    }
  }

  return res;
}
//======================================================================
std::vector<node> computeGraphCenters(Graph *graph) {
  assert(ConnectedTest::isConnected(graph));
  tlp::NodeStaticProperty<unsigned int> dist(graph);
  const std::vector<node> &nodes = graph->nodes();
  unsigned int nbNodes = nodes.size();
  unsigned int minD = UINT_MAX;
  unsigned int minPos = 0;

  TLP_PARALLEL_MAP_INDICES(nbNodes, [&](unsigned int i) {
    tlp::NodeStaticProperty<unsigned int> tmp(graph);
    unsigned int maxD = tlp::maxDistance(graph, i, tmp, UNDIRECTED);
    dist[i] = maxD;
    TLP_LOCK_SECTION(COMPUTE_MIN) {
      if (minD > maxD) {
        minD = maxD;
        minPos = i;
      }
    }
    TLP_UNLOCK_SECTION(COMPUTE_MIN);
  });

  vector<node> result;

  for (unsigned int i = minPos; i < nbNodes; ++i) {
    if (dist[i] == minD) {
      result.push_back(nodes[i]);
    }
  }

  return result;
}
//======================================================================
node graphCenterHeuristic(Graph *graph, PluginProgress *pluginProgress) {
  assert(ConnectedTest::isConnected(graph));

  unsigned int nbNodes = graph->numberOfNodes();

  if (nbNodes == 0) {
    return node();
  }

  const vector<node> &nodes = graph->nodes();
  tlp::NodeStaticProperty<bool> toTreat(graph);
  toTreat.setAll(true);
  tlp::NodeStaticProperty<unsigned int> dist(graph);
  unsigned int i = 0, n = 0, result = 0;
  unsigned int cDist = UINT_MAX - 2;
  unsigned int nbTry = 2 + sqrt(nbNodes);
  unsigned int maxTries = nbTry;

  while (nbTry) {
    --nbTry;

    if (pluginProgress) {
      pluginProgress->setComment("Computing graph center...");

      if (maxTries - nbTry % 200 == 0) {
        pluginProgress->progress(maxTries - nbTry, maxTries);
      }
    }

    if (toTreat[n]) {
      ++i;
      unsigned int di = tlp::maxDistance(graph, n, dist);
      toTreat[n] = false;

      if (di < cDist) {
        result = n;
        cDist = di;
      } else {
        unsigned int delta = di - cDist;

        for (unsigned int v = 0; v < nbNodes; v++) {
          if (dist[v] < delta) {
            // all the nodes at distance less than delta can't be center
            toTreat[v] = false;
          }
        }
      }

      unsigned int nextMax = 0;

      for (unsigned int v = 0; v < nbNodes; v++) {
        if (dist[v] > (di / 2 + di % 2)) {
          toTreat[v] = false;
        } else {
          if (toTreat[v]) {
            if (dist[v] > nextMax) {
              n = v;
              nextMax = dist[v];
            }
          }
        }
      }

      if (nextMax == 0) {
        break;
      }
    }
  }

  if (pluginProgress) {
    pluginProgress->setComment("Graph center computed");
    pluginProgress->progress(100, 100);
  }

  return nodes[result];
}
//======================================================================
void selectSpanningForest(Graph *graph, BooleanProperty *selectionProperty,
                          PluginProgress *pluginProgress) {
  list<node> fifo;

  NodeStaticProperty<bool> nodeFlag(graph);
  const std::vector<node> &nodes = graph->nodes();
  unsigned int nbNodes = nodes.size();

  unsigned int nbSelectedNodes = selectionProperty->numberOfNonDefaultValuatedNodes();

  // get previously selected nodes
  if (nbSelectedNodes) {
    for (unsigned int i = 0; i < nbNodes; ++i) {
      node n = nodes[i];

      if (selectionProperty->getNodeValue(n)) {
        fifo.push_back(n);
        nodeFlag[i] = true;
      }
    }
  } else {
    node n = graph->getOneNode();
    fifo.push_back(n);
    nodeFlag[n] = true;
    nbSelectedNodes = 1;
  }

  EdgeStaticProperty<bool> edgeSel(graph);
  edgeSel.setAll(true);

  // select all nodes
  for (auto n : graph->nodes()) {
    selectionProperty->setNodeValue(n, true);
  }

  bool ok = true;
  unsigned int edgeCount = 0;

  while (ok) {
    while (!fifo.empty()) {
      node n1 = fifo.front();
      fifo.pop_front();
      for (auto adjit : graph->getOutEdges(n1)) {
        node tgt = graph->target(adjit);
        unsigned int tgtPos = graph->nodePos(tgt);

        if (!nodeFlag[tgtPos]) {
          nodeFlag[tgtPos] = true;
          ++nbSelectedNodes;
          fifo.push_back(tgt);
        } else {
          edgeSel[adjit] = false;
        }

        if (pluginProgress) {
          pluginProgress->setComment("Computing a spanning forest...");
          ++edgeCount;

          if (edgeCount == 200) {
            if (pluginProgress->progress(nbSelectedNodes * 100 / graph->numberOfNodes(), 100) !=
                TLP_CONTINUE) {
              return;
            }

            edgeCount = 0;
          }
        }
      }
    }

    ok = false;
    bool degZ = false;
    node goodNode = graph->getOneNode();

    for (unsigned int i = 0; i < nbNodes; ++i) {
      node n = nodes[i];

      if (!nodeFlag[i]) {
        if (!ok) {
          goodNode = n;
          ok = true;
        }

        if (graph->indeg(n) == 0) {
          fifo.push_back(n);
          nodeFlag[i] = true;
          ++nbSelectedNodes;
          degZ = true;
        }

        if (!degZ) {

          if (graph->indeg(n) < graph->indeg(goodNode)) {
            goodNode = n;
          } else {
            if ((graph->indeg(n) == graph->indeg(goodNode)) &&
                (graph->outdeg(n) > graph->outdeg(goodNode)))
              goodNode = n;
          }
        }
      }
    }

    if (ok && (!degZ)) {
      fifo.push_back(goodNode);
      nodeFlag[goodNode] = true;
      ++nbSelectedNodes;
    }
  }

  edgeSel.copyToProperty(selectionProperty);
}
//======================================================================
void selectSpanningTree(Graph *graph, BooleanProperty *selection, PluginProgress *pluginProgress) {
  assert(ConnectedTest::isConnected(graph));
  selection->setAllNodeValue(false);
  selection->setAllEdgeValue(false);

  node root = graphCenterHeuristic(graph, pluginProgress);
  unsigned int size = graph->numberOfNodes();
  unsigned int nbNodes = 1, edgeCount = 0;
  vector<node> roots;
  unsigned int i = 0;
  selection->setNodeValue(root, true);
  roots.push_back(root);

  while (nbNodes != size) {
    root = roots[i];

    for (auto e : graph->getInOutEdges(root)) {

      if (!selection->getEdgeValue(e)) {
        node neighbour = graph->opposite(e, root);

        if (!selection->getNodeValue(neighbour)) {
          selection->setNodeValue(neighbour, true);
          roots.push_back(neighbour);
          nbNodes++;
          selection->setEdgeValue(e, true);

          if (pluginProgress) {
            pluginProgress->setComment("Computing spanning tree...");
            ++edgeCount;

            if (edgeCount % 200 == 0) {
              if (pluginProgress->progress(edgeCount, graph->numberOfEdges()) != TLP_CONTINUE)
                return;
            }
          }
        }
      }
    }

    i++;
  }

  if (pluginProgress) {
    pluginProgress->setComment("Spanning tree computed");
    pluginProgress->progress(100, 100);
  }
}
//======================================================================
struct ltEdge {
  NumericProperty *m;
  ltEdge(NumericProperty *metric) : m(metric) {}
  bool operator()(const edge e1, const edge e2) const {
    return (m->getEdgeDoubleValue(e1) < m->getEdgeDoubleValue(e2));
  }
};

void selectMinimumSpanningTree(Graph *graph, BooleanProperty *selection,
                               NumericProperty *edgeWeight, PluginProgress *pluginProgress) {
  assert(ConnectedTest::isConnected(graph));

  if (!edgeWeight) {
    // no weight return a spanning tree
    return selectSpanningTree(graph, selection, pluginProgress);
  }

  const vector<node> &nodes = graph->nodes();

  for (auto node : nodes) {
    selection->setNodeValue(node, true);
  }

  selection->setAllEdgeValue(false);

  NodeStaticProperty<unsigned int> classes(graph);

  unsigned int nbNodes = nodes.size();
  unsigned int numClasses = nbNodes;

  TLP_PARALLEL_MAP_INDICES(nbNodes, [&](unsigned int i) { classes[i] = i; });

  unsigned int maxCount = numClasses;
  unsigned int edgeCount = 0;

  std::vector<edge> sortedEdges(graph->edges());
  std::sort(sortedEdges.begin(), sortedEdges.end(), ltEdge(edgeWeight));
  unsigned int nbEdges = sortedEdges.size();
  unsigned int iE = 0;

  while (numClasses > 1) {
    edge cur;
    unsigned int srcClass = 0, tgtClass = 0;

    for (; iE < nbEdges; ++iE) {
      const auto &[src, tgt] = graph->ends(cur = sortedEdges[iE]);

      if ((srcClass = classes[src]) != (tgtClass = classes[tgt])) {
        break;
      }
    }

    selection->setEdgeValue(cur, true);

    if (pluginProgress) {
      pluginProgress->setComment("Computing minimum spanning tree...");
      ++edgeCount;

      if (edgeCount == 200) {
        if (pluginProgress->progress((maxCount - numClasses) * 100 / maxCount, 100) != TLP_CONTINUE)
          return;

        edgeCount = 0;
      }
    }

    TLP_PARALLEL_MAP_INDICES(nbNodes, [&](unsigned int i) {
      if (classes[i] == tgtClass)
        classes[i] = srcClass;
    });

    --numClasses;
  }
}
//======================================================================

static void bfs(const Graph *graph, node root, NodeStaticProperty<bool> &visited,
                vector<node> &nodes) {
  if (visited[root]) {
    return;
  }
  visited[root] = true;
  list<node> queue;
  queue.push_back(root);

  while (!queue.empty()) {
    node current = queue.front();
    queue.pop_front();
    nodes.push_back(current);

    for (auto neigh : graph->getInOutNodes(current)) {
      if (!visited[neigh]) {
        visited[neigh] = true;
        queue.push_back(neigh);
      }
    }
  }
}

// bfs from a root node
std::vector<tlp::node> bfs(const Graph *graph, node root) {
  vector<node> nodes;
  if (!graph->isEmpty()) {
    if (!root.isValid()) {
      root = graph->getSource();

      if (!root.isValid()) {
        root = graph->getOneNode();
      }
    }

    assert(graph->isElement(root));
    NodeStaticProperty<bool> visited(graph);
    visited.setAll(false);
    bfs(graph, root, visited, nodes);
  }
  return nodes;
}

// cumulative bfs from every node of the graph
std::vector<tlp::node> bfs(const Graph *graph) {
  vector<node> nodes;
  NodeStaticProperty<bool> visited(graph);
  visited.setAll(false);
  for (auto n : graph->nodes()) {
    bfs(graph, n, visited, nodes);
  }
  return nodes;
}

//======================================================================

static void dfs(const Graph *graph, node root, NodeStaticProperty<bool> &visited,
                vector<node> &nodes) {
  if (visited[root]) {
    return;
  }

  std::stack<tlp::node> toVisit;
  toVisit.push(root);
  visited[root] = true;

  while (!toVisit.empty()) {
    node current = toVisit.top();
    toVisit.pop();
    nodes.push_back(current);

    for (auto e : reversed(graph->allEdges(current))) {
      if (graph != graph->getRoot() && !graph->isElement(e)) {
        continue;
      }
      node neigh = graph->opposite(e, current);
      if (!visited[neigh]) {
        visited[neigh] = true;
        toVisit.push(neigh);
      }
    }
  }
}

// dfs from a root node
std::vector<node> dfs(const Graph *graph, node root) {
  vector<node> nodes;
  if (!graph->isEmpty()) {
    if (!root.isValid()) {
      root = graph->getSource();

      if (!root.isValid()) {
        root = graph->getOneNode();
      }
    }

    assert(graph->isElement(root));
    NodeStaticProperty<bool> visited(graph);
    visited.setAll(false);
    dfs(graph, root, visited, nodes);
  }
  return nodes;
}

// cumulative dfs from every node of the graph
std::vector<tlp::node> dfs(const Graph *graph) {
  vector<node> nodes;
  NodeStaticProperty<bool> visited(graph);
  visited.setAll(false);
  for (auto n : graph->nodes()) {
    dfs(graph, n, visited, nodes);
  }
  return nodes;
}

//==================================================
void buildNodesUniformQuantification(const Graph *graph, const NumericProperty *prop,
                                     unsigned int k, std::map<double, int> &nodeMapping) {
  // build the histogram of node values
  map<double, int> histogram;
  const std::vector<node> &nodes = graph->nodes();
  unsigned int nbNodes = nodes.size();

  for (unsigned int i = 0; i < nbNodes; ++i) {
    double value = prop->getNodeDoubleValue(nodes[i]);

    if (auto it = histogram.find(value); it == histogram.end()) {
      histogram[value] = 1;
    } else {
      ++(it->second);
    }
  }

  // Build the color map
  double sum = 0;
  double cK = double(nbNodes) / double(k);
  int k2 = 0;

  for (const auto &[val, count] : histogram) {
    sum += count;
    nodeMapping[val] = k2;

    if (sum > cK * (k2 + 1)) {
      k2 = ceil(sum / cK) - 1;
    }
  }
}
//==================================================
void buildEdgesUniformQuantification(const Graph *graph, const NumericProperty *prop,
                                     unsigned int k, std::map<double, int> &edgeMapping) {
  // build the histogram of edges values
  map<double, int> histogram;
  for (auto e : graph->edges()) {
    double value = prop->getEdgeDoubleValue(e);

    if (auto it = histogram.find(value); it == histogram.end()) {
      histogram[value] = 1;
    } else {
      ++(it->second);
    }
  }

  // Build the color map
  double sum = 0;
  double cK = double(graph->numberOfEdges()) / double(k);
  int k2 = 0;

  for (const auto &[val, count] : histogram) {
    sum += count;
    edgeMapping[val] = k2;

    if (sum > cK * (k2 + 1)) {
      k2 = ceil(sum / cK) - 1;
    }
  }
}

unsigned makeSelectionGraph(const Graph *graph, BooleanProperty *selection, bool *test) {
  Observable::holdObservers();
  unsigned added = 0;
  for (auto e : selection->getEdgesEqualTo(true, graph)) {
    const auto &[src, tgt] = graph->ends(e);

    if (!selection->getNodeValue(src)) {
#ifndef NDEBUG
      tlp::debug() << "[Make selection a graph] node #" << src.id << " source of edge #" << e.id
                   << " automatically added to selection.";
#endif
      selection->setNodeValue(src, true);
      added++;

      if (test) {
        *test = false;
        return -1;
      }
    }

    if (!selection->getNodeValue(tgt)) {
#ifndef NDEBUG
      tlp::debug() << "[Make selection a graph] node #" << tgt << " target of edge #" << e.id
                   << " automatically added to selection.";
#endif
      selection->setNodeValue(tgt, true);
      added++;

      if (test) {
        *test = false;
        return -1;
      }
    }
  }
  Observable::unholdObservers();

  if (test) {
    *test = true;
  }

  return added;
}

#define SMALLEST_WEIGHT 1.E-6

bool selectShortestPaths(const Graph *const graph, node src, node tgt, ShortestPathType pathType,
                         const DoubleProperty *const weights, BooleanProperty *result) {
  EDGE_TYPE direction;

  switch (pathType) {
  case ShortestPathType::OnePath:
  case ShortestPathType::AllPaths:
    direction = UNDIRECTED;
    break;
  case ShortestPathType::OneDirectedPath:
  case ShortestPathType::AllDirectedPaths:
    direction = DIRECTED;
    break;
  case ShortestPathType::OneReversedPath:
  case ShortestPathType::AllReversedPaths:
  default:
    direction = INV_DIRECTED;
  }

  EdgeStaticProperty<double> eWeights(graph);
  if (!weights) {
    eWeights.setAll(SMALLEST_WEIGHT);
  } else {
    auto fn = [&](edge e, unsigned int i) {
      double val(weights->getEdgeValue(e));

      eWeights[i] = val ? val : SMALLEST_WEIGHT;
    };
    TLP_PARALLEL_MAP_EDGES_AND_INDICES(graph, fn);
  }

  NodeStaticProperty<double> nodeDistance(graph);
  Dijkstra dijkstra(graph, src, eWeights, nodeDistance, direction);

  if (uint(pathType) < ShortestPathType::AllPaths) {
    return dijkstra.searchPath(tgt, result);
  }
  return dijkstra.searchPaths(tgt, result);
}

void markReachableNodes(const Graph *graph, const node startNode,
                        std::unordered_map<node, bool> &result, unsigned int maxDistance,
                        EDGE_TYPE direction) {
  deque<node> fifo;
  MutableContainer<bool> visited;
  MutableContainer<unsigned int> distance;
  visited.setAll(false);
  distance.setAll(graph->numberOfNodes());
  fifo.push_back(startNode);
  visited.set(startNode.id, true);
  distance.set(startNode.id, 0);

  auto getNodes = getNodesIterator(direction);

  while (!fifo.empty()) {
    node current = fifo.front();
    unsigned int curDist = distance.get(current.id);
    fifo.pop_front();

    if (curDist < maxDistance) {
      for (auto itn : getNodes(graph, current)) {
        if (!visited.get(itn.id)) {
          fifo.push_back(itn);
          result[itn] = true;
          visited.set(itn.id, true);
          distance.set(itn.id, curDist + 1);
        }
      }
    }
  }
}

void computeDijkstra(const Graph *const graph, node src, const EdgeStaticProperty<double> &weights,
                     NodeStaticProperty<double> &nodeDistance, EDGE_TYPE direction,
                     unordered_map<node, std::list<node>> &ancestors, std::stack<node> *queueNodes,
                     MutableContainer<int> *numberOfPaths) {
  Dijkstra dijkstra(graph, src, weights, nodeDistance, direction, queueNodes, numberOfPaths);
  dijkstra.ancestors(ancestors);
}
}
