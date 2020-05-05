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

#include "talipot/Settings.h"

#include <talipot/MetaTypes.h>
#include <talipot/PropertyTypes.h>
#include <talipot/GlGraphStaticData.h>
#include <talipot/GlyphManager.h>
#include <talipot/Release.h>
#include <talipot/ViewSettings.h>
#include <talipot/TlpTools.h>
#include <talipot/TlpQtTools.h>

#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

using namespace tlp;
using namespace std;

Settings *Settings::_instance = nullptr;

static const QString TS_RemoteLocations = "app/remote_locations";
static const QString TS_RecentDocuments = "app/recent_documents";
static const QString TS_PluginsToRemove = "app/pluginsToRemove";
static const QString TS_DefaultColor = "graph/defaults/color/";
static const QString TS_DefaultLabelColor = "graph/defaults/color/labels";
static const QString TS_DefaultSize = "graph/defaults/size/";
static const QString TS_DefaultShape = "graph/defaults/shape/";
static const QString TS_DefaultSelectionColor = "graph/defaults/selectioncolor/";
// add a specific key which must never exists in user settings
static const QString TS_DefaultOfDefault = "never written in user file";
static const QString TS_FavoriteAlgorithms = "app/algorithms/favorites";

static const QString TS_FirstRun = "app/talipot/firstRun";
static const QString TS_FirstRunMM = QString("app/") + TALIPOT_MM_VERSION + "/firstRun";

static const QString TS_ProxyEnabled = "app/proxy/enabled";
static const QString TS_ProxyType = "app/proxy/type";
static const QString TS_ProxyHost = "app/proxy/host";
static const QString TS_ProxyPort = "app/proxy/port";
static const QString TS_ProxyUseAuth = "app/proxy/user";
static const QString TS_ProxyUsername = "app/proxy/useAuth";
static const QString TS_ProxyPassword = "app/proxy/passwd";

static const QString TS_AutomaticDisplayDefaultViews = "graph/auto/defaultViews";
static const QString TS_AutomaticPerfectAspectRatio = "graph/auto/ratio";
static const QString TS_AutomaticCentering = "graph/auto/center";
static const QString TS_ViewOrtho = "graph/auto/ortho";
static const QString TS_AutomaticMapMetric = "graph/auto/colors";

static const QString TS_ResultPropertyStored = "graph/auto/result";

static const QString TS_LogPluginCall = "graph/auto/log";
static const QString TS_UseTlpbFileFormat = "graph/auto/usetlpb";
static const QString TS_SeedForRandomSequence = "graph/auto/seed";

static const QString TS_WarnUserAboutGraphicsCard = "app/warn_about_graphics_card";
static const QString TS_LoggerAnchored = "app/gui/logger_anchored";
static const QString TS_PythonIDEAnchored = "app/gui/python_ide_anchored";

Settings::Settings() : QSettings("TalipotFramework", "Talipot") {}

Settings &Settings::instance() {
  if (!_instance) {
    _instance = new Settings;
    ViewSettings::instance().addListener(_instance);
    GlDefaultSelectionColorManager::setManager(_instance);
  }

  return *_instance;
}

void Settings::synchronizeViewSettings() {
  ViewSettings::instance().setDefaultColor(NODE, defaultColor(NODE));
  ViewSettings::instance().setDefaultColor(EDGE, defaultColor(EDGE));
  ViewSettings::instance().setDefaultSize(NODE, defaultSize(NODE));
  ViewSettings::instance().setDefaultSize(EDGE, defaultSize(EDGE));
  ViewSettings::instance().setDefaultShape(NODE, defaultShape(NODE));
  ViewSettings::instance().setDefaultShape(EDGE, defaultShape(EDGE));
  ViewSettings::instance().setDefaultLabelColor(defaultLabelColor());
}

QStringList Settings::recentDocuments() const {
  return value(TS_RecentDocuments).toStringList();
}

void Settings::checkRecentDocuments() {
  QList<QVariant> recentDocumentsValue = value(TS_RecentDocuments).toList();

  for (const QVariant &doc : recentDocumentsValue) {
    if (!QFileInfo(doc.toString()).exists()) {
      recentDocumentsValue.removeAll(doc);
    }
  }

  setValue(TS_RecentDocuments, recentDocumentsValue);
}

