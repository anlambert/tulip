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

#include <list>
#include <unordered_map>

#include <talipot/GlyphManager.h>
#include <talipot/Glyph.h>

//====================================================
using namespace std;

namespace tlp {
static std::list<std::string> glyphList;
static std::unordered_map<int, std::string> glyphIdToName;
static std::unordered_map<std::string, int> nameToGlyphId;

//====================================================
string GlyphManager::glyphName(int id) {
  auto it = glyphIdToName.find(id);
  if (it != glyphIdToName.end()) {
    return it->second;
  } else {
    assert(false);
    tlp::warning() << "Invalid glyph id: " << id << endl;
    return string("invalid");
  }
}
//====================================================
int GlyphManager::glyphId(const string &name, bool warnIfNotFound) {
  auto it = nameToGlyphId.find(name);
  if (it != nameToGlyphId.end()) {
    return it->second;
  } else {
    if (warnIfNotFound) {
      assert(false);
      tlp::warning() << "Invalid glyph name: \"" << name.c_str() << '"' << endl;
    }
    return 0;
  }
}
//====================================================
void GlyphManager::loadGlyphPlugins() {
  glyphList = PluginsManager::availablePlugins<Glyph>();

  for (std::string pluginName : glyphList) {
    int pluginId = PluginsManager::pluginInformation(pluginName).id();
    glyphIdToName[pluginId] = pluginName;
    nameToGlyphId[pluginName] = pluginId;
  }
}

void GlyphManager::initGlyphList(Graph **graph, GlGraphInputData *glGraphInputData,
                                 MutableContainer<Glyph *> &glyphs) {
  // first destroy current default glyph
  Glyph *defaultGlyph = glyphs.getDefault();

  if (defaultGlyph) {
    delete defaultGlyph;
  }

  // then set a new one
  GlyphContext gc = GlyphContext(graph, glGraphInputData);
  glyphs.setAll(PluginsManager::getPluginObject<Glyph>("3D - Cube OutLined", &gc));

  for (const std::string &glyphName : glyphList) {
    Glyph *newGlyph = PluginsManager::getPluginObject<Glyph>(glyphName, &gc);
    glyphs.set(PluginsManager::pluginInformation(glyphName).id(), newGlyph);
  }
}

void GlyphManager::clearGlyphList(Graph **, GlGraphInputData *, MutableContainer<Glyph *> &glyphs) {

  for (const std::string &glyphName : glyphList) {
    delete glyphs.get(PluginsManager::pluginInformation(glyphName).id());
  }

  delete glyphs.getDefault();
}
}
