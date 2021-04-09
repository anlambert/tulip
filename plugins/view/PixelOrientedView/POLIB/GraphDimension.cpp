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

#include "GraphDimension.h"

#include <talipot/DoubleProperty.h>
#include <talipot/IntegerProperty.h>
#include <talipot/StringProperty.h>
#include <talipot/ConversionIterator.h>
#include <talipot/ConcatIterator.h>

using namespace tlp;
using namespace std;

map<Graph *, unsigned int> GraphDimension::graphDimensionsMap;

GraphDimension::GraphDimension(Graph *graph, const string &dimName)
    : graph(graph), dimName(dimName) {
  nodeSorter = NodeMetricSorter::instance(graph);
  nodeSorter->sortNodesForProperty(dimName);
  propertyType = graph->getProperty(dimName)->getTypename();

  if (graphDimensionsMap.find(graph) == graphDimensionsMap.end()) {
    graphDimensionsMap[graph] = 1;
  } else {
    ++graphDimensionsMap[graph];
  }
}

GraphDimension::~GraphDimension() {
  --graphDimensionsMap[graph];

  if (graphDimensionsMap[graph] == 0) {
    delete nodeSorter;
    graphDimensionsMap.erase(graph);
  }
}

void GraphDimension::updateNodesRank() {
  nodeSorter->sortNodesForProperty(dimName);
}

unsigned int GraphDimension::numberOfItems() const {
  return graph->numberOfNodes();
}

unsigned int GraphDimension::numberOfValues() const {
  return nodeSorter->getNbValuesForProperty(dimName);
}

template <typename PROPERTY>
double GraphDimension::getNodeValue(const node n) const {
  auto *dimValues = graph->getProperty<PROPERTY>(dimName);
  double d = dimValues->getNodeValue(n);
  double delta = maxValue() - minValue();
  return (d - minValue()) / delta;
}

std::string GraphDimension::getItemLabelAtRank(const unsigned int rank) const {
  node n = nodeSorter->getNodeAtRankForProperty(rank, dimName);
  string label = graph->getStringProperty("viewLabel")->getNodeValue(n);
  return label;
}

std::string GraphDimension::getItemLabel(const unsigned int itemId) const {
  string label = graph->getStringProperty("viewLabel")->getNodeValue(node(itemId));
  return label;
}

double GraphDimension::getItemValue(const unsigned int itemId) const {
  if (propertyType == "double") {
    return getNodeValue<DoubleProperty>(node(itemId));
  } else if (propertyType == "int") {
    return getNodeValue<IntegerProperty>(node(itemId));
  } else {
    return 0;
  }
}

double GraphDimension::getItemValueAtRank(const unsigned int rank) const {
  node n = nodeSorter->getNodeAtRankForProperty(rank, dimName);

  if (propertyType == "double") {
    return getNodeValue<DoubleProperty>(n);
  } else if (propertyType == "int") {
    return getNodeValue<IntegerProperty>(n);
  } else {
    return 0;
  }
}

unsigned int GraphDimension::getItemIdAtRank(const unsigned int rank) {
  node n = nodeSorter->getNodeAtRankForProperty(rank, dimName);
  return n.id;
}

unsigned int GraphDimension::getRankForItem(const unsigned int itemId) {
  return nodeSorter->getNodeRankForProperty(node(itemId), dimName);
}

double GraphDimension::minValue() const {
  if (propertyType == "double") {
    return graph->getDoubleProperty(dimName)->getNodeMin(graph);
  } else if (propertyType == "int") {
    return graph->getIntegerProperty(dimName)->getNodeMin(graph);
  } else {
    return 0;
  }
}

double GraphDimension::maxValue() const {
  if (propertyType == "double") {
    return graph->getDoubleProperty(dimName)->getNodeMax(graph);
  } else if (propertyType == "int") {
    return graph->getIntegerProperty(dimName)->getNodeMax(graph);
  } else {
    return 0;
  }
}

vector<unsigned int> GraphDimension::links(const unsigned int itemId) const {
  return iteratorVector(conversionIterator<unsigned int>(
      concatIterator(graph->getInNodes(node(itemId)), graph->getOutNodes(node(itemId))),
      [](node n) { return n.id; }));
}
