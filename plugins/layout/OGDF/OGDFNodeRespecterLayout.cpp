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

#include <ogdf/energybased/NodeRespecterLayout.h>

#include <talipot/OGDFLayoutPluginBase.h>

static const char *paramHelp[] = {
    // Even angles
    "Subtrees may be assigned even angles or angles depending on their size."};

class OGDFNodeRespecterLayout : public tlp::OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Node Respecter Layout (OGDF)", "Max Ilsen", "2017",
                    "This is a force-directed layout algorithm respecting the shapes and sizes of "
                    "nodes. It aims to minimize the number of node overlaps as well as the number "
                    "of edges crossing through non-incident nodes. In order to achieve this, the "
                    "algorithm adapts its forces to the node sizes and bends edges around close-by "
                    "nodes. The edge bends are created by introducing dummy nodes into the graph, "
                    "positioning all nodes according to forces acting upon them, filtering out "
                    "unnecessary dummy nodes, and then replacing the remaining dummy nodes by edge "
                    "bends.\n\n"
                    "The algorithm is documented in and was developed for the bachelor thesis: "
                    "Max Ilsen: <i>Energy-Based Layout Algorithms for Graphs with Large Nodes</i>. "
                    "University of Osnabrueck, 2017",
                    "1.0", "Force Directed")

  OGDFNodeRespecterLayout(const tlp::PluginContext *context)
      : tlp::OGDFLayoutPluginBase(context, new ogdf::NodeRespecterLayout()) {}

  void beforeCall() override {}
};

PLUGIN(OGDFNodeRespecterLayout)

// *     <tr><td> #m_randomInitialPlacement </td><td> true </td></tr>
//  *     <tr><td> #m_postProcessing </td><td> PostProcessingMode::Complete </td></tr>
//  *     <tr><td> #m_bendNormalizationAngle </td><td> Math::pi </td></tr>
//  *     <tr><td> #m_numberOfIterations </td><td> 30000 </td></tr>
//  *     <tr><td> #m_minimalTemperature </td><td> 1.0 </td></tr>
//  *     <tr><td> #m_initialTemperature </td><td> 10.0 </td></tr>
//  *     <tr><td> #m_temperatureDecreaseOffset </td><td> 0.0 </td></tr>
//  *     <tr><td> #m_gravitation </td><td> 1.0/16.0 </td></tr>
//  *     <tr><td> #m_oscillationAngle </td><td> Math::pi_2 </td></tr>
//  *     <tr><td> #m_desiredMinEdgeLength </td><td> LayoutStandards::defaultNodeSeparation()
//  </td></tr>
//  *     <tr><td> #m_initDummiesPerEdge </td><td> 1 </td></tr>
//  *     <tr><td> #m_maxDummiesPerEdge </td><td> 3 </td></tr>
//  *     <tr><td> #m_dummyInsertionThreshold </td><td> 5 </td></tr>
//  *     <tr><td> #m_maxDisturbance </td><td> 0 </td></tr>
//  *     <tr><td> #m_repulsionDistance </td><td> 2*#m_desiredMinEdgeLength </td></tr>
//  *     <tr><td> #m_minDistCC </td><td> LayoutStandards::defaultCCSeparation() </td></tr>
//  *     <tr><td> #m_pageRatio </td><td> 1.0 </td></tr>