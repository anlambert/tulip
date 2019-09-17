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

#ifndef THRESHOLDINTERACTOR_H_
#define THRESHOLDINTERACTOR_H_

#include <tulip/OpenGlIncludes.h>

#include "EditColorScaleInteractor.h"

#include <tulip/GlComposite.h>
#include <tulip/Observable.h>
#include <tulip/Size.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>

#include <QMutex>

namespace tlp {

class GlLabelledColorScale;
class GlPolygon;
class GlQuad;
class GlLabel;

class Slider {
public:
  virtual ~Slider() {}

  virtual float getLeftBound() = 0;
  virtual float getRightBound() = 0;
  virtual void beginShift() = 0;
  virtual void shift(float shift) = 0;
  virtual void endShift() = 0;
};

class ColorScaleSlider : public Slider, public GlComposite, public Observable {
public:
  enum SliderWay { ToLeft, ToRight };
  ColorScaleSlider(SliderWay way, Size size, GlLabelledColorScale *colorScale,
                   const std::string &textureName);
  ~ColorScaleSlider() override;
  void draw(float lod, tlp::Camera *camera) override;
  void setColor(Color c);
  Coord getBasePosition() {
    return position;
  }
  Size getSize() {
    return size;
  }
  SliderWay getWay() {
    return way;
  }
  void setLinkedSlider(ColorScaleSlider *linkedSlider);
  ColorScaleSlider *getLinkedSlider() {
    return linkedSlider;
  }
  float getLeftBound() override;
  float getRightBound() override;
  void beginShift() override;
  void shift(float shift) override;
  void endShift() override;
  double getValue();
  float getCurrentShift() const {
    return currentShift;
  }

  void setValue(double value);
  void update(std::set<Observable *>::iterator begin, std::set<Observable *>::iterator end);
  void observableDestroyed(Observable *);

protected:
  void buildComposite(const std::string &textureName);
  void updatePosition();
  void computeBoundingBox();
  SliderWay way;
  Coord position;
  Size size;
  GlPolygon *arrow;
  GlQuad *rect;
  GlLabel *label;
  ColorScaleSlider *linkedSlider;
  GlLabelledColorScale *linkedScale;

  float currentShift;
};

class SliderBar : public Slider, public tlp::GlSimpleEntity {

public:
  SliderBar(ColorScaleSlider *left, ColorScaleSlider *right, const std::string &textureName);
  ~SliderBar() override;
  float getLeftBound() override;
  float getRightBound() override;
  void beginShift() override;
  void shift(float shift) override;
  void endShift() override;
  void draw(float lod, tlp::Camera *camera) override;
  void getXML(std::string &) override {}
  void setWithXML(const std::string &, unsigned int &) override {}

protected:
  ColorScaleSlider *left;
  ColorScaleSlider *right;
  std::string texture;
  bool isVisible;
};

class ThresholdInteractor : public EditColorScaleInteractor {
public:
  ThresholdInteractor();
  ~ThresholdInteractor() override;
  bool draw(tlp::GlMainWidget *glMainWidget) override;
  bool eventFilter(QObject *, QEvent *) override;
  void setView(View *view);

protected:
  bool screenSizeChanged(SOMView *somView) override;
  void propertyChanged(SOMView *somView, const std::string &propertyName,
                       tlp::NumericProperty *newProperty) override;
  void performSelection(SOMView *somView, Iterator<node> *it);
  void buildSliders(SOMView *somView);
  void clearSliders();
  void generateSliderTexture();

  tlp::GlLayer *layer;

  Slider *mouvingSlider;
  ColorScaleSlider *rSlider;
  ColorScaleSlider *lSlider;
  SliderBar *bar;
  bool startDrag;
  int XPosCursor;

  QMutex lock;
  std::string textureName;
  GLuint textureId;
};
} // namespace tlp
#endif /* THRESHOLDINTERACTOR_H_ */
