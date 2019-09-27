/**
 *
 * Copyright (C) 2019  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include <talipot/GlGraphComposite.h>
#include <talipot/GraphProperty.h>
#include <talipot/GlXMLTools.h>
#include <talipot/GlTools.h>
#include <talipot/GlScene.h>
#include <talipot/GlVertexArrayManager.h>
#include <talipot/GlBoundingBoxSceneVisitor.h>
#include <talipot/OcclusionTest.h>
#include <talipot/GlGraphHighDetailsRenderer.h>

using namespace std;

namespace tlp {

GlGraphComposite::GlGraphComposite(Graph *graph, GlGraphRenderer *graphRenderer)
    : inputData(graph, &parameters), graphRenderer(graphRenderer), nodesModified(true) {
  if (graphRenderer == nullptr) {
    this->graphRenderer = new GlGraphHighDetailsRenderer(&inputData);
  }

  if (!graph) {
    rootGraph = nullptr;
  } else {
    rootGraph = graph->getRoot();
    graph->addListener(this);
    graph->getRoot()->getProperty<GraphProperty>("viewMetaGraph")->addListener(this);

    for (auto n : graph->nodes()) {
      if (graph->getNodeMetaInfo(n))
        metaNodes.insert(n);
    }
  }
}

GlGraphComposite::GlGraphComposite(Graph *graph, GlScene *scene)
    : inputData(graph, &parameters), nodesModified(true) {
  this->graphRenderer = new GlGraphHighDetailsRenderer(&inputData, scene);

  if (!graph) {
    rootGraph = nullptr;
  } else {
    rootGraph = graph->getRoot();
    graph->addListener(this);
    graph->getRoot()->getProperty<GraphProperty>("viewMetaGraph")->addListener(this);

    for (auto n : graph->nodes()) {
      if (graph->getNodeMetaInfo(n))
        metaNodes.insert(n);
    }
  }
}

GlGraphComposite::~GlGraphComposite() {
  delete graphRenderer;
}

void GlGraphComposite::acceptVisitor(GlSceneVisitor *visitor) {
  GlBoundingBoxSceneVisitor bbVisitor(&inputData);
  graphRenderer->visitGraph(&bbVisitor);
  boundingBox = bbVisitor.getBoundingBox();

  if (visitor && boundingBox.isValid())
    visitor->visit(this);
}

void GlGraphComposite::acceptVisitorOnGraph(GlSceneVisitor *visitor) {
  graphRenderer->visitGraph(visitor);
}

//===================================================================
void GlGraphComposite::draw(float lod, Camera *camera) {
  graphRenderer->draw(lod, camera);
}
//===================================================================
void GlGraphComposite::selectEntities(Camera *camera, RenderingEntitiesFlag type, int x, int y,
                                      int w, int h, vector<SelectedEntity> &selectedEntities) {
  graphRenderer->selectEntities(camera, type, x, y, w, h, selectedEntities);
}
//===================================================================
const GlGraphRenderingParameters &GlGraphComposite::getRenderingParameters() {
  return parameters;
}
//===================================================================
void GlGraphComposite::setRenderingParameters(const GlGraphRenderingParameters &parameter) {
  if (parameters.isElementOrdered() != parameter.isElementOrdered()) {
    parameters = parameter;
    graphRenderer->setGraphModified(true);
  } else {
    parameters = parameter;
  }
}
//===================================================================
GlGraphRenderingParameters *GlGraphComposite::getRenderingParametersPointer() {
  return &parameters;
}
//===================================================================
GlGraphInputData *GlGraphComposite::getInputData() {
  return &inputData;
}
//====================================================
void GlGraphComposite::getXML(string &outString) {
  GlXMLTools::createProperty(outString, "type", "GlGraphComposite", "GlEntity");
}
//====================================================
void GlGraphComposite::setWithXML(const string &, unsigned int &) {}

void GlGraphComposite::treatEvent(const Event &evt) {
  const GraphEvent *graphEvent = dynamic_cast<const GraphEvent *>(&evt);

  if (graphEvent) {
    switch (graphEvent->getType()) {

    case GraphEvent::TLP_ADD_NODE:
    case GraphEvent::TLP_DEL_NODE:
      nodesModified = true;
      graphRenderer->setGraphModified(true);
      break;

    case GraphEvent::TLP_ADD_EDGE:
    case GraphEvent::TLP_DEL_EDGE:
    case GraphEvent::TLP_REVERSE_EDGE:
    case GraphEvent::TLP_AFTER_SET_ENDS:
      graphRenderer->setGraphModified(true);
      break;

    default:
      break;
    }
  } else if (evt.type() == Event::TLP_DELETE) {
    Graph *g = dynamic_cast<Graph *>(evt.sender());

    if (g && inputData.getGraph() == g) {
      inputData.graph = nullptr;
    }
  } else {
    const PropertyEvent *propertyEvent = dynamic_cast<const PropertyEvent *>(&evt);

    if (propertyEvent && propertyEvent->getType() == PropertyEvent::TLP_AFTER_SET_NODE_VALUE) {
      nodesModified = true;
    }
  }
}

void GlGraphComposite::setRenderer(tlp::GlGraphRenderer *renderer) {
  delete graphRenderer;

  if (renderer == nullptr) {
    graphRenderer = new GlGraphHighDetailsRenderer(&inputData);
  } else {
    graphRenderer = renderer;
  }
}
} // namespace tlp