void Settings::addToRecentDocuments(const QString &name) {
  QList<QVariant> recentDocumentsValue = value(TS_RecentDocuments).toList();

  if (recentDocumentsValue.contains(name)) {
    recentDocumentsValue.removeAll(name);
  }

  recentDocumentsValue.push_front(name);

  while (recentDocumentsValue.size() > 5) {
    recentDocumentsValue.pop_back();
  }

  setValue(TS_RecentDocuments, recentDocumentsValue);
}

void Settings::addRemoteLocation(const QString &remoteLocation) {
  QStringList remoteLocations = value(TS_RemoteLocations).toStringList();

  if (!remoteLocations.contains(remoteLocation)) {
    remoteLocations.append(remoteLocation);
  }

  setValue(TS_RemoteLocations, remoteLocations);
}

void Settings::removeRemoteLocation(const QString &remoteLocation) {
  QStringList remoteLocations = value(TS_RemoteLocations).toStringList();

  if (remoteLocations.contains(remoteLocation)) {
    remoteLocations.removeOne(remoteLocation);
  }

  setValue(TS_RemoteLocations, remoteLocations);
}

const QStringList Settings::remoteLocations() const {
  return value(TS_RemoteLocations).toStringList();
}

const QStringList Settings::pluginsToRemove() const {
  return value(TS_PluginsToRemove).toStringList();
}

void Settings::markPluginForRemoval(const QString &pluginLibrary) {
  QStringList markedPlugins = value(TS_PluginsToRemove).toStringList();

  if (!markedPlugins.contains(pluginLibrary)) {
    markedPlugins.append(pluginLibrary);
  }

  setValue(TS_PluginsToRemove, markedPlugins);
}

void Settings::unmarkPluginForRemoval(const QString &pluginLibrary) {
  QStringList markedPlugins = value(TS_PluginsToRemove).toStringList();

  if (markedPlugins.contains(pluginLibrary)) {
    markedPlugins.removeAll(pluginLibrary);
  }

  setValue(TS_PluginsToRemove, markedPlugins);
}

QString Settings::elementKey(const QString &configEntry, tlp::ElementType elem) {
  return configEntry + (elem == tlp::NODE ? "node" : "edge");
}

tlp::Color Settings::defaultColor(tlp::ElementType elem, bool talipotDefault) {
  QString val = value(elementKey(talipotDefault ? TS_DefaultOfDefault : TS_DefaultColor, elem),
                      (elem == tlp::NODE ? "(255, 95, 95)" : "(180,180,180)"))
                    .toString();
  Color result;
  ColorType::fromString(result, QStringToTlpString(val));
  return result;
}

void Settings::setDefaultColor(tlp::ElementType elem, const tlp::Color &color) {
  QString value = tlp::ColorType::toString(color).c_str();
  setValue(elementKey(TS_DefaultColor, elem), value);
  ViewSettings::instance().setDefaultColor(elem, color);
}

Color Settings::defaultLabelColor(bool talipotDefault) {
  QString val =
      value(talipotDefault ? TS_DefaultOfDefault : TS_DefaultLabelColor, "(0, 0, 0)").toString();
  Color result;
  ColorType::fromString(result, QStringToTlpString(val));
  return result;
}

void Settings::setDefaultLabelColor(const Color &color) {
  QString value = tlp::ColorType::toString(color).c_str();
  setValue(TS_DefaultLabelColor, value);
  ViewSettings::instance().setDefaultLabelColor(color);
}

tlp::Size Settings::defaultSize(tlp::ElementType elem, bool talipotDefault) {
  QString val = value(elementKey(talipotDefault ? TS_DefaultOfDefault : TS_DefaultSize, elem),
                      (elem == tlp::NODE ? "(1,1,1)" : "(0.125,0.125,0.5)"))
                    .toString();
  Size result;
  SizeType::fromString(result, QStringToTlpString(val));
  return result;
}

void Settings::setDefaultSize(tlp::ElementType elem, const tlp::Size &size) {
  QString value = tlp::SizeType::toString(size).c_str();
  setValue(elementKey(TS_DefaultSize, elem), value);
  ViewSettings::instance().setDefaultSize(elem, size);
}

