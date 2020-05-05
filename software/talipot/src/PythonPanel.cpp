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

#include <talipot/PythonInterpreter.h>

#include "PythonPanel.h"
#include "ui_PythonPanel.h"

#include <talipot/GraphHierarchiesModel.h>
#include <talipot/Mimes.h>
#include <talipot/MetaTypes.h>

static const QString setCurrentGraphFunction = "graph = None\n"
                                               "def setCurrentGraph(g):\n"
                                               "	global graph\n"
                                               "	graph = g\n";

PythonPanel::PythonPanel(QWidget *parent) : QWidget(parent), _ui(new Ui::PythonPanel) {
  _ui->setupUi(this);
  connect(_ui->graphCombo, &TreeViewComboBox::currentItemChanged, this,
          &PythonPanel::graphComboIndexChanged);
  tlp::PythonInterpreter::getInstance()->runString(setCurrentGraphFunction);
  connect(_ui->pythonShellWidget, &tlp::PythonShellWidget::beginCurrentLinesExecution, this,
          &PythonPanel::beginCurrentLinesExecution);
  connect(_ui->pythonShellWidget, &tlp::PythonShellWidget::endCurrentLinesExecution, this,
          &PythonPanel::endCurrentLinesExecution);
}

PythonPanel::~PythonPanel() {
  delete _ui;
}

void PythonPanel::setModel(tlp::GraphHierarchiesModel *model) {
  _ui->graphCombo->setModel(model);
}

void PythonPanel::graphComboIndexChanged() {
  tlp::Graph *g = _ui->graphCombo->model()
                      ->data(_ui->graphCombo->selectedIndex(), tlp::Model::GraphRole)
                      .value<tlp::Graph *>();
  tlp::PythonInterpreter::getInstance()->runGraphScript("__main__", "setCurrentGraph", g);
  _ui->pythonShellWidget->getAutoCompletionDb()->setGraph(g);
}

void PythonPanel::dragEnterEvent(QDragEnterEvent *dragEv) {
  const tlp::GraphMimeType *mimeType = dynamic_cast<const tlp::GraphMimeType *>(dragEv->mimeData());

  if (mimeType != nullptr) {
    dragEv->accept();
  }
}

void PythonPanel::dropEvent(QDropEvent *dropEv) {
  const tlp::GraphMimeType *mimeType = dynamic_cast<const tlp::GraphMimeType *>(dropEv->mimeData());

  if (mimeType != nullptr) {
    tlp::GraphHierarchiesModel *model =
        static_cast<tlp::GraphHierarchiesModel *>(_ui->graphCombo->model());
    QModelIndex graphIndex = model->indexOf(mimeType->graph());

    if (graphIndex == _ui->graphCombo->selectedIndex()) {
      return;
    }

    _ui->graphCombo->selectIndex(graphIndex);
    dropEv->accept();
  }
}

void PythonPanel::beginCurrentLinesExecution() {
  tlp::Graph *g = _ui->graphCombo->model()
                      ->data(_ui->graphCombo->selectedIndex(), tlp::Model::GraphRole)
                      .value<tlp::Graph *>();

  // undo/redo management
  if (g) {
    g->push();
  }
}

void PythonPanel::endCurrentLinesExecution() {
  tlp::Graph *g = _ui->graphCombo->model()
                      ->data(_ui->graphCombo->selectedIndex(), tlp::Model::GraphRole)
                      .value<tlp::Graph *>();
  // undo/redo management
  g->popIfNoUpdates();
}
