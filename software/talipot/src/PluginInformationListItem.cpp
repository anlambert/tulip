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

#include "PluginInformationListItem.h"
#include "ui_PluginInformationListItem.h"

#include <talipot/TlpTools.h>
#include <talipot/TlpQtTools.h>
#include <talipot/IconicFont.h>
#include <talipot/FontIconManager.h>

#include <QApplication>
#include <QPainter>

using namespace tlp;

PluginInformationListItem::PluginInformationListItem(const Plugin &plugin, QWidget *parent)
    : QWidget(parent), _ui(new Ui::PluginInformationListItem) {
  _ui->setupUi(this);
  QPixmap pix = QPixmap(tlpStringToQString(plugin.icon()));
  if (IconicFont::isIconSupported(plugin.icon())) {
    pix = FontIconManager::icon(tlpStringToQString(plugin.icon())).pixmap(QSize(32, 32));
  } else {
    pix = pix.scaled(32, 32);
  }
  _ui->icon->setPixmap(pix);
  _ui->name->setText(tlpStringToQString(plugin.name()) + " " +
                     tlpStringToQString(plugin.release()));
  _ui->desc->setText(tlpStringToQString(plugin.info()) +
                     "\n\nAuthor: " + tlpStringToQString(plugin.author()));
}

PluginInformationListItem::~PluginInformationListItem() {
  delete _ui;
}

QWidget *PluginInformationListItem::description() {
  return _ui->desc;
}

void PluginInformationListItem::focusOut() {
  _ui->contentsFrame->setProperty("highlighted", false);
  _ui->contentsFrame->setStyleSheet(qApp->styleSheet());
}

void PluginInformationListItem::focusIn() {
  _ui->contentsFrame->setProperty("highlighted", true);
  _ui->contentsFrame->setStyleSheet(qApp->styleSheet());
}

void PluginInformationListItem::enterEvent(QEvent *) {
  emit focused();
}
