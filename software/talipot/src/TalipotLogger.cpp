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

#include "TalipotLogger.h"

#include "ui_TalipotLogger.h"
#include <iostream>

#include <QKeyEvent>
#include <QClipboard>
#include <QMenu>
#include <QPushButton>
#include <QShowEvent>
#include <QHideEvent>
#include <QList>

#include <talipot/TlpQtTools.h>
#include <talipot/Settings.h>

TalipotLogger::TalipotLogger(QWidget *parent)
    : QDialog(parent), _logType(QtDebugMsg), _ui(new Ui::TalipotLogger), _pythonOutput(false),
      _nbLog(0), _emptyIcon(16, 16) {
  _emptyIcon.fill(Qt::transparent);
  _ui->setupUi(this);
  _ui->listWidget->installEventFilter(this);
  _ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  QPushButton *copybutton =
      new QPushButton(QIcon(":/talipot/gui/icons/16/clipboard.png"), "&Copy selection", this);
  copybutton->setToolTip("Copy the selected lines into the clipboard");
  _ui->buttonBox->addButton(copybutton, QDialogButtonBox::ActionRole);
  QPushButton *clearbutton = new QPushButton("Clear", this);
  clearbutton->setToolTip("Remove all messages");
  _ui->buttonBox->addButton(clearbutton, QDialogButtonBox::ActionRole);
  connect(_ui->listWidget, &QWidget::customContextMenuRequested, this,
          &TalipotLogger::showContextMenu);
  connect(copybutton, &QAbstractButton::clicked, this, &TalipotLogger::copy);
  connect(clearbutton, &QAbstractButton::clicked, this, &TalipotLogger::clear);
  _ui->buttonBox->button(QDialogButtonBox::Close)->setToolTip("Close this window");
  QPushButton *resetb = _ui->buttonBox->button(QDialogButtonBox::Reset);
  resetb->setToolTip("Remove all messages and close this window");
  connect(resetb, &QAbstractButton::clicked, this, &TalipotLogger::clear);
  connect(resetb, &QAbstractButton::clicked, this, &QWidget::hide);
  connect(_ui->anchoredCB, &QAbstractButton::toggled, this, &TalipotLogger::setAnchored);
  _ui->anchoredCB->setChecked(tlp::Settings::instance().loggerAnchored());
}

TalipotLogger::~TalipotLogger() {
  delete _ui;
}

TalipotLogger::LogType TalipotLogger::getLastLogType() const {
  if (_pythonOutput) {
    return Python;
  }

  switch (_logType) {
  case QtDebugMsg:
  case QtInfoMsg:
    return Info;

  case QtWarningMsg:
    return Warning;

  case QtCriticalMsg:
  case QtFatalMsg:
    return Error;
  }

  return Info;
}

int TalipotLogger::count() const {
  return _ui->listWidget->count();
}

int TalipotLogger::countByType(LogType logType) const {
  return _logCounts[logType];
}

void TalipotLogger::log(QtMsgType type, const QMessageLogContext &, const QString &msg) {
  logImpl(type, msg);
}

void TalipotLogger::logImpl(QtMsgType type, const QString &msg) {

  if (msg.isEmpty()) {
    return;
  }

  QList<QString> msgPrefixToFilter;
  msgPrefixToFilter.append("QGraphicsScene::sendEvent");
  msgPrefixToFilter.append("QXcbConnection: XCB error:");

  for (const auto &prefix : msgPrefixToFilter) {
    if (msg.startsWith(prefix)) {
      return;
    }
  }

  if (type == QtFatalMsg) {
    std::cerr << tlp::QStringToTlpString(msg) << std::endl;
    abort();
  }

  _logType = type;
  QString msgClean = msg;

  if (msg.startsWith("[Python")) {
    // remove quotes around message added by Qt
    msgClean = msg.mid(14).mid(2, msg.length() - 17);
    _pythonOutput = true;
  } else {
    _pythonOutput = false;
  }

  LogType lastLogType = getLastLogType();
  QBrush c = (_nbLog % 2 == 0) ? palette().base() : palette().alternateBase();
  auto lines = msgClean.split('\n');
  for (int i = 0; i < lines.count(); ++i) {
    QListWidgetItem *item = nullptr;
    if (i == 0) {
      item = new QListWidgetItem(QIcon(icon(lastLogType)), lines[i]);
    } else {
      item = new QListWidgetItem(QIcon(_emptyIcon), lines[i]);
    }
    item->setBackground(c);
    _ui->listWidget->addItem(item);
  }
  _logCounts[lastLogType] += 1;
  _nbLog += 1;
}

