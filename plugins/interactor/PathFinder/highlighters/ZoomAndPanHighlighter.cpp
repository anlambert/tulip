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

#include "ZoomAndPanHighlighter.h"

#include <talipot/GlView.h>
#include <talipot/DrawingTools.h>
#include <talipot/GlGraph.h>
#include <talipot/QtGlSceneZoomAndPanAnimator.h>

using namespace std;
using namespace tlp;

void ZoomAndPanHighlighter::highlight(const PathFinder *, GlWidget *glWidget,
                                      BooleanProperty *selection, node, node) {
  GlGraphInputData *inputData(getInputData(glWidget));
  Graph *graph = glWidget->getScene()->getGlGraph()->getGraph();
  LayoutProperty *layout = inputData->getElementLayout();
  BoundingBox bbox(computeBoundingBox(graph, layout, inputData->getElementSize(),
                                      inputData->getElementRotation(), selection));

  QtGlSceneZoomAndPanAnimator animator(glWidget, bbox);
  animator.animateZoomAndPan();
}

void ZoomAndPanHighlighter::draw(GlWidget *) {}

bool ZoomAndPanHighlighter::isConfigurable() const {
  return false;
}

QWidget *ZoomAndPanHighlighter::getConfigurationWidget() {
  return nullptr;
}
