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

#include <QMouseEvent>

#include <talipot/Graph.h>
#include <talipot/LayoutProperty.h>
#include <talipot/ColorProperty.h>
#include <talipot/GlMainWidget.h>
#include <talipot/GlMainView.h>
#include <talipot/GlTools.h>
#include <talipot/GlLine.h>
#include <talipot/GlGraph.h>
#include <talipot/MouseEdgeBuilder.h>
#include <talipot/Camera.h>

using namespace std;
using namespace tlp;

MouseEdgeBuilder::MouseEdgeBuilder()
    : _source(node()), _started(false), _graph(nullptr), _layoutProperty(nullptr),
      glMainWidget(nullptr) {}

bool MouseEdgeBuilder::eventFilter(QObject *widget, QEvent *e) {

  if (glMainWidget == nullptr) {
    glMainWidget = dynamic_cast<GlMainWidget *>(widget);
  }

  assert(glMainWidget);

  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(e);

    SelectedEntity selectedEntity;
    GlGraphInputData *inputData = glMainWidget->getGlGraphInputData();
    Graph *_graph = inputData->getGraph();

    LayoutProperty *mLayout = inputData->getElementLayout();

    if (qMouseEv->buttons() == Qt::LeftButton) {
      if (!_started) {
        bool result = glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity);

        if (result && (selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED)) {
          _started = true;
          initObserver(_graph);
          _source = node(selectedEntity.getComplexEntityId());
          _curPos = _startPos = mLayout->getNodeValue(_source);
          return true;
        }

        return false;
      } else {
        bool result = glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity);

        if (result && (selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED)) {
          Observable::holdObservers();
          clearObserver();
          // allow to undo
          _graph->push();
          addLink(_source, node(selectedEntity.getComplexEntityId()));
          // edge finished. clear _source and _started
          _source = node();
          _started = false;
          Observable::unholdObservers();

        } else {
          Coord point = {glMainWidget->width() - float(qMouseEv->x()), float(qMouseEv->y())};
          _bends.push_back(glMainWidget->getScene()->getGraphCamera().viewportTo3DWorld(
              glMainWidget->screenToViewport(point)));
          glMainWidget->redraw();
        }
      }

      return true;
    }

    if (qMouseEv->buttons() == Qt::MiddleButton) {
      _bends.clear();
      _started = false;
      _source = node();
      clearObserver();
      glMainWidget->draw();
      return true;
    }
  }

  if (e->type() == QEvent::MouseMove) {
    QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(e);

    if (!_started) {
      SelectedEntity selectedEntity;
      bool hoveringOverNode =
          glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity) &&
          selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED;

      if (!hoveringOverNode) {
        glMainWidget->setCursor(QCursor(Qt::ArrowCursor));
        return false;
      } else {
        glMainWidget->setCursor(QCursor(Qt::CrossCursor));
      }
    } else {
      SelectedEntity selectedEntity;

      if (glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity) &&
          selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
        glMainWidget->setCursor(QCursor(Qt::CrossCursor));
      } else {
        glMainWidget->setCursor(QCursor(Qt::ArrowCursor));
      }

      Coord point = Coord(glMainWidget->width() - qMouseEv->x(), qMouseEv->y());
      point = glMainWidget->getScene()->getGraphCamera().viewportTo3DWorld(
          glMainWidget->screenToViewport(point));
      _curPos.set(point[0], point[1], point[2]);
      glMainWidget->redraw();
    }

    return true;
  }

  return false;
}

bool MouseEdgeBuilder::draw(GlMainWidget *glMainWidget) {
  if (!_started) {
    return false;
  }

  glDisable(GL_STENCIL_TEST);
  glMainWidget->getScene()->getGraphCamera().initGl();
  vector<Coord> lineVertices;
  lineVertices.push_back(_startPos);
  lineVertices.insert(lineVertices.end(), _bends.begin(), _bends.end());
  lineVertices.push_back(_curPos);
  vector<Color> lineColors;
  lineColors.resize(lineVertices.size(), Color(255, 0, 0, 255));
  GlLine editedEdge(lineVertices, lineColors);
  editedEdge.draw(0, nullptr);
  return true;
}

void MouseEdgeBuilder::initObserver(Graph *newGraph) {
  newGraph->addListener(this);
  _graph = newGraph;
  _layoutProperty = newGraph->getLayoutProperty("viewLayout");
  _layoutProperty->addListener(this);
}

void MouseEdgeBuilder::clearObserver() {
  if (_graph) {
    _graph->removeListener(this);
  }

  _graph = nullptr;

  if (_layoutProperty) {
    _layoutProperty->removeListener(this);
  }

  _layoutProperty = nullptr;
}

void MouseEdgeBuilder::treatEvent(const Event &evt) {
  if (typeid(evt) == typeid(GraphEvent)) {
    const GraphEvent *graphEvent = dynamic_cast<const GraphEvent *>(&evt);

    if (graphEvent && graphEvent->getType() == GraphEvent::TLP_DEL_NODE &&
        graphEvent->getNode() == _source) {
      _bends.clear();
      _started = false;
      clearObserver();
    }
  } else {
    const PropertyEvent *propertyEvent = dynamic_cast<const PropertyEvent *>(&evt);

    if (propertyEvent && propertyEvent->getType() == PropertyEvent::TLP_AFTER_SET_NODE_VALUE &&
        propertyEvent->getNode() == _source && propertyEvent->getProperty() == _layoutProperty) {
      _startPos = _layoutProperty->getNodeValue(_source);
    }
  }
}

void MouseEdgeBuilder::clear() {
  if (glMainWidget) {
    glMainWidget->setCursor(QCursor());
  }
}

void MouseEdgeBuilder::addLink(const node &source, const node &target) {
  assert(glMainWidget);
  Graph *g = glMainWidget->getGlGraphInputData()->getGraph();

  LayoutProperty *mLayout = glMainWidget->getGlGraphInputData()->getElementLayout();
  edge newEdge = g->addEdge(source, target);
  mLayout->setEdgeValue(newEdge, bends());
  _bends.clear();
}
