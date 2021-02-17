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

#include <talipot/GlMainView.h>

#include <QApplication>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QAction>
#include <QMenu>
#include <QPushButton>
#include <QTimer>

#include <talipot/GlWidgetGraphicsItem.h>
#include <talipot/GlWidget.h>
#include <talipot/SceneConfigWidget.h>
#include <talipot/SceneLayersConfigWidget.h>
#include <talipot/GlBoundingBoxSceneVisitor.h>
#include <talipot/GlOverviewGraphicsItem.h>
#include <talipot/QuickAccessBar.h>
#include <talipot/QtGlSceneZoomAndPanAnimator.h>
#include <talipot/GlGraph.h>
#include <talipot/Gl2DRect.h>
#include <talipot/ViewActionsManager.h>
#include <talipot/FontIconManager.h>
#include <talipot/MaterialDesignIcons.h>

using namespace tlp;

GlMainView::GlMainView(bool needtturlManager)
    : _glWidget(nullptr), _overviewItem(nullptr), _viewActionsManager(nullptr),
      _showOvButton(nullptr), _showQabButton(nullptr), needQuickAccessBar(false),
      _needTooltipAndUrlManager(needtturlManager), _quickAccessBarItem(nullptr),
      _quickAccessBar(nullptr), _sceneConfigurationWidget(nullptr),
      _sceneLayersConfigurationWidget(nullptr), _overviewPosition(OVERVIEW_BOTTOM_RIGHT),
      _updateOverview(true) {}

GlMainView::~GlMainView() {
  delete _sceneConfigurationWidget;
  delete _sceneLayersConfigurationWidget;
  delete _overviewItem;
  delete _viewActionsManager;
}

void GlMainView::draw() {
  _glWidget->draw();
}

void GlMainView::redraw() {
  _glWidget->redraw();
}

void GlMainView::refresh() {
  _glWidget->draw(false);
}

void GlMainView::setOverviewPosition(const OverviewPosition &position) {
  _overviewPosition = position;
  drawOverview();
}

GlMainView::OverviewPosition GlMainView::overviewPosition() const {
  return _overviewPosition;
}

void GlMainView::setUpdateOverview(bool updateOverview) {
  _updateOverview = updateOverview;
}

bool GlMainView::updateOverview() const {
  return _updateOverview;
}

void GlMainView::setState(const tlp::DataSet &data) {
  View::setState(data);

  bool overviewVisible = true;

  if (data.get<bool>("overviewVisible", overviewVisible)) {
    setOverviewVisible(overviewVisible);
  }

  bool quickAccessBarVisible = true;

  if (data.get<bool>("quickAccessBarVisible", quickAccessBarVisible)) {
    needQuickAccessBar = true;
    setQuickAccessBarVisible(quickAccessBarVisible);
  }
}

tlp::DataSet GlMainView::state() const {
  DataSet data = View::state();
  data.set("overviewVisible", overviewVisible());

  if (needQuickAccessBar) {
    data.set("quickAccessBarVisible", quickAccessBarVisible());
  }

  return data;
}

void GlMainView::drawOverview(bool generatePixmap) {
  if (_overviewItem == nullptr) {
    _overviewItem = new GlOverviewGraphicsItem(this, *_glWidget->getScene());
    addToScene(_overviewItem);
    generatePixmap = true;
    // used to set the overview at the right place
    sceneRectChanged(QRectF(QPoint(0, 0), graphicsView()->size()));
  }

  if (_updateOverview) {
    _overviewItem->draw(generatePixmap);
  }
}

void GlMainView::assignNewGlWidget(GlWidget *glWidget, bool deleteOldGlWidget) {
  _glWidget = glWidget;

  if (_sceneLayersConfigurationWidget == nullptr) {
    _sceneLayersConfigurationWidget = new SceneLayersConfigWidget();
  }

  _sceneLayersConfigurationWidget->setGlWidget(_glWidget);
  connect(_sceneLayersConfigurationWidget, &SceneLayersConfigWidget::drawNeeded, this,
          &View::drawNeeded);

  setCentralWidget(_glWidget, deleteOldGlWidget);
  GlWidgetGraphicsItem *glWidgetGraphicsItem = static_cast<GlWidgetGraphicsItem *>(centralItem());
  delete _sceneConfigurationWidget;
  _sceneConfigurationWidget = new SceneConfigWidget();
  _sceneConfigurationWidget->setGlWidget(_glWidget);

  connect(glWidgetGraphicsItem, &GlWidgetGraphicsItem::widgetPainted, this,
          &GlMainView::glMainViewDrawn);
  // Tooltip events and url management
  if (_needTooltipAndUrlManager) {
    activateTooltipAndUrlManager(_glWidget);
  }
}

