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

#ifndef TALIPOT_VIEW_SETTINGS_H
#define TALIPOT_VIEW_SETTINGS_H

#include <talipot/config.h>
#include <talipot/Color.h>
#include <talipot/Size.h>
#include <talipot/Graph.h>
#include <talipot/Observable.h>
#include <talipot/Singleton.h>

#include <string>
#include <map>

namespace tlp {

class TLP_SCOPE NodeShape {

public:
  enum NodeShapes {
    Billboard = 7,
    BottomShadowedSphere = 21,
    Circle = 14,
    Cone = 3,
    Cross = 8,
    Cube = 0,
    CubeOutlined = 1,
    CubeOutlinedTransparent = 9,
    Cylinder = 6,
    Diamond = 5,
    GlowSphere = 16,
    HalfCylinder = 10,
    Hexagon = 13,
    LeftBottomShadowedSphere = 22,
    Pentagon = 12,
    RightBottomShadowedSphere = 23,
    Ring = 15,
    RoundedBox = 18,
    Sphere = 2,
    Square = 4,
    Triangle = 11,
    Window = 17,
    Star = 19,
    FontAwesomeIcon = 20,
    Icon = 20
  };
};

class TLP_SCOPE EdgeShape {

public:
  enum EdgeShapes { Polyline = 0, BezierCurve = 4, CatmullRomCurve = 8, CubicBSplineCurve = 16 };
};

class TLP_SCOPE EdgeExtremityShape {

public:
  enum EdgeExtremityShapes {
    None = -1,
    Arrow = 50,
    Circle = 14,
    Cone = 3,
    Cross = 8,
    Cube = 0,
    CubeOutlinedTransparent = 9,
    Cylinder = 6,
    Diamond = 5,
    GlowSphere = 16,
    Hexagon = 13,
    Pentagon = 12,
    Ring = 15,
    Sphere = 2,
    Square = 4,
    Star = 19,
    FontAwesomeIcon = 20,
    Icon = 20
  };
};

class TLP_SCOPE LabelPosition {

public:
  enum LabelPositions { Center = 0, Top, Bottom, Left, Right };
};

class ViewSettings;
DECLARE_DLL_TEMPLATE_INSTANCE(Singleton<ViewSettings>, TLP_TEMPLATE_DECLARE_SCOPE)

class TLP_SCOPE ViewSettings : public Observable, public Singleton<ViewSettings> {

  friend class Singleton<ViewSettings>;

public:
  typedef std::map<LabelPosition::LabelPositions, std::string> labelmap;

  static labelmap POSITION_LABEL_MAP;

  Color defaultColor(ElementType elem) const;
  void setDefaultColor(ElementType elem, const Color &color);

  Color defaultBorderColor(ElementType elem) const;
  void setDefaultBorderColor(ElementType elem, const Color &color);

  float defaultBorderWidth(ElementType elem) const;
  void setdefaultBorderWidth(ElementType elem, float borderWidth);

  Color defaultLabelColor() const;
  void setDefaultLabelColor(const Color &color);

  Color defaultLabelBorderColor() const;
  void setDefaultLabelBorderColor(const Color &color);

  float defaultLabelBorderWidth() const;
  void setDefaultLabelBorderWidth(float borderWidth);

  int defaultLabelPosition() const;
  void setDefaultLabelPosition(int position);

  Size defaultSize(ElementType elem) const;
  void setDefaultSize(ElementType elem, const Size &size);

  int defaultShape(ElementType elem) const;
  void setDefaultShape(ElementType elem, int shape);

  int defaultEdgeExtremitySrcShape() const;
  void setDefaultEdgeExtremitySrcShape(int shape);

  int defaultEdgeExtremityTgtShape() const;
  void setDefaultEdgeExtremityTgtShape(int shape);

  Size defaultEdgeExtremitySrcSize() const;
  void setDefaultEdgeExtremitySrcSize(const Size &size);

  Size defaultEdgeExtremityTgtSize() const;
  void setDefaultEdgeExtremityTgtSize(const Size &size);

  std::string defaultFont() const;
  void setDefaultFont(const std::string &fontFile);

  int defaultFontSize() const;
  void setDefaultFontSize(int fontSize);

private:
  ViewSettings() {}
};

class TLP_SCOPE ViewSettingsEvent : public tlp::Event {

public:
  enum ViewSettingsEventType {
    TLP_DEFAULT_COLOR_MODIFIED,
    TLP_DEFAULT_SHAPE_MODIFIED,
    TLP_DEFAULT_SIZE_MODIFIED,
    TLP_DEFAULT_LABEL_COLOR_MODIFIED
  };

  ViewSettingsEvent(ElementType elem, const Color &color)
      : Event(ViewSettings::instance(), Event::TLP_MODIFICATION), _type(TLP_DEFAULT_COLOR_MODIFIED),
        _elem(elem), _color(color) {}

  ViewSettingsEvent(ElementType elem, const Size &size)
      : Event(ViewSettings::instance(), Event::TLP_MODIFICATION), _type(TLP_DEFAULT_SIZE_MODIFIED),
        _elem(elem), _size(size) {}

  ViewSettingsEvent(ElementType elem, int shape)
      : Event(ViewSettings::instance(), Event::TLP_MODIFICATION), _type(TLP_DEFAULT_SHAPE_MODIFIED),
        _elem(elem), _shape(shape) {}

  ViewSettingsEvent(const Color &labelColor)
      : Event(ViewSettings::instance(), Event::TLP_MODIFICATION),
        _type(TLP_DEFAULT_LABEL_COLOR_MODIFIED), _color(labelColor) {}

  ViewSettingsEventType getType() const {
    return _type;
  }

  ElementType getElementType() const {
    return _elem;
  }

  Color getColor() const {
    return _color;
  }

  Size getSize() const {
    return _size;
  }

  int getShape() const {
    return _shape;
  }

private:
  ViewSettingsEventType _type;
  ElementType _elem;
  Color _color;
  Size _size;
  int _shape;
};
}

#endif // TALIPOT_VIEW_SETTINGS_H
