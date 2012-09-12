/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "TulipPerspectiveProcessHandler.h"

#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtCore/QDebug>
#include <QtGui/QFileDialog>
#include <QtGui/QPainter>
#include <QtGui/QDesktopServices>
#include <QtNetwork/QTcpSocket>
#include <tulip/TulipProject.h>

#include <time.h>
#include <iostream>
#include <CrashHandling.h>

#include "TulipPerspectiveCrashHandler.h"
#include "TulipMainWindow.h"

#ifdef _WIN32
#include <windows.h>
#endif

SelectionButton::SelectionButton(QWidget *parent): QPushButton(parent) {}
void SelectionButton::paintEvent(QPaintEvent *e) {
  QPushButton::paintEvent(e);
  QPainter p(this);
  QPixmap pixmap(":/tulip/app/ui/list_bullet_arrow.png");
  p.drawPixmap(10,height()/2-pixmap.height()/2,pixmap);
}

TulipPerspectiveProcessHandler *TulipPerspectiveProcessHandler::_instance = 0;


TulipPerspectiveProcessHandler::TulipPerspectiveProcessHandler() {
  listen(QHostAddress::LocalHost);
  connect(this,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

TulipPerspectiveProcessHandler *TulipPerspectiveProcessHandler::instance() {
  if (!_instance) {
    _instance = new TulipPerspectiveProcessHandler;
  }

  return _instance;
}

void TulipPerspectiveProcessHandler::createPerspective(const QString &perspective, const QString &file, const QVariantMap &parameters) {
  QStringList args;

  if (!perspective.isEmpty())
    args << "--perspective=" + perspective;

  if (!file.isEmpty())
    args << file;

  QString k;
  foreach(k,parameters.keys())
  args << "--" + k + "=" + parameters[k].toString();

  args << "--port=" + QString::number(serverPort());
  time_t perspectiveId = time(NULL);
  args << "--id=" + QString::number(perspectiveId);

  QDir appDir(QApplication::applicationDirPath());

  QProcess *process = new QProcess;
#ifdef WIN32
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("STDERR_NO_ANSI_ESCAPES", "1");
  process->setProcessEnvironment(env);
#endif
  connect(process,SIGNAL(error(QProcess::ProcessError)),this,SLOT(perspectiveCrashed(QProcess::ProcessError)));
  connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(perspectiveFinished(int,QProcess::ExitStatus)));
  process->setProcessChannelMode(QProcess::ForwardedChannels);
  process->start(appDir.absoluteFilePath("tulip_perspective"),args);
  _processInfos[process] = PerspectiveProcessInfos(perspective,parameters,file,perspectiveId);
}

void TulipPerspectiveProcessHandler::perspectiveCrashed(QProcess::ProcessError) {
  QProcess *process = static_cast<QProcess *>(sender());
  process->setReadChannel(QProcess::StandardError);
  PerspectiveProcessInfos infos = _processInfos[process];

  TulipPerspectiveCrashHandler crashHandler;

  QRegExp plateform("^" + QString(TLP_PLATEFORM_HEADER) + " (.*)\n"),
          arch("^" + QString(TLP_ARCH_HEADER) + " (.*)\n"),
          compiler("^" + QString(TLP_COMPILER_HEADER) + " (.*)\n"),
          version("^" + QString(TLP_VERSION_HEADER) + " (.*)\n");

  // TODO: replace reading process by reading file
  QFile f(QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).filePath("tulip_perspective-" + QString::number(infos._perspectiveId) + ".log"));
  f.open(QIODevice::ReadOnly);

  QMap<QRegExp *,QString> envInfos;
  envInfos[&plateform] = "";
  envInfos[&arch] = "";
  envInfos[&compiler] = "";
  envInfos[&version] = "";

  QString stackTrace;
  bool grabStackTrace = false;

  while (!f.atEnd()) {
    QString line(f.readLine());

    if (line.startsWith(TLP_STACK_BEGIN_HEADER)) {
      grabStackTrace = true;
      continue;
    }
    else if (line.startsWith(TLP_STACK_END_HEADER)) {
      grabStackTrace = false;
      continue;
    }

    if (grabStackTrace) {
      stackTrace += line;
    }
    else {
      QRegExp *re;
      foreach(re,envInfos.keys()) {
        if (re->exactMatch(line)) {
          envInfos[re] = re->cap(1);
          break;
        }
      }
    }
  }

  f.close();

  crashHandler.setEnvData(envInfos[&plateform],envInfos[&arch],envInfos[&compiler],envInfos[&version],stackTrace);
  crashHandler.setPerspectiveData(infos);
  crashHandler.exec();

}

void TulipPerspectiveProcessHandler::perspectiveFinished(int, QProcess::ExitStatus) {
  QProcess *process = static_cast<QProcess *>(sender());
  delete process;
  _processInfos.remove(process);
}

void TulipPerspectiveProcessHandler::acceptConnection() {
  QTcpSocket* socket = nextPendingConnection();
  connect(socket,SIGNAL(readyRead()),this,SLOT(perspectiveReadyRead()));
  connect(socket,SIGNAL(disconnected()),socket,SLOT(deleteLater()));
}

void TulipPerspectiveProcessHandler::perspectiveReadyRead() {
  QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
  QString data = socket->readAll();
  QStringList tokens = data.split(" ");
  QString args = data.remove(0,tokens[0].length()+1);

  if (tokens[0] == "SHOW_AGENT") {
    if (tokens[1] == "PLUGINS")
      emit showPluginsAgent();
    else if (tokens[1] == "PROJECTS")
      emit showProjectsAgent();
    else if (tokens[1] == "ABOUT")
      emit showAboutAgent();
  }
  else if (tokens[0] == "TRAY_MESSAGE")
    emit showTrayMessage(args);
  else if (tokens[0] == "OPEN_PROJECT")
    emit openProject(args);
  else if (tokens[0] == "CREATE_PERSPECTIVE")
    emit openPerspective(args);
}
