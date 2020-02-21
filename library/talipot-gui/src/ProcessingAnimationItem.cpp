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

#include "talipot/ProcessingAnimationItem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

ProcessingAnimationItem::ProcessingAnimationItem(const QPixmap &pixmap, const QSize &size,
                                                 QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), _currentFrame(0), _brush(Qt::transparent) {
  for (int y = 0; y < pixmap.height(); y += size.height()) {
    for (int x = 0; x < pixmap.width(); x += size.width()) {
      _pixmaps.push_back(pixmap.copy(x, y, size.width(), size.height()));
    }
  }

  _animationTimer.setInterval(50);
  _animationTimer.setSingleShot(false);
  connect(&_animationTimer, &QTimer::timeout, this, &ProcessingAnimationItem::animationTimeout);
  _animationTimer.start();
}

ProcessingAnimationItem::~ProcessingAnimationItem() {}

void ProcessingAnimationItem::animationTimeout() {
  _currentFrame = (_currentFrame + 1) % _pixmaps.size();
  setPixmap(_pixmaps[_currentFrame]);
  update();
}

void ProcessingAnimationItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                    QWidget *widget) {
  painter->setBrush(_brush);
  painter->setPen(Qt::transparent);
  painter->drawRect(boundingRect());
  QGraphicsPixmapItem::paint(painter, option, widget);
}
