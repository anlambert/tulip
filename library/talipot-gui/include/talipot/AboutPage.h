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

#ifndef TALIPOT_ABOUT_PAGE_H
#define TALIPOT_ABOUT_PAGE_H

#include <talipot/config.h>

#include <QWidget>

namespace Ui {
class AboutPage;
}

namespace tlp {

/**
  @brief The "About us/Getting help" screen displayed in the Talipot app startup window.
  This widget is mainly meant to provide links to the user and developer documentations, direct user
  to Talipot forums
  and provide basic offline support.
  */
class TLP_QT_SCOPE AboutPage : public QWidget {
  Q_OBJECT
public:
  explicit AboutPage(QWidget *parent = nullptr);
  ~AboutPage() override;

private slots:

  void openUrlInBrowser(const QString &url);

private:
  Ui::AboutPage *_ui;
};
}

#endif // TALIPOT_ABOUT_PAGE_H
