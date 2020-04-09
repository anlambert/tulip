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

#ifndef TALIPOT_MOUSE_EDGE_BEND_EDITOR_H
#define TALIPOT_MOUSE_EDGE_BEND_EDITOR_H

#include <talipot/GlCircle.h>
#include <talipot/GlTriangle.h>
#include <talipot/GLInteractor.h>
#include <talipot/GlScene.h>
#include <talipot/GlLines.h>

namespace tlp {

class LayoutProperty;
class BooleanProperty;
class SizeProperty;
class DoubleProperty;
class CoordVectorProperty;
class IntegerProperty;

class EdgeEntity : public GlEntity {

  Coord start;
  Coord end;
  std::vector<Coord> bends;

public:
  void setCoordinates(const Coord &startPos, const Coord &endPos,
                      const std::vector<Coord> &bendsPos) {
    boundingBox = BoundingBox();
    start = startPos;
    end = endPos;
    bends = bendsPos;
    boundingBox.expand(start);
    boundingBox.expand(end);

    for (const auto &c : bends) {
      boundingBox.expand(c);
    }
  }

  void draw(float, Camera *) override {
    GlLines::glDrawCurve(start, bends, end, 10, 0, Color(127, 127, 127, 255),
                         Color(127, 127, 127, 255));
  }

  void getXML(std::string &) override {}
  void setWithXML(const std::string &, unsigned int &) override {}
};

/// This interactor allows to move/add/delete EdgeBends
class TLP_QT_SCOPE MouseEdgeBendEditor : public GLInteractorComponent {

public:
  MouseEdgeBendEditor();
  ~MouseEdgeBendEditor() override;
  bool compute(GlMainWidget *glMainWidget) override;
  bool draw(GlMainWidget *) override;
  void clear() override;
  bool eventFilter(QObject *, QEvent *) override;

protected:
  enum EditOperation { NONE_OP = 0, TRANSLATE_OP, NEW_OP, DELETE_OP };

  EditOperation operation() const {
    return _operation;
  }

  tlp::edge getEdge() const {
    return mEdge;
  }

  void stopEdition();

private:
  enum OperationTarget { COORD = 0, SIZE, COORD_AND_SIZE };

  Graph *_graph;
  GlMainWidget *glMainWidget;
  LayoutProperty *_layout;
  BooleanProperty *_selection;
  DoubleProperty *_rotation;
  SizeProperty *_sizes;
  IntegerProperty *_shape;
  CoordVectorProperty *_coordsVectorProperty;

  void initProxies(GlMainWidget *glMainWidget);

  EditOperation _operation;
  OperationTarget mode;

  Coord editPosition;

  GlLayer *layer;
  std::vector<tlp::GlCircle> circles;
  EdgeEntity *edgeEntity;
  std::vector<Coord> coordinates;
  GlCircle basicCircle;
  GlTriangle targetTriangle;
  GlCircle sourceCircle;
  GlComposite *circleString;
  std::vector<SelectedEntity> select;
  bool edgeSelected;
  tlp::edge mEdge;
  node mNode;
  Coord start, end;
  std::string selectedEntity;
  bool mouseButtonPressOnEdge;
  bool belong(Coord, Coord, Coord, GlMainWidget *);
  bool haveSelection(GlMainWidget *);
  void computeSrcTgtEntities(GlMainWidget *);
  bool computeBendsCircles(GlMainWidget *);
  void mMouseTranslate(int, int, GlMainWidget *);
  void mMouseDelete();
  void mMouseCreate(int, int, GlMainWidget *);
};
}

#endif // TALIPOT_MOUSE_EDGE_BEND_EDITOR_H
