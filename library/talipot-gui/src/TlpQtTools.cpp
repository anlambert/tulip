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

#include <GL/glew.h>

#include <talipot/TlpQtTools.h>

#include <ostream>
#include <ios>
#include <unordered_map>

#include <QDebug>
#include <QColorDialog>
#include <QAbstractButton>
#include <QMessageBox>
#include <QImage>
#include <QEvent>
#include <QMetaEnum>
#include <QApplication>
#include <QDir>
#include <QApplication>
#include <QStandardPaths>
#include <QMainWindow>
#include <QPalette>

#include <talipot/DataSet.h>
#include <talipot/Settings.h>
#include <talipot/Interactor.h>
#include <talipot/View.h>
#include <talipot/BooleanProperty.h>
#include <talipot/ColorProperty.h>
#include <talipot/DoubleProperty.h>
#include <talipot/GraphProperty.h>
#include <talipot/IntegerProperty.h>
#include <talipot/LayoutProperty.h>
#include <talipot/SizeProperty.h>
#include <talipot/StringProperty.h>
#include <talipot/SystemDefinition.h>
#include <talipot/TlpTools.h>
#include <talipot/PluginLibraryLoader.h>
#include <talipot/GlyphManager.h>
#include <talipot/EdgeExtremityGlyphManager.h>
#include <talipot/OpenGlConfigManager.h>
#include <talipot/GlTextureManager.h>
#include <talipot/MetaTypes.h>
#include <talipot/PythonVersionChecker.h>
#include <talipot/FileDownloader.h>
#include <talipot/ItemEditorCreators.h>
#include <talipot/GlOffscreenRenderer.h>

/**
 * For openDataSetDialog function : see OpenDataSet.cpp
 */
using namespace std;
using namespace tlp;

/**
 * Init property type to property label conversion map
 **/
static unordered_map<string, QString> &buildPropertyTypeToPropertyTypeLabelMap() {
  static unordered_map<string, QString> propertyTypeToPropertyTypeLabel;
  propertyTypeToPropertyTypeLabel[BooleanProperty::propertyTypename] = QString("Boolean");
  propertyTypeToPropertyTypeLabel[ColorProperty::propertyTypename] = QString("Color");
  propertyTypeToPropertyTypeLabel[DoubleProperty::propertyTypename] = QString("Double");
  propertyTypeToPropertyTypeLabel[GraphProperty::propertyTypename] = QString("Graph");
  propertyTypeToPropertyTypeLabel[IntegerProperty::propertyTypename] = QString("Integer");
  propertyTypeToPropertyTypeLabel[LayoutProperty::propertyTypename] = QString("Layout");
  propertyTypeToPropertyTypeLabel[SizeProperty::propertyTypename] = QString("Size");
  propertyTypeToPropertyTypeLabel[StringProperty::propertyTypename] = QString("String");
  propertyTypeToPropertyTypeLabel[BooleanVectorProperty::propertyTypename] =
      QString("BooleanVector");
  propertyTypeToPropertyTypeLabel[ColorVectorProperty::propertyTypename] = QString("ColorVector");
  propertyTypeToPropertyTypeLabel[CoordVectorProperty::propertyTypename] = QString("CoordVector");
  propertyTypeToPropertyTypeLabel[DoubleVectorProperty::propertyTypename] = QString("DoubleVector");
  propertyTypeToPropertyTypeLabel[IntegerVectorProperty::propertyTypename] =
      QString("IntegerVector");
  propertyTypeToPropertyTypeLabel[SizeVectorProperty::propertyTypename] = QString("SizeVector");
  propertyTypeToPropertyTypeLabel[StringVectorProperty::propertyTypename] = QString("StringVector");
  return propertyTypeToPropertyTypeLabel;
}

// Property type to property label conversion map
static const unordered_map<string, QString> &propertyTypeToPropertyTypeLabelMap =
    buildPropertyTypeToPropertyTypeLabelMap();
/**
 * Init property type label to property type conversion map
 **/
static map<QString, string> buildPropertyTypeLabelToPropertyTypeMap() {
  static map<QString, string> propertyTypeLabelToPropertyType;
  propertyTypeLabelToPropertyType[QString("Boolean")] = BooleanProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Color")] = ColorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Double")] = DoubleProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Graph")] = GraphProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Integer")] = IntegerProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Layout")] = LayoutProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("Size")] = SizeProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("String")] = StringProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("BooleanVector")] =
      BooleanVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("ColorVector")] = ColorVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("CoordVector")] = CoordVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("DoubleVector")] = DoubleVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("IntegerVector")] =
      IntegerVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("SizeVector")] = SizeVectorProperty::propertyTypename;
  propertyTypeLabelToPropertyType[QString("StringVector")] = StringVectorProperty::propertyTypename;
  return propertyTypeLabelToPropertyType;
}
// Property type label to property type conversion map
static const map<QString, string> &propertyTypeLabelToPropertyTypeMap =
    buildPropertyTypeLabelToPropertyTypeMap();

