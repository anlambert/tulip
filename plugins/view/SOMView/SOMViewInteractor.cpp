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

#include "SOMViewInteractor.h"
#include <talipot/MouseInteractors.h>
#include <talipot/MouseSelector.h>
#include <talipot/MouseShowElementInfo.h>
#include "EditColorScaleInteractor.h"
#include "ThresholdInteractor.h"

#include "../../utils/StandardInteractorPriority.h"
#include "../../utils/PluginNames.h"

using namespace std;
using namespace tlp;

PLUGIN(SOMViewNavigation)
PLUGIN(SOMViewSelection)
PLUGIN(SOMViewProperties)
PLUGIN(SOMViewThreshold)

SOMViewInteractor::SOMViewInteractor(const QString &iconPath, const QString &text,
                                     const unsigned int priority)
    : NodeLinkDiagramViewInteractor(iconPath, text, priority) {}

bool SOMViewInteractor::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::SOMViewName);
}

SOMViewNavigation::SOMViewNavigation(PluginContext *)
    : SOMViewInteractor(":/talipot/gui/icons/i_navigation.png", "Navigate",
                        StandardInteractorPriority::Navigation) {}

void SOMViewNavigation::construct() {
  push_back(new MouseNKeysNavigator());
  push_back(new EditColorScaleInteractor());
}

SOMViewSelection::SOMViewSelection(PluginContext *)
    : SOMViewInteractor(":/talipot/gui/icons/i_selection.png", "Select",
                        StandardInteractorPriority::RectangleSelection) {}

void SOMViewSelection::construct() {
  push_back(new MouseSelector());
  push_back(new MousePanNZoomNavigator());
  push_back(new EditColorScaleInteractor());
}

SOMViewProperties::SOMViewProperties(PluginContext *)
    : SOMViewInteractor(":/talipot/gui/icons/i_select.png", "Properties",
                        StandardInteractorPriority::GetInformation) {}

void SOMViewProperties::construct() {
  push_back(new MouseShowElementInfo());
  push_back(new MousePanNZoomNavigator());
  push_back(new EditColorScaleInteractor());
}

SOMViewThreshold::SOMViewThreshold(PluginContext *)
    : SOMViewInteractor(":/i_slider.png", "Threshold Selection",
                        StandardInteractorPriority::ViewInteractor1) {}

void SOMViewThreshold::construct() {
  setConfigurationWidgetText(QString(
      "<H1>Threshold Interactor</H1><p>This interactor is used to select nodes with a value "
      "between those indicated by the two sliders</p><p>Move the each slider to change the "
      "bound.</p><p>Press the Ctrl button to add the new threshold selection to the current "
      "selection. If Ctrl is not pressed the old selection will be replaced</p>"));
  push_back(new MouseNKeysNavigator());
  push_back(new ThresholdInteractor());
}
