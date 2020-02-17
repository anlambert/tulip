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

#include "talipot2ogdf/TalipotToOGDF.h"
#include <talipot/PropertyTypes.h>
#include <talipot/LayoutProperty.h>
#include <talipot/SizeProperty.h>
#include <talipot/NumericProperty.h>

#include <ogdf/basic/geometry.h>

#include <vector>

using namespace std;

TalipotToOGDF::TalipotToOGDF(tlp::Graph *g, bool importEdgeBends)
    : talipotGraph(g), ogdfNodes(g), ogdfEdges(g) {

  // needed to initialize some ogdfAttributes fields
  long attributes =
      // x, y, width, height fields
      ogdf::GraphAttributes::nodeGraphics |
      // bends field
      ogdf::GraphAttributes::edgeGraphics |
      // doubleWeight field
      ogdf::GraphAttributes::edgeDoubleWeight |
      // weight field
      ogdf::GraphAttributes::nodeWeight |
      // z coordinate
      ogdf::GraphAttributes::threeD;

  ogdfAttributes = ogdf::GraphAttributes(ogdfGraph, attributes);

  tlp::SizeProperty *sizeProp = talipotGraph->getSizeProperty("viewSize");
  tlp::LayoutProperty *layoutProp = talipotGraph->getLayoutProperty("viewLayout");

  const std::vector<tlp::node> &nodes = talipotGraph->nodes();
  unsigned int nbElts = nodes.size();

  for (unsigned int i = 0; i < nbElts; ++i) {
    ogdf::node nOGDF = ogdfGraph.newNode(i);
    ogdfNodes[i] = nOGDF;
    tlp::node nTlp = nodes[i];
    const tlp::Coord &c = layoutProp->getNodeValue(nTlp);
    ogdfAttributes.x(nOGDF) = c.getX();
    ogdfAttributes.y(nOGDF) = c.getY();
    ogdfAttributes.z(nOGDF) = c.getZ();
    const tlp::Size &s = sizeProp->getNodeValue(nTlp);
    ogdfAttributes.width(nOGDF) = s.getW();
    ogdfAttributes.height(nOGDF) = s.getH();
  }

  const std::vector<tlp::edge> &edges = talipotGraph->edges();
  nbElts = edges.size();

  for (unsigned int i = 0; i < nbElts; ++i) {
    tlp::edge eTlp = edges[i];
    std::pair<tlp::node, tlp::node> ends = talipotGraph->ends(eTlp);
    ogdf::edge eogdf = ogdfGraph.newEdge(ogdfNodes[ends.first], ogdfNodes[ends.second], i);
    ogdfEdges[i] = eogdf;

    if (importEdgeBends) {

      const vector<tlp::Coord> &v = layoutProp->getEdgeValue(eTlp);
      ogdf::DPolyline bends;

      for (const Coord &coord : v) {
        bends.pushBack(ogdf::DPoint(coord.getX(), coord.getY()));
      }

      ogdfAttributes.bends(eogdf) = bends;
    }

    ogdfAttributes.doubleWeight(eogdf) = 1.0;
  }
}

tlp::Graph &TalipotToOGDF::getTlp() {
  return *talipotGraph;
}

ogdf::GraphAttributes &TalipotToOGDF::getOGDFGraphAttr() {
  return ogdfAttributes;
}

ogdf::Graph &TalipotToOGDF::getOGDFGraph() {
  return ogdfGraph;
}

ogdf::node TalipotToOGDF::getOGDFGraphNode(unsigned int nodeIndex) {
  return ogdfNodes[nodeIndex];
}

ogdf::edge TalipotToOGDF::getOGDFGraphEdge(unsigned int edgeIndex) {
  return ogdfEdges[edgeIndex];
}

tlp::Coord TalipotToOGDF::getNodeCoordFromOGDFGraphAttr(unsigned int nodeIndex) {
  ogdf::node n = ogdfNodes[nodeIndex];

  double x = ogdfAttributes.x(n);
  double y = ogdfAttributes.y(n);
  double z = ogdfAttributes.z(n);

  return tlp::Coord(x, y, z);
}

vector<tlp::Coord> TalipotToOGDF::getEdgeCoordFromOGDFGraphAttr(unsigned int edgeIndex) {
  ogdf::edge e = ogdfEdges[edgeIndex];
  ogdf::DPolyline line = ogdfAttributes.bends(e);
  vector<tlp::Coord> v;

  for (ogdf::ListIterator<ogdf::DPoint> p = line.begin(); p.valid(); ++p) {
    v.push_back(tlp::Coord((*p).m_x, (*p).m_y, 0.));
  }

  return v;
}

void TalipotToOGDF::copyTlpNumericPropertyToOGDFEdgeLength(tlp::NumericProperty *metric) {
  if (!metric)
    return;

  const std::vector<tlp::edge> &edges = talipotGraph->edges();
  unsigned int nbEdges = edges.size();

  for (unsigned int i = 0; i < nbEdges; ++i) {
    ogdfAttributes.doubleWeight(ogdfEdges[i]) = metric->getEdgeDoubleValue(edges[i]);
  }
}

void TalipotToOGDF::copyTlpNodeSizeToOGDF(tlp::SizeProperty *size) {
  if (!size)
    return;

  const std::vector<tlp::edge> &edges = talipotGraph->edges();
  unsigned int nbEdges = edges.size();

  for (unsigned int i = 0; i < nbEdges; ++i) {
    std::pair<tlp::node, tlp::node> ends = talipotGraph->ends(edges[i]);
    ogdf::node srcOgdf = ogdfNodes[ends.first];
    tlp::Size s = size->getNodeValue(ends.first);
    ogdf::node tgtOgdf = ogdfNodes[ends.second];
    tlp::Size s2 = size->getNodeValue(ends.second);

    ogdfAttributes.width(srcOgdf) = s.getW();
    ogdfAttributes.height(srcOgdf) = s.getH();
    ogdfAttributes.width(tgtOgdf) = s2.getW();
    ogdfAttributes.height(tgtOgdf) = s2.getH();

    ogdf::edge eOgdf = ogdfEdges[i];
    ogdfAttributes.doubleWeight(eOgdf) =
        ogdfAttributes.doubleWeight(eOgdf) + s.getW() / 2. + s2.getW() / 2. - 1.;
  }
}

void TalipotToOGDF::copyTlpNumericPropertyToOGDFNodeWeight(tlp::NumericProperty *metric) {
  if (!metric)
    return;

  const std::vector<tlp::node> &nodes = talipotGraph->nodes();
  unsigned int nbNodes = nodes.size();

  for (unsigned int i = 0; i < nbNodes; ++i) {
    ogdfAttributes.weight(ogdfNodes[i]) = int(metric->getNodeDoubleValue(nodes[i]));
  }
}
