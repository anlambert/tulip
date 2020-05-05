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

#include <talipot/InteractorConfigWidget.h>
#include "ui_InteractorConfigWidget.h"

#include <QScrollArea>
#include <QLabel>
#include <QShowEvent>

#include <talipot/TlpQtTools.h>
#include <talipot/Interactor.h>

using namespace tlp;

InteractorConfigWidget::InteractorConfigWidget(QWidget *parent)
    : QDialog(parent), _ui(new Ui::InteractorConfigWidget), _interactor(nullptr) {
  _ui->setupUi(this);
  resize(500, 600);
}

InteractorConfigWidget::~InteractorConfigWidget() {
  delete _ui;
}

void InteractorConfigWidget::clearWidgets() {
  // removes widget from the layout to not delete the object and give back parenthood. It is up to
  // the interactor developer to delete its config widget
  if (_interactor != nullptr) {
    // take all widgets
    QWidget *oldConfig(_interactor->configurationWidget());
    // if old config is present and is only a QLabel => Documentation tab, else Options tab
    QWidget *DocWidget = nullptr;
    QWidget *OptionsWidget = nullptr;
    bool isOldDocConfigWidget(false), isOldOptionsConfigWidget(false);
    if (oldConfig != nullptr) {
      if (dynamic_cast<QLabel *>(oldConfig) != nullptr) {
        DocWidget = oldConfig;
        isOldDocConfigWidget = true;
      } else {
        OptionsWidget = oldConfig;
        isOldOptionsConfigWidget = true;
      }
    } else {
      DocWidget = _interactor->configurationDocWidget();
      OptionsWidget = _interactor->configurationOptionsWidget();
    }
    if ((isOldDocConfigWidget && _interactor->configurationWidget() != DocWidget) ||
        (_interactor->configurationDocWidget() != DocWidget)) {
      _ui->scrollAreaDoc->widget()->hide();
      _ui->scrollAreaDoc->takeWidget();
    }
    if ((isOldOptionsConfigWidget && _interactor->configurationWidget() != OptionsWidget) ||
        (_interactor->configurationOptionsWidget() != OptionsWidget)) {
      _ui->scrollAreaOptions->widget()->hide();
      _ui->scrollAreaOptions->takeWidget();
    }
    _interactor = nullptr;
  }
}

bool InteractorConfigWidget::setWidgets(Interactor *interactor) {
  // take all widgets
  QWidget *oldConfig(interactor->configurationWidget());
  // if old config is present and is only a QLabel => Documentation tab, else Options tab
  QWidget *DocWidget = nullptr;
  QWidget *OptionsWidget = nullptr;
  bool isOldDocConfigWidget(false), isOldOptionsConfigWidget(false);
  if (oldConfig != nullptr) {
    if (dynamic_cast<QLabel *>(oldConfig) != nullptr) {
      DocWidget = oldConfig;
      isOldDocConfigWidget = true;
    } else {
      OptionsWidget = oldConfig;
      isOldOptionsConfigWidget = true;
    }
  } else {
    DocWidget = interactor->configurationDocWidget();
    OptionsWidget = interactor->configurationOptionsWidget();
  }

  if ((DocWidget == nullptr) && (OptionsWidget == nullptr)) {
    clearWidgets();
    hide();
    return false;
  } else {
    setWindowTitle(tlpStringToQString(interactor->info()));
    // removes widget from the layout to not delete the object and give back parenthood. It is up to
    // the interactor developer to delete its config widget
    if (_interactor != nullptr) {
      if ((isOldDocConfigWidget && _interactor->configurationWidget() != DocWidget) ||
          (_interactor->configurationDocWidget() != DocWidget)) {
        if (_ui->tabWidget->isTabEnabled(0)) {
          _ui->scrollAreaDoc->widget()->hide();
          _ui->scrollAreaDoc->takeWidget();
        }
      }
      if ((isOldOptionsConfigWidget && _interactor->configurationWidget() != OptionsWidget) ||
          (_interactor->configurationOptionsWidget() != OptionsWidget)) {
        if (_ui->tabWidget->isTabEnabled(1)) {
          _ui->scrollAreaOptions->widget()->hide();
          _ui->scrollAreaOptions->takeWidget();
        }
      }
    }

    if (DocWidget != nullptr) {
      _ui->scrollAreaDoc->setWidget(DocWidget);
      _ui->tabWidget->setTabEnabled(0, true); // in case it was previously set to false
    } else {
      _ui->tabWidget->setTabEnabled(0, false);
    }

    if (OptionsWidget != nullptr) {
      _ui->scrollAreaOptions->setWidget(OptionsWidget);
      _ui->tabWidget->setTabEnabled(1, true); // in case it was previously set to false
    } else {
      _ui->tabWidget->setTabEnabled(1, false);
    }

    _interactor = interactor;
  }
  return true;
}

void InteractorConfigWidget::showEvent(QShowEvent *ev) {
  QDialog::showEvent(ev);

  if (parentWidget()) {
    move(parentWidget()->window()->frameGeometry().topLeft() +
         parentWidget()->window()->rect().center() - rect().center());
  }
}