// Allow to print a human readable representation of Qt events,
// for debugging purpose (through the use of qDebug() )
QDebug operator<<(QDebug str, const QEvent *ev) {
  str << "QEvent";

  if (ev) {
    static int eventEnumIndex = QEvent::staticMetaObject.indexOfEnumerator("Type");
    QString name = QEvent::staticMetaObject.enumerator(eventEnumIndex).valueToKey(ev->type());

    if (!name.isEmpty()) {
      str << name;
    } else {
      str << ev->type();
    }
  }

  return str.maybeSpace();
}

namespace tlp {

bool getColorDialog(const QColor &color, QWidget *parent, const QString &title, QColor &result) {

  QColor newColor = QColorDialog::getColor(
      color, parent, title, QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);

  if (newColor.isValid()) {
    result = newColor;
    return true;
  } else {
    return false;
  }
}

QString propertyTypeToPropertyTypeLabel(const string &typeName) {
  auto it = propertyTypeToPropertyTypeLabelMap.find(typeName);
  return it != propertyTypeToPropertyTypeLabelMap.end() ? it->second : QString();
}

string propertyTypeLabelToPropertyType(const QString &typeNameLabel) {
  auto it = propertyTypeLabelToPropertyTypeMap.find(typeNameLabel);
  return it != propertyTypeLabelToPropertyTypeMap.end() ? it->second : string();
}

QString getPluginPackageName(const QString &pluginName) {
  return pluginName.simplified().remove(' ').toLower() + "-" + TALIPOT_VERSION + "-" + OS_PLATFORM +
         OS_ARCHITECTURE + "-" + OS_COMPILER + ".zip";
}

QString getPluginLocalInstallationDir() {
  return QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0) + "/plugins";
}

QString localPluginsPath() {
  return getPluginLocalInstallationDir() + '/';
}

// we define a specific GlTextureLoader allowing to load a GlTexture
// from a QImage
class GlTextureFromQImageLoader : public GlTextureLoader {
public:
  // redefine the inherited method
  bool loadTexture(const std::string &filename, GlTexture &glTexture) override {

    QImage image;

    QString qFilename = tlpStringToQString(filename);

    if (qFilename.startsWith("http")) {
      FileDownloader fileDownloader;
      QByteArray imageData = fileDownloader.download(QUrl(qFilename));

      if (imageData.isEmpty()) {
        tlp::error() << "Error when downloading texture from url " << filename.c_str() << std::endl;
        return false;
      } else {
        bool imageLoaded = image.loadFromData(imageData);

        if (!imageLoaded) {
          tlp::error() << "Error when loading texture from url " << filename.c_str() << std::endl;
          return false;
        }
      }
    } else {

      QFile imageFile(qFilename);

      if (imageFile.open(QIODevice::ReadOnly)) {
        image.loadFromData(imageFile.readAll());
      }

      if (image.isNull()) {
        if (!imageFile.exists()) {
          tlp::error() << "Error when loading texture, the file named \"" << filename.c_str()
                       << "\" does not exist" << std::endl;
        } else {
          tlp::error() << "Error when loading texture from " << filename.c_str() << std::endl;
        }

        return false;
      }
    }

    // store icon preview of the loaded texture in the icon pool
    // used by the Tulip spreadsheet view
    if (!image.isNull()) {
      addIconToPool(qFilename, QIcon(QPixmap::fromImage(image)));
    }

    bool canUseMipmaps = OpenGlConfigManager::isExtensionSupported("GL_ARB_framebuffer_object") ||
                         OpenGlConfigManager::isExtensionSupported("GL_EXT_framebuffer_object");

    unsigned int width = image.width();
    unsigned int height = image.height();

    image = image.mirrored().convertToFormat(QImage::Format_RGBA8888);

    glTexture.width = width;
    glTexture.height = height;

    glGenTextures(1, &glTexture.id);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, glTexture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 image.constBits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (canUseMipmaps) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glDisable(GL_TEXTURE_2D);

    return true;
  }
};

void initTalipotSoftware(tlp::PluginLoader *loader) {

  QLocale::setDefault(QLocale(QLocale::English));
  Settings::instance().applyProxySettings();
  Settings::instance().initSeedOfRandomSequence();

  QDir::home().mkpath(tlp::localPluginsPath());
  QLocale::setDefault(QLocale(QLocale::English));

#if defined(__APPLE__)
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/../");
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/../lib/");
#endif

  tlp::initTalipotLib();
  initQTypeSerializers();
  // initialize Texture loader
  GlTextureManager::setTextureLoader(new GlTextureFromQImageLoader());
  // Load plugins
  tlp::PluginLibraryLoader::loadPluginsFromDir(
      tlp::TalipotPluginsPath, loader,
      QStringToTlpString(tlp::getPluginLocalInstallationDir()) + "/lib/talipot");
  tlp::PluginLibraryLoader::loadPluginsFromDir(
      QStringToTlpString(tlp::getPluginLocalInstallationDir()), loader);
  tlp::PluginsManager::checkLoadedPluginsDependencies(loader);
  tlp::InteractorLister::initInteractorsDependencies();
  tlp::GlyphManager::loadGlyphPlugins();
  tlp::EdgeExtremityGlyphManager::loadGlyphPlugins();

  // Explicitely create a shared OpenGL context to
  // ensure it is initialized before using it
  GlOffscreenRenderer::instance().getOpenGLContext();
}

