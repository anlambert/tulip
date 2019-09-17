/**
 *
 * Copyright (C) 2019  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#ifndef TULIPSPLASHSCREEN_H
#define TULIPSPLASHSCREEN_H

#include <QSplashScreen>
#include <QMap>

#include <tulip/PluginLoader.h>

/**
  @brief a splash screen used when loading plugins.
  Loading plugins in the tulip_app process allows to check for plugins error before loading any
  perspective. It provides the user with some basic preemptive information
  and forbid the use of invalid perspectives.
  */
class TulipSplashScreen : public tlp::PluginLoader, public QSplashScreen {
public:
  TulipSplashScreen();

  void start(const std::string &path) override;
  void loading(const std::string &filename) override;
  void loaded(const tlp::Plugin *info, const std::list<tlp::Dependency> &deps) override;
  void numberOfFiles(int n) override {
    _numberOfFiles = n;
    _fileCounter = 0;
  }
  void aborted(const std::string &filename, const std::string &erreurmsg) override;
  void finished(bool state, const std::string &msg) override;

  const QMap<QString, QString> &errors() {
    return _errors;
  }

protected:
  void drawContents(QPainter *painter) override;

private:
  QString _title;
  QString _message;

  int _fileCounter;
  int _numberOfFiles;

  QMap<QString, QString> _errors;
};

#endif // TULIPSPLASHSCREEN_H
