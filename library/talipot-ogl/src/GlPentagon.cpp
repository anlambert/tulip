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

#include <talipot/GlPentagon.h>

using namespace std;

namespace tlp {

GlPentagon::GlPentagon(const Coord &position, const Size &size, const Color &fillColor,
                       const Color &outlineColor, bool filled, bool outlined,
                       const string &textureName, float outlineSize)
    : GlRegularPolygon(position, size, 5, fillColor, outlineColor, filled, outlined, textureName,
                       outlineSize) {}
//=====================================================
GlPentagon::~GlPentagon() = default;
}
