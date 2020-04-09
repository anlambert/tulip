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

#ifndef TALIPOT_EDGE_EXTREMITY_GLYPH_MANAGER_H
#define TALIPOT_EDGE_EXTREMITY_GLYPH_MANAGER_H

#include <string>

#include <talipot/config.h>
#include <talipot/MutableContainer.h>

namespace tlp {
class GlGraphInputData;
class EdgeExtremityGlyph;
class Graph;

class TLP_GL_SCOPE EdgeExtremityGlyphManager {

public:
  /**
   * Return the name of glyph with given id
   */
  static std::string glyphName(int id);
  /**
   * Return the id if glyph with given name
   */
  static int glyphId(const std::string &name);
  /**
   * Load glyphs plugins
   */
  static void loadGlyphPlugins();

  /**
   * Create the glyph list and store it in glyphs parameter
   */
  static void initGlyphList(Graph **graph, GlGraphInputData *glGraphInputData,
                            MutableContainer<EdgeExtremityGlyph *> &glyphs);
  /**
   * Clear the glyph list
   */
  static void clearGlyphList(Graph **graph, GlGraphInputData *glGraphInputData,
                             MutableContainer<EdgeExtremityGlyph *> &glyphs);
};
}
#endif // TALIPOT_EDGE_EXTREMITY_GLYPH_MANAGER_H
