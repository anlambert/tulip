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

#include <talipot/GlScene.h>
#include <talipot/Circle.h>
#include <talipot/GlCircle.h>
#include <talipot/GlMainView.h>
#include <talipot/GlGraphInputData.h>
#include <talipot/GlMainWidget.h>
#include <talipot/ColorButton.h>

#include <QWidget>

#include "EnclosingCircleHighlighter.h"
#include "../PathFinderTools.h"
#include "ui_EnclosingCircleConfiguration.h"

using namespace tlp;
using namespace std;

EnclosingCircleConfigurationWidget::EnclosingCircleConfigurationWidget(Color &circleColor,
                                                                       QWidget *parent)
    : QWidget(parent), _ui(new Ui::EnclosingCircleConfigurationData) {
  _ui->setupUi(this);
  connect(_ui->solidColorRadio, &QAbstractButton::clicked, this,
          &EnclosingCircleConfigurationWidget::solidColorRadioChecked);
  connect(_ui->inverseColorRadio, &QAbstractButton::clicked, this,
          &EnclosingCircleConfigurationWidget::inverseColorRadioChecked);
  connect(_ui->circleColorBtn, &QAbstractButton::clicked, this,
          &EnclosingCircleConfigurationWidget::colorButtonClicked);
  connect(_ui->alphaSlider, &QAbstractSlider::valueChanged, this,
          &EnclosingCircleConfigurationWidget::alphaChanged);
  _ui->circleColorBtn->setDialogTitle("Choose the enclosing circle color");
  _ui->circleColorBtn->setColor(circleColor);
}

EnclosingCircleConfigurationWidget::~EnclosingCircleConfigurationWidget() {
  delete _ui;
}

void EnclosingCircleConfigurationWidget::circleColorBtnDisabled(const bool disabled) {
  _ui->circleColorBtn->setDisabled(disabled);
}

void EnclosingCircleConfigurationWidget::inverseColorRadioCheck(const bool checked) {
  _ui->inverseColorRadio->setChecked(checked);
}

void EnclosingCircleConfigurationWidget::alphaSliderSetValue(const int val) {
  _ui->alphaSlider->setValue(val);
}

void EnclosingCircleConfigurationWidget::solidColorRadioCheck(const bool checked) {
  _ui->solidColorRadio->setChecked(checked);
}

Color EnclosingCircleConfigurationWidget::getCircleColor() const {
  return _ui->circleColorBtn->talipotColor();
}

Color getInverseColor(const Color &c) {
  return Color(255 - c.getR(), 255 - c.getG(), 255 - c.getB(), c.getA());
}

//******************************************************
EnclosingCircleHighlighter::EnclosingCircleHighlighter()
    : PathHighlighter("Enclosing circle"), circleColor(200, 200, 200), outlineColor(0, 0, 0),
      alpha(128), inversedColor(false), configurationWidget(nullptr) {}

void EnclosingCircleHighlighter::highlight(const PathFinder *, GlMainWidget *glMainWidget,
                                           BooleanProperty *selection, node, node) {
  GlGraphInputData *inputData(getInputData(glMainWidget));
  GlScene *scene = glMainWidget->getScene();
  LayoutProperty *layout = inputData->getElementLayout();
  vector<Circlef> circles;
  float minDepth = -.5; // We'll draw the circle beyond the deeper node. Will work in most cases.
  for (auto n : selection->getNodesEqualTo(true)) {
    Circlef c;
    minDepth = min(minDepth, layout->getNodeValue(n).getZ());

    if (getNodeEnclosingCircle(c, inputData, n)) {
      circles.push_back(c);
    }
  }
  for (auto e : selection->getEdgesEqualTo(true)) {
    Circlef c;

    if (getEdgeEnclosingCircle(c, inputData, e)) {
      circles.push_back(c);
    }
  }
  Circlef enclosing(enclosingCircle<float>(circles));

  Color inside, outline;

  if (inversedColor) {
    inside = getInverseColor(glMainWidget->getScene()->getBackgroundColor());
    outline = inside;
  } else {
    inside = circleColor;
    outline = outlineColor;
  }

  inside.setA(alpha);

  GlCircle *glCircle = new GlCircle(Coord(enclosing[0], enclosing[1], minDepth), enclosing.radius,
                                    outline, inside, true, true, 0, 256);
  addGlEntity(scene, glCircle, true, "PathFinderCircle");
}

void EnclosingCircleHighlighter::draw(GlMainWidget *) {}

bool EnclosingCircleHighlighter::isConfigurable() const {
  return true;
}

EnclosingCircleHighlighter::~EnclosingCircleHighlighter() {
  // no need to delete the configurationWidget. Qt will do it well.
  // delete configurationWidget;
}

QWidget *EnclosingCircleHighlighter::getConfigurationWidget() {
  configurationWidget = new EnclosingCircleConfigurationWidget(circleColor);

  if (inversedColor) {
    configurationWidget->inverseColorRadioCheck(true);
    configurationWidget->circleColorBtnDisabled(true);
  } else {
    configurationWidget->solidColorRadioCheck(true);
  }

  configurationWidget->alphaSliderSetValue(alpha);
  connect(configurationWidget, &EnclosingCircleConfigurationWidget::solidColorRadioChecked, this,
          &EnclosingCircleHighlighter::solidColorRadioChecked);
  connect(configurationWidget, &EnclosingCircleConfigurationWidget::inverseColorRadioChecked, this,
          &EnclosingCircleHighlighter::inverseColorRadioChecked);
  connect(configurationWidget, &EnclosingCircleConfigurationWidget::colorButtonClicked, this,
          &EnclosingCircleHighlighter::colorButtonClicked);
  connect(configurationWidget, &EnclosingCircleConfigurationWidget::alphaChanged, this,
          &EnclosingCircleHighlighter::alphaChanged);
  return configurationWidget;
}

void EnclosingCircleHighlighter::solidColorRadioChecked(bool) {
  configurationWidget->circleColorBtnDisabled(false);
  inversedColor = false;
}

void EnclosingCircleHighlighter::inverseColorRadioChecked(bool) {
  configurationWidget->circleColorBtnDisabled(true);
  inversedColor = true;
}

void EnclosingCircleHighlighter::colorButtonClicked() {
  outlineColor = Color(0, 0, 0);
  circleColor = configurationWidget->getCircleColor();
}

void EnclosingCircleHighlighter::alphaChanged(int a) {
  alpha = a;
}