int Settings::defaultShape(tlp::ElementType elem, bool talipotDefault) {
  return value(elementKey(talipotDefault ? TS_DefaultOfDefault : TS_DefaultShape, elem),
               (elem == tlp::NODE ? int(NodeShape::Circle) : int(EdgeShape::Polyline)))
      .toInt();
}

void Settings::setDefaultShape(tlp::ElementType elem, int shape) {
  setValue(elementKey(TS_DefaultShape, elem), shape);
  ViewSettings::instance().setDefaultShape(elem, shape);
}

tlp::Color Settings::defaultSelectionColor(bool talipotDefault) {
  QString val =
      value(talipotDefault ? TS_DefaultOfDefault : TS_DefaultSelectionColor, "(23, 81, 228)")
          .toString();
  Color result;
  ColorType::fromString(result, QStringToTlpString(val));
  return result;
}

void Settings::setDefaultSelectionColor(const tlp::Color &color) {
  QString value = tlp::ColorType::toString(color).c_str();
  setValue(TS_DefaultSelectionColor, value);
}

QSet<QString> Settings::favoriteAlgorithms() const {
  QStringList list = value(TS_FavoriteAlgorithms, QStringList()).toStringList();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
  return QSet<QString>(list.begin(), list.end());
#else
  return list.toSet();
#endif
}

void Settings::addFavoriteAlgorithm(const QString &name) {
  QSet<QString> favAlgs = favoriteAlgorithms();
  favAlgs.insert(name);
  setFavoriteAlgorithms(favAlgs);
}

void Settings::removeFavoriteAlgorithm(const QString &name) {
  QSet<QString> favAlgs = favoriteAlgorithms();
  favAlgs.remove(name);
  setFavoriteAlgorithms(favAlgs);
}

bool Settings::isProxyEnabled() const {
  return value(TS_ProxyEnabled).toBool();
}

void Settings::setProxyEnabled(bool f) {
  setValue(TS_ProxyEnabled, f);
}

QNetworkProxy::ProxyType Settings::proxyType() const {
  return static_cast<QNetworkProxy::ProxyType>(value(TS_ProxyType).toInt());
}

void Settings::setProxyType(QNetworkProxy::ProxyType t) {
  setValue(TS_ProxyType, int(t));
}

QString Settings::proxyHost() const {
  return value(TS_ProxyHost).toString();
}

void Settings::setProxyHost(const QString &h) {
  setValue(TS_ProxyHost, h);
}

unsigned int Settings::proxyPort() const {
  return value(TS_ProxyPort).toUInt();
}

void Settings::setProxyPort(unsigned int p) {
  setValue(TS_ProxyPort, p);
}

bool Settings::isUseProxyAuthentification() const {
  return value(TS_ProxyUseAuth).toBool();
}

void Settings::setUseProxyAuthentification(bool f) {
  setValue(TS_ProxyUseAuth, f);
}

QString Settings::proxyUsername() const {
  return value(TS_ProxyUsername).toString();
}

void Settings::setProxyUsername(const QString &s) {
  setValue(TS_ProxyUsername, s);
}

QString Settings::proxyPassword() const {
  return value(TS_ProxyPassword).toString();
}

void Settings::setProxyPassword(const QString &s) {
  setValue(TS_ProxyPassword, s);
}

void Settings::applyProxySettings() {
  if (isProxyEnabled()) {
    QNetworkProxy proxy(proxyType(), proxyHost(), static_cast<qint16>(proxyPort()));

    if (isUseProxyAuthentification()) {
      proxy.setUser(proxyUsername());
      proxy.setPassword(proxyPassword());
    }
    QNetworkProxy::setApplicationProxy(proxy);
  } else {
    QNetworkProxyFactory::setUseSystemConfiguration(true);
  }
}

bool Settings::isFirstRun() const {
  return contains(TS_FirstRun) == false;
}

void Settings::setFirstRun(bool f) {
  setValue(TS_FirstRun, f);
}

bool Settings::isFirstTalipotMMRun() const {
  return contains(TS_FirstRunMM) == false;
}

void Settings::setFirstTalipotMMRun(bool f) {
  setValue(TS_FirstRunMM, f);
}

