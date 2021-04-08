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

#include <talipot/FontAwesome.h>
#include <talipot/TlpTools.h>

#include <map>
#include <unordered_map>

using namespace std;
using namespace tlp;

static vector<string> iconsNames;
static map<string, string> iconFamily = {
    {"fab", "fa-brands-400"}, {"far", "fa-regular-400"}, {"fas", "fa-solid-900"}};

#include "FontAwesomeData.cpp"

string FontAwesome::getTTFLocation(const string &iconName) {
  return TalipotShareDir + "fonts/FontAwesome/" + getIconFamily(iconName) + ".ttf";
}

string FontAwesome::getWOFFLocation(const string &iconName) {
  return TalipotShareDir + "fonts/FontAwesome/" + getIconFamily(iconName) + ".woff";
}

string FontAwesome::getWOFF2Location(const string &iconName) {
  return TalipotShareDir + "fonts/FontAwesome/" + getIconFamily(iconName) + ".woff2";
}

bool FontAwesome::isIconSupported(const string &iconName) {
  return iconCodePoint.find(iconName) != iconCodePoint.end();
}

const vector<string> &FontAwesome::getSupportedIcons() {
  if (iconsNames.empty()) {
    iconsNames.reserve(iconCodePoint.size());
    for (const auto &[iconName, codePoint] : iconCodePoint) {
      iconsNames.push_back(iconName);
    }
  }
  return iconsNames;
}

unsigned int FontAwesome::getIconCodePoint(const string &iconName) {
  if (const auto it = iconCodePoint.find(iconName); it != iconCodePoint.end()) {
    return it->second;
  }
  return 0;
}

string FontAwesome::getIconFamily(const string &iconName) {
  string prefix = iconName.substr(0, 3);
  if (prefix.length() == 3 && iconFamily.find(prefix) != iconFamily.end()) {
    return iconFamily[prefix];
  }
  return "";
}

string FontAwesome::getIconUtf8String(const string &iconName) {
  return utf32to8(u32string(1, static_cast<char32_t>(iconCodePoint.at(iconName))));
}
