/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <list>
#include <tulip/Graph.h>
#include <tulip/ConnectedTest.h>
#include <tulip/MutableContainer.h>
#include <tulip/StaticProperty.h>

using namespace std;
using namespace tlp;
//=================================================================
ConnectedTest *ConnectedTest::instance = nullptr;
//=================================================================
bool ConnectedTest::isConnected(const tlp::Graph *const graph) {
  if (instance == nullptr)
    instance = new ConnectedTest();

  return instance->compute(graph);
}
//=================================================================
void ConnectedTest::makeConnected(Graph *graph, vector<edge> &addedEdges) {
  if (instance == nullptr)
    instance = new ConnectedTest();

  graph->removeListener(instance);
  instance->resultsBuffer.erase(graph);
  vector<node> toLink;
  instance->connect(graph, toLink);

  for (unsigned int i = 1; i < toLink.size(); ++i)
    addedEdges.push_back(graph->addEdge(toLink[i - 1], toLink[i]));

  assert(ConnectedTest::isConnected(graph));
}
//=================================================================
unsigned int ConnectedTest::numberOfConnectedComponents(const tlp::Graph *const graph) {
  if (graph->numberOfNodes() == 0)
    return 0u;

  if (instance == nullptr)
    instance = new ConnectedTest();

  graph->removeListener(instance);
  vector<node> toLink;
  instance->connect(graph, toLink);
  unsigned int result;

  if (!toLink.empty())
    result = toLink.size();
  else
    result = 1u;

  instance->resultsBuffer[graph] = (result == 1u);
  graph->addListener(instance);
  return result;
}
//======================================================================
void ConnectedTest::computeConnectedComponents(const tlp::Graph *graph,
                                               vector<vector<node>> &components) {
  NodeStaticProperty<bool> visited(graph);
  visited.setAll(false);

  // do a bfs traversal for each node
  for (const node &n : graph->nodes()) {

    // check if curNode has been already visited
    if (!visited[n]) {
      // add a new component
      components.push_back(std::vector<node>());
      std::vector<node> &component = components.back();
      // and initialize it with current node
      component.push_back(n);
      // do a bfs traversal this node
      list<node> nodesToVisit;
      visited[n] = true;
      nodesToVisit.push_front(n);

      while (!nodesToVisit.empty()) {
        node curNode = nodesToVisit.front();
        nodesToVisit.pop_front();
        // loop on all neighbours
        Iterator<node> *itn = graph->getInOutNodes(curNode);

        while (itn->hasNext()) {
          node neighbour = itn->next();

          // check if neighbour has been visited
          if (!visited[neighbour]) {
            // mark neighbour as already visited
            visited[neighbour] = true;
            // insert it in current component
            component.push_back(neighbour);
            // push it for further deeper exploration
            nodesToVisit.push_back(neighbour);
          }
        }

        delete itn;
      }
    }
  }
}

//======================================================================
void ConnectedTest::computeConnectedComponents(const tlp::Graph *graph,
                                               vector<set<node>> &components) {
  std::vector<std::vector<node>> vComponents;
  computeConnectedComponents(graph, vComponents);
  unsigned int nbComp = vComponents.size();
  components.resize(nbComp);

  for (unsigned int i = 0; i < nbComp; ++i) {
    const std::vector<node> &vNodes = vComponents[i];
    std::set<node> &sNodes = components[i];
    sNodes.clear();
    unsigned int nbNodes = vNodes.size();

    for (unsigned int j = 0; j < nbNodes; ++j)
      sNodes.insert(vNodes[j]);
  }
}

//=================================================================
static unsigned int connectedTest(const Graph *const graph, node n,
                                  NodeStaticProperty<bool> &visited) {
  list<node> nodesToVisit;
  visited[n] = true;
  nodesToVisit.push_front(n);
  unsigned int count = 1;

  while (!nodesToVisit.empty()) {
    node r = nodesToVisit.front();
    nodesToVisit.pop_front();
    // loop on all neighbours
    Iterator<node> *itn = graph->getInOutNodes(r);

    while (itn->hasNext()) {
      node neighbour = itn->next();

      // check if neighbour has been visited
      if (!visited[neighbour]) {
        // mark neighbour as already visited
        visited[neighbour] = true;
        // push it for further deeper exploration
        nodesToVisit.push_back(neighbour);
        ++count;
      }
    }

    delete itn;
  }

  return count;
}
//=================================================================
ConnectedTest::ConnectedTest() {}
//=================================================================
bool ConnectedTest::compute(const tlp::Graph *const graph) {
  if (resultsBuffer.find(graph) != resultsBuffer.end())
    return resultsBuffer[graph];

  if (graph->numberOfNodes() == 0)
    return true;

  NodeStaticProperty<bool> visited(graph);
  visited.setAll(false);
  unsigned int count = connectedTest(graph, graph->getOneNode(), visited);
  bool result = (count == graph->numberOfNodes());
  resultsBuffer[graph] = result;
  graph->addListener(this);
  return result;
}
//=================================================================
void ConnectedTest::connect(const tlp::Graph *const graph, vector<node> &toLink) {
  if (resultsBuffer.find(graph) != resultsBuffer.end()) {
    if (resultsBuffer[graph])
      return;
  }

  if (graph->numberOfNodes() == 0)
    return;

  NodeStaticProperty<bool> visited(graph);
  visited.setAll(false);

  for (const node &n : graph->nodes()) {

    if (!visited[n]) {
      toLink.push_back(n);
      connectedTest(graph, n, visited);
    }
  }
}
//=================================================================
void ConnectedTest::treatEvent(const Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    switch (gEvt->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      resultsBuffer[graph] = false;
      break;

    case GraphEvent::TLP_DEL_NODE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_ADD_EDGE:

      if (resultsBuffer.find(graph) != resultsBuffer.end())
        if (resultsBuffer[graph])
          return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_DEL_EDGE:

      if (resultsBuffer.find(graph) != resultsBuffer.end())
        if (!resultsBuffer[graph])
          return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    default:
      break;
    }
  } else {

    Graph *graph = static_cast<Graph *>(evt.sender());

    if (evt.type() == Event::TLP_DELETE)
      resultsBuffer.erase(graph);
  }
}
