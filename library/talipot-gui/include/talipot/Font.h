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

#ifndef TALIPOT_FONT_H
#define TALIPOT_FONT_H

#include <QObject>
#include <QMap>

#include <talipot/config.h>

namespace tlp {

class TLP_QT_SCOPE Font : public QObject {
  static QMap<QString, int> FONT_IDS;
  static int fontId(const QString &);

  Q_OBJECT
  Q_PROPERTY(bool bold READ isBold WRITE setBold)
  bool _bold;
  Q_PROPERTY(bool italic READ isItalic WRITE setItalic)
  bool _italic;
  Q_PROPERTY(QString fontName READ fontName WRITE setFontName)
  QString _fontName;
  Q_PROPERTY(QString fontFile READ fontFile)
  QString _fontFile;
  Q_PROPERTY(bool exists READ exists)
  Q_PROPERTY(int fontId READ fontId)
  Q_PROPERTY(QString fontFamily READ fontFamily)

  void refreshFontFile();

public:
  static QString talipotFontsDirectory();
  static QStringList installedFontNames();
  static Font fromFile(const QString &);

  explicit Font(QObject *parent = nullptr);
  explicit Font(const QString fontName, QObject *parent = nullptr);
  Font(const Font &);

  Font &operator=(const Font &);
  bool isBold() const;
  bool isItalic() const;
  QString fontName() const;
  QString fontFile() const;
  bool exists() const;
  int fontId() const;
  QString fontFamily() const;

public slots:
  void setItalic(bool);
  void setBold(bool);
  void setFontName(const QString &);
};
}

#endif // TALIPOT_FONT_H
