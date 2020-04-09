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

#ifndef TALIPOT_SCENE_LAYERS_CONFIG_WIDGET_H
#define TALIPOT_SCENE_LAYERS_CONFIG_WIDGET_H

#include <QWidget>

#include <talipot/config.h>

namespace Ui {
class SceneLayersConfigWidget;
}

namespace tlp {

class GlMainWidget;

class TLP_QT_SCOPE SceneLayersConfigWidget : public QWidget {
  Q_OBJECT

  Ui::SceneLayersConfigWidget *_ui;
  tlp::GlMainWidget *_glMainWidget;

public:
  explicit SceneLayersConfigWidget(QWidget *parent = nullptr);
  ~SceneLayersConfigWidget() override;

  void setGlMainWidget(tlp::GlMainWidget *glMainWidget);

public slots:

  void resizeFirstColumn();

signals:
  void drawNeeded();
};
}

#endif // TALIPOT_SCENE_LAYERS_CONFIG_WIDGET_H
