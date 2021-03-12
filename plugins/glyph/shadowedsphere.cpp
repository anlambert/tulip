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

#include <talipot/ViewSettings.h>
#include <talipot/Glyph.h>
#include <talipot/AroundTexturedSphere.h>

using namespace std;
using namespace tlp;

namespace tlp {

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/**
 * This glyph draws a sphere with a bottom shadow
 */
class BottomShadowSphere : public AroundTexturedSphere {
public:
  GLYPHINFORMATION("3D - Sphere with bottom shadow", "Patrick Mary", "09/03/2018",
                   "Sphere with its drop shadow on bottom", "1.0", NodeShape::BottomShadowedSphere)
  BottomShadowSphere(const tlp::PluginContext *context = nullptr)
      : AroundTexturedSphere(context, "bottomShadowTexture.png") {}
};

PLUGIN(BottomShadowSphere)

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/**
 * This glyph draws a sphere with a left bottom shadow
 */
class LeftBottomShadowSphere : public AroundTexturedSphere {
public:
  GLYPHINFORMATION("3D - Sphere with left bottom shadow", "Patrick Mary", "09/03/2018",
                   "Sphere with its drop shadow on bottom left", "1.0",
                   NodeShape::LeftBottomShadowedSphere)
  LeftBottomShadowSphere(const tlp::PluginContext *context = nullptr)
      : AroundTexturedSphere(context, "leftBottomShadowTexture.png") {}
};

PLUGIN(LeftBottomShadowSphere)

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/**
 * This glyph draws a sphere with a right bottom shadow
 */
class RightBottomShadowSphere : public AroundTexturedSphere {
public:
  GLYPHINFORMATION("3D - Sphere with right bottom shadow", "Patrick Mary", "09/03/2018",
                   "Sphere with its drop shadow on right bottom", "1.0",
                   NodeShape::RightBottomShadowedSphere)
  RightBottomShadowSphere(const tlp::PluginContext *context = nullptr)
      : AroundTexturedSphere(context, "rightBottomShadowTexture.png") {}
};

PLUGIN(RightBottomShadowSphere)

}
