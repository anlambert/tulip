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

#ifndef PARALLEL_COORDINATES_VIEW_H
#define PARALLEL_COORDINATES_VIEW_H

#include <talipot/OpenGlIncludes.h>

#include "ParallelCoordinatesDrawing.h"
#include "ParallelCoordinatesGraphProxy.h"

#include "../../utils/PluginNames.h"

#include <talipot/GlView.h>
#include <talipot/Graph.h>

class QMenu;

namespace tlp {

struct SelectedEntity;
class GlGraph;
class ViewGraphPropertiesSelectionWidget;
class ParallelCoordinatesViewQuickAccessBar;
class ParallelCoordsDrawConfigWidget;
class QuantitativeParallelAxis;

/** \file
 *  \brief Parallel Coordinates view

 * This view plugin allows to create a parallel coordinates visualization
 * from graph elements.
 *
 * Parallel coordinates is a common way of visualizing high-dimensional geometry and analyzing
 multivariate data.
 * The idea is to visualize a set of points in an n-dimensional space. To do so, the n dimensions
 are represented
 * by n parallel axis, typically vertical and equally spaced. A point in n-dimensional space is
 represented as a
 * polyline with vertices on the parallel axis. The position of the vertex on the i-th axis
 corresponds to the
 * i-th coordinate of the point.
 *
 * By selecting a set of graph properties (supported types are Metric, Integer and String), nodes
 and edges
 * of a graph can be visualized as parallel coordinates. Each axis corresponds to a graph property
 and nodes
 * (or edges) of the graph are represented as polylines or curves.
 *
 * A set of interactors are bundled with the view to perform brushing, elements selection, data
 filtering, ...
 *
 *
 */
class ParallelCoordinatesView : public GlView {

  Q_OBJECT

public:
  PLUGININFORMATION(ViewName::ParallelCoordinatesViewName, "Antoine Lambert", "16/04/2008",
                    "<p>The Parallel Coordinates view allows to create a parallel coordinates "
                    "visualization from graph elements. "
                    "Parallel coordinates are a common way of visualizing high-dimensional "
                    "geometry and analyzing multivariate data.\n"
                    "The basic idea is to visualize a set of points in an n-dimensional space. To "
                    "do so, the n dimensions are represented by n parallel axis, typically "
                    "vertical and equally spaced. A point in n-dimensional space is represented as "
                    "a polyline with vertices on the parallel axis. The position of the vertex on "
                    "the i-th axis corresponds to the i-th coordinate of the point.</p>"
                    "<p>By selecting a set of graph properties (supported types are Double, "
                    "Integer and String), nodes and edges of a graph can be visualized as parallel "
                    "coordinates. Each axis corresponds to a graph property and nodes (or edges) "
                    "of the graph are represented as polylines or curves.</p>"
                    "<p>A set of interactors are bundled with the view to perform brushing, "
                    "elements selection, data filtering, ...</p>",
                    "1.2", "View")

  ParallelCoordinatesView(const tlp::PluginContext *);
  ~ParallelCoordinatesView() override;
  std::string icon() const override {
    return ":/parallel_coordinates_view.png";
  }

  QuickAccessBar *getQuickAccessBarImpl() override;

  // View methods
  void setState(const DataSet &dataSet) override;
  tlp::DataSet state() const override;
  bool eventFilter(QObject *object, QEvent *event) override;
  void fillContextMenu(QMenu *menu, const QPointF &point) override;
  QList<QWidget *> configurationWidgets() const override;
  bool getNodeOrEdgeAtViewportPos(int x, int y, node &n, edge &e) const override;

  // methods called by interactors
  void setDataUnderPointerSelectFlag(const int x, const int y, const bool selectFlag);
  void setDataInRegionSelectFlag(const int x, const int y, const uint width, const uint height,
                                 const bool selectFlag);
  void resetSelection();
  void deleteDataUnderPointer(const int x, const int y);
  bool getDataUnderPointerProperties(const int x, const int y, SelectedEntity &selectedEntity);
  void highlightDataUnderPointer(const int x, const int y, const bool addEltToMagnifyFlag);
  void highlightDataInRegion(const int x, const int y, const int width, const int height,
                             const bool addEltToMagnifyFlag);
  void resetHighlightedElements();
  bool highlightedElementsSet() const;
  ParallelAxis *getAxisUnderPointer(const int x, const int y) const;
  void swapAxis(ParallelAxis *firstAxis, ParallelAxis *secondAxis);
  void removeAxis(ParallelAxis *axis);
  void addAxis(ParallelAxis *axis);
  std::vector<ParallelAxis *> getAllAxis();
  void resetAxisLayoutNextUpdate() {
    parallelCoordsDrawing->resetAxisLayoutNextUpdate();
  }

