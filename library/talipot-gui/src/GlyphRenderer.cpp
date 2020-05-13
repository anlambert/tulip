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

#include <map>

#include <talipot/Node.h>
#include <talipot/Edge.h>
#include <talipot/GlyphRenderer.h>
#include <talipot/GlyphManager.h>
#include <talipot/Graph.h>
#include <talipot/GlOffscreenRenderer.h>
#include <talipot/GlGraphComposite.h>
#include <talipot/ViewSettings.h>
#include <talipot/EdgeExtremityGlyph.h>
#include <talipot/Camera.h>

using namespace tlp;
using namespace std;

QPixmap GlyphRenderer::render(int glyphId) {
  static std::unordered_map<int, QPixmap> previews;
  static bool inited = false;
  if (!inited) {
    inited = true;
    // Init graph parameters.
    GlGraphRenderingParameters parameters;
    auto graph = newGraph();
    auto node = graph->addNode();
    {
      // need a block to ensure inputData
      // will be destroyed before graph
      GlGraphInputData inputData(graph, &parameters);
      inputData.getElementSize()->setAllNodeValue(Size(1, 1, 1));
      inputData.getElementColor()->setAllNodeValue(Color(192, 192, 192));
      inputData.getElementBorderColor()->setAllNodeValue(Color(0, 0, 0));
      inputData.getElementBorderWidth()->setAllNodeValue(1);

      GlOffscreenRenderer &renderer = GlOffscreenRenderer::instance();
      renderer.setViewPortSize(16, 16);
      renderer.clearScene();
      renderer.addGraphToScene(graph);
      renderer.getScene()->centerScene();
      renderer.getScene()->getGraphCamera().setZoomFactor(0.9);
      // init previews
      for (const std::string &glyphName : PluginsManager::availablePlugins<Glyph>()) {
        auto glId = GlyphManager::glyphId(glyphName);
        // Create the glyph preview
        graph->getIntegerProperty("viewShape")->setNodeValue(node, glId);
        renderer.renderScene(false, true);
        previews[glId] = QPixmap::fromImage(renderer.getImage());
      }
      renderer.clearScene(true);
    }
    // graph is no longer needed
    delete graph;
  }
  auto it = previews.find(glyphId);
  if (it == previews.end()) {
    return QPixmap(":/talipot/gui/icons/i_invalid.png");
  }
  return it->second;
}

QPixmap EdgeExtremityGlyphRenderer::render(int glyphId) {
  static std::unordered_map<int, QPixmap> previews;
  static bool inited = false;
  if (!inited) {
    inited = true;
    previews[EdgeExtremityShape::None] = QPixmap();
    // Init graph parameters.
    GlGraphRenderingParameters parameters;
    auto graph = newGraph();
    node n1 = graph->addNode();
    node n2 = graph->addNode();
    edge e = graph->addEdge(n1, n2);
    {
      // need a block to ensure inputData
      // will be destroyed before graph
      GlGraphInputData inputData(graph, &parameters);
      Color white = {255, 255, 255, 0}, black = {0, 0, 0, 0};
      inputData.getElementSize()->setAllNodeValue(Size(0.01f, 0.2f, 0.1f));
      inputData.getElementSize()->setAllEdgeValue(Size(0.125f, 0.125f, 0.125f));
      inputData.getElementColor()->setAllNodeValue(white);
      inputData.getElementBorderColor()->setAllNodeValue(white);
      inputData.getElementColor()->setAllEdgeValue(Color(192, 192, 192));
      inputData.getElementBorderColor()->setAllEdgeValue(black);
      inputData.getElementLayout()->setNodeValue(n1, Coord(0, 0, 0));
      inputData.getElementLayout()->setNodeValue(n2, Coord(0.3f, 0, 0));
      vector<Coord> bends;
      bends.push_back(Coord(0.01f, 0, 0));
      inputData.getElementLayout()->setAllEdgeValue(bends);

      inputData.getElementSrcAnchorShape()->setAllEdgeValue(EdgeExtremityShape::None);
      inputData.getElementTgtAnchorSize()->setAllEdgeValue(Size(2, 2, 1));

      GlOffscreenRenderer &renderer = GlOffscreenRenderer::instance();
      renderer.setViewPortSize(16, 16);
      renderer.clearScene();
      renderer.addGraphToScene(graph);
      GlGraphRenderingParameters renderingParamerters =
          renderer.getScene()->getGlGraphComposite()->getRenderingParameters();
      renderingParamerters.setEdgeColorInterpolate(false);
      renderingParamerters.setEdgeSizeInterpolate(false);
      renderingParamerters.setViewArrow(true);
      renderer.getScene()->getGlGraphComposite()->setRenderingParameters(renderingParamerters);
      // init previews
      for (std::string glyphName : PluginsManager::availablePlugins<EdgeExtremityGlyph>()) {
        const tlp::Plugin &info = PluginsManager::pluginInformation(glyphName);
        int glId = info.id();
        // Create the glyph preview
        graph->getIntegerProperty("viewTgtAnchorShape")->setEdgeValue(e, glId);
        renderer.renderScene(true);
        previews[glId] = QPixmap::fromImage(renderer.getImage());
      }
      renderer.clearScene(true);
    }
    // graph is no longer needed
    delete graph;
  }
  auto it = previews.find(glyphId);
  if (it == previews.end()) {
    return QPixmap(":/talipot/gui/icons/i_invalid.png");
  }
  return it->second;
}
