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

#ifndef FISH_EYE_INTERACTOR_H
#define FISH_EYE_INTERACTOR_H

#include <memory>

#include <talipot/GLInteractor.h>
#include <talipot/Coord.h>

#include "../../utils/StandardInteractorPriority.h"

namespace tlp {

class FishEyeConfigWidget;
class GlShaderProgram;

class FishEyeInteractorComponent : public GLInteractorComponent {

public:
  FishEyeInteractorComponent(FishEyeConfigWidget *configWidget);
  FishEyeInteractorComponent(const FishEyeInteractorComponent &fisheyeInteractorComponent);

  bool eventFilter(QObject *widget, QEvent *e) override;

  bool compute(GlMainWidget *) override {
    return false;
  }

  void viewChanged(View *view) override;

  bool draw(GlMainWidget *glMainWidget) override;

private:
  FishEyeConfigWidget *configWidget;
  Coord fisheyeCenter;
  static std::unique_ptr<GlShaderProgram> fisheyeShader;
  bool activateFishEye;
};

/*@{*/
/** \file
 *  \brief Fisheye Interactor

 * This interactor plugin allow to perform a fisheye distortion effect on OpenGL views.
 * It allows to gain focus on a certain area of a visualization by magnifying it without losing
 * the context.
 *
 * Your graphic card must support shader programs otherwise the plugin won't work.
 *
 *
 */
class FishEyeInteractor : public GLInteractorComposite {

public:
  PLUGININFORMATION("FishEyeInteractor", "Antoine Lambert", "29/05/2009", "FishEye Interactor",
                    "1.0", "Visualization")

  FishEyeInteractor(const PluginContext *);
  ~FishEyeInteractor() override;

  void construct() override;

  void uninstall() override;

  QWidget *configurationWidget() const override;

  unsigned int priority() const override {
    return StandardInteractorPriority::FishEye;
  }

  bool isCompatible(const std::string &viewName) const override;

private:
  FishEyeConfigWidget *fisheyeConfigWidget;
};
}

#endif // FISH_EYE_INTERACTOR_H