GlOverviewGraphicsItem *GlMainView::overviewItem() const {
  return _overviewItem;
}

void GlMainView::setupWidget() {
  graphicsView()->viewport()->parentWidget()->installEventFilter(this);
  assignNewGlWidget(new GlWidget(nullptr, this), true);
  _viewActionsManager = new ViewActionsManager(this, _glWidget, false);
}

GlWidget *GlMainView::getGlWidget() const {
  return _glWidget;
}

void GlMainView::centerView(bool graphChanged) {

  float gvWidth = graphicsView()->width();
  // we apply a zoom factor to preserve a 50 px margin width
  // to ensure the scene will not be drawn under the configuration tabs title
  getGlWidget()->centerScene(graphChanged, (gvWidth - 50) / gvWidth);

  if (_overviewItem && _overviewItem->isVisible()) {
    drawOverview(graphChanged);
  }
}

void GlMainView::glMainViewDrawn(bool graphChanged) {
  if (_overviewItem && _overviewItem->isVisible()) {
    drawOverview(graphChanged);
  }
}

QList<QWidget *> GlMainView::configurationWidgets() const {
  return QList<QWidget *>() << _sceneConfigurationWidget << _sceneLayersConfigurationWidget;
}

void GlMainView::updateShowOverviewButton() {
  if (_showOvButton == nullptr) {
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
    _showOvButton = new QPushButton();
    _showOvButton->setMinimumSize(10, 10);
    _showOvButton->setMaximumSize(10, 10);
    _showOvButton->setStyleSheet("border: none;");
    _showOvButton->setCheckable(true);
    proxy->setWidget(_showOvButton);
    addToScene(proxy);
    proxy->setZValue(10);
    connect(_showOvButton, &QAbstractButton::toggled, this, &GlMainView::setOverviewVisible);
  }

  _showOvButton->setVisible(_overviewPosition == OVERVIEW_BOTTOM_RIGHT);

  if (_showOvButton->isVisible()) {
    QRectF rect(QPoint(0, 0), graphicsView()->size());

    _showOvButton->blockSignals(true);

    if (_overviewItem && _overviewItem->isVisible()) {
      _showOvButton->setIcon(FontIconManager::icon(MaterialDesignIcons::Close, 0.8));
      _showOvButton->setChecked(true);
      _showOvButton->setToolTip("Hide overview display");
      _showOvButton->move(
          rect.width() - _overviewItem->getWidth() - 1,
          rect.height() - _overviewItem->getHeight() -
              ((_quickAccessBar != nullptr) ? _quickAccessBarItem->size().height() : 0));
    } else {
      _showOvButton->setIcon(FontIconManager::icon(MaterialDesignIcons::ChevronLeft));
      _showOvButton->setChecked(false);
      _showOvButton->setToolTip("Show overview display");
      _showOvButton->move(
          rect.width() - _showOvButton->width(),
          rect.height() - _overviewItem->getHeight() -
              ((_quickAccessBar != nullptr) ? _quickAccessBarItem->size().height() : 0));
    }

    _showOvButton->blockSignals(false);
  }
}

void GlMainView::setOverviewVisible(bool display) {
  drawOverview(true);
  _overviewItem->setVisible(display);
  updateShowOverviewButton();
}

bool GlMainView::overviewVisible() const {
  return (_overviewItem && _overviewItem->isVisible());
}

void GlMainView::setViewOrtho(bool viewOrtho) {
  getGlWidget()->getScene()->setViewOrtho(viewOrtho);
  _glWidget->draw(false);
}

