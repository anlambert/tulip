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

#include "PathFinderConfigurationWidget.h"
#include "ui_PathFinderConfiguration.h"

using namespace tlp;
using namespace std;

PathFinderConfigurationWidget::PathFinderConfigurationWidget(QWidget *parent)
    : QWidget(parent), _ui(new Ui::PathFinderConfigurationData) {
  _ui->setupUi(this);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
  connect(_ui->weightCombo, &QComboBox::textActivated, this,
          &PathFinderConfigurationWidget::setWeightMetric);
  connect(_ui->edgeOrientationCombo, &QComboBox::textActivated, this,
          &PathFinderConfigurationWidget::setEdgeOrientation);
  connect(_ui->pathsTypeCombo, &QComboBox::textActivated, this,
          &PathFinderConfigurationWidget::setPathsType);
#else
  connect(_ui->weightCombo, QOverload<const QString &>::of(&QComboBox::activated), this,
          &PathFinderConfigurationWidget::setWeightMetric);
  connect(_ui->edgeOrientationCombo, QOverload<const QString &>::of(&QComboBox::activated), this,
          &PathFinderConfigurationWidget::setEdgeOrientation);
  connect(_ui->pathsTypeCombo, QOverload<const QString &>::of(&QComboBox::activated), this,
          &PathFinderConfigurationWidget::setPathsType);
#endif
  connect(_ui->toleranceCheck, &QAbstractButton::clicked, this,
          &PathFinderConfigurationWidget::activateTolerance);
  connect(_ui->toleranceSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &PathFinderConfigurationWidget::setTolerance);
}

PathFinderConfigurationWidget::~PathFinderConfigurationWidget() {
  delete _ui;
}

void PathFinderConfigurationWidget::addweightComboItem(const QString &s) {
  _ui->weightCombo->addItem(s);
}

void PathFinderConfigurationWidget::addedgeOrientationComboItem(const QString &s) {
  _ui->edgeOrientationCombo->addItem(s);
}

void PathFinderConfigurationWidget::addpathsTypeComboItem(const QString &s) {
  _ui->pathsTypeCombo->addItem(s);
}

void PathFinderConfigurationWidget::setCurrentweightComboIndex(const int i) {
  _ui->weightCombo->setCurrentIndex(i);
}

int PathFinderConfigurationWidget::weightComboFindText(const QString &text) const {
  return _ui->weightCombo->findText(text);
}

void PathFinderConfigurationWidget::setCurrentedgeOrientationComboIndex(const int i) {
  _ui->edgeOrientationCombo->setCurrentIndex(i);
}

int PathFinderConfigurationWidget::edgeOrientationComboFindText(const QString &text) const {
  return _ui->edgeOrientationCombo->findText(text);
}

void PathFinderConfigurationWidget::toleranceChecked(const bool checked) {
  _ui->toleranceCheck->setChecked(checked);
}

void PathFinderConfigurationWidget::setToleranceSpinValue(const int val) {
  _ui->toleranceSpin->setValue(val);
}

void PathFinderConfigurationWidget::highlightersLabelDisabled(const bool disable) {
  _ui->highlightersLabel->setDisabled(disable);
}

void PathFinderConfigurationWidget::addbottomWidget(QWidget *w) {
  _ui->bottomArea->addWidget(w, 0, Qt::AlignLeft);
}

void PathFinderConfigurationWidget::toleranceDisabled(const bool disabled) {
  _ui->toleranceCheck->setDisabled(disabled);
  _ui->toleranceSpin->setDisabled(disabled);
  _ui->toleranceLabel->setDisabled(disabled);
  _ui->tolerancePercentLabel->setDisabled(disabled);
}
