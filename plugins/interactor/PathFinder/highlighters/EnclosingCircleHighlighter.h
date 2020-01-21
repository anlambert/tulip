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

#ifndef ENCLOSING_CIRCLE_HIGHLIGHTER_H
#define ENCLOSING_CIRCLE_HIGHLIGHTER_H

#include "PathHighlighter.h"

#include <talipot/Color.h>

#include <QWidget>
#include <QObject>

namespace Ui {
class EnclosingCircleConfiguration;
}

namespace tlp {

class EnclosingCircleConfigurationWidget : public QWidget {

  Q_OBJECT

  Ui::EnclosingCircleConfiguration *_ui;

public:
  EnclosingCircleConfigurationWidget(Color &circleColor, QWidget *parent = nullptr);
  ~EnclosingCircleConfigurationWidget() override;

  void circleColorBtnDisabled(const bool disabled);
  void inverseColorRadioCheck(const bool checked);
  void solidColorRadioCheck(const bool checked);
  void alphaSliderSetValue(const int val);
  Color getCircleColor() const;

signals:
  void solidColorRadioChecked(bool);
  void inverseColorRadioChecked(bool);
  void colorButtonClicked();
  void alphaChanged(int);
};

class EnclosingCircleHighlighter : public QObject, public PathHighlighter {
  Q_OBJECT

public:
  EnclosingCircleHighlighter();
  ~EnclosingCircleHighlighter() override;
  void highlight(const PathFinder *parent, GlMainWidget *glMainWidget, BooleanProperty *selection,
                 tlp::node src, tlp::node tgt) override;
  void draw(GlMainWidget *glMainWidget) override;
  bool isConfigurable() const override;
  QWidget *getConfigurationWidget() override;

public slots:
  void inverseColorRadioChecked(bool checked);
  void solidColorRadioChecked(bool checked);
  void colorButtonClicked();
  void alphaChanged(int);

private:
  Color circleColor, outlineColor;
  int alpha;
  bool inversedColor;

  EnclosingCircleConfigurationWidget *configurationWidget;
};
}
#endif // ENCLOSING_CIRCLE_HIGHLIGHTER_H