void GlMainView::updateShowQuickAccessBarButton() {
  if (needQuickAccessBar) {
    if (_showQabButton == nullptr) {
      QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
      _showQabButton = new QPushButton();
      _showQabButton->setStyleSheet("border: none;");
      _showQabButton->setMinimumSize(10, 10);
      _showQabButton->setMaximumSize(10, 10);
      _showQabButton->setCheckable(true);
      proxy->setWidget(_showQabButton);
      addToScene(proxy);
      proxy->setZValue(11);
      connect(_showQabButton, &QAbstractButton::toggled, this,
              &GlMainView::setQuickAccessBarVisible);
    }

    QRectF rect(QPoint(0, 0), graphicsView()->size());

    _showQabButton->blockSignals(true);

    if (quickAccessBarVisible()) {
      _showQabButton->setIcon(FontIconManager::icon(MaterialDesignIcons::Close, 0.8));
      _showQabButton->setChecked(true);
      _showQabButton->setToolTip("Hide quick access bar");
      _showQabButton->move(0, rect.height() - _quickAccessBarItem->size().height() -
                                  _showQabButton->height());
    } else {
      _showQabButton->setIcon(FontIconManager::icon(MaterialDesignIcons::ChevronUp));
      _showQabButton->setChecked(false);
      _showQabButton->setToolTip("Show quick access bar");
      _showQabButton->move(0, rect.height() - _showQabButton->height());
    }

    _showQabButton->blockSignals(false);
  }
}

QuickAccessBar *GlMainView::getQuickAccessBarImpl() {
  return new QuickAccessBarImpl(_quickAccessBarItem);
}

void GlMainView::setQuickAccessBarVisible(bool visible) {
  if (!visible) {
    delete _quickAccessBar;
    _quickAccessBar = nullptr;
    _quickAccessBarItem = nullptr;
  }

  else if (!quickAccessBarVisible()) {
    needQuickAccessBar = true;
    _quickAccessBarItem = new QGraphicsProxyWidget();
    _quickAccessBar = getQuickAccessBarImpl();
    connect(_quickAccessBar, &QuickAccessBar::settingsChanged, _sceneConfigurationWidget,
            &SceneConfigWidget::resetChanges);
    connect(_sceneConfigurationWidget, &SceneConfigWidget::settingsApplied, _quickAccessBar,
            &QuickAccessBar::reset);

    _quickAccessBar->setGlMainView(this);
    _quickAccessBarItem->setWidget(_quickAccessBar);
    addToScene(_quickAccessBarItem);
    _quickAccessBarItem->setZValue(10);
  }

  sceneRectChanged(QRectF(QPoint(0, 0), graphicsView()->size()));
}

bool GlMainView::quickAccessBarVisible() const {
  return _quickAccessBarItem != nullptr;
}

void GlMainView::sceneRectChanged(const QRectF &rect) {
  if (_quickAccessBar != nullptr) {
    _quickAccessBarItem->setPos(0, rect.height() - _quickAccessBarItem->size().height());
    _quickAccessBarItem->resize(rect.width(), _quickAccessBarItem->size().height());
  }

  updateShowQuickAccessBarButton();

  if (_overviewItem != nullptr) {
    // put overview in the bottom right corner
    if (_overviewPosition == OVERVIEW_BOTTOM_RIGHT) {
      _overviewItem->setPos(
          rect.width() - _overviewItem->getWidth() - 1,
          rect.height() - _overviewItem->getHeight() -
              ((_quickAccessBarItem != nullptr) ? _quickAccessBarItem->size().height() : 0));
    } else if (_overviewPosition == OVERVIEW_BOTTOM_LEFT) {
      _overviewItem->setPos(
          0, rect.height() - _overviewItem->getHeight() -
                 ((_quickAccessBarItem != nullptr) ? _quickAccessBarItem->size().height() : 0));
    } else if (_overviewPosition == OVERVIEW_TOP_LEFT) {
      _overviewItem->setPos(0, 0);
    } else if (_overviewPosition == OVERVIEW_TOP_RIGHT) {
      _overviewItem->setPos(rect.width() - _overviewItem->getWidth() - 1, 0);
    }

    updateShowOverviewButton();
  }
}

QPixmap GlMainView::snapshot(const QSize &outputSize) const {
  if (_glWidget == nullptr) {
    return QPixmap();
  }

  QSize realSize = outputSize;

  if (!realSize.isValid()) {
    realSize = _glWidget->size();
  }

  return QPixmap::fromImage(_glWidget->createPicture(realSize.width(), realSize.height(), false));
}

QImage GlMainView::getRGBImage() const {
  QSize currentSize = _glWidget->size();
  return _glWidget->createPicture(currentSize.width(), currentSize.height(), false,
                                  QImage::Format_RGB888);
}

void GlMainView::undoCallback() {
  float gvWidth = graphicsView()->width();
  // we apply a zoom factor to preserve a 50 px margin width
  // to ensure the scene will not be drawn under the configuration tabs title
  getGlWidget()->centerScene(true, (gvWidth - 50) / gvWidth);
  draw();
}

