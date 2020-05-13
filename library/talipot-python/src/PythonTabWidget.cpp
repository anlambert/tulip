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

#include "talipot/PythonInterpreter.h"
#include "talipot/PythonTabWidget.h"

#include <QPainter>
#include <QTabBar>
#include <QLinearGradient>

using namespace tlp;

PythonTabWidget::PythonTabWidget(QWidget *parent)
    : QTabWidget(parent), _drawGradient(false), _textColor(QColor(100, 100, 100)) {}

void PythonTabWidget::paintEvent(QPaintEvent *event) {
  QTabWidget::paintEvent(event);
  static QImage pythonLogoImg(":/talipot/python/icons/python.png");
  static QString pythonVersion(PythonInterpreter::instance().getPythonVersionStr());
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
                         QPainter::SmoothPixmapTransform);

  if (_drawGradient) {
    QLinearGradient gradient(width() / 2, tabBar()->pos().y(), width() / 2,
                             tabBar()->pos().y() + tabBar()->height());
    gradient.setColorAt(0.0, "#838383");
    gradient.setColorAt(0.4, "#707070");
    gradient.setColorAt(0.401, "#636363");
    gradient.setColorAt(1.0, "#4a4a4a");
    painter.fillRect(x(), tabBar()->pos().y(), width(), tabBar()->height(), gradient);
  }

  painter.setPen(_textColor);
#ifndef __APPLE__
  painter.setFont(QFont("Arial", int(10 * tabBar()->height() / 27.0)));
#else
  painter.setFont(QFont("Arial", int(12 * tabBar()->height() / 27.0)));
#endif
  int imageWidth = int(25 * tabBar()->height() / 27.0);
  int labelWidth = int(80 * tabBar()->height() / 27.0);
  int offset = tabBar()->height() - imageWidth;
  QRectF rect(width() - (imageWidth + labelWidth), tabBar()->pos().y() + offset / 2, imageWidth,
              imageWidth);
  QRectF rect2(width() - labelWidth, tabBar()->pos().y(), labelWidth, tabBar()->height());
  painter.drawImage(rect, pythonLogoImg);
  painter.drawText(rect2, Qt::AlignCenter, QString("Python ") + pythonVersion);
}
