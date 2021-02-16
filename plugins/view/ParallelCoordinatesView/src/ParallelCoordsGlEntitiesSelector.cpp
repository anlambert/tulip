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

#include <talipot/GlMainWidget.h>
#include <talipot/GlGraph.h>

#include "ParallelCoordsGlEntitiesSelector.h"
#include "ParallelCoordinatesView.h"

using namespace std;

namespace tlp {

bool ParallelCoordsGlEntitiesSelector::eventFilter(QObject *widget, QEvent *e) {

  ParallelCoordinatesView *parallelView = static_cast<ParallelCoordinatesView *>(view());
  GlMainWidget *glMainWidget = static_cast<GlMainWidget *>(widget);

  if (e->type() == QEvent::MouseButtonPress) {

    QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(e);

    if (qMouseEv->buttons() == Qt::LeftButton) {

      if (!started) {
        x = qMouseEv->x();
        y = qMouseEv->y();
        w = 0;
        h = 0;
        started = true;
        graph = glMainWidget->getGlGraphInputData()->getGraph();
      }

      return true;
    }
  }

  if (e->type() == QEvent::MouseMove) {

    QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(e);

    if (qMouseEv->buttons() & Qt::LeftButton && started) {
      if ((qMouseEv->x() > 0) && (qMouseEv->x() < glMainWidget->width())) {
        w = qMouseEv->x() - x;
      }

      if ((qMouseEv->y() > 0) && (qMouseEv->y() < glMainWidget->height())) {
        h = qMouseEv->y() - y;
      }

      parallelView->refresh();
      return true;
    }
  }

  if (e->type() == QEvent::MouseButtonRelease) {

    QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(e);

    if (started) {
      Observable::holdObservers();
      bool boolVal = true; // add to selection

      if (qMouseEv->modifiers() != Qt::ControlModifier) {
        if (qMouseEv->modifiers() !=
#if defined(__APPLE__)
            Qt::AltModifier
#else
            Qt::ShiftModifier
#endif
        ) {

          unselectAllEntitiesHandler(parallelView);

        } else
          boolVal = false; // remove from selection
      }

      if ((w == 0) && (h == 0)) {

        selectedEntitiesHandler(parallelView, x, y, boolVal);

      } else {

        if (w < 0) {
          w *= -1;
          x -= w;
        }

        if (h < 0) {
          h *= -1;
          y -= h;
        }

        selectedEntitiesHandler(parallelView, x, y, w, h, boolVal);
      }

      started = false;
      Observable::unholdObservers();
      return true;
    }
  }

  return false;
}
}
