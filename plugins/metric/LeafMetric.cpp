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

#include <talipot/AcyclicTest.h>

#include "LeafMetric.h"

PLUGIN(LeafMetric)

using namespace std;
using namespace tlp;

//=======================================================================
LeafMetric::LeafMetric(const tlp::PluginContext *context) : DoubleAlgorithm(context) {}

// structure below is used to implement dfs loop
struct dfsLeafStruct {
  node current;
  Iterator<node> *outNodes;
  double res;

  dfsLeafStruct(node n, Iterator<node> *nodes) : current(n), outNodes(nodes), res(0.0) {}
};
//=======================================================================
// original recursive algorithm
/*double LeafMetric::getNodeValue(const tlp::node n) {
    if (result->getNodeValue(n) != 0)
      return result->getNodeValue(n);
    double res=0;
    for (auto n : graph->getOutNodes(n))
      res += getNodeValue(n);
    if (res==0) res=1.0;
    result->setNodeValue(n, res);
    return res;
  }*/
//=======================================================================
double LeafMetric::getNodeValue(tlp::node current) {
  double value = result->getNodeValue(current);

  if (value != 0.0) {
    return value;
  }

  // dfs loop
  stack<dfsLeafStruct> dfsLevels;
  Iterator<node> *outNodes = graph->getOutNodes(current);
  dfsLeafStruct dfsParams(current, outNodes);
  double res = 0.0;
  dfsLevels.push(dfsParams);

  while (!dfsLevels.empty()) {
    while (outNodes->hasNext()) {
      node neighbour = outNodes->next();
      value = result->getNodeValue(neighbour);

      // compute res
      if (value != 0.0) {
        res += value;
      } else {
        // store res for current
        dfsLevels.top().res = res;
        // push new dfsParams on stack
        current = dfsParams.current = neighbour;
        outNodes = dfsParams.outNodes = graph->getOutNodes(neighbour);
        res = dfsParams.res = 0.0;
        dfsLevels.push(dfsParams);
        // and go deeper
        break;
      }
    }

    if (outNodes->hasNext()) {
      // new dfsParams has been pushed on stack
      continue;
    }

    // save current res
    if (res == 0.0) {
      res = 1.0;
    }

    result->setNodeValue(current, res);
    // unstack current dfsParams
    delete outNodes;
    dfsLevels.pop();

    if (dfsLevels.empty()) {
      break;
    }

    // get dfsParams on top of dfsLevels
    dfsParams = dfsLevels.top();
    current = dfsParams.current;
    outNodes = dfsParams.outNodes;
    // update new current res if any
    dfsParams.res += res;
    res = dfsParams.res;
  }

  return res;
}
//=======================================================================
bool LeafMetric::run() {
  result->setAllNodeValue(0);
  result->setAllEdgeValue(0);
  for (auto n : graph->nodes()) {
    result->setNodeValue(n, getNodeValue(n));
  }
  return true;
}
//=======================================================================
bool LeafMetric::check(std::string &erreurMsg) {
  if (!AcyclicTest::isAcyclic(graph)) {
    erreurMsg = "The graph must be a acyclic.";
    return false;
  }

  return true;
}
//=======================================================================
