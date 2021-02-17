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

#ifndef ZOOM_AND_PAN_HIGHLIGHTER_H
#define ZOOM_AND_PAN_HIGHLIGHTER_H

#include "PathHighlighter.h"

#include <QObject>

namespace tlp {

class ZoomAndPanHighlighter : public PathHighlighter, public QObject {
public:
  ZoomAndPanHighlighter() : PathHighlighter("Zoom and pan") {}
  void highlight(const PathFinder *parent, tlp::GlWidget *glWidget, tlp::BooleanProperty *selection,
                 tlp::node src, tlp::node tgt) override;
  void draw(tlp::GlWidget *glWidget) override;
  bool isConfigurable() const override;
  QWidget *getConfigurationWidget() override;
};
}
#endif // ZOOM_AND_PAN_HIGHLIGHTER_H
