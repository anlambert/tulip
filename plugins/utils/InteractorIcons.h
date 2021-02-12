/**
 *
 * Copyright (C) 2020-2021  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#ifndef INTERACTOR_ICONS_H
#define INTERACTOR_ICONS_H

#include <talipot/FontIconManager.h>
#include <talipot/MaterialDesignIcons.h>

namespace tlp {

enum InteractorType {
  AddEdge,
  DeleteElement,
  EditEdgeBends,
  Fisheye,
  GetInformation,
  Lasso,
  MagnifyingGlass,
  Navigation,
  NeighborhoodHighlighting,
  PathFinding,
  RectangleZoom,
  Selection,
  SelectionModifier,
};

static QIcon interactorIcon(InteractorType interactorType, const QColor &iconColor = Qt::white) {
  QIcon icon;
  switch (interactorType) {
  case AddEdge: {
    icon = FontIconManager::icon(MaterialDesignIcons::VectorPolyline, iconColor);
    break;
  }
  case DeleteElement: {
    icon = FontIconManager::icon(MaterialDesignIcons::DeleteOutline, iconColor);
    break;
  }
  case EditEdgeBends: {
    icon = FontIconManager::icon(MaterialDesignIcons::VectorCurve, iconColor, 1, 90);
    break;
  }
  case Fisheye: {
    icon = FontIconManager::icon(MaterialDesignIcons::Web, iconColor);
    break;
  }
  case GetInformation: {
    QIcon backIcon = FontIconManager::icon(MaterialDesignIcons::CursorDefault, iconColor, 0.9, 0,
                                           QPointF(-20, 0));
    QIcon frontIcon =
        FontIconManager::icon(MaterialDesignIcons::Help, iconColor, 0.6, 0, QPointF(40, -20));
    icon = FontIconManager::stackIcons(backIcon, frontIcon);
    break;
  }
  case Lasso: {
    icon = FontIconManager::icon(MaterialDesignIcons::VectorPolygon, iconColor);
    break;
  }
  case MagnifyingGlass: {
    icon = FontIconManager::icon(MaterialDesignIcons::Magnify, iconColor);
    break;
  }
  case Navigation: {
    icon = FontIconManager::icon(MaterialDesignIcons::CursorPointer, iconColor);
    break;
  }
  case NeighborhoodHighlighting: {
    icon = FontIconManager::icon(MaterialDesignIcons::GoogleCirclesExtended, iconColor);
    break;
  }
  case PathFinding: {
    icon = FontIconManager::icon(MaterialDesignIcons::Routes, iconColor);
    break;
  }
  case RectangleZoom: {
    QIcon backIcon = FontIconManager::icon(MaterialDesignIcons::Selection, iconColor);
    QIcon frontIcon = FontIconManager::icon(MaterialDesignIcons::MagnifyPlus, iconColor, 0.6);
    icon = FontIconManager::stackIcons(backIcon, frontIcon);
    break;
  }
  case Selection: {
    icon = FontIconManager::icon(MaterialDesignIcons::Selection, iconColor);
    break;
  }
  case SelectionModifier: {
    QIcon backIcon = FontIconManager::icon(MaterialDesignIcons::Selection, iconColor);
    QIcon frontIcon = FontIconManager::icon(MaterialDesignIcons::ArrowAll, iconColor, 0.6);
    icon = FontIconManager::stackIcons(backIcon, frontIcon);
    break;
  }
  default:
    break;
  }
  return icon;
}

}
#endif // INTERACTOR_ICONS_H