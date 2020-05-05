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

#include "talipot/MouseNodeBuilder.h"

#include <QMouseEvent>

#include <talipot/Graph.h>
#include <talipot/LayoutProperty.h>
#include <talipot/ColorProperty.h>
#include <talipot/GlMainWidget.h>
#include <talipot/GlGraphComposite.h>
#include <talipot/GlMainView.h>
#include <talipot/Camera.h>

using namespace tlp;
using namespace std;

bool MouseNodeBuilder::eventFilter(QObject *widget, QEvent *e) {
  QMouseEvent *qMouseEv = dynamic_cast<QMouseEvent *>(e);

  if (qMouseEv != nullptr) {
    SelectedEntity selectedEntity;

    if (glMainWidget == nullptr) {
      glMainWidget = static_cast<GlMainWidget *>(widget);
    }

    if (e->type() == QEvent::MouseMove) {
      if (glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity) &&
          selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
        glMainWidget->setCursor(Qt::ForbiddenCursor);
      } else {
        glMainWidget->setCursor(Qt::ArrowCursor);
      }

      return false;
    }

    if (e->type() == _eventType) {
      if (qMouseEv->button() == Qt::LeftButton) {
        if (glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity) &&
            selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
          return true;
        }

        GlGraphInputData *inputData =
            glMainWidget->getScene()->getGlGraphComposite()->getInputData();
        Graph *_graph = inputData->getGraph();
        LayoutProperty *mLayout = inputData->getElementLayout();
        // allow to undo
        _graph->push();
        Observable::holdObservers();
        node newNode;
        newNode = _graph->addNode();
        Coord point = Coord(glMainWidget->width() - float(qMouseEv->x()), float(qMouseEv->y()));
        point = glMainWidget->getScene()->getGraphCamera().viewportTo3DWorld(
            glMainWidget->screenToViewport(point));

        // This code is here to block z coordinate to 0 when we are in "2D mode"
        Coord cameraDirection = glMainWidget->getScene()->getGraphCamera().getEyes() -
                                glMainWidget->getScene()->getGraphCamera().getCenter();

        if (cameraDirection[0] == 0 && cameraDirection[1] == 0) {
          point[2] = 0;
        }

        mLayout->setNodeValue(newNode, point);
        Observable::unholdObservers();

        return true;
      }
    }
  }

  return false;
}

void MouseNodeBuilder::clear() {
  if (glMainWidget) {
    glMainWidget->setCursor(QCursor());
  }
}