bool Settings::displayDefaultViews() const {
  return value(TS_AutomaticDisplayDefaultViews, true).toBool();
}

void Settings::setDisplayDefaultViews(bool f) {
  setValue(TS_AutomaticDisplayDefaultViews, f);
}

bool Settings::isAutomaticMapMetric() const {
  return value(TS_AutomaticMapMetric, false).toBool();
}

void Settings::setAutomaticMapMetric(bool f) {
  setValue(TS_AutomaticMapMetric, f);
}

bool Settings::isAutomaticRatio() const {
  return value(TS_AutomaticPerfectAspectRatio, false).toBool();
}

void Settings::setAutomaticRatio(bool f) {
  setValue(TS_AutomaticPerfectAspectRatio, f);
}

bool Settings::isAutomaticCentering() const {
  return value(TS_AutomaticCentering, true).toBool();
}

void Settings::setAutomaticCentering(bool f) {
  setValue(TS_AutomaticCentering, f);
}

bool Settings::isViewOrtho() const {
  return value(TS_ViewOrtho, true).toBool();
}

void Settings::setViewOrtho(bool f) {
  setValue(TS_ViewOrtho, f);
}

void Settings::setFavoriteAlgorithms(const QSet<QString> &lst) {
  setValue(TS_FavoriteAlgorithms, static_cast<QStringList>(lst.values()));
}

bool Settings::isResultPropertyStored() const {
  return value(TS_ResultPropertyStored, false).toBool();
}

void Settings::setResultPropertyStored(bool f) {
  setValue(TS_ResultPropertyStored, f);
}

unsigned int Settings::logPluginCall() const {
  return value(TS_LogPluginCall, NoLog).toUInt();
}

void Settings::setLogPluginCall(unsigned int val) {
  setValue(TS_LogPluginCall, val);
}

bool Settings::isUseTlpbFileFormat() const {
  return value(TS_UseTlpbFileFormat, true).toBool();
}

void Settings::setUseTlpbFileFormat(bool f) {
  setValue(TS_UseTlpbFileFormat, f);
}

unsigned int Settings::seedOfRandomSequence() const {
  return value(TS_SeedForRandomSequence, tlp::getSeedOfRandomSequence()).toUInt();
}

void Settings::setSeedOfRandomSequence(unsigned int seed) {
  setValue(TS_SeedForRandomSequence, seed);
}

void Settings::initSeedOfRandomSequence() {
  tlp::setSeedOfRandomSequence(seedOfRandomSequence());
}

bool Settings::warnUserAboutGraphicsCard() const {
  return value(TS_WarnUserAboutGraphicsCard, true).toBool();
}

void Settings::setWarnUserAboutGraphicsCard(bool f) {
  setValue(TS_WarnUserAboutGraphicsCard, f);
}

bool Settings::loggerAnchored() const {
  return value(TS_LoggerAnchored, true).toBool();
}

void Settings::setLoggerAnchored(bool f) {
  setValue(TS_LoggerAnchored, f);
}

bool Settings::pythonIDEAnchored() const {
  return value(TS_PythonIDEAnchored, true).toBool();
}

void Settings::setPythonIDEAnchored(bool f) {
  setValue(TS_PythonIDEAnchored, f);
}

void Settings::treatEvent(const Event &message) {
  const ViewSettingsEvent *sev = dynamic_cast<const ViewSettingsEvent *>(&message);

  if (sev && sev->getType() == ViewSettingsEvent::TLP_DEFAULT_COLOR_MODIFIED) {
    setDefaultColor(sev->getElementType(), sev->getColor());
  } else if (sev && sev->getType() == ViewSettingsEvent::TLP_DEFAULT_SIZE_MODIFIED) {
    setDefaultSize(sev->getElementType(), sev->getSize());
  } else if (sev && sev->getType() == ViewSettingsEvent::TLP_DEFAULT_SHAPE_MODIFIED) {
    setDefaultShape(sev->getElementType(), sev->getShape());
  } else if (sev && sev->getType() == ViewSettingsEvent::TLP_DEFAULT_LABEL_COLOR_MODIFIED) {
    setDefaultLabelColor(sev->getColor());
  }
}
