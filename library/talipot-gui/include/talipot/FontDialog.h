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

#ifndef TALIPOT_FONT_DIALOG_H
#define TALIPOT_FONT_DIALOG_H

#include <QDialog>

#include <talipot/config.h>
#include <talipot/Font.h>

namespace Ui {
class FontDialog;
}

namespace tlp {

class TLP_QT_SCOPE FontDialog : public QDialog {
  Q_OBJECT

  Ui::FontDialog *_ui;
  Font previousFont;
  int ok;

public:
  explicit FontDialog(QWidget *parent = nullptr);
  ~FontDialog() override;
  Font font() const;
  int fontSize() const;
  void done(int res) override {
    ok = res;
    QDialog::done(res);
  }
  Font getSelectedFont() const {
    return ok == QDialog::Accepted ? font() : previousFont;
  }
  void showEvent(QShowEvent *ev) override;

  static Font getFont(QWidget *parent = nullptr, const Font &selectedFont = Font());

public slots:
  void selectFont(const Font &);

protected slots:
  void fontChanged();
};
}

#endif // TALIPOT_FONT_DIALOG_H
