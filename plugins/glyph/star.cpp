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

#include <talipot/StringProperty.h>
#include <talipot/ColorProperty.h>
#include <talipot/DoubleProperty.h>
#include <talipot/Size.h>
#include <talipot/Coord.h>
#include <talipot/Glyph.h>
#include <talipot/EdgeExtremityGlyph.h>
#include <talipot/GlGraphRenderingParameters.h>
#include <talipot/GlStar.h>
#include <talipot/Graph.h>
#include <talipot/GlGraphInputData.h>
#include <talipot/ViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

static void drawStar(const Color &fillColor, const Color &borderColor, float borderWidth,
                     const std::string &textureName, float lod) {
  static GlStar star(Coord(0, 0, 0), Size(.5, .5, 0), 5);
  star.setFillColor(fillColor);

  if (borderWidth > 0) {
    star.setOutlineMode(true);
    star.setOutlineColor(borderColor);
    star.setOutlineSize(borderWidth);
  } else {
    star.setOutlineMode(false);
  }

  star.setTextureName(textureName);
  star.draw(lod, nullptr);
}

/** \addtogroup glyph */

/// A 2D glyph
/**
 * This glyph draws a textured star using the "viewTexture"
 * node property value. If this property has no value, the star
 * is then colored using the "viewColor" node property value.
 */
class Star : public Glyph {
public:
  GLYPHINFORMATION("2D - Star", "David Auber", "09/07/2002", "Textured Star", "1.0",
                   NodeShape::Star)
  Star(const tlp::PluginContext *context = nullptr);
  ~Star() override;
  void getIncludeBoundingBox(BoundingBox &boundingBox, node) override;
  void draw(node n, float lod) override;
};
PLUGIN(Star)
Star::Star(const tlp::PluginContext *context) : Glyph(context) {}
Star::~Star() {}
void Star::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
  boundingBox[0] = Coord(-0.3f, -0.35f, 0);
  boundingBox[1] = Coord(0.3f, 0.35f, 0);
}
void Star::draw(node n, float lod) {
  string textureName = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (!textureName.empty()) {
    textureName = glGraphInputData->parameters->getTexturePath() + textureName;
  }

  drawStar(glGraphInputData->getElementColor()->getNodeValue(n),
           glGraphInputData->getElementBorderColor()->getNodeValue(n),
           glGraphInputData->getElementBorderWidth()->getNodeValue(n), textureName, lod);
}

class EEStar : public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("2D - Star extremity", "David Auber", "09/07/2002",
                   "Textured Star for edge extremities", "1.0", EdgeExtremityShape::Star)

  EEStar(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}

  void draw(edge e, node, const Color &glyphColor, const Color &borderColor, float lod) override {
    string textureName = edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);

    if (!textureName.empty()) {
      textureName = edgeExtGlGraphInputData->parameters->getTexturePath() + textureName;
    }

    drawStar(glyphColor, borderColor,
             edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e), textureName, lod);
  }
};
PLUGIN(EEStar)

} // end of namespace tlp
