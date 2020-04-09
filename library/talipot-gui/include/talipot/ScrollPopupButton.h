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

#ifndef TALIPOT_SCROLL_POPUP_BUTTON_H
#define TALIPOT_SCROLL_POPUP_BUTTON_H

#include <QPushButton>

#include <talipot/config.h>

class QSlider;

class TLP_QT_SCOPE ScrollPopupButton : public QPushButton {
  Q_OBJECT
  Q_PROPERTY(int value READ value WRITE setValue)
  Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
  Q_PROPERTY(int maximum READ maximum WRITE setMaximum)

  QSlider *_slider;

public:
  explicit ScrollPopupButton(QWidget *parent = nullptr);
  int value() const;
  int maximum() const;
  int minimum() const;

public slots:
  virtual void showPopup();
  virtual void hidePopup();
  void setPopupVisible(bool);
  void setValue(int);
  void setMinimum(int);
  void setMaximum(int);

signals:
  void valueChanged(int);

protected:
  bool eventFilter(QObject *, QEvent *) override;
};

#endif // TALIPOT_SCROLL_POPUP_BUTTON_H
