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

#ifndef TALIPOT_CAPTION_GRAPHICS_SUB_ITEMS_H
#define TALIPOT_CAPTION_GRAPHICS_SUB_ITEMS_H

#include <QGraphicsPixmapItem>
#include <QObject>

class QGradient;

namespace tlp {

class ConfigurationIconItem : public QObject, public QGraphicsPixmapItem {

  Q_OBJECT

public:
  ConfigurationIconItem() {}

signals:

  void configurationIconPressed();

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *) override {
    emit configurationIconPressed();
  }
};

class SelectionArrowItem : public QObject, public QGraphicsPathItem {

  Q_OBJECT

public:
  SelectionArrowItem(float initRangePos, const QPoint &initPos);

  bool sceneEvent(QEvent *event) override;

signals:

  void circleMoved();

protected:
  int yPos;
  QPoint initPos;
};

class SelectionTextItem : public QGraphicsTextItem {

public:
  SelectionTextItem();

protected:
  bool sceneEvent(QEvent *event) override {
    return static_cast<SelectionArrowItem *>(parentItem())->sceneEvent(event);
  }
};

class MovableRectItem : public QObject, public QGraphicsRectItem {

  Q_OBJECT

public:
  MovableRectItem(const QRectF &rect, const QRectF &size, SelectionArrowItem *topCircle,
                  SelectionArrowItem *bottomCircle);

  void setInternalRect(const QRectF &rect);

signals:

  // begin and end in 0,1 range
  void moved(float begin, float end);

protected:
  bool sceneEvent(QEvent *event) override;

  QRectF _currentRect;
  QPoint _initPos;
  SelectionArrowItem *_topCircle;
  SelectionArrowItem *_bottomCircle;
};

class MovablePathItem : public QObject, public QGraphicsPathItem {

  Q_OBJECT

public:
  MovablePathItem(const QRectF &rect, QGraphicsPathItem *topPathItem,
                  QGraphicsPathItem *bottomPathItem, SelectionArrowItem *topCircle,
                  SelectionArrowItem *bottomCircle);

  void setDataToPath(const std::vector<std::pair<double, float>> &metricToSizeFilteredList,
                     double minMetric, double maxMetric);

  void setRect(const QRectF &rect);

signals:

  // begin and end in 0,1 range
  void moved(float begin, float end);

protected:
  void updatePath();

  bool sceneEvent(QEvent *event) override;

  std::vector<std::pair<double, float>> _metricToSizeFilteredList;
  double _minMetric;
  double _maxMetric;

  QRectF _currentRect;
  QGraphicsPathItem *_topPathItem;
  QGraphicsPathItem *_bottomPathItem;
  SelectionArrowItem *_topCircle;
  SelectionArrowItem *_bottomCircle;
};

class CaptionGraphicsBackgroundItem : public QObject, public QGraphicsRectItem {

  Q_OBJECT

public:
  CaptionGraphicsBackgroundItem(const QRect &rect);

  void generateColorCaption(const QGradient &activeGradient, const QGradient &hideGradient,
                            const std::string &propertyName, double minValue, double maxValue);

  void generateSizeCaption(const std::vector<std::pair<double, float>> &metricToSizeFilteredList,
                           const std::string &propertyName, double minValue, double maxValue);

public slots:

  void updateCaption();
  // begin and end in 0,1 range
  void updateCaption(float begin, float end);
  void configurationIconPressedSlot();

  void activateInteractions();
  void removeInteractions();

signals:

  void filterChanged(float begin, float end);
  void configurationIconPressed();

  void interactionsActivated();
  void interactionsRemoved();

protected:
  void activateInteractions(bool);

  bool sceneEvent(QEvent *event) override;
  void updateSelectionText(float begin, float end);

  bool _interactionsActivated;
  float _beginBackup;
  float _endBackup;
  QPoint _captionContentPos;

  double _minValue;
  double _maxValue;

  // Global Items
  QGraphicsTextItem *_minTextItem;
  QGraphicsTextItem *_maxTextItem;
  QGraphicsTextItem *_min2TextItem;
  QGraphicsTextItem *_max2TextItem;
  QGraphicsLineItem *_min2LineItem;
  QGraphicsLineItem *_max2LineItem;
  QGraphicsRectItem *_captionRectBorder;
  SelectionArrowItem *_rangeSelector1Item;
  SelectionArrowItem *_rangeSelector2Item;
  SelectionTextItem *_rangeSelector1TextItem;
  SelectionTextItem *_rangeSelector2TextItem;

  // Color caption Items
  QGraphicsRectItem *_topCaptionRectItem;
  MovableRectItem *_middleCaptionRectItem;
  QGraphicsRectItem *_bottomCaptionRectItem;

  // Size caption Items
  // MovableRectItem *_selectionSizeRectItem;
  MovablePathItem *_sizeCaptionPathItem;
  QGraphicsPathItem *_topSizeCaptionPathItem;
  QGraphicsPathItem *_bottomSizeCaptionPathItem;
};
}

#endif // TALIPOT_CAPTION_GRAPHICS_SUB_ITEMS_H
