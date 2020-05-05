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
#include <talipot/Size.h>
#include <talipot/Coord.h>
#include <talipot/Glyph.h>
#include <talipot/EdgeExtremityGlyph.h>
#include <talipot/GlGraphRenderingParameters.h>
#include <talipot/Graph.h>
#include <talipot/GlBox.h>
#include <talipot/GlGraphInputData.h>
#include <talipot/ViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

/** \addtogroup glyph */

/// A 3D glyph.
/** This glyph draws a textured cube using the "viewTexture" node
 * property value. If this property has no value, the cube is then colored
 * using the "viewColor" node property value.
 */
class Cube : public NoShaderGlyph {
public:
  GLYPHINFORMATION("3D - Cube", "Bertrand Mathieu", "09/07/2002", "Textured cube", "1.0",
                   NodeShape::Cube)
  Cube(const tlp::PluginContext *context = nullptr);
  ~Cube() override;
  void draw(node n, float lod) override;
  Coord getAnchor(const Coord &vector) const override;

protected:
};
PLUGIN(Cube)
Cube::Cube(const tlp::PluginContext *context) : NoShaderGlyph(context) {}
Cube::~Cube() {}
void Cube::draw(node n, float lod) {
  string textureName = glGraphInputData->getElementTexture()->getNodeValue(n);
  if (!textureName.empty()) {
    textureName = textureName + glGraphInputData->parameters->getTexturePath();
  }

  GlBox::draw(glGraphInputData->getElementColor()->getNodeValue(n),
              glGraphInputData->getElementColor()->getNodeValue(n),
              glGraphInputData->getElementBorderWidth()->getNodeValue(n), textureName, lod);
}
Coord Cube::getAnchor(const Coord &vector) const {
  return GlBox::getAnchor(vector);
}

class EECube : public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("3D - Cube extremity", "Bertrand Mathieu", "09/07/2002",
                   "Textured cube for edge extremities", "1.0", EdgeExtremityShape::Cube)

  EECube(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}

  void draw(edge e, node, const Color &glyphColor, const Color &borderColor, float lod) override {
    string textureName = edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);
    if (!textureName.empty()) {
      textureName = textureName + edgeExtGlGraphInputData->parameters->getTexturePath();
    }

    glEnable(GL_LIGHTING);
    GlBox::draw(glyphColor, borderColor,
                edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e), textureName,
                lod);
    glDisable(GL_LIGHTING);
  }
};

PLUGIN(EECube)

} // end of namespace tlp
