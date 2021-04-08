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

#include <talipot/EdgeExtremityGlyph.h>
#include <talipot/EdgeExtremityGlyphManager.h>
#include <talipot/Glyph.h>
#include <talipot/ViewSettings.h>

using namespace std;

namespace tlp {
static std::list<std::string> plugins;
static std::unordered_map<int, std::string> eeglyphIdToName;
static std::unordered_map<std::string, int> nameToEeGlyphId;

//====================================================
string EdgeExtremityGlyphManager::glyphName(int id) {
  if (id == EdgeExtremityShape::None) {
    return string("NONE");
  }

  auto it = eeglyphIdToName.find(id);
  if (it != eeglyphIdToName.end()) {
    return it->second;
  } else {
    tlp::warning() << __PRETTY_FUNCTION__ << endl;
    tlp::warning() << "Invalid glyph id" << endl;
    return string("invalid");
  }
}
//====================================================
int EdgeExtremityGlyphManager::glyphId(const string &name) {
  if (name == "NONE") {
    return EdgeExtremityShape::None;
  }

  auto it = nameToEeGlyphId.find(name);
  if (it != nameToEeGlyphId.end()) {
    return it->second;
  } else {
    tlp::warning() << __PRETTY_FUNCTION__ << endl;
    tlp::warning() << "Invalid glyph name" << endl;
    return 0;
  }
}
//====================================================
void EdgeExtremityGlyphManager::loadGlyphPlugins() {
  plugins = PluginsManager::availablePlugins<EdgeExtremityGlyph>();

  for (const auto &pluginName : plugins) {
    int pluginId = PluginsManager::pluginInformation(pluginName).id();
    eeglyphIdToName[pluginId] = pluginName;
    nameToEeGlyphId[pluginName] = pluginId;
  }
}
//====================================================
void EdgeExtremityGlyphManager::initGlyphList(Graph **graph, GlGraphInputData *glGraphInputData,
                                              MutableContainer<EdgeExtremityGlyph *> &glyphs) {
  GlyphContext gc = GlyphContext(graph, glGraphInputData);
  glyphs.setAll(nullptr);

  for (const auto &glyphName : plugins) {
    auto *newGlyph = PluginsManager::getPluginObject<EdgeExtremityGlyph>(glyphName, &gc);
    glyphs.set(PluginsManager::pluginInformation(glyphName).id(), newGlyph);
  }
}

void EdgeExtremityGlyphManager::clearGlyphList(Graph **, GlGraphInputData *,
                                               MutableContainer<EdgeExtremityGlyph *> &glyphs) {

  for (const auto &glyphName : plugins) {
    delete glyphs.get(PluginsManager::pluginInformation(glyphName).id());
  }
}
}
