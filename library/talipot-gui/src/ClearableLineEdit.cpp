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

#include "talipot/ClearableLineEdit.h"

#include <QPaintEvent>
#include <QPainter>

QPixmap *ClearableLineEdit::CLEAR_PIXMAP = nullptr;

void ClearableLineEdit::initPixmap() {
  if (CLEAR_PIXMAP == nullptr) {
    CLEAR_PIXMAP = new QPixmap(":/talipot/gui/ui/clearbutton.png");
  }
}

ClearableLineEdit::ClearableLineEdit(QWidget *parent)
    : QLineEdit(parent), _clearButtonHovered(false) {
  setMouseTracking(true);
}

void ClearableLineEdit::paintEvent(QPaintEvent *ev) {
  QLineEdit::paintEvent(ev);
  QPainter p(this);
  p.setOpacity(_clearButtonHovered ? 1 : 0.7);
  initPixmap();
  p.drawPixmap(pixmapRect(), *CLEAR_PIXMAP);
}

QRect ClearableLineEdit::pixmapRect() {
  initPixmap();
  QRect pixmapRect(width() - CLEAR_PIXMAP->width() - 5, height() / 2 - CLEAR_PIXMAP->height() / 2,
                   CLEAR_PIXMAP->width(), CLEAR_PIXMAP->height());
  return pixmapRect;
}

void ClearableLineEdit::mouseMoveEvent(QMouseEvent *ev) {
  QLineEdit::mouseMoveEvent(ev);
  bool oldValue = _clearButtonHovered;
  _clearButtonHovered = pixmapRect().contains(ev->pos());

  if (oldValue != _clearButtonHovered) {
    repaint();
  }
}

void ClearableLineEdit::mousePressEvent(QMouseEvent *ev) {
  QLineEdit::mousePressEvent(ev);

  if (pixmapRect().contains(ev->pos())) {
    clear();
    emit textEdited("");
    emit editingFinished();
  }
}