// tlp::debug redirection
class QDebugOStream : public std::ostream {
  class QDebugStreamBuf : public std::streambuf {
  protected:
    string buf;
    int_type overflow(int c) override {
      if (c == '\n') {
        qDebug() << buf;
        buf.clear();
      } else {
        buf += c;
      }

      return c;
    }

    std::streamsize xsputn(const char *p, std::streamsize n) override {
      if (p[n - 1] == '\n') {
        buf += std::string(p, n - 1);
        qDebug() << buf.c_str();
        buf.clear();
      } else {
        buf += std::string(p, n);
      }

      return n;
    }
  };

  QDebugStreamBuf qDebugBuf;

public:
  QDebugOStream() : std::ostream(&qDebugBuf) {}
};

static QDebugOStream *qDebugStream = nullptr;

void redirectDebugOutputToQDebug() {
  if (qDebugStream == nullptr) {
    qDebugStream = new QDebugOStream();
  }

  tlp::setDebugOutput(*qDebugStream);
}

// tlp::warning redirection
class QWarningOStream : public std::ostream {
  class QWarningStreamBuf : public std::streambuf {
  protected:
    string buf;
    int_type overflow(int c) override {
      if (c == '\n') {
        qWarning() << buf.c_str();
        buf.clear();
      } else {
        buf += c;
      }

      return c;
    }

    std::streamsize xsputn(const char *p, std::streamsize n) override {
      if (p[n - 1] == '\n') {
        buf += std::string(p, n - 1);
        qWarning() << buf.c_str();
        buf.clear();
      } else {
        buf += std::string(p, n);
      }

      return n;
    }
  };

  QWarningStreamBuf qWarningBuf;

public:
  QWarningOStream() : std::ostream(&qWarningBuf) {}
};

static QWarningOStream *qWarningStream = nullptr;

void redirectWarningOutputToQWarning() {
  if (qWarningStream == nullptr) {
    qWarningStream = new QWarningOStream();
  }

  tlp::setWarningOutput(*qWarningStream);
}

// tlp::error redirection
class QErrorOStream : public std::ostream {
  class QErrorStreamBuf : public std::streambuf {
  protected:
    string buf;
    int_type overflow(int c) override {
      if (c == '\n') {
        qCritical() << buf.c_str();
        buf.clear();
      } else {
        buf += c;
      }

      return c;
    }

    std::streamsize xsputn(const char *p, std::streamsize n) override {
      if (p[n - 1] == '\n') {
        buf += std::string(p, n - 1);
        qCritical() << buf.c_str();
        buf.clear();
      } else {
        buf += std::string(p, n);
      }

      return n;
    }
  };

  QErrorStreamBuf qErrorBuf;

public:
  QErrorOStream() : std::ostream(&qErrorBuf) {}
};

static QErrorOStream *qErrorStream = nullptr;

void redirectErrorOutputToQCritical() {
  if (qErrorStream == nullptr) {
    qErrorStream = new QErrorOStream();
  }

  tlp::setErrorOutput(*qErrorStream);
}

class NoQtUserInputFilter : public QObject {
protected:
  bool eventFilter(QObject *obj, QEvent *event) override;
};

bool NoQtUserInputFilter::eventFilter(QObject *, QEvent *event) {
  switch (event->type()) {
  case QEvent::KeyPress:
  case QEvent::KeyRelease:
  case QEvent::MouseButtonPress:
  case QEvent::MouseButtonDblClick:
  case QEvent::MouseMove:
  case QEvent::HoverEnter:
  case QEvent::HoverLeave:
  case QEvent::HoverMove:
  case QEvent::DragEnter:
  case QEvent::DragLeave:
  case QEvent::DragMove:
  case QEvent::Drop:
    return true;

  default:
    return false;
  }
}

static NoQtUserInputFilter *disableQtUserInputFilter = nullptr;

void disableQtUserInput() {
  if (disableQtUserInputFilter) {
    return;
  }

  disableQtUserInputFilter = new NoQtUserInputFilter();
  QCoreApplication::instance()->installEventFilter(disableQtUserInputFilter);
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

void enableQtUserInput() {
  if (!disableQtUserInputFilter) {
    return;
  }

  QCoreApplication::instance()->removeEventFilter(disableQtUserInputFilter);
  delete disableQtUserInputFilter;
  disableQtUserInputFilter = nullptr;
  QApplication::restoreOverrideCursor();
}

QMainWindow *getMainWindow() {
  for (QWidget *widget : qApp->topLevelWidgets()) {
    if (QMainWindow *mainWindow = qobject_cast<QMainWindow *>(widget)) {
      return mainWindow;
    }
  }
  return nullptr;
}

bool applicationIsInDarkMode() {
  return backgroundColor().lightnessF() < 0.5;
}

const QColor darkColor = QColor(50, 50, 50);

const QColor &backgroundColor() {
  return QApplication::palette().color(QPalette::Base);
}

const QColor &textColor() {
  return QApplication::palette().color(QPalette::WindowText);
}

}