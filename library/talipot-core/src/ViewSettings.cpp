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

#include <talipot/ViewSettings.h>
#include <talipot/TlpTools.h>

namespace tlp {

INSTANTIATE_DLL_TEMPLATE(Singleton<ViewSettings>, TLP_TEMPLATE_DEFINE_SCOPE)

static Color _defaultSelectionColor = {23, 81, 228};
static Color _defaultNodeColor(Color::Red);
static Color _defaultEdgeColor(Color::Black);
static Color _defaultNodeBorderColor(Color::Black);
static Color _defaultEdgeBorderColor(Color::Black);
static Color _defaultLabelColor(Color::Black);
static Color _defaultLabelBorderColor(Color::Black);
static float _defaultNodeBorderWidth(0);
static float _defaultEdgeBorderWidth(0);
static float _defaultLabelBorderWidth(1);
static int _defaultLabelPosition(LabelPosition::Center);
static Size _defaultNodeSize(1);
static Size _defaultEdgeSize(0.125, 0.125, 0.5);
static int _defaultNodeShape(NodeShape::Circle);
static int _defaultEdgeShape(EdgeShape::Polyline);
static int _defaultEdgeExtremitySrcShape(EdgeExtremityShape::None);
static int _defaultEdgeExtremityTgtShape(EdgeExtremityShape::Arrow);
static Size _defaultEdgeExtremitySrcSize(1, 1, 0);
static Size _defaultEdgeExtremityTgtSize(1, 1, 0);
static std::string _defaultFont("DejaVu Sans-Book");
static int _defaultFontSize(18);

Color ViewSettings::defaultSelectionColor() {
  return _defaultSelectionColor;
}

void ViewSettings::setDefaultSelectionColor(const Color &color) {
  if (color == defaultSelectionColor()) {
    return;
  }
  _defaultSelectionColor = color;
  instance().sendEvent(
      ViewSettingsEvent(color, ViewSettingsEvent::TLP_DEFAULT_SELECTION_COLOR_MODIFIED));
}

Color ViewSettings::defaultColor(ElementType elem) {
  if (elem == NODE) {
    return _defaultNodeColor;
  } else {
    return _defaultEdgeColor;
  }
}

void ViewSettings::setDefaultColor(ElementType elem, const Color &color) {
  if (color == defaultColor(elem)) {
    return;
  }

  if (elem == NODE) {
    _defaultNodeColor = color;
  } else {
    _defaultEdgeColor = color;
  }

  instance().sendEvent(ViewSettingsEvent(elem, color));
}

Color ViewSettings::defaultBorderColor(ElementType elem) {
  if (elem == NODE) {
    return _defaultNodeBorderColor;
  } else {
    return _defaultEdgeBorderColor;
  }
}

void ViewSettings::setDefaultBorderColor(ElementType elem, const Color &color) {
  if (elem == NODE) {
    _defaultNodeBorderColor = color;
  } else {
    _defaultEdgeBorderColor = color;
  }
}

float ViewSettings::defaultBorderWidth(ElementType elem) {
  if (elem == NODE) {
    return _defaultNodeBorderWidth;
  } else {
    return _defaultEdgeBorderWidth;
  }
}

void ViewSettings::setdefaultBorderWidth(ElementType elem, float borderWidth) {
  if (elem == NODE) {
    _defaultNodeBorderWidth = borderWidth;
  } else {
    _defaultEdgeBorderWidth = borderWidth;
  }
}

Color ViewSettings::defaultLabelColor() {
  return _defaultLabelColor;
}

void ViewSettings::setDefaultLabelColor(const Color &color) {
  if (color == _defaultLabelColor) {
    return;
  }

  _defaultLabelColor = color;
  instance().sendEvent(
      ViewSettingsEvent(color, ViewSettingsEvent::TLP_DEFAULT_LABEL_COLOR_MODIFIED));
}

Color ViewSettings::defaultLabelBorderColor() {
  return _defaultLabelBorderColor;
}

void ViewSettings::setDefaultLabelBorderColor(const Color &color) {
  _defaultLabelBorderColor = color;
}

float ViewSettings::defaultLabelBorderWidth() {
  return _defaultLabelBorderWidth;
}

void ViewSettings::setDefaultLabelBorderWidth(float borderWidth) {
  _defaultLabelBorderWidth = borderWidth;
}

ViewSettings::labelmap ViewSettings::POSITION_LABEL_MAP = {{LabelPosition::Center, "Center"},
                                                           {LabelPosition::Top, "Top"},
                                                           {LabelPosition::Bottom, "Bottom"},
                                                           {LabelPosition::Left, "Left"},
                                                           {LabelPosition::Right, "Right"}};

int ViewSettings::defaultLabelPosition() {
  return _defaultLabelPosition;
}

void ViewSettings::setDefaultLabelPosition(int position) {
  _defaultLabelPosition = position;
}

Size ViewSettings::defaultSize(ElementType elem) {
  if (elem == NODE) {
    return _defaultNodeSize;
  } else {
    return _defaultEdgeSize;
  }
}

void ViewSettings::setDefaultSize(ElementType elem, const Size &size) {
  if (size == defaultSize(elem)) {
    return;
  }

  if (elem == NODE) {
    _defaultNodeSize = size;
  } else {
    _defaultEdgeSize = size;
  }

  instance().sendEvent(ViewSettingsEvent(elem, size));
}

int ViewSettings::defaultShape(ElementType elem) {
  if (elem == NODE) {
    return _defaultNodeShape;
  } else {
    return _defaultEdgeShape;
  }
}

void ViewSettings::setDefaultShape(ElementType elem, int shape) {
  if (shape == defaultShape(elem)) {
    return;
  }

  if (elem == NODE) {
    _defaultNodeShape = shape;
  } else {
    _defaultEdgeShape = shape;
  }

  instance().sendEvent(ViewSettingsEvent(elem, shape));
}

int ViewSettings::defaultEdgeExtremitySrcShape() {
  return _defaultEdgeExtremitySrcShape;
}

void ViewSettings::setDefaultEdgeExtremitySrcShape(int shape) {
  _defaultEdgeExtremitySrcShape = shape;
}

int ViewSettings::defaultEdgeExtremityTgtShape() {
  return _defaultEdgeExtremityTgtShape;
}

void ViewSettings::setDefaultEdgeExtremityTgtShape(int shape) {
  _defaultEdgeExtremityTgtShape = shape;
}

Size ViewSettings::defaultEdgeExtremitySrcSize() {
  return _defaultEdgeExtremitySrcSize;
}

void ViewSettings::setDefaultEdgeExtremitySrcSize(const Size &size) {
  _defaultEdgeExtremitySrcSize = size;
}

Size ViewSettings::defaultEdgeExtremityTgtSize() {
  return _defaultEdgeExtremityTgtSize;
}

void ViewSettings::setDefaultEdgeExtremityTgtSize(const Size &size) {
  _defaultEdgeExtremityTgtSize = size;
}

std::string ViewSettings::defaultFont() {
  return _defaultFont;
}

void ViewSettings::setDefaultFont(const std::string &fontFile) {
  _defaultFont = fontFile;
}

int ViewSettings::defaultFontSize() {
  return _defaultFontSize;
}

void ViewSettings::setDefaultFontSize(int fontSize) {
  _defaultFontSize = fontSize;
}
}
