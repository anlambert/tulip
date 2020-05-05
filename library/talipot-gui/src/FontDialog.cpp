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

#include <talipot/TlpTools.h>
#include <talipot/FontDialog.h>
#include "ui_FontDialog.h"

#include <QFontDatabase>
#include <QPushButton>
#include <QDebug>

using namespace tlp;

FontDialog::FontDialog(QWidget *parent)
    : QDialog(parent), _ui(new Ui::FontDialog), ok(QDialog::Rejected) {
  _ui->setupUi(this);

  for (const QString &font : Font::installedFontNames()) {
    _ui->nameList->addItem(font);
  }

  bool hasFont = _ui->nameList->count() > 0;
  _ui->settingsWidget->setEnabled(hasFont);
  _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(hasFont);

  if (hasFont) {
    _ui->nameList->setCurrentItem(_ui->nameList->item(0));
    fontChanged();
  }
}

FontDialog::~FontDialog() {
  delete _ui;
}

Font FontDialog::font() const {
  Font result;
  result.setFontName(_ui->nameList->currentItem()->text());
  result.setBold(_ui->styleList->currentIndex().row() == 1 ||
                 _ui->styleList->currentIndex().row() == 3);
  result.setItalic(_ui->styleList->currentIndex().row() == 2 ||
                   _ui->styleList->currentIndex().row() == 3);
  return result;
}

void FontDialog::fontChanged() {
  if (_ui->nameList->currentItem() == nullptr || _ui->styleList->currentItem() == nullptr) {
    return;
  }

  if (sender() == _ui->sizeList) {
    _ui->sizeSpin->setValue(_ui->sizeList->currentItem()->text().toInt());
    return;
  }

  Font selectedFont = font();
  _ui->preview->setStyleSheet("font-family: " + selectedFont.fontFamily() + "; " +
                              (selectedFont.isItalic() ? "font-style: italic; " : "") +
                              (selectedFont.isBold() ? "font-weight: bold; " : "") +
                              "font-size: " + QString::number(_ui->sizeSpin->value()) + "px; ");
}
int FontDialog::fontSize() const {
  return _ui->sizeSpin->value();
}

void FontDialog::selectFont(const Font &f) {
  QList<QListWidgetItem *> items = _ui->nameList->findItems(f.fontName(), Qt::MatchExactly);

  previousFont = f;

  if (items.empty()) {
    return;
  }

  _ui->nameList->setCurrentItem(items[0]);

  if (f.isBold()) {
    if (f.isItalic()) {
      _ui->styleList->setCurrentRow(3);
    } else {
      _ui->styleList->setCurrentRow(1);
    }
  } else if (f.isItalic()) {
    _ui->styleList->setCurrentRow(2);
  } else {
    _ui->styleList->setCurrentRow(0);
  }
}

Font FontDialog::getFont(QWidget * /*parent*/, const Font &selectedFont) {
  FontDialog dlg;
  dlg.selectFont(selectedFont);

  if (dlg.exec() != QDialog::Accepted || !dlg.font().exists()) {
    return Font();
  } else {
    return dlg.font();
  }
}

void FontDialog::showEvent(QShowEvent *ev) {
  QDialog::showEvent(ev);

  if (parentWidget()) {
    move(parentWidget()->window()->frameGeometry().topLeft() +
         parentWidget()->window()->rect().center() - rect().center());
  }
}
