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

#ifndef PLUGIN_INFORMATION_LIST_ITEM_H
#define PLUGIN_INFORMATION_LIST_ITEM_H

#include <QWidget>
#include <talipot/Plugin.h>

namespace Ui {
class PluginInformationListItemData;
}

class PluginInformationListItem : public QWidget {
  Q_OBJECT
  Ui::PluginInformationListItemData *_ui;

public:
  explicit PluginInformationListItem(const tlp::Plugin &plugin, QWidget *parent = nullptr);
  ~PluginInformationListItem() override;
  QWidget *description();

public slots:
  void focusOut();
  void focusIn();

signals:
  void focused();

protected:
  void enterEvent(QEvent *) override;
};

#endif // PLUGIN_INFORMATION_LIST_ITEM_H