QPixmap TalipotLogger::icon(LogType logType) const {
  QString pxUrl(":/talipot/app/icons/16/logger-");

  switch (logType) {
  case Python:
    return QPixmap(":/talipot/app/icons/16/python.png");

  case Info:
    pxUrl += "info";
    break;

  case Warning:
    pxUrl += "danger";
    break;

  case Error:
    pxUrl += "error";
    break;
  }

  pxUrl += ".png";

  return pxUrl;
}

void TalipotLogger::clear() {
  _ui->listWidget->clear();
  _logType = QtDebugMsg;
  emit cleared();
  _logCounts[Info] = 0;
  _logCounts[Warning] = 0;
  _logCounts[Error] = 0;
  _logCounts[Python] = 0;
  _nbLog = 0;
}

void TalipotLogger::copy() {
  QStringList strings;

  for (auto item : _ui->listWidget->selectedItems()) {
    strings << item->text();
  }

  if (!strings.isEmpty()) {
    QApplication::clipboard()->setText(strings.join("\n"));
  }
}

void TalipotLogger::showContextMenu(const QPoint &pos) {
  QMenu m;
  QAction *clear = m.addAction("Clear content", this, &TalipotLogger::clear);
  QAction *copy = m.addAction("Copy", this, &TalipotLogger::copy, QKeySequence::Copy);
  m.addAction("Close", this, &QWidget::close, QKeySequence::Close);
  copy->setEnabled(_ui->listWidget->count() != 0);
  clear->setEnabled(_ui->listWidget->count() != 0);
  m.exec(_ui->listWidget->mapToGlobal(pos));
}

// catch the copy to clipboard event of the QListWidget and reimplement
// its behaviour in order to be able to copy the text of all the selected rows
// (only the text of the current item is copied otherwise)
bool TalipotLogger::eventFilter(QObject *, QEvent *event) {
  QKeyEvent *ke = dynamic_cast<QKeyEvent *>(event);

  if (ke && ke->matches(QKeySequence::Copy)) {
    copy();
    return true;
  }

  return false;
}

void TalipotLogger::showEvent(QShowEvent *evt) {
  QDialog::showEvent(evt);

  if (!_windowGeometry.isNull()) {
    restoreGeometry(_windowGeometry);
  }
}

void TalipotLogger::hideEvent(QHideEvent *evt) {

  _windowGeometry = saveGeometry();
  QDialog::hideEvent(evt);
}

void TalipotLogger::setGeometry(int x, int y, int w, int h) {
  setMinimumSize(QSize(0, 0));
  setMaximumSize(QSize(16777215, 16777215));
  QDialog::setGeometry(x, y, w, h);
  _windowGeometry = saveGeometry();

  if (_anchored) {
    setMinimumSize(size());
    setMaximumSize(size());
  }
}

void TalipotLogger::setAnchored(bool anchored) {
  _anchored = anchored;
  bool visible = isVisible();

  if (_anchored) {
    setAttribute(Qt::WA_X11NetWmWindowTypeDialog, false);
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setMinimumSize(size());
    setMaximumSize(size());
    emit resetLoggerPosition();
  } else {
    setAttribute(Qt::WA_X11NetWmWindowTypeDialog, true);
    setWindowFlags(Qt::Dialog);
    setMinimumSize(QSize(0, 0));
    setMaximumSize(QSize(16777215, 16777215));
  }

  tlp::Settings::instance().setLoggerAnchored(anchored);

  // force the update of the window after modifying its flags
  if (visible) {
    show();
  }
}
