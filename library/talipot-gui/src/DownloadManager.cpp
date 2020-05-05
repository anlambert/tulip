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

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QTimer>
#include <QUrl>

#include <talipot/DownloadManager.h>

#include <cstdio>

DownloadManager *DownloadManager::_instance = nullptr;

DownloadManager *DownloadManager::getInstance() {
  if (!_instance) {
    _instance = new DownloadManager();
  }

  return _instance;
}

DownloadManager::DownloadManager() {
  connect(this, &QNetworkAccessManager::finished, this, &DownloadManager::downloadFinished);
}

QNetworkReply *DownloadManager::downloadPlugin(const QUrl &url, const QString &destination) {
  downloadDestinations[url] = destination;

  QNetworkRequest request(url);
  QNetworkReply *reply = get(request);
  currentDownloads.append(reply);
  return reply;
}

bool DownloadManager::saveToDisk(const QString &filename, QIODevice *data) {
  QFile file(filename);

  if (!file.open(QIODevice::WriteOnly)) {
    fprintf(stderr, "Could not open %s for writing: %s\n", qPrintable(filename),
            qPrintable(file.errorString()));
    return false;
  }

  file.write(data->readAll());
  file.close();

  return true;
}

void DownloadManager::downloadFinished(QNetworkReply *reply) {
  QUrl url = reply->url();

  if (currentDownloads.contains(reply)) {

    if (reply->error() == QNetworkReply::NoError) {
      QString filename = downloadDestinations[url];

      if (saveToDisk(filename, reply)) {
        printf("Download of %s succeeded (saved to %s)\n", url.toEncoded().constData(),
               qPrintable(filename));
      }
    } else {
      fprintf(stderr, "Download of %s failed: %s\n", url.toEncoded().constData(),
              qPrintable(reply->errorString()));
    }

    currentDownloads.removeAll(reply);
  }

  reply->deleteLater();
}
