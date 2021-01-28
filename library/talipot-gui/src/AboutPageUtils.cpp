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

#include <QString>
#include <QFile>
#include <QTextStream>

#include <talipot/TlpTools.h>
#include <talipot/TlpQtTools.h>

namespace tlp {

QString getSipVersion() {
  return SIP_VERSION;
}

QString getTalipotGitRevision() {
  QFile gitCommitFile(tlpStringToQString(TalipotShareDir + "GIT_COMMIT"));

  if (gitCommitFile.open(QFile::ReadOnly | QFile::Text)) {
    QTextStream in(&gitCommitFile);
    in.setCodec("UTF-8");
    return in.readAll().replace("\n", "");
  }
  return "";
}

QString getCppStandard() {
  return CPP_STANDARD;
}

QString getCppCompilerInfo() {
  return QString(CPP_COMPILER_ID).replace("GNU", "GCC") + QString(" ") + CPP_COMPILER_VERSION;
}

}
