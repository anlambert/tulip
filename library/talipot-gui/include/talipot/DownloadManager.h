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

#include <QNetworkAccessManager>
#include <QUrl>
#include <QMap>

#include <talipot/config.h>

class QNetworkReply;

class TLP_QT_SCOPE DownloadManager : public QNetworkAccessManager {
  Q_OBJECT
  QList<QNetworkReply *> currentDownloads;
  QMap<QUrl, QString> downloadDestinations;

  static DownloadManager *_instance;
  DownloadManager();

public:
  static DownloadManager *getInstance();
  QNetworkReply *downloadPlugin(const QUrl &url, const QString &destination);
  bool saveToDisk(const QString &filename, QIODevice *data);

public slots:
  void downloadFinished(QNetworkReply *reply);
};
