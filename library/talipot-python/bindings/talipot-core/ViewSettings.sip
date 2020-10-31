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

%ModuleHeaderCode
#include <talipot/ViewSettings.h>
%End

namespace tlp {

class NodeShape /NoDefaultCtors/ {
%TypeHeaderCode
#include <talipot/ViewSettings.h>
%End

public:

  enum NodeShapes {
    Billboard=7,
    Circle=14,
    Cone=3,
    Cross=8,
    Cube=0,
    CubeOutlined=1,
    CubeOutlinedTransparent=9,
    Cylinder=6,
    Diamond=5,
    GlowSphere=16,
    HalfCylinder=10,
    Hexagon=13,
    Pentagon=12,
    Ring=15,
    RoundedBox=18,
    Sphere=2 ,
    Square=4,
    Triangle=11,
    Window=17,
    Star=19,
    FontAwesomeIcon=20,
    Icon=20
  };

};

class EdgeShape /NoDefaultCtors/ {
%TypeHeaderCode
#include <talipot/ViewSettings.h>
%End

public:

  enum EdgeShapes {
    Polyline=0,
    BezierCurve=4,
    CatmullRomCurve=8,
    CubicBSplineCurve=16
  };

};

class EdgeExtremityShape /NoDefaultCtors/ {
%TypeHeaderCode
#include <talipot/ViewSettings.h>
%End

public:

  enum EdgeExtremityShapes {
    None=-1,
    Arrow=50,
    Circle=14,
    Cone=3,
    Cross=8,
    Cube=0,
    CubeOutlinedTransparent=9,
    Cylinder=6,
    Diamond=5,
    GlowSphere=16,
    Hexagon=13,
    Pentagon=12,
    Ring=15,
    Sphere=2 ,
    Square=4,
    Star=19,
    FontAwesomeIcon=20,
    Icon=20
  };

};

class LabelPosition /NoDefaultCtors/ {
%TypeHeaderCode
#include <talipot/ViewSettings.h>
%End

public:

  enum LabelPositions {
    Center=0,
    Top,
    Bottom,
    Left,
    Right
  };

};

class ViewSettings : tlp::Observable /NoDefaultCtors/ {
%TypeHeaderCode
#include <talipot/ViewSettings.h>
%End

public:

  struct ApplicationDefault /NoDefaultCtors/ {
    static const Color SelectionColor;
    static const Color NodeColor;
    static const Color EdgeColor;
    static const Color NodeBorderColor;
    static const Color EdgeBorderColor;
    static const Color LabelColor;
    static const Color LabelBorderColor;
    static const float NodeBorderWidth;
    static const float EdgeBorderWidth;
    static const float LabelBorderWidth;
    static const int LabelPosition;
    static const Size NodeSize;
    static const Size EdgeSize;
    static const int NodeShape;
    static const int EdgeShape;
    static const int EdgeExtremitySrcShape;
    static const int EdgeExtremityTgtShape;
    static const Size EdgeExtremitySrcSize;
    static const Size EdgeExtremityTgtSize;
    static const std::string Font;
    static const int FontSize;
  };

  static tlp::Color defaultSelectionColor();

  static void setDefaultSelectionColor(const tlp::Color& color);

  static tlp::Color defaultColor(tlp::ElementType elem);

  static void setDefaultColor(tlp::ElementType elem, const tlp::Color& color);

  static tlp::Color defaultBorderColor(tlp::ElementType elem);

  static void setDefaultBorderColor(tlp::ElementType elem, const tlp::Color& color);

  static float defaultBorderWidth(tlp::ElementType elem);

  static void setdefaultBorderWidth(tlp::ElementType elem, float borderWidth);

  static tlp::Color defaultLabelColor();

  static void setDefaultLabelColor(const tlp::Color& color);

  static tlp::Color defaultLabelBorderColor();

  static void setDefaultLabelBorderColor(const tlp::Color& color);

  static float defaultLabelBorderWidth();

  static void setDefaultLabelBorderWidth(float borderWidth);

  static int defaultLabelPosition();

  static void setDefaultLabelPosition(int position);

  static tlp::Size defaultSize(tlp::ElementType elem);

  static void setDefaultSize(tlp::ElementType elem, const tlp::Size& size);

  static int defaultShape(tlp::ElementType elem);

  static void setDefaultShape(tlp::ElementType elem, int shape);

  static int defaultEdgeExtremitySrcShape();

  static void setDefaultEdgeExtremitySrcShape(int shape);

  static int defaultEdgeExtremityTgtShape();

  static void setDefaultEdgeExtremityTgtShape(int shape);

  static tlp::Size defaultEdgeExtremitySrcSize();

  static void setDefaultEdgeExtremitySrcSize(const tlp::Size &size);

  static tlp::Size defaultEdgeExtremityTgtSize();

  static void setDefaultEdgeExtremityTgtSize(const tlp::Size &size);

  static std::string defaultFont();

  static void setDefaultFont(const std::string &fontFile);

  static int defaultFontSize();

  static void setDefaultFontSize(int fontSize);

private:

  ViewSettings();

};

class ViewSettingsEvent : public tlp::Event /NoDefaultCtors/ {
%TypeHeaderCode
#include <talipot/ViewSettings.h>
%End

%ConvertToSubClassCode
   if (dynamic_cast<tlp::ViewSettingsEvent*>(sipCpp)) {
        sipType = sipFindType("tlp::ViewSettingsEvent");
   } else {
        sipType = NULL;
   }
%End

public:

  enum ViewSettingsEventType {
    TLP_DEFAULT_COLOR_MODIFIED,
    TLP_DEFAULT_SHAPE_MODIFIED,
    TLP_DEFAULT_SIZE_MODIFIED,
    TLP_DEFAULT_LABEL_COLOR_MODIFIED
  };

  ViewSettingsEventType getType();

  tlp::ElementType getElementType();

  tlp::Color getColor();

  tlp::Size getSize();

  int getShape();

};
};
