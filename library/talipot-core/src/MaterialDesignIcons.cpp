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

#include <talipot/MaterialDesignIcons.h>
#include <talipot/TlpTools.h>

#include <unordered_map>

using namespace std;
using namespace tlp;

namespace tlp {

static vector<string> iconsNames;

#include "MaterialDesignIconsData.cpp"

string MaterialDesignIcons::getTTFLocation() {
  return TalipotShareDir + "fonts/MaterialDesignIcons/" + "materialdesignicons-webfont.ttf";
}

string MaterialDesignIcons::getWOFF2Location() {
  return TalipotShareDir + "fonts/MaterialDesignIcons/" + "materialdesignicons-webfont.woff2";
}

bool MaterialDesignIcons::isIconSupported(const string &iconName) {
  return iconCodePoint.find(iconName) != iconCodePoint.end();
}

const vector<string> &MaterialDesignIcons::getSupportedIcons() {
  if (iconsNames.empty()) {
    iconsNames.reserve(iconCodePoint.size());
    for (const auto &[iconName, codePoint] : iconCodePoint) {
      iconsNames.push_back(iconName);
    }
  }
  return iconsNames;
}

unsigned int MaterialDesignIcons::getIconCodePoint(const string &iconName) {
  if (const auto it = iconCodePoint.find(iconName); it != iconCodePoint.end()) {
    return it->second;
  }
  return 0;
}

string MaterialDesignIcons::getIconFamily(const string &) {
  return "materialdesignicons";
}

string MaterialDesignIcons::getIconUtf8String(const string &iconName) {
  return utf32to8(u32string(1, static_cast<char32_t>(iconCodePoint.at(iconName))));
}
}
