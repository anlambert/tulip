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

#include "talipot/Font.h"

#include <QFontDatabase>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include <talipot/TlpTools.h>
#include <talipot/TlpQtTools.h>

using namespace tlp;

QMap<QString, int> Font::FONT_IDS = QMap<QString, int>();

QString Font::talipotFontsDirectory() {
  return tlpStringToQString(TalipotBitmapDir) + "fonts/";
}

QStringList Font::installedFontNames() {
  QStringList result;
  QDir installedFontsDir(talipotFontsDirectory());

  for (const QFileInfo &fontDirInfo :
       installedFontsDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs)) {
    QString fontName(fontDirInfo.fileName());
    // sanity checks
    Font normalFont(fontName), boldFont(normalFont), italicFont(normalFont),
        boldItalicFont(normalFont);
    boldFont.setBold(true);
    italicFont.setItalic(true);
    boldItalicFont.setBold(true);
    boldItalicFont.setItalic(true);

    if (!normalFont.exists() || !boldFont.exists() || !italicFont.exists() ||
        !boldItalicFont.exists()) {
      continue;
    }

    result << fontName;
  }

  return result;
}

Font Font::fromFile(const QString &path) {
  Font result("");
  result._fontFile = path;
  result._bold = path.endsWith("_Bold.ttf") || path.endsWith("_Bold_Italic.ttf");
  result._italic = path.endsWith("_Italic.ttf") || path.endsWith("_Bold_Italic.ttf");
  result._fontName = QFileInfo(path).fileName().remove("_Bold").remove("_Italic").remove(".ttf");
  return result;
}

Font &Font::operator=(const Font &other) {
  _bold = other._bold;
  _italic = other._italic;
  _fontName = other._fontName;
  _fontFile = other._fontFile;
  return *this;
}

Font::Font(const Font &other)
    : QObject(other.parent()), _bold(other._bold), _italic(other._italic),
      _fontName(other._fontName), _fontFile(other._fontFile) {}

Font::Font(const QString fontName, QObject *parent)
    : QObject(parent), _bold(false), _italic(false), _fontName(fontName) {
  refreshFontFile();
}

Font::Font(QObject *parent) : QObject(parent), _bold(false), _italic(false) {
  refreshFontFile();
}

bool Font::isBold() const {
  return _bold;
}

bool Font::isItalic() const {
  return _italic;
}

QString Font::fontName() const {
  return _fontName;
}

int Font::fontId(const QString &path) {
  int result = -1;

  if (!FONT_IDS.contains(path)) {
    result = QFontDatabase::addApplicationFont(path);
    FONT_IDS[path] = result;
  } else {
    result = FONT_IDS[path];
  }

  return result;
}

int Font::fontId() const {
  return fontId(_fontFile);
}

QString Font::fontFamily() const {
  QStringList families(QFontDatabase::applicationFontFamilies(fontId()));
  QString family("Unregistered font");

  if (!families.empty()) {
    family = families[0];
  }

  return family;
}

QString Font::fontFile() const {
  return _fontFile;
}

void Font::setItalic(bool i) {
  _italic = i;
  refreshFontFile();
}

void Font::setBold(bool b) {
  _bold = b;
  refreshFontFile();
}

void Font::setFontName(const QString &n) {
  _fontName = n;
  refreshFontFile();
}

bool Font::exists() const {
  return !_fontFile.isEmpty() && QFileInfo(fontFile()).exists();
}

void Font::refreshFontFile() {
  _fontFile = talipotFontsDirectory() + fontName() + "/" + fontName();

  if (isBold()) {
    _fontFile += "_Bold";
  }

  if (isItalic()) {
    _fontFile += "_Italic";
  }

  _fontFile += ".ttf";
}
