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

#ifndef TALIPOT_PROCESSING_ANIMATION_ITEM_H
#define TALIPOT_PROCESSING_ANIMATION_ITEM_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QObject>
#include <QBrush>

class ProcessingAnimationItem : public QObject, public QGraphicsPixmapItem {
  Q_OBJECT

  QVector<QPixmap> _pixmaps;
  QTimer _animationTimer;
  unsigned int _currentFrame;
  QBrush _brush;

public:
  explicit ProcessingAnimationItem(const QPixmap &pixmap, const QSize &size,
                                   QGraphicsItem *parent = nullptr);
  ~ProcessingAnimationItem() override;

  QBrush brush() const {
    return _brush;
  }
  void setBrush(const QBrush &brush) {
    _brush = brush;
  }

protected slots:
  void animationTimeout();
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // TALIPOT_PROCESSING_ANIMATION_ITEM_H