void GlMainView::fillContextMenu(QMenu *menu, const QPointF &pf) {
  _viewActionsManager->fillContextMenu(menu);

  QAction *viewOrtho = menu->addAction("Use orthogonal projection");
  viewOrtho->setToolTip("Enable to switch between true perspective and orthogonal");
  viewOrtho->setCheckable(true);
  viewOrtho->setChecked(_glWidget->getScene()->isViewOrtho());
  connect(viewOrtho, &QAction::triggered, this, &GlMainView::setViewOrtho);

  menu->addSeparator();
  menu->addAction("Augmented display")->setEnabled(false);
  menu->addSeparator();

  QAction *a = menu->addAction("Show overview", this, &GlMainView::setOverviewVisible);
  a->setToolTip("Show/hide the overview in a corner of the view");
  a->setCheckable(true);
  a->setChecked(overviewVisible());

  if (needQuickAccessBar) {
    QAction *quickbarAction =
        menu->addAction("Show quick access bar", this, &GlMainView::setQuickAccessBarVisible);
    quickbarAction->setToolTip("Show/hide the quick access bar");
    quickbarAction->setCheckable(true);
    quickbarAction->setChecked(quickAccessBarVisible());
  }
  View::fillContextMenu(menu, pf);
}

void GlMainView::applySettings() {
  _sceneConfigurationWidget->applySettings();
}

bool GlMainView::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::Resize) {
    // ensure automatic resize of the viewport
    QResizeEvent *resizeEvent = static_cast<QResizeEvent *>(event);
    graphicsView()->viewport()->setFixedSize(resizeEvent->size());
    // same for the configuration widgets
    QList<QWidget *> list = configurationWidgets();

    sceneRectChanged(QRectF(QPoint(0, 0), graphicsView()->size()));

    if (!list.isEmpty() &&
        list.first()->parentWidget()) { // test if the current view has a configuration widget
      QWidget *pqw = list.first()->parentWidget()->parentWidget();
      QSize sSize = pqw->size();
      sSize.setHeight(resizeEvent->size().height() - 50);
      pqw->resize(sSize);
      sSize.setHeight(resizeEvent->size().height() - 60);
      sSize = list.first()->size();

      for (auto c : list) { // resize each configuration widget
        c->resize(sSize);
      }
    }

    return true;
  }

  // standard event processing
  return ViewWidget::eventFilter(obj, event);
}

bool GlMainView::getNodeOrEdgeAtViewportPos(GlWidget *glw, int x, int y, node &n, edge &e) {
  SelectedEntity type;
  if (glw->pickNodesEdges(x, y, type)) {
    node tmpNode = type.getNode();

    if (tmpNode.isValid()) {
      n = tmpNode;
      return true;
    } else {
      edge tmpEdge = type.getEdge();
      if (tmpEdge.isValid()) {
        e = tmpEdge;
        return true;
      }
    }
  }
  return false;
}

bool GlMainView::pickNodeEdge(const int x, const int y, node &n, edge &e, bool pickNode,
                              bool pickEdge) {
  SelectedEntity selectedEntity;
  bool foundEntity =
      getGlWidget()->pickNodesEdges(x, y, selectedEntity, nullptr, pickNode, pickEdge);
  n = node();
  e = edge();
  if (foundEntity) {
    if (selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
      n.id = selectedEntity.getComplexEntityId();
      return true;
    }
    if (selectedEntity.getEntityType() == SelectedEntity::EDGE_SELECTED) {
      e.id = selectedEntity.getComplexEntityId();
      return true;
    }
  }
  return false;
}

void GlMainView::zoomAndPanAnimation(const tlp::BoundingBox &boundingBox, const double duration) {
  BoundingBox bb;
  if (bb.isValid()) {
    bb = boundingBox;
  } else {
    auto scene = getGlWidget()->getScene();
    GlGraphInputData *inputData = scene->getGlGraph()->getInputData();
    GlBoundingBoxSceneVisitor bbVisitor(inputData);
    scene->getLayer("Main")->acceptVisitor(&bbVisitor);
    bb = bbVisitor.getBoundingBox();
  }
  QtGlSceneZoomAndPanAnimator zoomAnPan(getGlWidget(), bb, duration);
  zoomAnPan.animateZoomAndPan();
}