  void toggleInteractors(bool activate);

  void
  updateWithAxisSlidersRange(ParallelAxis *axis,
                             ParallelCoordinatesDrawing::HighlightedEltsSetOp highlightedEltsSetOp);
  void updateAxisSlidersPosition();
  bool hasHighlightedElts() const {
    return graphProxy->highlightedEltsSet();
  }
  void highlightDataInAxisBoxPlotRange(QuantitativeParallelAxis *axis);

  ParallelCoordinatesDrawing::LayoutType getLayoutType() const;
  ParallelCoordinatesDrawing::LinesType getLinesType() const;
  ParallelCoordinatesDrawing::LinesThickness getLinesThickness() const;

  // uint getSpaceBetweenAxis() {return drawConfigWidget->getSpaceBetweenAxis();}

public slots:

  void draw() override;
  void refresh() override;
  void init();
  ParallelCoordinatesGraphProxy *getGraphProxy() {
    return graphProxy;
  }

  // TODO : reactivate this
  void elementSelectedSlot(uint, bool) {
    // emit elementSelected(id,isNode);
  }
  void centerSetupAndDrawView();
  void setupAndDrawView();

  void applySettings() override;
  void resetHighlightedElementsSlot();

protected slots:

  void graphChanged(tlp::Graph *) override;

  void axisConfigurationSlot();
  void removeAxisSlot();
  void selectHighlightedElementsSlot();
  void addSelectHighlightedElementsSlot();
  void removeSelectHighlightedElementsSlot();

private:
  bool mapGlEntitiesInRegionToData(std::set<uint> &mappedData, const int x, const int y,
                                   const uint width = 1, const uint height = 1) const;
  void initGlWidget();
  void buildContextMenu();
  void showAxisConfigDialog(ParallelAxis *axis);

  void updateWithProgressBar();
  void updateWithoutProgressBar();

  void addEmptyViewLabel();
  void removeEmptyViewLabel();

  void registerTriggers();
  void removeTriggers();

  void interactorsInstalled(const QList<tlp::Interactor *> &) override;

  QMenu *viewSetupMenu;
  QAction *classicLayout;
  QAction *circularLayout;
  QAction *straightLinesType;
  QAction *catmullRomSplineLinesType;
  QAction *cubicBSplineInterpolationLinesType;
  QAction *thickLines;
  QAction *thinLines;
  QAction *addRemoveDataFromSelection;
  QAction *selectData;
  QAction *deleteData;
  QAction *showDataProperties;
  QAction *axisMenuSeparator;
  QAction *axisConfiguration;
  QAction *removeAxisAction;
  QAction *highlightMenuSeparator;
  QAction *selectHighlightedElements;
  QAction *addSelectHighlightedElements;
  QAction *removeSelectHighlightedElements;
  QAction *resetHightlightedElements;

  uint dataUnderMousePointer;
  GlLayer *mainLayer;
  GlLayer *axisSelectionLayer;

  GlGraph *glGraph;
  Graph *axisPointsGraph;
  DataSet *dataSet;

  ParallelCoordinatesGraphProxy *graphProxy;
  ParallelCoordinatesDrawing *parallelCoordsDrawing;
  ParallelAxis *axisUnderPointer;

  ViewGraphPropertiesSelectionWidget *dataConfigWidget;
  ParallelCoordsDrawConfigWidget *drawConfigWidget;

  bool firstSet;
  uint lastNbSelectedProperties;
  bool center;

  int lastViewWindowWidth, lastViewWindowHeight;

  bool isConstruct;
  bool dontCenterViewAfterConfLoaded;
  bool needDraw;
};
}

#endif // PARALLEL_COORDINATES_VIEW_H
