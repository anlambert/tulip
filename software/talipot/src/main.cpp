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

#include <QDir>
#include <QApplication>
#include <QMessageBox>
#include <QProxyStyle>
#include <QRegularExpression>

#include <CrashHandler.h>

#include <talipot/Exception.h>
#include <talipot/Interactor.h>
#include <talipot/Settings.h>
#include <talipot/PythonInterpreter.h>
#include <talipot/GlOffscreenRenderer.h>
#include <talipot/GlTextureManager.h>
#include <talipot/FontIconManager.h>

#include "TalipotMainWindow.h"
#include "SplashScreen.h"
#include "PluginsCenter.h"
#include "ThemeUtils.h"

#ifdef WIN32
#include <windows.h>
#endif

#ifdef interface
#undef interface
#endif

using namespace std;
using namespace tlp;

void usage(const QString &error) {
  int returnCode = 0;

  if (!error.isEmpty()) {
    QMessageBox::warning(nullptr, "Error", error);
    returnCode = 1;
  }

  cout << "Usage: talipot [OPTION] [FILE]" << endl
       << endl
       << "FILE: a graph file supported by Talipot to open. " << endl
       << "List of options:" << endl
       << endl
       << "  --help (-h)\tDisplay this help message and ignore other options." << endl
       << endl;

  exit(returnCode);
}

class TalipotProxyStyle : public QProxyStyle {

public:
  TalipotProxyStyle(const QString &key) : QProxyStyle(key) {}
  TalipotProxyStyle(QStyle *style) : QProxyStyle(style) {}

  QIcon standardIcon(QStyle::StandardPixmap standardIcon, const QStyleOption *option = nullptr,
                     const QWidget *widget = nullptr) const override {
    switch (standardIcon) {
    case QStyle::SP_DialogCancelButton:
      return FontIconManager::icon(MaterialDesignIcons::Cancel, 0.8);
    case QStyle::SP_DialogCloseButton:
      return FontIconManager::icon(MaterialDesignIcons::Close);
    case QStyle::SP_DialogDiscardButton:
      return FontIconManager::icon(MaterialDesignIcons::TrashCanOutline);
    case QStyle::SP_DialogNoButton:
      return FontIconManager::icon(MaterialDesignIcons::Close);
    case QStyle::SP_DialogOkButton:
      return FontIconManager::icon(MaterialDesignIcons::Check);
    case QStyle::SP_DialogSaveButton:
      return FontIconManager::icon(MaterialDesignIcons::FileExportOutline);
    case QStyle::SP_DialogYesButton:
      return FontIconManager::icon(MaterialDesignIcons::Check);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    case QStyle::SP_DialogYesToAllButton:
      return FontIconManager::icon(MaterialDesignIcons::CheckAll);
#endif
    case QStyle::SP_MessageBoxInformation:
      return FontIconManager::icon(MaterialDesignIcons::Information, QColor("#407fb2"));
    case QStyle::SP_MessageBoxWarning:
      return FontIconManager::icon(MaterialDesignIcons::Alert, QColor("#e18d2b"));
    case QStyle::SP_MessageBoxCritical:
      return FontIconManager::icon(MaterialDesignIcons::MinusCircle, QColor("#c42730"));
    case QStyle::SP_MessageBoxQuestion:
      return FontIconManager::icon(MaterialDesignIcons::HelpCircle, QColor("#934db1"));

    default:
      return QProxyStyle::standardIcon(standardIcon, option, widget);
    }
  }
};

int main(int argc, char **argv) {

  CrashHandler::install();
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
  QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);

#ifdef QT_HAS_WEBENGINE
  QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);
#endif

  QApplication talipot(argc, argv);
  talipot.setApplicationName("Talipot");

#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
  // Use Qt Fusion widgets style on MacOS / Windows as default style
  // does not integrate nicely with Talipot custom stylesheet
  talipot.setStyle(new TalipotProxyStyle("Fusion"));
#else
  talipot.setStyle(new TalipotProxyStyle(talipot.style()));
#endif

  setApplicationGuiTheme(Settings::guiTheme());

  // Check arguments
  QString inputFilePath;
  QVariantMap extraParams;

  static QRegularExpression extraParametersRegexp("^\\-\\-([^=]*)=(.*)");
  QRegularExpressionMatch match;

  QStringList args = QApplication::arguments();

  bool debugPluginsLoad = false;

  for (int i = 1; i < args.size(); ++i) {
    QString a = args[i];

    if ((a == "--help") || (a == "-h")) {
      usage("");
    } else if (a == "--debug-plugins-load") {
      debugPluginsLoad = true;
    } else if (a.indexOf(extraParametersRegexp, 0, &match) && match.hasMatch()) {
      extraParams[match.captured(1)] = match.captured(2);
    } else {
      inputFilePath = a;
    }
  }

  initTalipotLib(QStringToTlpString(QApplication::applicationDirPath()).c_str());

#ifdef _MSC_VER
  // Add path to Talipot pdb files generated by Visual Studio
  // (for configurations Debug and RelWithDebInfo)
  // It allows to get a detailed stack trace when Talipot crashes.
  CrashHandler::setExtraSymbolsSearchPaths(tlp::TalipotShareDir + "pdb");
#endif

  // initialize embedded Python interpreter
  PythonInterpreter::instance();

  // initialize Talipot
  QMap<QString, QString> pluginErrors;
  try {
    SplashScreen loader(debugPluginsLoad);
    tlp::initTalipotSoftware(&loader);
    pluginErrors = loader.errors();
  } catch (tlp::Exception &e) {
    QMessageBox::warning(nullptr, "Error", e.what());
    exit(1);
  }

  QFileInfo fileInfo(inputFilePath);

  if (!inputFilePath.isEmpty() && (!fileInfo.exists() || fileInfo.isDir())) {
    usage("File " + inputFilePath + " not found or is a directory");
  }

  // Create and initialize Talipot main window
  TalipotMainWindow &mainWindow = TalipotMainWindow::instance();
  mainWindow.pluginsCenter()->reportPluginErrors(pluginErrors);

  mainWindow.show();
  mainWindow.start(inputFilePath);

  Settings::setFirstRun(false);
  Settings::setFirstTalipotMMRun(false);

  int result = talipot.exec();

  mainWindow.deleteEarly();

  // We need to clear allocated OpenGL resources to avoid a
  // segfault when we close talipot
  GlTextureManager::deleteAllTextures();
  GlOffscreenRenderer::instance().deleteEarly();

  return result;
}
