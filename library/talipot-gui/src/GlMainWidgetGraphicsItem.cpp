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

#include "talipot/GlMainWidgetGraphicsItem.h"

#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QGraphicsProxyWidget>
#include <QContextMenuEvent>
#include <QGraphicsScene>
#include <QPainter>

#include <talipot/GlTextureManager.h>
#include <talipot/GlQuad.h>
#include <talipot/GlTools.h>
#include <talipot/GlMainWidget.h>
#include <talipot/GlOffscreenRenderer.h>

using namespace std;
using namespace tlp;

GlMainWidgetGraphicsItem::GlMainWidgetGraphicsItem(GlMainWidget *glMainWidget, int width,
                                                   int height)
    : QGraphicsObject(), glMainWidget(glMainWidget), _redrawNeeded(true), _graphChanged(true) {

  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setFlag(QGraphicsItem::ItemIsFocusable, true);
  setAcceptHoverEvents(true);
  setHandlesChildEvents(false);
  setAcceptDrops(true);

  connect(glMainWidget, &GlMainWidget::viewDrawn, this,
          &GlMainWidgetGraphicsItem::glMainWidgetDraw);
  connect(glMainWidget, &GlMainWidget::viewRedrawn, this,
          &GlMainWidgetGraphicsItem::glMainWidgetRedraw);

  resize(width, height);
  glMainWidget->installEventFilter(this);
  setHandlesChildEvents(false);
}

void GlMainWidgetGraphicsItem::setGlMainWidget(GlMainWidget *glmw) {
  if (glmw) {
    disconnect(glMainWidget, &GlMainWidget::viewDrawn, this,
               &GlMainWidgetGraphicsItem::glMainWidgetDraw);
    disconnect(glMainWidget, &GlMainWidget::viewRedrawn, this,
               &GlMainWidgetGraphicsItem::glMainWidgetRedraw);
    glMainWidget->removeEventFilter(this);
    glMainWidget = glmw;
    connect(glMainWidget, &GlMainWidget::viewDrawn, this,
            &GlMainWidgetGraphicsItem::glMainWidgetDraw);
    connect(glMainWidget, &GlMainWidget::viewRedrawn, this,
            &GlMainWidgetGraphicsItem::glMainWidgetRedraw);
    glMainWidget->installEventFilter(this);
  }
}

GlMainWidgetGraphicsItem::~GlMainWidgetGraphicsItem() {
  delete glMainWidget;
}

QRectF GlMainWidgetGraphicsItem::boundingRect() const {
  return QRectF(0, 0, width, height);
}

void GlMainWidgetGraphicsItem::resize(int width, int height) {

  this->width = width;
  this->height = height;
  glMainWidget->resize(width, height);
  glMainWidget->resizeGL(width, height);
  _redrawNeeded = true;
  _graphChanged = true;
  prepareGeometryChange();
}

void GlMainWidgetGraphicsItem::glMainWidgetDraw(GlMainWidget *, bool graphChanged) {
  _redrawNeeded = true;
  _graphChanged = graphChanged;
  update();
}

void GlMainWidgetGraphicsItem::glMainWidgetRedraw(GlMainWidget *) {
  update();
}

void GlMainWidgetGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                                     QWidget *) {

  if (_redrawNeeded) {
    emit widgetPainted(_graphChanged);
  }

  GlOffscreenRenderer::getInstance()->setViewPortSize(width, height);

  if (_redrawNeeded) {
    GlOffscreenRenderer::getInstance()->renderGlMainWidget(glMainWidget);
    _redrawNeeded = false;
  } else {
    GlOffscreenRenderer::getInstance()->renderGlMainWidget(glMainWidget, false);
  }

  painter->drawImage(QRect(0, 0, width, height), GlOffscreenRenderer::getInstance()->getImage());
}

void GlMainWidgetGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QMouseEvent eventModif(QEvent::MouseMove, QPoint(event->pos().x(), event->pos().y()),
                         Qt::NoButton, event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QMouseEvent eventModif(QEvent::MouseButtonPress, QPoint(event->pos().x(), event->pos().y()),
                         event->button(), event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
  QMouseEvent eventModif(QEvent::MouseButtonDblClick, QPoint(event->pos().x(), event->pos().y()),
                         event->button(), event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QMouseEvent eventModif(QEvent::MouseButtonRelease, QPoint(event->pos().x(), event->pos().y()),
                         event->button(), event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::wheelEvent(QGraphicsSceneWheelEvent *event) {
  QWheelEvent eventModif(QPoint(event->pos().x(), event->pos().y()), event->delta(),
                         event->buttons(), event->modifiers(), event->orientation());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
  QMouseEvent eventModif(QEvent::MouseMove, QPoint(event->pos().x(), event->pos().y()),
                         Qt::NoButton, Qt::NoButton, event->modifiers());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  QContextMenuEvent eventModif(static_cast<QContextMenuEvent::Reason>(event->reason()),
                               QPoint(event->pos().x(), event->pos().y()));
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::keyReleaseEvent(QKeyEvent *event) {
  QKeyEvent eventModif(event->type(), event->key(), event->modifiers(), event->text(),
                       event->isAutoRepeat(), event->count());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::keyPressEvent(QKeyEvent *event) {
  QKeyEvent eventModif(event->type(), event->key(), event->modifiers(), event->text(),
                       event->isAutoRepeat(), event->count());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
  if (glMainWidget->isEnabled() && glMainWidget->acceptDrops()) {
    QDragEnterEvent proxyDragEnter(event->pos().toPoint(), event->dropAction(), event->mimeData(),
                                   event->buttons(), event->modifiers());
    proxyDragEnter.setAccepted(event->isAccepted());
    QApplication::sendEvent(glMainWidget, &proxyDragEnter);
    event->setAccepted(proxyDragEnter.isAccepted());
    event->setDropAction(proxyDragEnter.dropAction());
  } else {
    event->ignore();
  }
}
void GlMainWidgetGraphicsItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event) {
  QDragLeaveEvent proxyDragLeave;
  QApplication::sendEvent(glMainWidget, &proxyDragLeave);
  event->setAccepted(proxyDragLeave.isAccepted());
}

void GlMainWidgetGraphicsItem::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
  if (glMainWidget->isEnabled() && glMainWidget->acceptDrops()) {
    QDragMoveEvent dragMove(event->pos().toPoint(), event->possibleActions(), event->mimeData(),
                            event->buttons(), event->modifiers());
    QApplication::sendEvent(glMainWidget, &dragMove);
    event->setAccepted(dragMove.isAccepted());
    event->setDropAction(dragMove.dropAction());
  } else {
    event->ignore();
  }
}

void GlMainWidgetGraphicsItem::dropEvent(QGraphicsSceneDragDropEvent *event) {
  QDropEvent dropEvent(event->pos().toPoint(), event->possibleActions(), event->mimeData(),
                       event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget, &dropEvent);
  event->setAccepted(dropEvent.isAccepted());
}

bool GlMainWidgetGraphicsItem::eventFilter(QObject *, QEvent *evt) {
  if (evt->type() == QEvent::CursorChange) {
    setCursor(glMainWidget->cursor());
  }

// There is a bug with Qt5 on windows that leads to an incorrect viewport size of 160x160
// when initializing a Tulip OpenGL view.
// Seems that a delayed resize event is sent to the GlMainWidget, don't really know why ...
// In the context of a GlMainWidgetGraphicsItem, the internally used GlMainWidget is hidden,
// so it should not receive that event but it does once after it has been created.
// As a workaround to fix that annoying behaviour, catch that resize event, discard it
// and restore the correct size that was previously set.
#if defined(WIN32)

  if (evt->type() == QEvent::Resize) {
    glMainWidget->resize(width, height);
    return true;
  }

#endif
  return false;
}
