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

#include <talipot/GlLines.h>
#include <talipot/GlNode.h>
#include <talipot/GlEdge.h>
#include <talipot/GlComplexPolygon.h>
#include <talipot/GlWidget.h>
#include <talipot/GlGraph.h>
#include <talipot/GlQuantitativeAxis.h>
#include <talipot/GlLabel.h>

#include <QMenu>
#include <QEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>

#include "ScatterPlotCorrelCoeffSelector.h"
#include "ScatterPlotCorrelCoeffSelectorOptionsWidget.h"
#include "ScatterPlot2DView.h"
#include "ScatterPlot2D.h"

using namespace std;

namespace tlp {

const float POINT_RADIUS = 3.0f;

bool pointInPolygon(const vector<Coord> &polygon, const Coord &point) {
  size_t i, j;
  bool ret = false;

  for (i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++) {
    if ((((polygon[i].getY() <= point.getY()) && (point.getY() < polygon[j].getY())) ||
         ((polygon[j].getY() <= point.getY()) && (point.getY() < polygon[i].getY()))) &&
        (point.getX() < (polygon[j].getX() - polygon[i].getX()) *
                                (point.getY() - polygon[i].getY()) /
                                (polygon[j].getY() - polygon[i].getY()) +
                            polygon[i].getX())) {
      ret = !ret;
    }
  }

  return ret;
}

bool isPolygonAincludesInB(const vector<Coord> &A, const vector<Coord> &B) {
  bool ret = true;

  for (const auto &p : A) {
    ret = ret && pointInPolygon(B, p);

    if (!ret) {
      break;
    }
  }

  return ret;
}

GlEditableComplexPolygon::GlEditableComplexPolygon(std::vector<Coord> polygonPoints,
                                                   const Color &color)
    : polygonPoints(polygonPoints), color(color), selected(false) {
  basicCircle.resizePoints(30);
  basicCircle.setFillMode(true);
  basicCircle.setOutlineMode(true);
  basicCircle.setFillColor(Color(204, 255, 0, 255));
  basicCircle.setOutlineColor(Color(0, 0, 255, 255));
}

void GlEditableComplexPolygon::translate(const Coord &move) {
  for (auto &polygonPoint : polygonPoints) {
    polygonPoint += move;
  }
}

BoundingBox GlEditableComplexPolygon::getBoundingBox() {
  BoundingBox ret;

  for (const auto &polygonPoint : polygonPoints) {
    ret.expand(polygonPoint);
  }

  return ret;
}

bool GlEditableComplexPolygon::pointInsidePolygon(const Coord &point) {
  return pointInPolygon(polygonPoints, point);
}

Coord *
GlEditableComplexPolygon::getPolygonVertexUnderPointerIfAny(const Coord &pointerViewportCoord,
                                                            Camera *camera) {
  Coord *ret = nullptr;
  camera->initGl();

  for (const auto &c : polygonPoints) {
    Coord pointCenter = camera->worldTo2DViewport(c);

    if (pointerViewportCoord.getX() > (pointCenter.getX() - POINT_RADIUS) &&
        pointerViewportCoord.getX() < (pointCenter.getX() + POINT_RADIUS) &&
        pointerViewportCoord.getY() > (pointCenter.getY() - POINT_RADIUS) &&
        pointerViewportCoord.getY() < (pointCenter.getY() + POINT_RADIUS)) {
      ret = new Coord(c);
      break;
    }
  }

  return ret;
}

std::pair<Coord, Coord> *
GlEditableComplexPolygon::getPolygonSegmentUnderPointerIfAny(const Coord &pointerSceneCoord) {
  vector<Coord> polygonPointsCp(polygonPoints);
  polygonPointsCp.push_back(polygonPoints.front());

  for (size_t i = 0; i < polygonPointsCp.size() - 1; ++i) {
    double startToEndDist = polygonPointsCp[i].dist(polygonPointsCp[i + 1]);
    double startToPointDist = polygonPointsCp[i].dist(pointerSceneCoord);
    double pointToEndDist = pointerSceneCoord.dist(polygonPointsCp[i + 1]);

    if ((((startToPointDist + pointToEndDist) - startToEndDist) / startToEndDist < 1E-3)) {
      return new pair<Coord, Coord>(polygonPointsCp[i], polygonPointsCp[i + 1]);
    }
  }

  return nullptr;
}

void GlEditableComplexPolygon::movePolygonVertexToPoint(const Coord &polygonVertex,
                                                        const Coord &targetPoint) {
  std::replace(polygonPoints.begin(), polygonPoints.end(), polygonVertex, targetPoint);
}

void GlEditableComplexPolygon::addPolygonVertex(std::pair<Coord, Coord> polygonSegment,
                                                const Coord &newVertex) {
  for (auto it = polygonPoints.begin(); it != polygonPoints.end(); ++it) {
    if ((*it) == polygonSegment.second) {
      polygonPoints.insert(it, newVertex);
      return;
    }
  }

  if (polygonSegment.first == polygonPoints.back() &&
      polygonSegment.second == polygonPoints.front()) {
    polygonPoints.push_back(newVertex);
  }
}

void GlEditableComplexPolygon::removePolygonVertex(const Coord &vertex) {
  if (find(polygonPoints.begin(), polygonPoints.end(), vertex) != polygonPoints.end()) {
    polygonPoints.erase(std::remove(polygonPoints.begin(), polygonPoints.end(), vertex),
                        polygonPoints.end());
  }
}

void GlEditableComplexPolygon::draw(float lod, Camera *camera) {
  camera->initGl();
  GlComplexPolygon complexPolygon(polygonPoints, color, color, 1);
  glDisable(GL_DEPTH_TEST);
  complexPolygon.draw(lod, camera);

  if (selected) {
    Camera camera2D(camera->getScene(), false);
    camera2D.setScene(camera->getScene());

    for (const auto &polygonPoint : polygonPoints) {
      camera->initGl();
      Coord tmp = camera->worldTo2DViewport(polygonPoint);
      camera2D.initGl();
      basicCircle.set(tmp, POINT_RADIUS, 0.);
      basicCircle.draw(lod, camera);
    }
  }
}

ScatterPlotCorrelCoeffSelector::ScatterPlotCorrelCoeffSelector(
    ScatterPlotCorrelCoeffSelectorOptionsWidget *optionsWidget)
    : optionsWidget(optionsWidget), scatterView(nullptr), selectedPolygon(nullptr),
      selectedPolygonPoint(nullptr), dragStarted(false), x(0), y(0) {
  basicCircle.resizePoints(30);
  basicCircle.setFillMode(true);
  basicCircle.setOutlineMode(true);
  basicCircle.setFillColor(Color(255, 0, 0, 255));
  basicCircle.setOutlineColor(Color(0, 0, 255, 255));
}

ScatterPlotCorrelCoeffSelector::ScatterPlotCorrelCoeffSelector(
    const ScatterPlotCorrelCoeffSelector &scatterPlotCorrelCoeffSelector)
    : scatterView(nullptr), selectedPolygon(nullptr), selectedPolygonPoint(nullptr),
      dragStarted(false), x(0), y(0) {
  optionsWidget = scatterPlotCorrelCoeffSelector.optionsWidget;
  basicCircle.resizePoints(30);
  basicCircle.setFillMode(true);
  basicCircle.setOutlineMode(true);
  basicCircle.setFillColor(Color(255, 0, 0, 255));
  basicCircle.setOutlineColor(Color(0, 0, 255, 255));
}

ScatterPlotCorrelCoeffSelector::~ScatterPlotCorrelCoeffSelector() = default;

bool ScatterPlotCorrelCoeffSelector::eventFilter(QObject *obj, QEvent *e) {
  auto *glWidget = static_cast<GlWidget *>(obj);

  Camera &camera = glWidget->getScene()->getLayer("Main")->getCamera();
  Graph *graph = glWidget->getGlGraphInputData()->getGraph();
  BooleanProperty *viewSelection = graph->getBooleanProperty("viewSelection");

  if (!glWidget->hasMouseTracking()) {
    glWidget->setMouseTracking(true);
  }

  if (e->type() == QEvent::MouseMove) {
    auto *me = static_cast<QMouseEvent *>(e);
    x = glWidget->screenToViewport(glWidget->width() - me->x());
    y = glWidget->screenToViewport(me->y());
    Coord newPointerSceneCoords = camera.viewportTo3DWorld(Coord(x, y, 0));
    Coord translationVectorScene = newPointerSceneCoords - currentPointerSceneCoords;
    currentPointerSceneCoords = newPointerSceneCoords;

    if (polygonEdit.empty()) {
      if (!dragStarted) {
        getPolygonAndPointUnderPointerIfAny(currentPointerSceneCoords, &camera);

        if (selectedPolygonPoint != nullptr) {
          glWidget->setCursor(QCursor(Qt::ClosedHandCursor));
        } else if (selectedPolygon != nullptr) {
          glWidget->setCursor(QCursor(Qt::SizeAllCursor));
        } else {
          glWidget->setCursor(QCursor(Qt::ArrowCursor));
        }
      } else {
        if (selectedPolygon != nullptr && selectedPolygonPoint != nullptr) {
          selectedPolygon->movePolygonVertexToPoint(*selectedPolygonPoint,
                                                    currentPointerSceneCoords);
          delete selectedPolygonPoint;
          selectedPolygonPoint = new Coord(currentPointerSceneCoords);
        } else if (selectedPolygon != nullptr) {
          selectedPolygon->translate(translationVectorScene);

          if (selectedPolygonPoint != nullptr) {
            *selectedPolygonPoint += translationVectorScene;
          }
        }
      }
    } else if (polygonEdit.size() > 2) {
      Coord startPolygonPointScr = camera.worldTo2DViewport(polygonEdit[0]);
      Coord pointerGlViewportCoord =
          Coord(glWidget->screenToViewport(me->x()),
                glWidget->screenToViewport(glWidget->height() - me->y()));
      bool underFirstPoint =
          (pointerGlViewportCoord.getX() > (startPolygonPointScr.getX() - POINT_RADIUS) &&
           pointerGlViewportCoord.getX() < (startPolygonPointScr.getX() + POINT_RADIUS) &&
           pointerGlViewportCoord.getY() > (startPolygonPointScr.getY() - POINT_RADIUS) &&
           pointerGlViewportCoord.getY() < (startPolygonPointScr.getY() + POINT_RADIUS));

      if (underFirstPoint) {
        glWidget->setCursor(QCursor(Qt::SizeAllCursor));
      } else {
        glWidget->setCursor(QCursor(Qt::ArrowCursor));
      }
    }

    glWidget->redraw();
    return true;
  } else if (e->type() == QEvent::MouseButtonPress) {
    auto *me = static_cast<QMouseEvent *>(e);
    x = glWidget->screenToViewport(glWidget->width() - me->x());
    y = glWidget->screenToViewport(me->y());
    currentPointerSceneCoords = camera.viewportTo3DWorld(Coord(x, y, 0));

    if (me->buttons() == Qt::LeftButton) {
      if (selectedPolygon != nullptr || selectedPolygonPoint != nullptr) {
        dragStarted = true;
      } else if (polygonEdit.size() < 2) {
        polygonEdit.push_back(currentPointerSceneCoords);
      } else {
        Coord startPolygonPointScr = camera.worldTo2DViewport(polygonEdit[0]);
        Coord pointerGlViewportCoord =
            Coord(glWidget->screenToViewport(me->x()),
                  glWidget->screenToViewport(glWidget->height() - me->y()));
        bool underFirstPoint =
            (pointerGlViewportCoord.getX() > (startPolygonPointScr.getX() - POINT_RADIUS) &&
             pointerGlViewportCoord.getX() < (startPolygonPointScr.getX() + POINT_RADIUS) &&
             pointerGlViewportCoord.getY() > (startPolygonPointScr.getY() - POINT_RADIUS) &&
             pointerGlViewportCoord.getY() < (startPolygonPointScr.getY() + POINT_RADIUS));

        if (underFirstPoint) {
          polygons.push_back(new GlEditableComplexPolygon(polygonEdit, Color(0, 255, 0, 100)));
          polygonEdit.clear();
          mapPolygonColorToCorrelCoeffOfData(polygons.back(), glWidget);
          glWidget->setCursor(QCursor(Qt::ArrowCursor));
        } else {
          polygonEdit.push_back(currentPointerSceneCoords);
        }

        glWidget->redraw();
      }
    }
    return true;
  } else if (e->type() == QEvent::MouseButtonRelease) {
    dragStarted = false;

    if (selectedPolygon != nullptr) {
      mapPolygonColorToCorrelCoeffOfData(selectedPolygon, glWidget);
      glWidget->redraw();
    }

    return true;
  } else if (e->type() == QEvent::MouseButtonDblClick) {
    auto *me = static_cast<QMouseEvent *>(e);
    x = glWidget->screenToViewport(glWidget->width() - me->x());
    y = glWidget->screenToViewport(me->y());
    currentPointerSceneCoords = camera.viewportTo3DWorld(Coord(x, y, 0.0f));

    if (selectedPolygon != nullptr && selectedPolygonPoint != nullptr) {
      if (selectedPolygon->getNumberOfVertex() > 3) {
        selectedPolygon->removePolygonVertex(*selectedPolygonPoint);
        delete selectedPolygonPoint;
        selectedPolygonPoint = nullptr;
      }
    } else if (selectedPolygon != nullptr) {
      pair<Coord, Coord> *polygonSegment =
          selectedPolygon->getPolygonSegmentUnderPointerIfAny(currentPointerSceneCoords);

      if (polygonSegment != nullptr) {
        selectedPolygon->addPolygonVertex(*polygonSegment, currentPointerSceneCoords);
        delete polygonSegment;
      }
    }

    glWidget->redraw();
  } else if (e->type() == QEvent::ContextMenu) {
    auto *cme = static_cast<QContextMenuEvent *>(e);
    if (!polygonEdit.empty()) {
      polygonEdit.clear();
      glWidget->redraw();
    } else if (selectedPolygon != nullptr) {
      QMenu contextMenu(glWidget);
      QAction *deletePoly = contextMenu.addAction("Remove polygon");
      QAction *selectData = contextMenu.addAction("Select nodes under polygon");
      QAction *selectedAction = contextMenu.exec(cme->globalPos());

      if (selectedAction == deletePoly) {
        polygons.erase(std::remove(polygons.begin(), polygons.end(), selectedPolygon),
                       polygons.end());
        polygonsToNodesSubsetAndCorrelCoeff.erase(selectedPolygon);
        delete selectedPolygon;
        selectedPolygon = nullptr;
        delete selectedPolygonPoint;
        selectedPolygonPoint = nullptr;
        glWidget->redraw();
      } else if (selectedAction == selectData) {
        Observable::holdObservers();

        for (auto n : polygonsToNodesSubsetAndCorrelCoeff[selectedPolygon].first) {
          viewSelection->setNodeValue(n, true);
        }

        for (size_t i = 0; i < polygonsToNodesSubsetAndCorrelCoeff[selectedPolygon].first.size();
             ++i) {
          for (size_t j = i + 1;
               j < polygonsToNodesSubsetAndCorrelCoeff[selectedPolygon].first.size(); ++j) {
            edge e = graph->existEdge(polygonsToNodesSubsetAndCorrelCoeff[selectedPolygon].first[i],
                                      polygonsToNodesSubsetAndCorrelCoeff[selectedPolygon].first[j],
                                      false);

            if (e.isValid()) {
              viewSelection->setEdgeValue(e, true);
            }
          }
        }

        Observable::unholdObservers();
      }
      return true;
    }
  }

  return false;
}

bool ScatterPlotCorrelCoeffSelector::draw(GlWidget *glWidget) {
  Camera &camera = glWidget->getScene()->getLayer("Main")->getCamera();

  glDisable(GL_STENCIL_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (auto *polygon : polygons) {
    polygon->draw(0, &camera);
  }

  camera.initGl();

  if (selectedPolygon != nullptr) {
    GlQuantitativeAxis *xAxis = scatterView->getDetailedScatterPlot()->getXAxis();
    GlQuantitativeAxis *yAxis = scatterView->getDetailedScatterPlot()->getYAxis();
    GlLabel coeffLabel(
        Coord(xAxis->getAxisBaseCoord().getX() + (1.0f / 2.0f) * xAxis->getAxisLength(),
              yAxis->getAxisBaseCoord().getY() + yAxis->getAxisLength() + yAxis->getLabelHeight()),
        Size(3.0f * (xAxis->getAxisLength() / 4.0f), yAxis->getLabelHeight()),
        selectedPolygon->getColor());
    ostringstream oss;
    oss << "correlation coefficient for data under polygon = "
        << polygonsToNodesSubsetAndCorrelCoeff[selectedPolygon].second;
    coeffLabel.setText(oss.str());
    coeffLabel.draw(0, &camera);
  }

  Camera camera2D(camera.getScene(), false);

  Color backgroundColor = glWidget->getScene()->getBackgroundColor();
  Color foregroundColor;
  int bgV = backgroundColor.getV();

  if (bgV < 128) {
    foregroundColor = Color(255, 255, 255);
  } else {
    foregroundColor = Color(0, 0, 0);
  }

  if (!polygonEdit.empty()) {
    for (size_t i = 0; i < polygonEdit.size() - 1; ++i) {
      camera.initGl();
      Coord lineStart = camera.worldTo2DViewport(polygonEdit[i]);
      Coord lineEnd = camera.worldTo2DViewport(polygonEdit[i + 1]);
      camera2D.initGl();
      GlLines::glDrawLine(lineStart, lineEnd, 1.0f, GlLines::TLP_DASHED, foregroundColor,
                          foregroundColor);
    }

    camera.initGl();
    Coord lineStart = camera.worldTo2DViewport(polygonEdit[polygonEdit.size() - 1]);
    Coord lineEnd = camera.worldTo2DViewport(currentPointerSceneCoords);
    camera2D.initGl();
    GlLines::glDrawLine(lineStart, lineEnd, 1.0f, GlLines::TLP_DASHED, foregroundColor,
                        foregroundColor);
  }

  for (const auto &p : polygonEdit) {
    camera.initGl();
    Coord pointCoord = camera.worldTo2DViewport(p);
    camera2D.initGl();
    basicCircle.set(pointCoord, POINT_RADIUS, 0.);
    basicCircle.draw(0, nullptr);
  }

  return true;
}

bool ScatterPlotCorrelCoeffSelector::compute(GlWidget *) {
  return false;
}

void ScatterPlotCorrelCoeffSelector::viewChanged(View *view) {
  scatterView = dynamic_cast<ScatterPlot2DView *>(view);
}

void ScatterPlotCorrelCoeffSelector::getPolygonAndPointUnderPointerIfAny(
    const Coord &pointerSceneCoord, Camera *camera) {
  selectedPolygon = nullptr;
  delete selectedPolygonPoint;
  selectedPolygonPoint = nullptr;
  Coord pointerViewportCoord = camera->worldTo2DViewport(pointerSceneCoord);

  for (auto *polygon : polygons) {
    selectedPolygonPoint = polygon->getPolygonVertexUnderPointerIfAny(pointerViewportCoord, camera);

    if (selectedPolygonPoint != nullptr) {
      selectedPolygon = polygon;
      break;
    }
  }

  if (selectedPolygon == nullptr) {
    for (auto *polygon : polygons) {
      if (polygon->pointInsidePolygon(pointerSceneCoord)) {
        selectedPolygon = polygon;
        break;
      }
    }
  }

  for (auto *polygon : polygons) {
    if (polygon == selectedPolygon) {
      polygon->setSelected(true);
    } else {
      polygon->setSelected(false);
    }
  }
}

void ScatterPlotCorrelCoeffSelector::mapPolygonColorToCorrelCoeffOfData(
    GlEditableComplexPolygon *polygon, GlWidget *glWidget) {

  Graph *graph = glWidget->getGlGraphInputData()->getGraph();
  Camera &camera = glWidget->getScene()->getLayer("Main")->getCamera();

  BoundingBox polygonSceneBB = polygon->getBoundingBox();
  BoundingBox polygonScrBB;
  polygonScrBB.expand(camera.worldTo2DViewport(Coord(polygonSceneBB[0])));
  polygonScrBB.expand(camera.worldTo2DViewport(Coord(polygonSceneBB[1])));
  vector<Coord> polygonScr;
  const vector<Coord> &polygonVertices = polygon->getPolygonVertices();

  for (const auto &polygonVertice : polygonVertices) {
    polygonScr.push_back(camera.worldTo2DViewport(polygonVertice));
  }

  polygonScr.push_back(camera.worldTo2DViewport(polygonVertices[0]));

  int xStart = int(polygonScrBB[0][0]);
  int yStart = int(polygonScrBB[0][1]);
  int xEnd = int(polygonScrBB[1][0]);
  int yEnd = int(polygonScrBB[1][1]);

  vector<SelectedEntity> tmpNodes;
  vector<SelectedEntity> tmpEdges;
  glWidget->pickNodesEdges(xStart, glWidget->height() - yEnd, (xEnd - xStart), (yEnd - yStart),
                           tmpNodes, tmpEdges);
  vector<node> selectedNodes;
  double correlationCoeff = 0;

  if (!tmpNodes.empty()) {
    GlNode glNode;

    for (const auto &tmpNode : tmpNodes) {
      glNode.n = node(tmpNode.getComplexEntityId());
      BoundingBox nodeBB(glNode.getBoundingBox(glWidget->getGlGraphInputData()));
      float dx = nodeBB[1][0] - nodeBB[0][0];
      float dy = nodeBB[1][1] - nodeBB[0][1];
      float dz = nodeBB[1][2] - nodeBB[0][2];
      const float f = 0.2f;
      nodeBB[0][0] = nodeBB[0][0] + f * dx;
      nodeBB[0][1] = nodeBB[0][1] + f * dy;
      nodeBB[0][2] = nodeBB[0][2] + f * dz;
      nodeBB[1][0] = nodeBB[1][0] - f * dx;
      nodeBB[1][1] = nodeBB[1][1] - f * dy;
      nodeBB[1][2] = nodeBB[1][2] - f * dz;
      vector<float> xVec;
      vector<float> yVec;
      Coord nodeBBBLBScr = camera.worldTo2DViewport(nodeBB[0]);
      xVec.push_back(nodeBBBLBScr.getX());
      yVec.push_back(nodeBBBLBScr.getY());
      Coord nodeBBTLBScr =
          camera.worldTo2DViewport(Coord(nodeBB[0][0], nodeBB[1][1], nodeBB[0][2]));
      xVec.push_back(nodeBBTLBScr.getX());
      yVec.push_back(nodeBBTLBScr.getY());
      Coord nodeBBTRBScr =
          camera.worldTo2DViewport(Coord(nodeBB[1][0], nodeBB[1][1], nodeBB[0][2]));
      xVec.push_back(nodeBBTRBScr.getX());
      yVec.push_back(nodeBBTRBScr.getY());
      Coord nodeBBBRBScr =
          camera.worldTo2DViewport(Coord(nodeBB[1][0], nodeBB[0][1], nodeBB[0][2]));
      xVec.push_back(nodeBBBRBScr.getX());
      yVec.push_back(nodeBBBRBScr.getY());
      Coord nodeBBBLFScr =
          camera.worldTo2DViewport(Coord(nodeBB[0][0], nodeBB[0][1], nodeBB[1][2]));
      xVec.push_back(nodeBBBLFScr.getX());
      yVec.push_back(nodeBBBLFScr.getY());
      Coord nodeBBTLFScr =
          camera.worldTo2DViewport(Coord(nodeBB[0][0], nodeBB[1][1], nodeBB[1][2]));
      xVec.push_back(nodeBBTLFScr.getX());
      yVec.push_back(nodeBBTLFScr.getY());
      Coord nodeBBTRFScr = camera.worldTo2DViewport(Coord(nodeBB[1]));
      xVec.push_back(nodeBBTRFScr.getX());
      yVec.push_back(nodeBBTRFScr.getY());
      Coord nodeBBBRFScr =
          camera.worldTo2DViewport(Coord(nodeBB[1][0], nodeBB[0][1], nodeBB[1][2]));
      xVec.push_back(nodeBBBRFScr.getX());
      yVec.push_back(nodeBBBRFScr.getY());
      vector<Coord> quad;
      quad.push_back(
          Coord(*min_element(xVec.begin(), xVec.end()), *min_element(yVec.begin(), yVec.end())));
      quad.push_back(
          Coord(*min_element(xVec.begin(), xVec.end()), *max_element(yVec.begin(), yVec.end())));
      quad.push_back(
          Coord(*max_element(xVec.begin(), xVec.end()), *max_element(yVec.begin(), yVec.end())));
      quad.push_back(
          Coord(*max_element(xVec.begin(), xVec.end()), *min_element(yVec.begin(), yVec.end())));
      quad.push_back(quad[0]);

      if (isPolygonAincludesInB(quad, polygonScr)) {
        selectedNodes.push_back(node(tmpNode.getComplexEntityId()));
      }
    }
  }

  if (!selectedNodes.empty()) {
    string xDim(scatterView->getDetailedScatterPlot()->getXDim());
    string yDim(scatterView->getDetailedScatterPlot()->getYDim());
    NumericProperty *xProp = nullptr, *yProp = nullptr;

    xProp = static_cast<NumericProperty *>(graph->getProperty(xDim));
    yProp = static_cast<NumericProperty *>(graph->getProperty(yDim));

    double sumxiyi = 0, sumxi = 0, sumyi = 0, sumxi2 = 0, sumyi2 = 0;

    for (auto n : selectedNodes) {
      double xValue = 0, yValue = 0;

      xValue = xProp->getNodeDoubleValue(n);

      sumxi += xValue;
      sumxi2 += (xValue * xValue);

      yValue = yProp->getNodeDoubleValue(n);

      sumyi += yValue;
      sumyi2 += (yValue * yValue);
      sumxiyi += (xValue * yValue);
    }

    double numerator = sumxiyi - (1. / selectedNodes.size()) * sumxi * sumyi;
    double denominator = sqrt(sumxi2 - (1. / selectedNodes.size()) * (sumxi * sumxi)) *
                         sqrt(sumyi2 - (1. / selectedNodes.size()) * (sumyi * sumyi));

    if (denominator == 0) {
      correlationCoeff = 0.0;
    } else {
      correlationCoeff = numerator / denominator;
    }

    Color startColor(optionsWidget->getZeroColor()), endColor, polygonColor;

    if (correlationCoeff < 0) {
      endColor = optionsWidget->getMinusOneColor();
    } else {
      endColor = optionsWidget->getOneColor();
    }

    for (unsigned int i = 0; i < 4; ++i) {
      polygonColor[i] =
          uchar((double(startColor[i]) +
                 (double(endColor[i]) - double(startColor[i])) * abs(correlationCoeff)));
    }

    polygon->setColor(polygonColor);
  } else {
    polygon->setColor(optionsWidget->getZeroColor());
  }

  polygonsToNodesSubsetAndCorrelCoeff[polygon] = make_pair(selectedNodes, correlationCoeff);
}
}
