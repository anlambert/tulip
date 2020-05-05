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

#include "OutputPanelButton.h"

#include <QPainter>
#include <QApplication>

OutputPanelButton::OutputPanelButton(QWidget *parent) : QPushButton(parent), _number(0) {}

QSize OutputPanelButton::sizeHint() const {
  ensurePolished();

  QSize s = fontMetrics().size(Qt::TextSingleLine, _title);

  // Expand to account for border image set by stylesheet above
  s.rwidth() += 19 + 5 + 2;
  s.rheight() += 2 + 2;

  return s.expandedTo(QApplication::globalStrut());
}

#include <iostream>

void OutputPanelButton::paintEvent(QPaintEvent *event) {
  QPushButton::paintEvent(event);

  auto fm = fontMetrics();
  int baseLine = (height() - fm.height() + 1) / 2 + fm.ascent();
#if (QT_VERSION < QT_VERSION_CHECK(5, 11, 0))
  const int numberWidth = fm.width(QString::number(_number));
#else
  const int numberWidth = fm.horizontalAdvance(QString::number(_number));
#endif

  QPainter p(this);
  p.setFont(font());
  p.setPen(Qt::white);
  p.drawText((20 - numberWidth) / 2, baseLine, QString::number(_number));

  if (!isChecked()) {
    p.setPen(Qt::black);
  }

#ifdef __APPLE__ // On MacOS: force the baseLine to 2 to get the text drawn in the button
  baseLine = 2;
#endif
  int leftPart = 22;

  p.drawText(leftPart, baseLine, width() - leftPart, height() - baseLine * 2,
             Qt::AlignCenter | Qt::AlignVCenter,
             fm.elidedText(_title, Qt::ElideMiddle, width() - leftPart - 1));
}

void OutputPanelButton::setGlowColor(const QColor &color) {
  QString colorStr = "rgba(";
  QString str;

  str.setNum(color.red());
  colorStr.append(str + ",");

  str.setNum(color.green());
  colorStr.append(str + ",");

  str.setNum(color.blue());
  colorStr.append(str + ",");

  str.setNum(color.alpha());
  colorStr.append(str + ")");

  setStyleSheet(QString("OutputPanelButton { background-color: qlineargradient(spread:pad, x1:0, "
                        "y1:0, x2:1, y2:0,") +
                "stop:0 rgba(255,255,255, 0), stop:0.25 " + colorStr + ", stop:0.9 " + colorStr +
                ", stop:1 rgba(255,255,255, 0)); }");
}

void OutputPanelButton::resetBackgroundColor() {
  setStyleSheet("OutputPanelButton { background-color: transparent; }");
}
