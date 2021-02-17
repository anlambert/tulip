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

#ifndef ZOOM_UTILS_H
#define ZOOM_UTILS_H

#include <cmath>

namespace tlp {
class GlWidget;
struct BoundingBox;

void zoomOnScreenRegion(GlWidget *glWidget, const BoundingBox &boundingBox,
                        const bool optimalPath = true, const double velocity = 1.1,
                        const double p = std::sqrt(1.6));
void zoomOnScreenRegionWithoutAnimation(GlWidget *glWidget, const BoundingBox &boundingBox);
}
#endif // ZOOM_UTILS_H
