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

#ifndef TALIPOT_TALIPOT_TO_OGDF_H
#define TALIPOT_TALIPOT_TO_OGDF_H

#include <talipot/config.h>
#include <talipot/Graph.h>
#include <talipot/TlpTools.h>
#include <talipot/StringProperty.h>
#include <talipot/NumericProperty.h>
#include <talipot/StaticProperty.h>

#include <ogdf/basic/GraphAttributes.h>

#include <exception>
#include <string>
#include <iostream>
#include <vector>

using namespace tlp;
using namespace ogdf;
using namespace std;

namespace tlp {
class DoubleProperty;
class SizeProperty;
}

class TLP_OGDF_SCOPE TalipotToOGDF {
public:
  TalipotToOGDF(tlp::Graph *g, bool importEdgeBends = true);

  tlp::Graph &getTlp();
  ogdf::GraphAttributes &getOGDFGraphAttr();
  ogdf::Graph &getOGDFGraph();

  ogdf::node getOGDFGraphNode(unsigned int nodeIndex);
  ogdf::edge getOGDFGraphEdge(unsigned int edgeIndex);

  tlp::Coord getNodeCoordFromOGDFGraphAttr(unsigned int nodeIndex);

  void copyTlpNumericPropertyToOGDFEdgeLength(tlp::NumericProperty *);
  void copyTlpNumericPropertyToOGDFNodeWeight(tlp::NumericProperty *);
  void copyTlpNodeSizeToOGDF(tlp::SizeProperty *);

  vector<tlp::Coord> getEdgeCoordFromOGDFGraphAttr(unsigned int edgeIndex);

private:
  tlp::Graph *talipotGraph;
  tlp::NodeStaticProperty<ogdf::node> ogdfNodes;
  tlp::EdgeStaticProperty<ogdf::edge> ogdfEdges;
  ogdf::Graph ogdfGraph;
  ogdf::GraphAttributes ogdfAttributes;
};

#endif // TALIPOT_TALIPOT_TO_OGDF_H
