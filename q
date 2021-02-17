[1mdiff --git a/demos/interactor_plugin/interactor_plugin.cpp b/demos/interactor_plugin/interactor_plugin.cpp[m
[1mindex 2f64610fb..04e79cb90 100644[m
[1m--- a/demos/interactor_plugin/interactor_plugin.cpp[m
[1m+++ b/demos/interactor_plugin/interactor_plugin.cpp[m
[36m@@ -5,7 +5,7 @@[m
 [m
 #include <talipot/MouseInteractors.h>[m
 #include <talipot/Interactor.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlWidget.h>[m
 #include <talipot/GlScene.h>[m
 [m
[36m@@ -64,7 +64,7 @@[m [mpublic:[m
     QMouseEvent *qMouseEv = dynamic_cast<QMouseEvent *>(e);[m
 [m
     if (qMouseEv != nullptr) {[m
[31m-      GlMainView *glMainView = static_cast<GlMainView *>(view());[m
[32m+[m[32m      GlView *glView = static_cast<GlView *>(view());[m
 [m
       /*[m
       Check if event is a left mouse button press[m
[36m@@ -86,8 +86,7 @@[m [mpublic:[m
           */[m
           SelectedEntity selectedEntity;[m
 [m
[31m-          if (glMainView->getGlWidget()->pickNodesEdges(qMouseEv->x(), qMouseEv->y(),[m
[31m-                                                        selectedEntity)) {[m
[32m+[m[32m          if (glView->getGlWidget()->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity)) {[m
 [m
             /*[m
             Change text of the information label with[m
[1mdiff --git a/doc/python/talipotguireference.rst b/doc/python/talipotguireference.rst[m
[1mindex f8cff024d..44b02b725 100644[m
[1m--- a/doc/python/talipotguireference.rst[m
[1m+++ b/doc/python/talipotguireference.rst[m
[36m@@ -46,10 +46,10 @@[m [mtlpgui.View[m
 .. autoclass:: talipotgui::tlpgui.View[m
         :members:[m
 [m
[31m-tlpgui.GlMainView[m
[32m+[m[32mtlpgui.GlView[m
 ------------------[m
 [m
[31m-.. autoclass:: talipotgui::tlpgui.GlMainView[m
[32m+[m[32m.. autoclass:: talipotgui::tlpgui.GlView[m
         :members:[m
 [m
 tlpgui.NodeLinkDiagramView[m
[1mdiff --git a/library/talipot-gui/include/CMakeLists.txt b/library/talipot-gui/include/CMakeLists.txt[m
[1mindex f0f4b5955..150e943ba 100644[m
[1m--- a/library/talipot-gui/include/CMakeLists.txt[m
[1m+++ b/library/talipot-gui/include/CMakeLists.txt[m
[36m@@ -2,7 +2,7 @@[m [mINSTALL([m
   FILES talipot/CaptionItem.h[m
         talipot/ColorScaleConfigDialog.h[m
         talipot/GLInteractor.h[m
[31m-        talipot/GlMainView.h[m
[32m+[m[32m        talipot/GlView.h[m
         talipot/GlWidget.h[m
         talipot/GlOffscreenRenderer.h[m
         talipot/GraphPropertiesSelectionWidget.h[m
[1mdiff --git a/library/talipot-gui/include/talipot/CaptionGraphicsItem.h b/library/talipot-gui/include/talipot/CaptionGraphicsItem.h[m
[1mindex 98ed1b2d2..2958f2ccb 100644[m
[1m--- a/library/talipot-gui/include/talipot/CaptionGraphicsItem.h[m
[1m+++ b/library/talipot-gui/include/talipot/CaptionGraphicsItem.h[m
[36m@@ -26,7 +26,7 @@[m [mclass QPushButton;[m
 [m
 namespace tlp {[m
 [m
[31m-class GlMainView;[m
[32m+[m[32mclass GlView;[m
 class View;[m
 class CaptionGraphicsBackgroundItem;[m
 [m
[1mdiff --git a/library/talipot-gui/include/talipot/GlOverviewGraphicsItem.h b/library/talipot-gui/include/talipot/GlOverviewGraphicsItem.h[m
[1mindex 4cb4ee2cc..91a349eab 100644[m
[1m--- a/library/talipot-gui/include/talipot/GlOverviewGraphicsItem.h[m
[1m+++ b/library/talipot-gui/include/talipot/GlOverviewGraphicsItem.h[m
[36m@@ -25,7 +25,7 @@[m
 [m
 namespace tlp {[m
 [m
[31m-class GlMainView;[m
[32m+[m[32mclass GlView;[m
 class GlScene;[m
 [m
 class TLP_QT_SCOPE GlOverviewGraphicsItem : public QObject, public QGraphicsRectItem {[m
[36m@@ -33,7 +33,7 @@[m [mclass TLP_QT_SCOPE GlOverviewGraphicsItem : public QObject, public QGraphicsRect[m
   Q_OBJECT[m
 [m
 public:[m
[31m-  GlOverviewGraphicsItem(GlMainView *view, GlScene &scene);[m
[32m+[m[32m  GlOverviewGraphicsItem(GlView *view, GlScene &scene);[m
   ~GlOverviewGraphicsItem() override;[m
 [m
   void setSize(unsigned int width, unsigned int height);[m
[36m@@ -64,7 +64,7 @@[m [mprivate:[m
   void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;[m
   void setScenePosition(QPointF pos);[m
 [m
[31m-  GlMainView *view;[m
[32m+[m[32m  GlView *view;[m
   GlScene &baseScene;[m
   unsigned int width, height;[m
 [m
[1mdiff --git a/library/talipot-gui/include/talipot/GlView.h b/library/talipot-gui/include/talipot/GlView.h[m
[1mindex fb0fc7987..9e7cc3b7c 100644[m
[1m--- a/library/talipot-gui/include/talipot/GlView.h[m
[1m+++ b/library/talipot-gui/include/talipot/GlView.h[m
[36m@@ -34,7 +34,7 @@[m [mclass ViewActionsManager;[m
  *[m
  * @brief An abstract view that displays a GlWidget as its central widget.[m
  *[m
[31m- * The GlMainView subclasses ViewWidget and always uses a GlWidget as the central widget of the[m
[32m+[m[32m * The GlView subclasses ViewWidget and always uses a GlWidget as the central widget of the[m
  panel. It also adds the following features:[m
  * @list[m
  * @li An overview of the scene that can be toggled on or off.[m
[36m@@ -44,22 +44,22 @@[m [mclass ViewActionsManager;[m
  * @li The possibility to make snapshots of the current scene[m
  * @endlist[m
  *[m
[31m- * Subclassing GlMainView means you will only want to display graphs in a single GlWidget.[m
[32m+[m[32m * Subclassing GlView means you will only want to display graphs in a single GlWidget.[m
  Switching the central widget can only be achieved from the ViewWidget class.[m
  *[m
  * @warning It is strongly unadvised to re-implement methods already implemented into tlp::View or[m
  tlp::ViewWidget. If you have to add custom behavior to those method, make sure to call the[m
  upper-class methods first:[m
  @code[m
[31m- void MyView::setupWidget() { // Where MyView is a subclass of tlp::GlMainView[m
[31m-   GlMainView::setupWidget(); // call this first[m
[32m+[m[32m void MyView::setupWidget() { // Where MyView is a subclass of tlp::GlView[m
[32m+[m[32m   GlView::setupWidget(); // call this first[m
    // insert custom behavior here[m
  }[m
  @endcode[m
 [m
  * @see tlp::ViewWidget[m
  */[m
[31m-class TLP_QT_SCOPE GlMainView : public tlp::ViewWidget {[m
[32m+[m[32mclass TLP_QT_SCOPE GlView : public tlp::ViewWidget {[m
 [m
   Q_OBJECT[m
 [m
[36m@@ -84,8 +84,8 @@[m [mpublic:[m
     OVERVIEW_BOTTOM_RIGHT[m
   };[m
 [m
[31m-  GlMainView(bool needTooltipAndUrlManager = false);[m
[31m-  ~GlMainView() override;[m
[32m+[m[32m  GlView(bool needTooltipAndUrlManager = false);[m
[32m+[m[32m  ~GlView() override;[m
   tlp::GlWidget *getGlWidget() const;[m
   QList<QWidget *> configurationWidgets() const override;[m
   bool overviewVisible() const;[m
[36m@@ -201,7 +201,7 @@[m [mpublic slots:[m
   void refresh() override;[m
 [m
   /**[m
[31m-   * @brief Force the overview to be redrawn. Since GlMainView already detects graph's[m
[32m+[m[32m   * @brief Force the overview to be redrawn. Since GlView already detects graph's[m
    * modifications, this method should not be called manually to avoid extra rendering.[m
    */[m
   virtual void drawOverview(bool generatePixmap = true);[m
[36m@@ -245,7 +245,7 @@[m [mpublic slots:[m
   QImage getRGBImage() const;[m
 [m
 protected slots:[m
[31m-  virtual void glMainViewDrawn(bool graphChanged);[m
[32m+[m[32m  virtual void glViewDrawn(bool graphChanged);[m
   virtual void sceneRectChanged(const QRectF &);[m
   void fillContextMenu(QMenu *menu, const QPointF &) override;[m
 [m
[1mdiff --git a/library/talipot-gui/include/talipot/NodeLinkDiagramView.h b/library/talipot-gui/include/talipot/NodeLinkDiagramView.h[m
[1mindex a6423464a..737b78d1d 100644[m
[1m--- a/library/talipot-gui/include/talipot/NodeLinkDiagramView.h[m
[1m+++ b/library/talipot-gui/include/talipot/NodeLinkDiagramView.h[m
[36m@@ -14,7 +14,7 @@[m
 #ifndef TALIPOT_NODE_LINK_DIAGRAM_VIEW_H[m
 #define TALIPOT_NODE_LINK_DIAGRAM_VIEW_H[m
 [m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/Camera.h>[m
 [m
 namespace Ui {[m
[36m@@ -31,7 +31,7 @@[m [mclass GlGraphRenderingParameters;[m
 class PropertyInterface;[m
 class StringProperty;[m
 [m
[31m-class TLP_QT_SCOPE NodeLinkDiagramView : public tlp::GlMainView {[m
[32m+[m[32mclass TLP_QT_SCOPE NodeLinkDiagramView : public tlp::GlView {[m
   Q_OBJECT[m
 [m
   GlGrid *_grid;[m
[1mdiff --git a/library/talipot-gui/include/talipot/QuickAccessBar.h b/library/talipot-gui/include/talipot/QuickAccessBar.h[m
[1mindex 312de761d..135f3a2df 100644[m
[1m--- a/library/talipot-gui/include/talipot/QuickAccessBar.h[m
[1m+++ b/library/talipot-gui/include/talipot/QuickAccessBar.h[m
[36m@@ -28,7 +28,7 @@[m [mclass QuickAccessBar;[m
 }[m
 [m
 namespace tlp {[m
[31m-class GlMainView;[m
[32m+[m[32mclass GlView;[m
 class GlGraphRenderingParameters;[m
 class GlGraphInputData;[m
 class GlScene;[m
[36m@@ -42,7 +42,7 @@[m [mclass TLP_QT_SCOPE QuickAccessBar : public QWidget {[m
   Q_OBJECT[m
 [m
 protected:[m
[31m-  GlMainView *_mainView;[m
[32m+[m[32m  GlView *_mainView;[m
   GlScene *scene() const;[m
   GlGraphInputData *inputData() const;[m
   GlGraphRenderingParameters *renderingParameters() const;[m
[36m@@ -50,7 +50,7 @@[m [mprotected:[m
 public:[m
   QuickAccessBar(QWidget *parent = nullptr);[m
 public slots:[m
[31m-  void setGlMainView(tlp::GlMainView *);[m
[32m+[m[32m  void setGlView(tlp::GlView *);[m
   virtual void reset() = 0;[m
 [m
 signals:[m
[1mdiff --git a/library/talipot-gui/src/CMakeLists.txt b/library/talipot-gui/src/CMakeLists.txt[m
[1mindex 3cd8ac78d..69c722211 100644[m
[1m--- a/library/talipot-gui/src/CMakeLists.txt[m
[1m+++ b/library/talipot-gui/src/CMakeLists.txt[m
[36m@@ -12,7 +12,7 @@[m [mSET(SRCS[m
     DownloadManager.cpp[m
     DragHandle.cpp[m
     GlCompositeHierarchyManager.cpp[m
[31m-    GlMainView.cpp[m
[32m+[m[32m    GlView.cpp[m
     GlWidget.cpp[m
     GlOffscreenRenderer.cpp[m
     GraphPropertiesSelectionWidget.cpp[m
[36m@@ -182,7 +182,7 @@[m [mQTX_WRAP_CPP([m
   ../include/talipot/DragHandle.h[m
   ../include/talipot/FileDownloader.h[m
   ../include/talipot/GLInteractor.h[m
[31m-  ../include/talipot/GlMainView.h[m
[32m+[m[32m  ../include/talipot/GlView.h[m
   ../include/talipot/GlWidget.h[m
   ../include/talipot/GlWidgetGraphicsItem.h[m
   ../include/talipot/GlOverviewGraphicsItem.h[m
[1mdiff --git a/library/talipot-gui/src/GlOverviewGraphicsItem.cpp b/library/talipot-gui/src/GlOverviewGraphicsItem.cpp[m
[1mindex 37b54a03d..6ffdd3eee 100644[m
[1m--- a/library/talipot-gui/src/GlOverviewGraphicsItem.cpp[m
[1m+++ b/library/talipot-gui/src/GlOverviewGraphicsItem.cpp[m
[36m@@ -20,7 +20,7 @@[m
 [m
 #include <talipot/GlOffscreenRenderer.h>[m
 #include <talipot/GlWidget.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlGraph.h>[m
 #include <talipot/GlOffscreenRenderer.h>[m
 [m
[36m@@ -28,7 +28,7 @@[m [musing namespace std;[m
 [m
 namespace tlp {[m
 [m
[31m-GlOverviewGraphicsItem::GlOverviewGraphicsItem(GlMainView *view, GlScene &scene)[m
[32m+[m[32mGlOverviewGraphicsItem::GlOverviewGraphicsItem(GlView *view, GlScene &scene)[m
     : QGraphicsRectItem(0, 0, 128, 128), view(view), baseScene(scene), width(128), height(128),[m
       mouseClicked(false), _frameColor(Color::Gray), _frameWidth(2) {}[m
 [m
[1mdiff --git a/library/talipot-gui/src/GlView.cpp b/library/talipot-gui/src/GlView.cpp[m
[1mindex d00381ad1..8e7f925fb 100644[m
[1m--- a/library/talipot-gui/src/GlView.cpp[m
[1m+++ b/library/talipot-gui/src/GlView.cpp[m
[36m@@ -11,7 +11,7 @@[m
  *[m
  */[m
 [m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 [m
 #include <QApplication>[m
 #include <QGraphicsProxyWidget>[m
[36m@@ -37,7 +37,7 @@[m
 [m
 using namespace tlp;[m
 [m
[31m-GlMainView::GlMainView(bool needtturlManager)[m
[32m+[m[32mGlView::GlView(bool needtturlManager)[m
     : _glWidget(nullptr), _overviewItem(nullptr), _viewActionsManager(nullptr),[m
       _showOvButton(nullptr), _showQabButton(nullptr), needQuickAccessBar(false),[m
       _needTooltipAndUrlManager(needtturlManager), _quickAccessBarItem(nullptr),[m
[36m@@ -45,43 +45,43 @@[m [mGlMainView::GlMainView(bool needtturlManager)[m
       _sceneLayersConfigurationWidget(nullptr), _overviewPosition(OVERVIEW_BOTTOM_RIGHT),[m
       _updateOverview(true) {}[m
 [m
[31m-GlMainView::~GlMainView() {[m
[32m+[m[32mGlView::~GlView() {[m
   delete _sceneConfigurationWidget;[m
   delete _sceneLayersConfigurationWidget;[m
   delete _overviewItem;[m
   delete _viewActionsManager;[m
 }[m
 [m
[31m-void GlMainView::draw() {[m
[32m+[m[32mvoid GlView::draw() {[m
   _glWidget->draw();[m
 }[m
 [m
[31m-void GlMainView::redraw() {[m
[32m+[m[32mvoid GlView::redraw() {[m
   _glWidget->redraw();[m
 }[m
 [m
[31m-void GlMainView::refresh() {[m
[32m+[m[32mvoid GlView::refresh() {[m
   _glWidget->draw(false);[m
 }[m
 [m
[31m-void GlMainView::setOverviewPosition(const OverviewPosition &position) {[m
[32m+[m[32mvoid GlView::setOverviewPosition(const OverviewPosition &position) {[m
   _overviewPosition = position;[m
   drawOverview();[m
 }[m
 [m
[31m-GlMainView::OverviewPosition GlMainView::overviewPosition() const {[m
[32m+[m[32mGlView::OverviewPosition GlView::overviewPosition() const {[m
   return _overviewPosition;[m
 }[m
 [m
[31m-void GlMainView::setUpdateOverview(bool updateOverview) {[m
[32m+[m[32mvoid GlView::setUpdateOverview(bool updateOverview) {[m
   _updateOverview = updateOverview;[m
 }[m
 [m
[31m-bool GlMainView::updateOverview() const {[m
[32m+[m[32mbool GlView::updateOverview() const {[m
   return _updateOverview;[m
 }[m
 [m
[31m-void GlMainView::setState(const tlp::DataSet &data) {[m
[32m+[m[32mvoid GlView::setState(const tlp::DataSet &data) {[m
   View::setState(data);[m
 [m
   bool overviewVisible = true;[m
[36m@@ -98,7 +98,7 @@[m [mvoid GlMainView::setState(const tlp::DataSet &data) {[m
   }[m
 }[m
 [m
[31m-tlp::DataSet GlMainView::state() const {[m
[32m+[m[32mtlp::DataSet GlView::state() const {[m
   DataSet data = View::state();[m
   data.set("overviewVisible", overviewVisible());[m
 [m
[36m@@ -109,7 +109,7 @@[m [mtlp::DataSet GlMainView::state() const {[m
   return data;[m
 }[m
 [m
[31m-void GlMainView::drawOverview(bool generatePixmap) {[m
[32m+[m[32mvoid GlView::drawOverview(bool generatePixmap) {[m
   if (_overviewItem == nullptr) {[m
     _overviewItem = new GlOverviewGraphicsItem(this, *_glWidget->getScene());[m
     addToScene(_overviewItem);[m
[36m@@ -123,7 +123,7 @@[m [mvoid GlMainView::drawOverview(bool generatePixmap) {[m
   }[m
 }[m
 [m
[31m-void GlMainView::assignNewGlWidget(GlWidget *glWidget, bool deleteOldGlWidget) {[m
[32m+[m[32mvoid GlView::assignNewGlWidget(GlWidget *glWidget, bool deleteOldGlWidget) {[m
   _glWidget = glWidget;[m
 [m
   if (_sceneLayersConfigurationWidget == nullptr) {[m
[36m@@ -140,29 +140,28 @@[m [mvoid GlMainView::assignNewGlWidget(GlWidget *glWidget, bool deleteOldGlWidget) {[m
   _sceneConfigurationWidget = new SceneConfigWidget();[m
   _sceneConfigurationWidget->setGlWidget(_glWidget);[m
 [m
[31m-  connect(glWidgetGraphicsItem, &GlWidgetGraphicsItem::widgetPainted, this,[m
[31m-          &GlMainView::glMainViewDrawn);[m
[32m+[m[32m  connect(glWidgetGraphicsItem, &GlWidgetGraphicsItem::widgetPainted, this, &GlView::glViewDrawn);[m
   // Tooltip events and url management[m
   if (_needTooltipAndUrlManager) {[m
     activateTooltipAndUrlManager(_glWidget);[m
   }[m
 }[m
 [m
[31m-GlOverviewGraphicsItem *GlMainView::overviewItem() const {[m
[32m+[m[32mGlOverviewGraphicsItem *GlView::overviewItem() const {[m
   return _overviewItem;[m
 }[m
 [m
[31m-void GlMainView::setupWidget() {[m
[32m+[m[32mvoid GlView::setupWidget() {[m
   graphicsView()->viewport()->parentWidget()->installEventFilter(this);[m
   assignNewGlWidget(new GlWidget(nullptr, this), true);[m
   _viewActionsManager = new ViewActionsManager(this, _glWidget, false);[m
 }[m
 [m
[31m-GlWidget *GlMainView::getGlWidget() const {[m
[32m+[m[32mGlWidget *GlView::getGlWidget() const {[m
   return _glWidget;[m
 }[m
 [m
[31m-void GlMainView::centerView(bool graphChanged) {[m
[32m+[m[32mvoid GlView::centerView(bool graphChanged) {[m
 [m
   float gvWidth = graphicsView()->width();[m
   // we apply a zoom factor to preserve a 50 px margin width[m
[36m@@ -174,17 +173,17 @@[m [mvoid GlMainView::centerView(bool graphChanged) {[m
   }[m
 }[m
 [m
[31m-void GlMainView::glMainViewDrawn(bool graphChanged) {[m
[32m+[m[32mvoid GlView::glViewDrawn(bool graphChanged) {[m
   if (_overviewItem && _overviewItem->isVisible()) {[m
     drawOverview(graphChanged);[m
   }[m
 }[m
 [m
[31m-QList<QWidget *> GlMainView::configurationWidgets() const {[m
[32m+[m[32mQList<QWidget *> GlView::configurationWidgets() const {[m
   return QList<QWidget *>() << _sceneConfigurationWidget << _sceneLayersConfigurationWidget;[m
 }[m
 [m
[31m-void GlMainView::updateShowOverviewButton() {[m
[32m+[m[32mvoid GlView::updateShowOverviewButton() {[m
   if (_showOvButton == nullptr) {[m
     QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();[m
     _showOvButton = new QPushButton();[m
[36m@@ -195,7 +194,7 @@[m [mvoid GlMainView::updateShowOverviewButton() {[m
     proxy->setWidget(_showOvButton);[m
     addToScene(proxy);[m
     proxy->setZValue(10);[m
[31m-    connect(_showOvButton, &QAbstractButton::toggled, this, &GlMainView::setOverviewVisible);[m
[32m+[m[32m    connect(_showOvButton, &QAbstractButton::toggled, this, &GlView::setOverviewVisible);[m
   }[m
 [m
   _showOvButton->setVisible(_overviewPosition == OVERVIEW_BOTTOM_RIGHT);[m
[36m@@ -227,22 +226,22 @@[m [mvoid GlMainView::updateShowOverviewButton() {[m
   }[m
 }[m
 [m
[31m-void GlMainView::setOverviewVisible(bool display) {[m
[32m+[m[32mvoid GlView::setOverviewVisible(bool display) {[m
   drawOverview(true);[m
   _overviewItem->setVisible(display);[m
   updateShowOverviewButton();[m
 }[m
 [m
[31m-bool GlMainView::overviewVisible() const {[m
[32m+[m[32mbool GlView::overviewVisible() const {[m
   return (_overviewItem && _overviewItem->isVisible());[m
 }[m
 [m
[31m-void GlMainView::setViewOrtho(bool viewOrtho) {[m
[32m+[m[32mvoid GlView::setViewOrtho(bool viewOrtho) {[m
   getGlWidget()->getScene()->setViewOrtho(viewOrtho);[m
   _glWidget->draw(false);[m
 }[m
 [m
[31m-void GlMainView::updateShowQuickAccessBarButton() {[m
[32m+[m[32mvoid GlView::updateShowQuickAccessBarButton() {[m
   if (needQuickAccessBar) {[m
     if (_showQabButton == nullptr) {[m
       QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();[m
[36m@@ -254,8 +253,7 @@[m [mvoid GlMainView::updateShowQuickAccessBarButton() {[m
       proxy->setWidget(_showQabButton);[m
       addToScene(proxy);[m
       proxy->setZValue(11);[m
[31m-      connect(_showQabButton, &QAbstractButton::toggled, this,[m
[31m-              &GlMainView::setQuickAccessBarVisible);[m
[32m+[m[32m      connect(_showQabButton, &QAbstractButton::toggled, this, &GlView::setQuickAccessBarVisible);[m
     }[m
 [m
     QRectF rect(QPoint(0, 0), graphicsView()->size());[m
[36m@@ -279,11 +277,11 @@[m [mvoid GlMainView::updateShowQuickAccessBarButton() {[m
   }[m
 }[m
 [m
[31m-QuickAccessBar *GlMainView::getQuickAccessBarImpl() {[m
[32m+[m[32mQuickAccessBar *GlView::getQuickAccessBarImpl() {[m
   return new QuickAccessBarImpl(_quickAccessBarItem);[m
 }[m
 [m
[31m-void GlMainView::setQuickAccessBarVisible(bool visible) {[m
[32m+[m[32mvoid GlView::setQuickAccessBarVisible(bool visible) {[m
   if (!visible) {[m
     delete _quickAccessBar;[m
     _quickAccessBar = nullptr;[m
[36m@@ -299,7 +297,7 @@[m [mvoid GlMainView::setQuickAccessBarVisible(bool visible) {[m
     connect(_sceneConfigurationWidget, &SceneConfigWidget::settingsApplied, _quickAccessBar,[m
             &QuickAccessBar::reset);[m
 [m
[31m-    _quickAccessBar->setGlMainView(this);[m
[32m+[m[32m    _quickAccessBar->setGlView(this);[m
     _quickAccessBarItem->setWidget(_quickAccessBar);[m
     addToScene(_quickAccessBarItem);[m
     _quickAccessBarItem->setZValue(10);[m
[36m@@ -308,11 +306,11 @@[m [mvoid GlMainView::setQuickAccessBarVisible(bool visible) {[m
   sceneRectChanged(QRectF(QPoint(0, 0), graphicsView()->size()));[m
 }[m
 [m
[31m-bool GlMainView::quickAccessBarVisible() const {[m
[32m+[m[32mbool GlView::quickAccessBarVisible() const {[m
   return _quickAccessBarItem != nullptr;[m
 }[m
 [m
[31m-void GlMainView::sceneRectChanged(const QRectF &rect) {[m
[32m+[m[32mvoid GlView::sceneRectChanged(const QRectF &rect) {[m
   if (_quickAccessBar != nullptr) {[m
     _quickAccessBarItem->setPos(0, rect.height() - _quickAccessBarItem->size().height());[m
     _quickAccessBarItem->resize(rect.width(), _quickAccessBarItem->size().height());[m
[36m@@ -341,7 +339,7 @@[m [mvoid GlMainView::sceneRectChanged(const QRectF &rect) {[m
   }[m
 }[m
 [m
[31m-QPixmap GlMainView::snapshot(const QSize &outputSize) const {[m
[32m+[m[32mQPixmap GlView::snapshot(const QSize &outputSize) const {[m
   if (_glWidget == nullptr) {[m
     return QPixmap();[m
   }[m
[36m@@ -355,13 +353,13 @@[m [mQPixmap GlMainView::snapshot(const QSize &outputSize) const {[m
   return QPixmap::fromImage(_glWidget->createPicture(realSize.width(), realSize.height(), false));[m
 }[m
 [m
[31m-QImage GlMainView::getRGBImage() const {[m
[32m+[m[32mQImage GlView::getRGBImage() const {[m
   QSize currentSize = _glWidget->size();[m
   return _glWidget->createPicture(currentSize.width(), currentSize.height(), false,[m
                                   QImage::Format_RGB888);[m
 }[m
 [m
[31m-void GlMainView::undoCallback() {[m
[32m+[m[32mvoid GlView::undoCallback() {[m
   float gvWidth = graphicsView()->width();[m
   // we apply a zoom factor to preserve a 50 px margin width[m
   // to ensure the scene will not be drawn under the configuration tabs title[m
[36m@@ -369,27 +367,27 @@[m [mvoid GlMainView::undoCallback() {[m
   draw();[m
 }[m
 [m
[31m-void GlMainView::fillContextMenu(QMenu *menu, const QPointF &pf) {[m
[32m+[m[32mvoid GlView::fillContextMenu(QMenu *menu, const QPointF &pf) {[m
   _viewActionsManager->fillContextMenu(menu);[m
 [m
   QAction *viewOrtho = menu->addAction("Use orthogonal projection");[m
   viewOrtho->setToolTip("Enable to switch between true perspective and orthogonal");[m
   viewOrtho->setCheckable(true);[m
   viewOrtho->setChecked(_glWidget->getScene()->isViewOrtho());[m
[31m-  connect(viewOrtho, &QAction::triggered, this, &GlMainView::setViewOrtho);[m
[32m+[m[32m  connect(viewOrtho, &QAction::triggered, this, &GlView::setViewOrtho);[m
 [m
   menu->addSeparator();[m
   menu->addAction("Augmented display")->setEnabled(false);[m
   menu->addSeparator();[m
 [m
[31m-  QAction *a = menu->addAction("Show overview", this, &GlMainView::setOverviewVisible);[m
[32m+[m[32m  QAction *a = menu->addAction("Show overview", this, &GlView::setOverviewVisible);[m
   a->setToolTip("Show/hide the overview in a corner of the view");[m
   a->setCheckable(true);[m
   a->setChecked(overviewVisible());[m
 [m
   if (needQuickAccessBar) {[m
     QAction *quickbarAction =[m
[31m-        menu->addAction("Show quick access bar", this, &GlMainView::setQuickAccessBarVisible);[m
[32m+[m[32m        menu->addAction("Show quick access bar", this, &GlView::setQuickAccessBarVisible);[m
     quickbarAction->setToolTip("Show/hide the quick access bar");[m
     quickbarAction->setCheckable(true);[m
     quickbarAction->setChecked(quickAccessBarVisible());[m
[36m@@ -397,11 +395,11 @@[m [mvoid GlMainView::fillContextMenu(QMenu *menu, const QPointF &pf) {[m
   View::fillContextMenu(menu, pf);[m
 }[m
 [m
[31m-void GlMainView::applySettings() {[m
[32m+[m[32mvoid GlView::applySettings() {[m
   _sceneConfigurationWidget->applySettings();[m
 }[m
 [m
[31m-bool GlMainView::eventFilter(QObject *obj, QEvent *event) {[m
[32m+[m[32mbool GlView::eventFilter(QObject *obj, QEvent *event) {[m
   if (event->type() == QEvent::Resize) {[m
     // ensure automatic resize of the viewport[m
     QResizeEvent *resizeEvent = static_cast<QResizeEvent *>(event);[m
[36m@@ -432,7 +430,7 @@[m [mbool GlMainView::eventFilter(QObject *obj, QEvent *event) {[m
   return ViewWidget::eventFilter(obj, event);[m
 }[m
 [m
[31m-bool GlMainView::getNodeOrEdgeAtViewportPos(GlWidget *glw, int x, int y, node &n, edge &e) {[m
[32m+[m[32mbool GlView::getNodeOrEdgeAtViewportPos(GlWidget *glw, int x, int y, node &n, edge &e) {[m
   SelectedEntity type;[m
   if (glw->pickNodesEdges(x, y, type)) {[m
     node tmpNode = type.getNode();[m
[36m@@ -451,8 +449,8 @@[m [mbool GlMainView::getNodeOrEdgeAtViewportPos(GlWidget *glw, int x, int y, node &n[m
   return false;[m
 }[m
 [m
[31m-bool GlMainView::pickNodeEdge(const int x, const int y, node &n, edge &e, bool pickNode,[m
[31m-                              bool pickEdge) {[m
[32m+[m[32mbool GlView::pickNodeEdge(const int x, const int y, node &n, edge &e, bool pickNode,[m
[32m+[m[32m                          bool pickEdge) {[m
   SelectedEntity selectedEntity;[m
   bool foundEntity =[m
       getGlWidget()->pickNodesEdges(x, y, selectedEntity, nullptr, pickNode, pickEdge);[m
[36m@@ -471,7 +469,7 @@[m [mbool GlMainView::pickNodeEdge(const int x, const int y, node &n, edge &e, bool p[m
   return false;[m
 }[m
 [m
[31m-void GlMainView::zoomAndPanAnimation(const tlp::BoundingBox &boundingBox, const double duration) {[m
[32m+[m[32mvoid GlView::zoomAndPanAnimation(const tlp::BoundingBox &boundingBox, const double duration) {[m
   BoundingBox bb;[m
   if (bb.isValid()) {[m
     bb = boundingBox;[m
[1mdiff --git a/library/talipot-gui/src/MouseEdgeBuilder.cpp b/library/talipot-gui/src/MouseEdgeBuilder.cpp[m
[1mindex 0be3d1dec..e708c0202 100644[m
[1m--- a/library/talipot-gui/src/MouseEdgeBuilder.cpp[m
[1m+++ b/library/talipot-gui/src/MouseEdgeBuilder.cpp[m
[36m@@ -17,7 +17,7 @@[m
 #include <talipot/LayoutProperty.h>[m
 #include <talipot/ColorProperty.h>[m
 #include <talipot/GlWidget.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlTools.h>[m
 #include <talipot/GlLine.h>[m
 #include <talipot/GlGraph.h>[m
[1mdiff --git a/library/talipot-gui/src/MouseInteractors.cpp b/library/talipot-gui/src/MouseInteractors.cpp[m
[1mindex cd0603991..b00ee4623 100644[m
[1m--- a/library/talipot-gui/src/MouseInteractors.cpp[m
[1m+++ b/library/talipot-gui/src/MouseInteractors.cpp[m
[36m@@ -21,7 +21,7 @@[m
 #include <talipot/View.h>[m
 #include <talipot/Observable.h>[m
 #include <talipot/GlGraph.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlBoundingBoxSceneVisitor.h>[m
 #include <talipot/DrawingTools.h>[m
 #include <talipot/QtGlSceneZoomAndPanAnimator.h>[m
[1mdiff --git a/library/talipot-gui/src/MouseNodeBuilder.cpp b/library/talipot-gui/src/MouseNodeBuilder.cpp[m
[1mindex 7734dc6ee..a8da93999 100644[m
[1m--- a/library/talipot-gui/src/MouseNodeBuilder.cpp[m
[1m+++ b/library/talipot-gui/src/MouseNodeBuilder.cpp[m
[36m@@ -20,7 +20,7 @@[m
 #include <talipot/ColorProperty.h>[m
 #include <talipot/GlWidget.h>[m
 #include <talipot/GlGraph.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/Camera.h>[m
 [m
 using namespace tlp;[m
[1mdiff --git a/library/talipot-gui/src/MouseShowElementInfo.cpp b/library/talipot-gui/src/MouseShowElementInfo.cpp[m
[1mindex 39b81cca7..5c142544a 100644[m
[1m--- a/library/talipot-gui/src/MouseShowElementInfo.cpp[m
[1m+++ b/library/talipot-gui/src/MouseShowElementInfo.cpp[m
[36m@@ -22,7 +22,7 @@[m
 [m
 #include <talipot/GraphElementModel.h>[m
 #include <talipot/ItemDelegate.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlWidget.h>[m
 #include <talipot/GlScene.h>[m
 #include <talipot/MouseShowElementInfo.h>[m
[1mdiff --git a/library/talipot-gui/src/NodeLinkDiagramView.cpp b/library/talipot-gui/src/NodeLinkDiagramView.cpp[m
[1mindex 828e0ea1c..ce7d54ad6 100644[m
[1m--- a/library/talipot-gui/src/NodeLinkDiagramView.cpp[m
[1m+++ b/library/talipot-gui/src/NodeLinkDiagramView.cpp[m
[36m@@ -46,7 +46,7 @@[m [musing namespace std;[m
 const string NodeLinkDiagramView::viewName("Node Link Diagram");[m
 [m
 NodeLinkDiagramView::NodeLinkDiagramView(const tlp::PluginContext *)[m
[31m-    : GlMainView(true), _grid(nullptr), _gridOptions(nullptr), manager(nullptr), _hasHulls(false),[m
[32m+[m[32m    : GlView(true), _grid(nullptr), _gridOptions(nullptr), manager(nullptr), _hasHulls(false),[m
       grid_ui(nullptr) {}[m
 [m
 NodeLinkDiagramView::~NodeLinkDiagramView() {[m
[36m@@ -113,7 +113,7 @@[m [mvoid NodeLinkDiagramView::updateGrid() {[m
 [m
 void NodeLinkDiagramView::draw() {[m
   updateGrid();[m
[31m-  GlMainView::draw();[m
[32m+[m[32m  GlView::draw();[m
 }[m
 [m
 void NodeLinkDiagramView::setState(const tlp::DataSet &data) {[m
[36m@@ -135,7 +135,7 @@[m [mvoid NodeLinkDiagramView::setState(const tlp::DataSet &data) {[m
 [m
   setOverviewVisible(true);[m
   setQuickAccessBarVisible(true);[m
[31m-  GlMainView::setState(data);[m
[32m+[m[32m  GlView::setState(data);[m
 [m
   bool keepSPOV = false;[m
   data.get<bool>("keepScenePointOfViewOnSubgraphChanging", keepSPOV);[m
[36m@@ -260,7 +260,7 @@[m [mvoid NodeLinkDiagramView::createScene(Graph *graph, DataSet dataSet) {[m
 //==================================================[m
 DataSet NodeLinkDiagramView::sceneData() const {[m
   GlScene *scene = getGlWidget()->getScene();[m
[31m-  DataSet outDataSet = GlMainView::state();[m
[32m+[m[32m  DataSet outDataSet = GlView::state();[m
   outDataSet.set("Display", scene->getGlGraph()->getRenderingParameters().getParameters());[m
   std::string out;[m
   scene->getXML(out);[m
[36m@@ -534,7 +534,7 @@[m [mvoid NodeLinkDiagramView::fillContextMenu(QMenu *menu, const QPointF &point) {[m
       View::fillContextMenu(menu, edge(entity.getComplexEntityId()));[m
     }[m
   } else {[m
[31m-    GlMainView::fillContextMenu(menu, point);[m
[32m+[m[32m    GlView::fillContextMenu(menu, point);[m
 [m
     QAction *action = menu->addAction("Use Z ordering");[m
     action->setToolTip([m
[1mdiff --git a/library/talipot-gui/src/QuickAccessBar.cpp b/library/talipot-gui/src/QuickAccessBar.cpp[m
[1mindex bdca6f6e6..9e8ebb73e 100644[m
[1m--- a/library/talipot-gui/src/QuickAccessBar.cpp[m
[1m+++ b/library/talipot-gui/src/QuickAccessBar.cpp[m
[36m@@ -30,7 +30,7 @@[m
 #include <talipot/GlGraphRenderingParameters.h>[m
 #include <talipot/TlpQtTools.h>[m
 #include <talipot/GlWidget.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlGraphInputData.h>[m
 #include <talipot/SnapshotDialog.h>[m
 #include <talipot/ItemDelegate.h>[m
[36m@@ -219,7 +219,7 @@[m [mQuickAccessBarImpl::~QuickAccessBarImpl() {[m
   delete _ui;[m
 }[m
 [m
[31m-void QuickAccessBar::setGlMainView(GlMainView *v) {[m
[32m+[m[32mvoid QuickAccessBar::setGlView(GlView *v) {[m
   _mainView = v;[m
   reset();[m
 }[m
[1mdiff --git a/library/talipot-gui/src/SceneLayersConfigWidget.cpp b/library/talipot-gui/src/SceneLayersConfigWidget.cpp[m
[1mindex bb434117d..38584fadc 100644[m
[1m--- a/library/talipot-gui/src/SceneLayersConfigWidget.cpp[m
[1m+++ b/library/talipot-gui/src/SceneLayersConfigWidget.cpp[m
[36m@@ -14,7 +14,7 @@[m
 #include "talipot/SceneLayersConfigWidget.h"[m
 #include "ui_SceneLayersConfigWidget.h"[m
 [m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlWidget.h>[m
 #include <talipot/SceneLayersModel.h>[m
 [m
[1mdiff --git a/library/talipot-python/bindings/talipot-gui/CMakeLists.txt b/library/talipot-python/bindings/talipot-gui/CMakeLists.txt[m
[1mindex 912c319aa..e66ae9b7b 100644[m
[1m--- a/library/talipot-python/bindings/talipot-gui/CMakeLists.txt[m
[1m+++ b/library/talipot-python/bindings/talipot-gui/CMakeLists.txt[m
[36m@@ -2,7 +2,7 @@[m [mSET(TALIPOT_GUI_SIP_FILES[m
     Interactor.sip[m
     Module.sip[m
     View.sip[m
[31m-    GlMainView.sip[m
[32m+[m[32m    GlView.sip[m
     NodeLinkDiagramView.sip[m
     TalipotGui.sip[m
     ColorScalesManager.sip)[m
[1mdiff --git a/library/talipot-python/bindings/talipot-gui/GlMainView.sip b/library/talipot-python/bindings/talipot-gui/GlMainView.sip[m
[1mindex 70774368e..8870e83c8 100644[m
[1m--- a/library/talipot-python/bindings/talipot-gui/GlMainView.sip[m
[1m+++ b/library/talipot-python/bindings/talipot-gui/GlMainView.sip[m
[36m@@ -13,9 +13,9 @@[m
 [m
 namespace tlp {[m
 [m
[31m-class GlMainView : tlp::View /Abstract/ {[m
[32m+[m[32mclass GlView : tlp::View /Abstract/ {[m
 %TypeHeaderCode[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <vector>[m
 %End[m
 [m
[36m@@ -27,12 +27,12 @@[m [mThis is the based class of Talipot OpenGL views.[m
 [m
 public:[m
 [m
[31m-  GlMainView();[m
[31m-  virtual ~GlMainView();[m
[32m+[m[32m  GlView();[m
[32m+[m[32m  virtual ~GlView();[m
 [m
   bool overviewVisible() const;[m
 %Docstring[m
[31m-tlpgui.GlMainView.overviewVisible()[m
[32m+[m[32mtlpgui.GlView.overviewVisible()[m
 [m
 Returns the visibility state of the overview window.[m
 [m
[36m@@ -43,7 +43,7 @@[m [mReturns the visibility state of the overview window.[m
 [m
   void setOverviewVisible(bool);[m
 %Docstring[m
[31m-tlpgui.GlMainView.setOverviewVisible(state)[m
[32m+[m[32mtlpgui.GlView.setOverviewVisible(state)[m
 [m
 Sets the visibility state of the overview window.[m
 [m
[36m@@ -55,7 +55,7 @@[m [mSets the visibility state of the overview window.[m
 [m
   bool quickAccessBarVisible() const;[m
 %Docstring[m
[31m-tlpgui.GlMainView.quickAccessBarVisible()[m
[32m+[m[32mtlpgui.GlView.quickAccessBarVisible()[m
 [m
 Returns the visibility state of the quick access bar.[m
 [m
[36m@@ -66,7 +66,7 @@[m [mReturns the visibility state of the quick access bar.[m
 [m
   void setQuickAccessBarVisible(bool);[m
 %Docstring[m
[31m-tlpgui.GlMainView.setQuickAccessBarVisible(state)[m
[32m+[m[32mtlpgui.GlView.setQuickAccessBarVisible(state)[m
 [m
 Allows to control the quiack access bar visibility.[m
 [m
[36m@@ -78,7 +78,7 @@[m [mAllows to control the quiack access bar visibility.[m
 [m
   virtual void drawOverview(bool generatePixmap=true);[m
 %Docstring[m
[31m-tlpgui.GlMainView.drawOverview()[m
[32m+[m[32mtlpgui.GlView.drawOverview()[m
 [m
 Forces the redraw of the overview window.[m
 %End[m
[36m@@ -87,7 +87,7 @@[m [mForces the redraw of the overview window.[m
 [m
   virtual void centerView(bool graphChanged = false);[m
 %Docstring[m
[31m-tlpgui.GlMainView.centerView()[m
[32m+[m[32mtlpgui.GlView.centerView()[m
 [m
 Centers the OpenGL scene in the view window.[m
 %End[m
[36m@@ -97,7 +97,7 @@[m [mCenters the OpenGL scene in the view window.[m
 [m
   void setViewOrtho(bool);[m
 %Docstring[m
[31m-tlpgui.GlMainView.setViewOrtho(state)[m
[32m+[m[32mtlpgui.GlView.setViewOrtho(state)[m
 [m
 Activates/deactivate the orthogonal projection for OpenGL rendering.[m
 Perspective projection is used when orthogonal one is disabled.[m
[36m@@ -110,7 +110,7 @@[m [mPerspective projection is used when orthogonal one is disabled.[m
 [m
   bool pickNodeEdge(int x, int y, tlp::node &n /Out/, tlp::edge &e /Out/, bool pickNode = true, bool pickEdge = true)/TypeHint="Tuple[bool, node, edge]"/ ;[m
 %Docstring[m
[31m-tlpgui.GlMainView.pickNodeEdge(x, y, pickNode, pickEdge)[m
[32m+[m[32mtlpgui.GlView.pickNodeEdge(x, y, pickNode, pickEdge)[m
 [m
 Returns a tuple of 3 elements. The first one is a bool indicating if a node/edge has been found found at the (x,y) position in the view, the second element is the node found, the third the edge found.[m
 [m
[36m@@ -129,7 +129,7 @@[m [mReturns a tuple of 3 elements. The first one is a bool indicating if a node/edge[m
 [m
   void rotateCamera(int x, int y, int z);[m
 %Docstring[m
[31m-tlpgui.GlMainView.rotateCamera(x, y, z)[m
[32m+[m[32mtlpgui.GlView.rotateCamera(x, y, z)[m
 [m
 Rotate the view camera by (x, y, z)[m
 [m
[36m@@ -145,7 +145,7 @@[m [mRotate the view camera by (x, y, z)[m
 [m
   void translateCamera(int x, int y, int z);[m
 %Docstring[m
[31m-tlpgui.GlMainView.translateCamera(x, y, z)[m
[32m+[m[32mtlpgui.GlView.translateCamera(x, y, z)[m
 [m
 Translate the view camera by (x, y, z)[m
 [m
[36m@@ -161,7 +161,7 @@[m [mTranslate the view camera by (x, y, z)[m
 [m
   Coord viewToWorld(const Coord &vpos);[m
 %Docstring[m
[31m-tlpgui.GlMainView.viewToWorld(vpos)[m
[32m+[m[32mtlpgui.GlView.viewToWorld(vpos)[m
 [m
 Return the 3D world position for the given view position[m
 [m
[36m@@ -173,7 +173,7 @@[m [mReturn the 3D world position for the given view position[m
 [m
   Coord worldToView(const Coord &wpos);[m
 %Docstring[m
[31m-tlpgui.GlMainView.viewToWorld(wpos)[m
[32m+[m[32mtlpgui.GlView.viewToWorld(wpos)[m
 [m
 Return the view position for the given 3D position[m
 [m
[36m@@ -185,7 +185,7 @@[m [mReturn the view position for the given 3D position[m
 [m
   void zoomXY(int step, int x, int y);[m
 %Docstring[m
[31m-tlpgui.GlMainView.zoomXY(step, x, y)[m
[32m+[m[32mtlpgui.GlView.zoomXY(step, x, y)[m
 [m
 Zoom by step to the given (x,y) view position[m
 [m
[36m@@ -201,7 +201,7 @@[m [mZoom by step to the given (x,y) view position[m
 [m
   void zoom(int step);[m
 %Docstring[m
[31m-tlpgui.GlMainView.zoom(step)[m
[32m+[m[32mtlpgui.GlView.zoom(step)[m
 [m
 Zoom by step[m
 [m
[36m@@ -213,7 +213,7 @@[m [mZoom by step[m
 [m
   void zoomFactor(float factor);[m
 %Docstring[m
[31m-tlpgui.GlMainView.zoom(factor)[m
[32m+[m[32mtlpgui.GlView.zoom(factor)[m
 [m
 Zoom by factor[m
 [m
[36m@@ -225,7 +225,7 @@[m [mZoom by factor[m
 [m
   void zoomAndPanAnimation(const tlp::BoundingBox &bb, double duration = 1000.);[m
 %Docstring[m
[31m-tlpgui.GlMainView.zoomAndPanAnimation(bb, duration)[m
[32m+[m[32mtlpgui.GlView.zoomAndPanAnimation(bb, duration)[m
 [m
 Do a zoom and pan animation during a specific duration.[m
 At the end of the animation, the view will be zoomed and centered on the content of the given bounding box.[m
[36m@@ -240,7 +240,7 @@[m [mAt the end of the animation, the view will be zoomed and centered on the content[m
 [m
   PyObject *getRGBImage(int &width /Out/, int &height /Out/);[m
 %Docstring[m
[31m-tlpgui.GlMainView.getRGBImage()[m
[32m+[m[32mtlpgui.GlView.getRGBImage()[m
 [m
 Return a tuple of 3 elements. The first element is the image in RGB format (3 bytes per pixel) as a bytearray object; the second is the width of the image and the third is its height.[m
 [m
[1mdiff --git a/library/talipot-python/bindings/talipot-gui/Module.sip b/library/talipot-python/bindings/talipot-gui/Module.sip[m
[1mindex eeb2ccd3a..e662f6abf 100644[m
[1m--- a/library/talipot-python/bindings/talipot-gui/Module.sip[m
[1m+++ b/library/talipot-python/bindings/talipot-gui/Module.sip[m
[36m@@ -165,7 +165,7 @@[m [mnamespace tlp {[m
 [m
 %Include Interactor.sip[m
 %Include View.sip[m
[31m-%Include GlMainView.sip[m
[32m+[m[32m%Include GlView.sip[m
 %Include GlGraphRenderingParameters.sip[m
 %Include GlGraphInputData.sip[m
 %Include NodeLinkDiagramView.sip[m
[1mdiff --git a/library/talipot-python/bindings/talipot-gui/NodeLinkDiagramView.sip b/library/talipot-python/bindings/talipot-gui/NodeLinkDiagramView.sip[m
[1mindex 8a355078c..0a368f28f 100644[m
[1m--- a/library/talipot-python/bindings/talipot-gui/NodeLinkDiagramView.sip[m
[1m+++ b/library/talipot-python/bindings/talipot-gui/NodeLinkDiagramView.sip[m
[36m@@ -16,7 +16,7 @@[m
 %End[m
 [m
 namespace tlp {[m
[31m-class NodeLinkDiagramView : tlp::GlMainView  /NoDefaultCtors/ {[m
[32m+[m[32mclass NodeLinkDiagramView : tlp::GlView  /NoDefaultCtors/ {[m
 %TypeHeaderCode[m
 #include <talipot/NodeLinkDiagramView.h>[m
 #include <talipot/GlGraph.h>[m
[36m@@ -24,7 +24,7 @@[m [mclass NodeLinkDiagramView : tlp::GlMainView  /NoDefaultCtors/ {[m
 [m
 %Docstring[m
 [m
[31m-Bases: :class:`tlpgui.GlMainView`[m
[32m+[m[32mBases: :class:`tlpgui.GlView`[m
 [m
 That class represents the main type of view offered by Talipot : a node link diagram.[m
 %End[m
[1mdiff --git a/library/talipot-python/bindings/talipot-gui/View.sip b/library/talipot-python/bindings/talipot-gui/View.sip[m
[1mindex 2a5785f29..ece7aefbb 100644[m
[1m--- a/library/talipot-python/bindings/talipot-gui/View.sip[m
[1m+++ b/library/talipot-python/bindings/talipot-gui/View.sip[m
[36m@@ -13,7 +13,7 @@[m
 [m
 %ModuleHeaderCode[m
 #include <talipot/NodeLinkDiagramView.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/View.h>[m
 #include <QPixmap>[m
 #include <QImage>[m
[36m@@ -32,8 +32,8 @@[m [mclass View  : tlp::Plugin /Abstract/ {[m
 %ConvertToSubClassCode[m
    if (dynamic_cast<tlp::NodeLinkDiagramView*>(sipCpp)) {[m
         sipType = sipFindType("tlp::NodeLinkDiagramView");[m
[31m-   } else if (dynamic_cast<tlp::GlMainView*>(sipCpp)) {[m
[31m-        sipType = sipFindType("tlp::GlMainView");[m
[32m+[m[32m   } else if (dynamic_cast<tlp::GlView*>(sipCpp)) {[m
[32m+[m[32m        sipType = sipFindType("tlp::GlView");[m
    } else {[m
         sipType = NULL;[m
    }[m
[1mdiff --git a/library/talipot-python/bindings/talipot-gui/ViewsUtils.cpp b/library/talipot-python/bindings/talipot-gui/ViewsUtils.cpp[m
[1mindex 500da8d2f..5f1152e7a 100644[m
[1m--- a/library/talipot-python/bindings/talipot-gui/ViewsUtils.cpp[m
[1m+++ b/library/talipot-python/bindings/talipot-gui/ViewsUtils.cpp[m
[36m@@ -17,7 +17,7 @@[m
 [m
 #include <talipot/PluginsManager.h>[m
 #include <talipot/WorkspacePanel.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlWidget.h>[m
 [m
 #include "ViewsUtils.h"[m
[36m@@ -250,7 +250,7 @@[m [mvoid ViewsManager::resizeView(tlp::View *view, int width, int height) {[m
 [m
   if (!workspace) {[m
     viewToWindow[view]->resize(width, height);[m
[31m-    tlp::GlMainView *glView = dynamic_cast<tlp::GlMainView *>(view);[m
[32m+[m[32m    tlp::GlView *glView = dynamic_cast<tlp::GlView *>(view);[m
     if (glView) {[m
       glView->getGlWidget()->resize(width, height);[m
       glView->getGlWidget()->resizeGL(width, height);[m
[1mdiff --git a/plugins/interactor/Fisheye/FisheyeInteractor.cpp b/plugins/interactor/Fisheye/FisheyeInteractor.cpp[m
[1mindex 4989e6ff6..93bc8c0ff 100644[m
[1m--- a/plugins/interactor/Fisheye/FisheyeInteractor.cpp[m
[1m+++ b/plugins/interactor/Fisheye/FisheyeInteractor.cpp[m
[36m@@ -24,7 +24,7 @@[m
 [m
 #include <talipot/PluginHeaders.h>[m
 #include <talipot/MouseInteractors.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlShaderProgram.h>[m
 #include <talipot/GlWidget.h>[m
 #include <talipot/NodeLinkDiagramView.h>[m
[36m@@ -141,7 +141,7 @@[m [mvoid FisheyeInteractorComponent::viewChanged(View *view) {[m
     return;[m
   }[m
 [m
[31m-  GlMainView *glView = static_cast<GlMainView *>(view);[m
[32m+[m[32m  GlView *glView = static_cast<GlView *>(view);[m
   GlWidget *glWidget = glView->getGlWidget();[m
 [m
   if (!glWidget->hasMouseTracking()) {[m
[1mdiff --git a/plugins/interactor/MouseLassoNodesSelector/MouseLassoNodesSelector.cpp b/plugins/interactor/MouseLassoNodesSelector/MouseLassoNodesSelector.cpp[m
[1mindex 26a5defd7..963441614 100644[m
[1m--- a/plugins/interactor/MouseLassoNodesSelector/MouseLassoNodesSelector.cpp[m
[1m+++ b/plugins/interactor/MouseLassoNodesSelector/MouseLassoNodesSelector.cpp[m
[36m@@ -21,7 +21,7 @@[m
 #include <talipot/MouseInteractors.h>[m
 #include <talipot/BooleanProperty.h>[m
 #include <talipot/GlComplexPolygon.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlGraph.h>[m
 #include <talipot/GlNode.h>[m
 #include <talipot/NodeLinkDiagramView.h>[m
[1mdiff --git a/plugins/interactor/MouseMagnifyingGlass/MouseMagnifyingGlass.cpp b/plugins/interactor/MouseMagnifyingGlass/MouseMagnifyingGlass.cpp[m
[1mindex 5cee6c35a..ee9e819dd 100644[m
[1m--- a/plugins/interactor/MouseMagnifyingGlass/MouseMagnifyingGlass.cpp[m
[1m+++ b/plugins/interactor/MouseMagnifyingGlass/MouseMagnifyingGlass.cpp[m
[36m@@ -18,7 +18,7 @@[m
 #include "../../utils/InteractorIcons.h"[m
 [m
 #include <talipot/GlRect.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlTextureManager.h>[m
 #include <talipot/MouseInteractors.h>[m
 #include <talipot/GlTools.h>[m
[36m@@ -282,7 +282,7 @@[m [mvoid MouseMagnifyingGlassInteractorComponent::viewChanged(View *view) {[m
     return;[m
   }[m
 [m
[31m-  GlMainView *glView = dynamic_cast<GlMainView *>(view);[m
[32m+[m[32m  GlView *glView = dynamic_cast<GlView *>(view);[m
   glWidget = glView->getGlWidget();[m
   radius = glWidget->screenToViewport(glWidget->width()) / 4;[m
   camera = &glWidget->getScene()->getLayer("Main")->getCamera();[m
[1mdiff --git a/plugins/interactor/NeighborhoodHighlighter/NeighborhoodHighlighterInteractor.cpp b/plugins/interactor/NeighborhoodHighlighter/NeighborhoodHighlighterInteractor.cpp[m
[1mindex cb8216df6..ace6b0329 100644[m
[1m--- a/plugins/interactor/NeighborhoodHighlighter/NeighborhoodHighlighterInteractor.cpp[m
[1m+++ b/plugins/interactor/NeighborhoodHighlighter/NeighborhoodHighlighterInteractor.cpp[m
[36m@@ -21,7 +21,7 @@[m
 #include <talipot/PluginHeaders.h>[m
 #include <talipot/MouseInteractors.h>[m
 #include <talipot/GlWidget.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlCircle.h>[m
 #include <talipot/OcclusionTest.h>[m
 #include <talipot/GlBoundingBoxSceneVisitor.h>[m
[36m@@ -176,7 +176,7 @@[m [mvoid NeighborhoodHighlighter::viewChanged(View *view) {[m
     return;[m
   }[m
 [m
[31m-  GlMainView *glView = static_cast<GlMainView *>(view);[m
[32m+[m[32m  GlView *glView = static_cast<GlView *>(view);[m
   glWidget = glView->getGlWidget();[m
 }[m
 [m
[1mdiff --git a/plugins/interactor/PathFinder/PathFinderComponent.cpp b/plugins/interactor/PathFinder/PathFinderComponent.cpp[m
[1mindex ae7837ffe..416bf9d47 100644[m
[1m--- a/plugins/interactor/PathFinder/PathFinderComponent.cpp[m
[1m+++ b/plugins/interactor/PathFinder/PathFinderComponent.cpp[m
[36m@@ -15,7 +15,7 @@[m
 [m
 #include <talipot/GlWidget.h>[m
 #include <talipot/GlGraph.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 [m
 #include "highlighters/PathHighlighter.h"[m
 #include "PathFinder.h"[m
[36m@@ -211,6 +211,6 @@[m [mQSet<PathHighlighter *> PathFinderComponent::getHighlighters() {[m
 }[m
 [m
 void PathFinderComponent::clear() {[m
[31m-  GlMainView *glMainView = static_cast<GlMainView *>(view());[m
[31m-  glMainView->getGlWidget()->setCursor(QCursor());[m
[32m+[m[32m  GlView *glView = static_cast<GlView *>(view());[m
[32m+[m[32m  glView->getGlWidget()->setCursor(QCursor());[m
 }[m
[1mdiff --git a/plugins/interactor/PathFinder/PathFinderComponent.h b/plugins/interactor/PathFinder/PathFinderComponent.h[m
[1mindex 6e1265c8c..6ebb72d5c 100644[m
[1m--- a/plugins/interactor/PathFinder/PathFinderComponent.h[m
[1m+++ b/plugins/interactor/PathFinder/PathFinderComponent.h[m
[36m@@ -21,7 +21,7 @@[m
 [m
 namespace tlp {[m
 class Graph;[m
[31m-class GlMainView;[m
[32m+[m[32mclass GlView;[m
 class BooleanProperty;[m
 class DoubleProperty;[m
 [m
[1mdiff --git a/plugins/interactor/PathFinder/highlighters/EnclosingCircleHighlighter.cpp b/plugins/interactor/PathFinder/highlighters/EnclosingCircleHighlighter.cpp[m
[1mindex 5d38974eb..5130dd8dc 100644[m
[1m--- a/plugins/interactor/PathFinder/highlighters/EnclosingCircleHighlighter.cpp[m
[1m+++ b/plugins/interactor/PathFinder/highlighters/EnclosingCircleHighlighter.cpp[m
[36m@@ -14,7 +14,7 @@[m
 #include <talipot/GlScene.h>[m
 #include <talipot/Circle.h>[m
 #include <talipot/GlCircle.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlGraphInputData.h>[m
 #include <talipot/GlWidget.h>[m
 #include <talipot/ColorButton.h>[m
[1mdiff --git a/plugins/interactor/PathFinder/highlighters/PathHighlighter.cpp b/plugins/interactor/PathFinder/highlighters/PathHighlighter.cpp[m
[1mindex 9faa63b09..c6cb74ed0 100644[m
[1m--- a/plugins/interactor/PathFinder/highlighters/PathHighlighter.cpp[m
[1m+++ b/plugins/interactor/PathFinder/highlighters/PathHighlighter.cpp[m
[36m@@ -14,7 +14,7 @@[m
 #include "PathHighlighter.h"[m
 [m
 #include <talipot/GlWidget.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlGraph.h>[m
 [m
 using namespace tlp;[m
[1mdiff --git a/plugins/interactor/PathFinder/highlighters/ZoomAndPanHighlighter.cpp b/plugins/interactor/PathFinder/highlighters/ZoomAndPanHighlighter.cpp[m
[1mindex 33afb660a..3d6e3dc19 100644[m
[1m--- a/plugins/interactor/PathFinder/highlighters/ZoomAndPanHighlighter.cpp[m
[1m+++ b/plugins/interactor/PathFinder/highlighters/ZoomAndPanHighlighter.cpp[m
[36m@@ -14,7 +14,7 @@[m
 #include "ZoomAndPanHighlighter.h"[m
 [m
 #include <talipot/Graph.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlGraphInputData.h>[m
 #include <talipot/LayoutProperty.h>[m
 #include <talipot/Coord.h>[m
[1mdiff --git a/plugins/view/GeographicView/GeographicView.h b/plugins/view/GeographicView/GeographicView.h[m
[1mindex edad41193..c8f97097c 100644[m
[1m--- a/plugins/view/GeographicView/GeographicView.h[m
[1m+++ b/plugins/view/GeographicView/GeographicView.h[m
[36m@@ -19,7 +19,7 @@[m
 #include <talipot/SceneLayersConfigWidget.h>[m
 #include <talipot/Interactor.h>[m
 #include <talipot/View.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/ViewActionsManager.h>[m
 [m
 #include <QGraphicsScene>[m
[36m@@ -121,7 +121,7 @@[m [mpublic:[m
   }[m
 [m
   bool getNodeOrEdgeAtViewportPos(int x, int y, node &n, edge &e) const override {[m
[31m-    return GlMainView::getNodeOrEdgeAtViewportPos(geoViewGraphicsView->getGlWidget(), x, y, n, e);[m
[32m+[m[32m    return GlView::getNodeOrEdgeAtViewportPos(geoViewGraphicsView->getGlWidget(), x, y, n, e);[m
   }[m
 [m
   GeographicViewConfigWidget *getConfigWidget() const {[m
[1mdiff --git a/plugins/view/HistogramView/HistogramView.cpp b/plugins/view/HistogramView/HistogramView.cpp[m
[1mindex 7159b51d5..92b13d52e 100644[m
[1m--- a/plugins/view/HistogramView/HistogramView.cpp[m
[1m+++ b/plugins/view/HistogramView/HistogramView.cpp[m
[36m@@ -52,7 +52,7 @@[m [mnamespace tlp {[m
 PLUGIN(HistogramView)[m
 [m
 HistogramView::HistogramView(const PluginContext *)[m
[31m-    : GlMainView(true), propertiesSelectionWidget(nullptr), histoOptionsWidget(nullptr),[m
[32m+[m[32m    : GlView(true), propertiesSelectionWidget(nullptr), histoOptionsWidget(nullptr),[m
       xAxisDetail(nullptr), yAxisDetail(nullptr), _histoGraph(nullptr), emptyGraph(nullptr),[m
       emptyGlGraph(nullptr), histogramsComposite(nullptr), labelsComposite(nullptr),[m
       axisComposite(nullptr), smallMultiplesView(true), mainLayer(nullptr),[m
[36m@@ -161,7 +161,7 @@[m [mvoid HistogramView::setState(const DataSet &dataSet) {[m
     histoOptionsWidget->setWidgetEnabled(false);[m
   }[m
 [m
[31m-  GlMainView::setState(dataSet);[m
[32m+[m[32m  GlView::setState(dataSet);[m
 [m
   Graph *lastGraph = _histoGraph;[m
   _histoGraph = graph();[m
[36m@@ -358,7 +358,7 @@[m [mDataSet HistogramView::state() const {[m
   vector<string> selectedPropertiesTmp = vector<string>(selectedProperties);[m
   map<string, Histogram *> histogramsMapTmp = map<string, Histogram *>(histogramsMap);[m
 [m
[31m-  DataSet dataSet = GlMainView::state();[m
[32m+[m[32m  DataSet dataSet = GlView::state();[m
   dataSet.set("Nodes/Edges", static_cast<unsigned>(dataLocation));[m
 [m
   for (size_t i = 0; i < selectedPropertiesTmp.size(); ++i) {[m
[36m@@ -443,7 +443,7 @@[m [mbool HistogramView::eventFilter(QObject *object, QEvent *event) {[m
     return true;[m
   }[m
 [m
[31m-  return GlMainView::eventFilter(object, event);[m
[32m+[m[32m  return GlView::eventFilter(object, event);[m
 }[m
 [m
 void HistogramView::addEmptyViewLabel() {[m
[1mdiff --git a/plugins/view/HistogramView/HistogramView.h b/plugins/view/HistogramView/HistogramView.h[m
[1mindex 863f24761..802fe09eb 100644[m
[1m--- a/plugins/view/HistogramView/HistogramView.h[m
[1m+++ b/plugins/view/HistogramView/HistogramView.h[m
[36m@@ -14,7 +14,7 @@[m
 #ifndef HISTOGRAM_VIEW_H[m
 #define HISTOGRAM_VIEW_H[m
 [m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 [m
 #include <vector>[m
 #include <unordered_map>[m
[36m@@ -53,7 +53,7 @@[m [mclass QuickAccessBarImpl;[m
  *[m
  *[m
  */[m
[31m-class HistogramView : public GlMainView {[m
[32m+[m[32mclass HistogramView : public GlView {[m
 [m
   Q_OBJECT[m
 [m
[36m@@ -148,25 +148,19 @@[m [mprivate:[m
 [m
   void setLayoutUpdateNeeded() {[m
     if (detailedHistogram) {[m
[31m-      {[m
[31m-        detailedHistogram->setLayoutUpdateNeeded();[m
[31m-      }[m
[32m+[m[32m      { detailedHistogram->setLayoutUpdateNeeded(); }[m
     }[m
   }[m
 [m
   void setSizesUpdateNeeded() {[m
     if (detailedHistogram) {[m
[31m-      {[m
[31m-        detailedHistogram->setSizesUpdateNeeded();[m
[31m-      }[m
[32m+[m[32m      { detailedHistogram->setSizesUpdateNeeded(); }[m
     }[m
   }[m
 [m
   void setUpdateNeeded() {[m
     if (detailedHistogram) {[m
[31m-      {[m
[31m-        detailedHistogram->setUpdateNeeded();[m
[31m-      }[m
[32m+[m[32m      { detailedHistogram->setUpdateNeeded(); }[m
     }[m
   }[m
 [m
[1mdiff --git a/plugins/view/MatrixView/MatrixView.cpp b/plugins/view/MatrixView/MatrixView.cpp[m
[1mindex d79f39031..f61693638 100644[m
[1m--- a/plugins/view/MatrixView/MatrixView.cpp[m
[1m+++ b/plugins/view/MatrixView/MatrixView.cpp[m
[36m@@ -190,7 +190,7 @@[m [mQList<QWidget *> MatrixView::configurationWidgets() const {[m
 }[m
 [m
 void MatrixView::fillContextMenu(QMenu *menu, const QPointF &point) {[m
[31m-  GlMainView::fillContextMenu(menu, point);[m
[32m+[m[32m  GlView::fillContextMenu(menu, point);[m
   // Check if a node/edge is under the mouse pointer[m
   SelectedEntity entity;[m
 [m
[1mdiff --git a/plugins/view/ParallelCoordinatesView/include/ParallelCoordinatesView.h b/plugins/view/ParallelCoordinatesView/include/ParallelCoordinatesView.h[m
[1mindex e74063d5c..af397a0ca 100644[m
[1m--- a/plugins/view/ParallelCoordinatesView/include/ParallelCoordinatesView.h[m
[1m+++ b/plugins/view/ParallelCoordinatesView/include/ParallelCoordinatesView.h[m
[36m@@ -21,7 +21,7 @@[m
 [m
 #include "../../utils/PluginNames.h"[m
 [m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/Graph.h>[m
 [m
 class QMenu;[m
[36m@@ -63,7 +63,7 @@[m [mclass QuantitativeParallelAxis;[m
  *[m
  *[m
  */[m
[31m-class ParallelCoordinatesView : public GlMainView {[m
[32m+[m[32mclass ParallelCoordinatesView : public GlView {[m
 [m
   Q_OBJECT[m
 [m
[1mdiff --git a/plugins/view/ParallelCoordinatesView/src/ParallelCoordinatesView.cpp b/plugins/view/ParallelCoordinatesView/src/ParallelCoordinatesView.cpp[m
[1mindex 410b1249b..94ce14724 100644[m
[1m--- a/plugins/view/ParallelCoordinatesView/src/ParallelCoordinatesView.cpp[m
[1m+++ b/plugins/view/ParallelCoordinatesView/src/ParallelCoordinatesView.cpp[m
[36m@@ -60,7 +60,7 @@[m [mstatic void toggleGraphView(GlGraph *glGraph, bool displayNodes) {[m
 PLUGIN(ParallelCoordinatesView)[m
 [m
 ParallelCoordinatesView::ParallelCoordinatesView(const PluginContext *)[m
[31m-    : GlMainView(true), viewSetupMenu(nullptr), classicLayout(nullptr), circularLayout(nullptr),[m
[32m+[m[32m    : GlView(true), viewSetupMenu(nullptr), classicLayout(nullptr), circularLayout(nullptr),[m
       straightLinesType(nullptr), catmullRomSplineLinesType(nullptr),[m
       cubicBSplineInterpolationLinesType(nullptr), thickLines(nullptr), thinLines(nullptr),[m
       addRemoveDataFromSelection(nullptr), selectData(nullptr), deleteData(nullptr),[m
[36m@@ -153,7 +153,7 @@[m [mvoid ParallelCoordinatesView::setState(const DataSet &dataSet) {[m
     isConstruct = true;[m
   }[m
 [m
[31m-  GlMainView::setState(dataSet);[m
[32m+[m[32m  GlView::setState(dataSet);[m
 [m
   removeTriggers();[m
 [m
[36m@@ -336,7 +336,7 @@[m [mvoid ParallelCoordinatesView::setState(const DataSet &dataSet) {[m
 [m
 DataSet ParallelCoordinatesView::state() const {[m
 [m
[31m-  DataSet dataSet = GlMainView::state();[m
[32m+[m[32m  DataSet dataSet = GlView::state();[m
 [m
   string sceneOut;[m
   getGlWidget()->getScene()->getXMLOnlyForCameras(sceneOut);[m
[36m@@ -531,7 +531,7 @@[m [mbool ParallelCoordinatesView::eventFilter(QObject *obj, QEvent *event) {[m
     Observable::unholdObservers();[m
   }[m
 [m
[31m-  return GlMainView::eventFilter(obj, event);[m
[32m+[m[32m  return GlView::eventFilter(obj, event);[m
 }[m
 [m
 bool ParallelCoordinatesView::getNodeOrEdgeAtViewportPos(int x, int y, node &n, edge &e) const {[m
[36m@@ -640,7 +640,7 @@[m [mvoid ParallelCoordinatesView::buildContextMenu() {[m
 }[m
 [m
 void ParallelCoordinatesView::fillContextMenu(QMenu *menu, const QPointF &point) {[m
[31m-  GlMainView::fillContextMenu(menu, point);[m
[32m+[m[32m  GlView::fillContextMenu(menu, point);[m
   menu->addAction(viewSetupMenu->menuAction());[m
 [m
   axisUnderPointer = getAxisUnderPointer(point.x(), point.y());[m
[1mdiff --git a/plugins/view/PixelOrientedView/PixelOrientedView.cpp b/plugins/view/PixelOrientedView/PixelOrientedView.cpp[m
[1mindex e36e425ee..d837c37d3 100644[m
[1m--- a/plugins/view/PixelOrientedView/PixelOrientedView.cpp[m
[1m+++ b/plugins/view/PixelOrientedView/PixelOrientedView.cpp[m
[36m@@ -65,7 +65,7 @@[m [mnamespace tlp {[m
 PLUGIN(PixelOrientedView)[m
 [m
 PixelOrientedView::PixelOrientedView(const PluginContext *)[m
[31m-    : GlMainView(true), pixelOrientedGraph(nullptr), glGraph(nullptr), mainLayer(nullptr),[m
[32m+[m[32m    : GlView(true), pixelOrientedGraph(nullptr), glGraph(nullptr), mainLayer(nullptr),[m
       overviewsComposite(nullptr), optionsWidget(nullptr), propertiesSelectionWidget(nullptr),[m
       pixelOrientedMediator(nullptr), lastNbNodes(0), overviewWidth(0), overviewHeight(0),[m
       minWidth(0), refSize(0), hilbertLayout(nullptr), squareLayout(nullptr),[m
[36m@@ -148,7 +148,7 @@[m [mvoid PixelOrientedView::setState(const DataSet &dataSet) {[m
     setOverviewVisible(true);[m
   }[m
 [m
[31m-  GlMainView::setState(dataSet);[m
[32m+[m[32m  GlView::setState(dataSet);[m
 [m
   Graph *lastGraph = this->pixelOrientedGraph;[m
 [m
[36m@@ -282,7 +282,7 @@[m [mvoid PixelOrientedView::setState(const DataSet &dataSet) {[m
 }[m
 [m
 DataSet PixelOrientedView::state() const {[m
[31m-  DataSet dataSet = GlMainView::state();[m
[32m+[m[32m  DataSet dataSet = GlView::state();[m
   DataSet selectedGraphPropertiesDataSet;[m
 [m
   for (size_t i = 0; i < selectedGraphProperties.size(); ++i) {[m
[1mdiff --git a/plugins/view/PixelOrientedView/PixelOrientedView.h b/plugins/view/PixelOrientedView/PixelOrientedView.h[m
[1mindex ff762bcdc..d6668b739 100644[m
[1m--- a/plugins/view/PixelOrientedView/PixelOrientedView.h[m
[1m+++ b/plugins/view/PixelOrientedView/PixelOrientedView.h[m
[36m@@ -14,7 +14,7 @@[m
 #ifndef PIXEL_ORIENTED_VIEW_H[m
 #define PIXEL_ORIENTED_VIEW_H[m
 [m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 [m
 #include <QWidget>[m
 [m
[36m@@ -55,7 +55,7 @@[m [mclass PixelOrientedViewQuickAccessBar;[m
  *[m
  *[m
  */[m
[31m-class PixelOrientedView : public GlMainView {[m
[32m+[m[32mclass PixelOrientedView : public GlView {[m
 [m
   static const unsigned int MIN_IMAGE_WIDTH = 512;[m
   static const unsigned int MIN_IMAGE_HEIGHT = 512;[m
[36m@@ -112,7 +112,7 @@[m [mpublic slots:[m
   void graphChanged(Graph *graph) override;[m
   void init();[m
   void applySettings() override;[m
[31m-  // inherited from GlMainView[m
[32m+[m[32m  // inherited from GlView[m
   void centerView(bool = false) override;[m
 [m
 private:[m
[1mdiff --git a/plugins/view/SOMView/SOMView.cpp b/plugins/view/SOMView/SOMView.cpp[m
[1mindex 1a38bdf9d..e23d5146d 100644[m
[1m--- a/plugins/view/SOMView/SOMView.cpp[m
[1m+++ b/plugins/view/SOMView/SOMView.cpp[m
[36m@@ -46,7 +46,7 @@[m [musing namespace tlp;[m
 PLUGIN(SOMView)[m
 [m
 SOMView::SOMView(PluginContext *)[m
[31m-    : GlMainView(true), glGraph(nullptr), graphLayoutProperty(nullptr), graphSizeProperty(nullptr),[m
[32m+[m[32m    : GlView(true), glGraph(nullptr), graphLayoutProperty(nullptr), graphSizeProperty(nullptr),[m
       mask(nullptr), somMask(nullptr), mapCompositeElements(nullptr), som(nullptr),[m
       previewWidget(nullptr), mapWidget(nullptr), isDetailledMode(false), mappingIsVisible(false),[m
       hideMappingAction(nullptr), showMappingAction(nullptr), computeMappingAction(nullptr),[m
[36m@@ -109,7 +109,7 @@[m [mvoid SOMView::construct(QWidget *) {[m
   mapWidget = new GlWidget(nullptr, this);[m
   mapWidget->installEventFilter(this);[m
 [m
[31m-  initGlMainViews();[m
[32m+[m[32m  initGlViews();[m
   mapWidget->installEventFilter(this);[m
 [m
   isDetailledMode = false;[m
[36m@@ -131,7 +131,7 @@[m [mvoid SOMView::construct(QWidget *) {[m
   som = nullptr;[m
 }[m
 [m
[31m-void SOMView::initGlMainViews() {[m
[32m+[m[32mvoid SOMView::initGlViews() {[m
 [m
   // Gl view initialisation[m
   // miniatures[m
[36m@@ -243,7 +243,7 @@[m [mvoid SOMView::setState(const DataSet &dataSet) {[m
 [m
   registerTriggers();[m
 [m
[31m-  GlMainView::setState(dataSet);[m
[32m+[m[32m  GlView::setState(dataSet);[m
 }[m
 [m
 void SOMView::changeMapViewGraph(tlp::Graph *graph) {[m
[36m@@ -280,7 +280,7 @@[m [mvoid SOMView::changeMapViewGraph(tlp::Graph *graph) {[m
 }[m
 [m
 DataSet SOMView::state() const {[m
[31m-  DataSet dataSet = GlMainView::state();[m
[32m+[m[32m  DataSet dataSet = GlView::state();[m
   // Store configurationWidget state.[m
   dataSet.set("propertiesWidget", properties->getData());[m
   return dataSet;[m
[36m@@ -313,7 +313,7 @@[m [mvoid SOMView::fillContextMenu(QMenu *menu, const QPointF &point) {[m
   }[m
 [m
   menu->addSeparator();[m
[31m-  GlMainView::fillContextMenu(menu, point);[m
[32m+[m[32m  GlView::fillContextMenu(menu, point);[m
 }[m
 [m
 void SOMView::createPicture(const std::string &pictureName, int width, int height) {[m
[36m@@ -859,7 +859,7 @@[m [mbool SOMView::eventFilter(QObject *obj, QEvent *event) {[m
     }[m
   }[m
 [m
[31m-  return GlMainView::eventFilter(obj, event);[m
[32m+[m[32m  return GlView::eventFilter(obj, event);[m
 }[m
 [m
 void SOMView::showMapping() {[m
[1mdiff --git a/plugins/view/SOMView/SOMView.h b/plugins/view/SOMView/SOMView.h[m
[1mindex 302876c49..1d6e92249 100644[m
[1m--- a/plugins/view/SOMView/SOMView.h[m
[1m+++ b/plugins/view/SOMView/SOMView.h[m
[36m@@ -14,7 +14,7 @@[m
 #ifndef SOM_VIEW_H[m
 #define SOM_VIEW_H[m
 [m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/MouseInteractors.h>[m
 [m
 #include <SOMAlgorithm.h>[m
[36m@@ -49,7 +49,7 @@[m [mclass SOMPropertiesWidget;[m
  * Training builds the map using input examples.[m
  * Mapping automatically classifies data on the map generated by training.[m
  */[m
[31m-class SOMView : public GlMainView {[m
[32m+[m[32mclass SOMView : public GlView {[m
   Q_OBJECT[m
 [m
   PLUGININFORMATION([m
[36m@@ -231,7 +231,7 @@[m [mprivate:[m
    * Init the GlWidget object. At this time only init the previewWidget as the mapWidget is[m
    * initialized when a new graph is given in the function changeMapViewGraph.[m
    */[m
[31m-  void initGlMainViews();[m
[32m+[m[32m  void initGlViews();[m
   /**[m
    * Init the menu.[m
    */[m
[1mdiff --git a/plugins/view/ScatterPlot2DView/ScatterPlot2DView.cpp b/plugins/view/ScatterPlot2DView/ScatterPlot2DView.cpp[m
[1mindex eb707181c..8c2cb9c9a 100644[m
[1m--- a/plugins/view/ScatterPlot2DView/ScatterPlot2DView.cpp[m
[1m+++ b/plugins/view/ScatterPlot2DView/ScatterPlot2DView.cpp[m
[36m@@ -63,7 +63,7 @@[m [mconst unsigned int nbPropertiesTypes = sizeof(propertiesTypes) / sizeof(string);[m
 const vector<string> propertiesTypesFilter(propertiesTypes, propertiesTypes + nbPropertiesTypes);[m
 [m
 ScatterPlot2DView::ScatterPlot2DView(const PluginContext *)[m
[31m-    : GlMainView(true), propertiesSelectionWidget(nullptr), optionsWidget(nullptr),[m
[32m+[m[32m    : GlView(true), propertiesSelectionWidget(nullptr), optionsWidget(nullptr),[m
       scatterPlotGraph(nullptr), emptyGraph(nullptr), mainLayer(nullptr), glGraph(nullptr),[m
       scatterPlotSize(nullptr), matrixComposite(nullptr), axisComposite(nullptr),[m
       labelsComposite(nullptr), detailedScatterPlot(nullptr),[m
[36m@@ -333,11 +333,11 @@[m [mvoid ScatterPlot2DView::setState(const DataSet &dataSet) {[m
     setQuickAccessBarVisible(true);[m
   }[m
 [m
[31m-  GlMainView::setState(dataSet);[m
[32m+[m[32m  GlView::setState(dataSet);[m
 }[m
 [m
 DataSet ScatterPlot2DView::state() const {[m
[31m-  DataSet dataSet = GlMainView::state();[m
[32m+[m[32m  DataSet dataSet = GlView::state();[m
   DataSet selectedGraphPropertiesDataSet;[m
 [m
   for (size_t i = 0; i < selectedGraphProperties.size(); ++i) {[m
[1mdiff --git a/plugins/view/ScatterPlot2DView/ScatterPlot2DView.h b/plugins/view/ScatterPlot2DView/ScatterPlot2DView.h[m
[1mindex 214f876b1..8e467eaee 100644[m
[1m--- a/plugins/view/ScatterPlot2DView/ScatterPlot2DView.h[m
[1m+++ b/plugins/view/ScatterPlot2DView/ScatterPlot2DView.h[m
[36m@@ -17,7 +17,7 @@[m
 #include <unordered_map>[m
 [m
 #include <talipot/Graph.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/BoundingBox.h>[m
 #include <talipot/Coord.h>[m
 [m
[36m@@ -59,7 +59,7 @@[m [mclass ScatterPlot2DOptionsWidget;[m
 class ViewGraphPropertiesSelectionWidget;[m
 class ScatterPlotQuickAccessBar;[m
 [m
[31m-class ScatterPlot2DView : public GlMainView {[m
[32m+[m[32mclass ScatterPlot2DView : public GlView {[m
 [m
   Q_OBJECT[m
 [m
[36m@@ -147,7 +147,7 @@[m [mpublic slots:[m
   void init();[m
   void viewConfigurationChanged();[m
 [m
[31m-  // inherited from GlMainView[m
[32m+[m[32m  // inherited from GlView[m
   void centerView(bool graphChanged = false) override;[m
   void applySettings() override;[m
 [m
[1mdiff --git a/software/talipot/include/TalipotMainWindow.h b/software/talipot/include/TalipotMainWindow.h[m
[1mindex 52d37f5aa..ec819c8b9 100644[m
[1m--- a/software/talipot/include/TalipotMainWindow.h[m
[1m+++ b/software/talipot/include/TalipotMainWindow.h[m
[36m@@ -125,14 +125,13 @@[m [mpublic slots:[m
   void showPythonDocumentation();[m
 [m
   void redrawPanels(bool center = false);[m
[31m-  void centerPanelsForGraph(tlp::Graph *, bool graphChanged, bool onlyGlMainView);[m
[32m+[m[32m  void centerPanelsForGraph(tlp::Graph *, bool graphChanged, bool onlyGlView);[m
   void centerPanelsForGraph(tlp::Graph *g) {[m
     centerPanelsForGraph(g, false, false);[m
   }[m
   void closePanelsForGraph(tlp::Graph *g = nullptr);[m
[31m-  bool[m
[31m-  setGlMainViewPropertiesForGraph(tlp::Graph *g,[m
[31m-                                  const std::map<std::string, tlp::PropertyInterface *> &propsMap);[m
[32m+[m[32m  bool setGlViewPropertiesForGraph(tlp::Graph *g,[m
[32m+[m[32m                                   const std::map<std::string, tlp::PropertyInterface *> &propsMap);[m
   void openPreferences();[m
 [m
   void setAutoCenterPanelsOnDraw(bool f);[m
[1mdiff --git a/software/talipot/src/AlgorithmRunnerItem.cpp b/software/talipot/src/AlgorithmRunnerItem.cpp[m
[1mindex 55ab6e1aa..9f52caea7 100644[m
[1m--- a/software/talipot/src/AlgorithmRunnerItem.cpp[m
[1m+++ b/software/talipot/src/AlgorithmRunnerItem.cpp[m
[36m@@ -210,7 +210,7 @@[m [mpublic:[m
       }[m
 [m
       // restore initial properties[m
[31m-      TalipotMainWindow::instance().setGlMainViewPropertiesForGraph(graph, outPropsMap);[m
[32m+[m[32m      TalipotMainWindow::instance().setGlViewPropertiesForGraph(graph, outPropsMap);[m
     }[m
   }[m
 [m
[36m@@ -229,7 +229,7 @@[m [mpublic:[m
 [m
       if (!outPropsMap.empty() &&[m
           // set temporary properties as drawing properties[m
[31m-          (TalipotMainWindow::instance().setGlMainViewPropertiesForGraph(graph, outPropsMap) ==[m
[32m+[m[32m          (TalipotMainWindow::instance().setGlViewPropertiesForGraph(graph, outPropsMap) ==[m
            false)) {[m
         // clear map if there is nothing to do[m
         outPropsMap.clear();[m
[1mdiff --git a/software/talipot/src/TalipotMainWindow.cpp b/software/talipot/src/TalipotMainWindow.cpp[m
[1mindex d0285829c..c9a55ea8a 100644[m
[1m--- a/software/talipot/src/TalipotMainWindow.cpp[m
[1m+++ b/software/talipot/src/TalipotMainWindow.cpp[m
[36m@@ -48,7 +48,7 @@[m
 #include <talipot/GraphModel.h>[m
 #include <talipot/GraphTableItemDelegate.h>[m
 #include <talipot/GraphPropertiesModel.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlWidget.h>[m
 #include <talipot/GlGraph.h>[m
 #include <talipot/Settings.h>[m
[36m@@ -1490,10 +1490,9 @@[m [mvoid TalipotMainWindow::applyDefaultLayout(Graph *g) {[m
   Observable::unholdObservers();[m
 }[m
 [m
[31m-void TalipotMainWindow::centerPanelsForGraph(tlp::Graph *g, bool graphChanged,[m
[31m-                                             bool onlyGlMainView) {[m
[32m+[m[32mvoid TalipotMainWindow::centerPanelsForGraph(tlp::Graph *g, bool graphChanged, bool onlyGlView) {[m
   for (auto v : _ui->workspace->panels()) {[m
[31m-    if ((v->graph() == g) && (!onlyGlMainView || dynamic_cast<tlp::GlMainView *>(v)))[m
[32m+[m[32m    if ((v->graph() == g) && (!onlyGlView || dynamic_cast<tlp::GlView *>(v)))[m
       v->centerView(graphChanged);[m
   }[m
 }[m
[36m@@ -1516,15 +1515,15 @@[m [mvoid TalipotMainWindow::closePanelsForGraph(tlp::Graph *g) {[m
   }[m
 }[m
 [m
[31m-bool TalipotMainWindow::setGlMainViewPropertiesForGraph([m
[32m+[m[32mbool TalipotMainWindow::setGlViewPropertiesForGraph([m
     tlp::Graph *g, const std::map<std::string, tlp::PropertyInterface *> &propsMap) {[m
   bool result = false;[m
 [m
   for (auto v : _ui->workspace->panels()) {[m
[31m-    GlMainView *glMainView = dynamic_cast<tlp::GlMainView *>(v);[m
[32m+[m[32m    GlView *glView = dynamic_cast<tlp::GlView *>(v);[m
 [m
[31m-    if (v->graph() == g && glMainView != nullptr) {[m
[31m-      if (glMainView->getGlWidget()->getGlGraphInputData()->installProperties(propsMap))[m
[32m+[m[32m    if (v->graph() == g && glView != nullptr) {[m
[32m+[m[32m      if (glView->getGlWidget()->getGlGraphInputData()->installProperties(propsMap))[m
         result = true;[m
     }[m
   }[m
[1mdiff --git a/software/talipot/src/main.cpp b/software/talipot/src/main.cpp[m
[1mindex f3ded97a5..60eff7517 100644[m
[1m--- a/software/talipot/src/main.cpp[m
[1m+++ b/software/talipot/src/main.cpp[m
[36m@@ -40,7 +40,7 @@[m
 #include <talipot/Settings.h>[m
 #include <talipot/WorkspacePanel.h>[m
 #include <talipot/View.h>[m
[31m-#include <talipot/GlMainView.h>[m
[32m+[m[32m#include <talipot/GlView.h>[m
 #include <talipot/GlWidget.h>[m
 #include <talipot/PythonInterpreter.h>[m
 #include <talipot/GlOffscreenRenderer.h>[m
