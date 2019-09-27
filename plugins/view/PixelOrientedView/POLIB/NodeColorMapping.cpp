/**
 *
 * Copyright (C) 2019  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include "NodeColorMapping.h"

#include <talipot/Color.h>
#include <talipot/ColorProperty.h>
#include <talipot/BooleanProperty.h>

using namespace tlp;
using namespace std;

const Color COLOR_SELECT(255, 102, 255, 255);

namespace pocore {

NodeColorMapping::NodeColorMapping(Graph *graph) : graph(graph) {}

RGBA NodeColorMapping::getColor(const double &, const unsigned int itemId) const {
  RGBA ret;

  if (graph->getProperty<BooleanProperty>("viewSelection")->getNodeValue(node(itemId))) {
    ret[0] = COLOR_SELECT.getR();
    ret[1] = COLOR_SELECT.getG();
    ret[2] = COLOR_SELECT.getB();
    ret[3] = COLOR_SELECT.getA();
  } else {
    Color nodeColor = graph->getProperty<ColorProperty>("viewColor")->getNodeValue(node(itemId));
    ret[0] = nodeColor.getR();
    ret[1] = nodeColor.getG();
    ret[2] = nodeColor.getB();
    ret[3] = nodeColor.getA();
  }

  return ret;
}
} // namespace pocore