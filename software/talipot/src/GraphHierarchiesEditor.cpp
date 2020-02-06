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

#include <vector>
#include "GraphHierarchiesEditor.h"

#include <QDebug>
#include <QContextMenuEvent>
#include <QMenu>
#include <QGraphicsEffect>
#include <QPainter>
#include <QTextDocument>
#include <QToolButton>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QMainWindow>
#include <QTimer>

#include <talipot/BooleanProperty.h>
#include <talipot/MetaTypes.h>
#include <talipot/GraphHierarchiesModel.h>
#include <talipot/FontIconManager.h>
#include <talipot/MaterialDesignIcons.h>

#include "TalipotMainWindow.h"
#include "ui_GraphHierarchiesEditor.h"

using namespace tlp;

CustomTreeView::CustomTreeView(QWidget *parent) : QTreeView(parent) {
  header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  connect(this, &QTreeView::collapsed, this, &CustomTreeView::resizeFirstColumnToContent);
  connect(this, &QTreeView::expanded, this, &CustomTreeView::resizeFirstColumnToContent);
}

int CustomTreeView::sizeHintForColumn(int col) const {
  if (!model() || col > 0) {
    return -1;
  }

  ensurePolished();
  int hint = 0;
  QModelIndex index = model()->index(0, col);

  while (index.isValid()) {
    if (viewport()->rect().contains(visualRect(index))) {
      hint = qMax(hint, visualRect(index).x() +
                            itemDelegate(index)->sizeHint(viewOptions(), index).width());
    }

    index = indexBelow(index);
  }

  return qMin(hint, viewport()->rect().width());
}

void CustomTreeView::scrollContentsBy(int dx, int dy) {
  if (dy != 0 && dx == 0) {
    resizeFirstColumnToContent();
  }

  QTreeView::scrollContentsBy(dx, dy);
}

void CustomTreeView::setModel(QAbstractItemModel *m) {
  if (model()) {
    disconnect(model(), &QAbstractItemModel::rowsInserted, this,
               &CustomTreeView::resizeFirstColumnToContent);
    disconnect(model(), &QAbstractItemModel::rowsRemoved, this,
               &CustomTreeView::resizeFirstColumnToContent);
  }

  connect(m, &QAbstractItemModel::rowsInserted, this, &CustomTreeView::resizeFirstColumnToContent);
  connect(m, &QAbstractItemModel::rowsRemoved, this, &CustomTreeView::resizeFirstColumnToContent);
  QTreeView::setModel(m);
  resizeFirstColumnToContent();
}

void CustomTreeView::setAllHierarchyVisible(const QModelIndex &index, bool visible) {
  auto iModel = index.model();
  int childCount = iModel->rowCount(index);
  for (int i = 0; i < childCount; i++) {
    auto child = iModel->index(i, 0, index);
    // Recursively call the function for each child node.
    setAllHierarchyVisible(child, visible);
  }

  if (visible) {
    if (!isExpanded(index)) {
      expand(index);
    }
  } else {
    if (isExpanded(index)) {
      collapse(index);
    }
  }
}

void CustomTreeView::resizeFirstColumnToContent() {
  QTimer::singleShot(100, this, &CustomTreeView::resizeFirstColumnToContentImpl);
}

void CustomTreeView::resizeFirstColumnToContentImpl() {
  resizeColumnToContents(0);
}

static QColor menuIconColor = QColor("#404244");

GraphHierarchiesEditor::GraphHierarchiesEditor(QWidget *parent)
    : QWidget(parent), _ui(new Ui::GraphHierarchiesEditor), _contextGraph(nullptr),
      _model(nullptr) {
  _ui->setupUi(this);
  _ui->hierarchiesTree->addAction(_ui->actionDelete_All);
  _ui->actionDelete_All->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  _ui->actionCreate_panel->setIcon(
      FontIconManager::icon(MaterialDesignIcons::PlusBox, menuIconColor));
  _ui->actionExport->setIcon(FontIconManager::icon(MaterialDesignIcons::Export, menuIconColor));
  _ui->actionSave_to_file->setIcon(
      FontIconManager::icon(MaterialDesignIcons::FileExport, menuIconColor));
  _ui->actionRename->setIcon(FontIconManager::icon(MaterialDesignIcons::RenameBox, menuIconColor));
  _ui->actionDelete_graph->setIcon(
      FontIconManager::icon(MaterialDesignIcons::Delete, menuIconColor));
  _ui->actionDelete_All->setIcon(FontIconManager::icon(MaterialDesignIcons::Delete, menuIconColor));
  _ui->actionDelete_all_nodes->setIcon(
      FontIconManager::icon(MaterialDesignIcons::Delete, menuIconColor));
  _ui->actionDelete_all_edges->setIcon(
      FontIconManager::icon(MaterialDesignIcons::Delete, menuIconColor));
  _ui->actionDelete_selection->setIcon(
      FontIconManager::icon(MaterialDesignIcons::Delete, menuIconColor));
  _ui->actionDelete_selection_from_root_graph->setIcon(
      FontIconManager::icon(MaterialDesignIcons::Delete, menuIconColor));
  _ui->actionAdd_sub_graph->setIcon(
      FontIconManager::icon(MaterialDesignIcons::Tournament, menuIconColor, 1.0, -90));
  _ui->actionClone_subgraph->setIcon(
      FontIconManager::icon(MaterialDesignIcons::Tournament, menuIconColor, 1.0, -90));
  _ui->actionCreate_induced_sub_graph->setIcon(
      FontIconManager::icon(MaterialDesignIcons::Tournament, menuIconColor, 1.0, -90));
  _ui->actionClone_sibling->setIcon(
      FontIconManager::icon(MaterialDesignIcons::Tournament, menuIconColor, 1.0, -90));
  _ui->actionClone_sibling_with_properties->setIcon(
      FontIconManager::icon(MaterialDesignIcons::Tournament, menuIconColor, 1.0, -90));
  _ui->actionExpand_hierarchy->setIcon(
      FontIconManager::icon(MaterialDesignIcons::FileTree, menuIconColor));

  QToolButton *linkButton = new QToolButton();
  linkButton->setObjectName("linkButton");
  linkButton->setIcon(FontIconManager::icon(MaterialDesignIcons::LinkVariant, Qt::white, 0.8));
  linkButton->setToolTip("Click here to disable the synchronization with workspace active "
                         "panel.\nWhen synchronization is enabled, the graph currently "
                         "displayed\nin the active panel, becomes the current one in the Graphs "
                         "panel.");
  linkButton->setIconSize(QSize(23, 23));
  linkButton->setMinimumSize(25, 25);
  linkButton->setMaximumSize(25, 25);
  linkButton->setCheckable(true);
  linkButton->setChecked(true);
  _ui->header->insertWidget(linkButton);
  _linkButton = linkButton;
  connect(linkButton, &QAbstractButton::toggled, this,
          &GraphHierarchiesEditor::toggleSynchronization);
  _ui->hierarchiesTree->installEventFilter(this);

  connect(_ui->hierarchiesTree, &QAbstractItemView::clicked, this,
          &GraphHierarchiesEditor::clicked);
}

bool GraphHierarchiesEditor::synchronized() const {
  return _linkButton->isChecked();
}

void GraphHierarchiesEditor::setModel(tlp::GraphHierarchiesModel *model) {
  _model = model;
  QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(_ui->hierarchiesTree);
  proxyModel->setSourceModel(model);
  proxyModel->setDynamicSortFilter(false);
  _ui->hierarchiesTree->setModel(proxyModel);
  _ui->hierarchiesTree->header()->resizeSection(0, 100);
  _ui->hierarchiesTree->header()->setSectionResizeMode(0, QHeaderView::Interactive);
  connect(_ui->hierarchiesTree->selectionModel(), &QItemSelectionModel::currentChanged, this,
          &GraphHierarchiesEditor::currentChanged);
}

GraphHierarchiesEditor::~GraphHierarchiesEditor() {
  delete _ui;
}

void GraphHierarchiesEditor::contextMenuRequested(const QPoint &p) {
  _contextIndex = _ui->hierarchiesTree->indexAt(p);

  if (_contextIndex.isValid()) {
    _contextGraph = _contextIndex.data(tlp::GraphHierarchiesModel::GraphRole).value<tlp::Graph *>();
    QMenu menu;
    menu.setToolTipsVisible(true);
    menu.addAction(_ui->actionCreate_panel);
    menu.addSeparator();
    menu.addAction(_ui->actionExport);
    menu.addAction(_ui->actionSave_to_file);
    menu.addSeparator();
    menu.addAction(_ui->actionRename);
    menu.addSeparator();
    menu.addAction(_ui->actionAdd_sub_graph);
    menu.addAction(_ui->actionCreate_induced_sub_graph);
    menu.addAction(_ui->actionClone_subgraph);

    if (_contextGraph->getRoot() != _contextGraph) {
      menu.addAction(_ui->actionClone_sibling);
      menu.addAction(_ui->actionClone_sibling_with_properties);
    }
    menu.addSeparator();
    if (_contextGraph->getRoot() != _contextGraph) {
      menu.addAction(_ui->actionDelete_graph);
    }

    menu.addAction(_ui->actionDelete_All);
    menu.addAction(_ui->actionDelete_all_nodes);
    menu.addAction(_ui->actionDelete_all_edges);
    menu.addAction(_ui->actionDelete_selection);
    if (_contextGraph->getRoot() != _contextGraph) {
      menu.addAction(_ui->actionDelete_selection_from_root_graph);
    }
    if (!_contextGraph->subGraphs().empty()) {
      menu.addSeparator();
      if (!_ui->hierarchiesTree->isExpanded(_contextIndex)) {
        menu.addAction(_ui->actionExpand_hierarchy);
      } else {
        menu.addAction(_ui->actionCollapse_hierarchy);
      }
    }
    menu.exec(_ui->hierarchiesTree->viewport()->mapToGlobal(p));
    _contextIndex = QModelIndex();
    _contextGraph = nullptr;
  }
}

void GraphHierarchiesEditor::clicked(const QModelIndex &index) {
  if (!index.isValid() || index.internalPointer() == nullptr) {
    return;
  }

  _contextGraph = index.data(tlp::Model::GraphRole).value<tlp::Graph *>();
  _model->setCurrentGraph(_contextGraph);
  _contextGraph = nullptr;
}

void GraphHierarchiesEditor::doubleClicked(const QModelIndex &index) {
  if (!index.isValid() || index.internalPointer() == nullptr) {
    return;
  }

  _contextGraph = index.data(tlp::Model::GraphRole).value<tlp::Graph *>();
  _model->setCurrentGraph(_contextGraph);
  createPanel();
  _contextGraph = nullptr;
}

void GraphHierarchiesEditor::currentChanged(const QModelIndex &index, const QModelIndex &previous) {
  if (synchronized() && index.isValid() && index.internalPointer()) {
    if (index == previous) {
      return;
    }

    _contextGraph = index.data(tlp::Model::GraphRole).value<tlp::Graph *>();
    disconnect(_ui->hierarchiesTree->selectionModel(), &QItemSelectionModel::currentChanged, this,
               &GraphHierarchiesEditor::currentChanged);
    _model->setCurrentGraph(_contextGraph);
    connect(_ui->hierarchiesTree->selectionModel(), &QItemSelectionModel::currentChanged, this,
            &GraphHierarchiesEditor::currentChanged);
    _contextGraph = nullptr;
  }
}

void GraphHierarchiesEditor::addSubGraph() {
  if (_contextGraph == nullptr) {
    return;
  }

  _contextGraph->push();
  _contextGraph->addSubGraph("empty subgraph");
}

void GraphHierarchiesEditor::cloneSubGraph() {
  if (_contextGraph == nullptr) {
    return;
  }

  _contextGraph->push();
  std::string sgName("clone subgraph of ");
  _contextGraph->addCloneSubGraph(sgName + _contextGraph->getName());
}

void GraphHierarchiesEditor::cloneSibling() {
  if (_contextGraph == nullptr) {
    return;
  }

  _contextGraph->push();
  std::string sgName("clone sibling of ");
  _contextGraph->addCloneSubGraph(sgName + _contextGraph->getName(), true);
}

void GraphHierarchiesEditor::cloneSiblingWithProperties() {
  if (_contextGraph == nullptr) {
    return;
  }

  _contextGraph->push();
  std::string sgName("clone sibling of ");
  _contextGraph->addCloneSubGraph(sgName + _contextGraph->getName(), true, true);
}

void GraphHierarchiesEditor::addInducedSubGraph() {
  if (_contextGraph == nullptr) {
    return;
  }

  TalipotMainWindow::instance().createSubGraph(_contextGraph);
}

void GraphHierarchiesEditor::delGraph() {
  if (_contextGraph == nullptr &&
      !_ui->hierarchiesTree->selectionModel()->selectedRows(0).empty()) {
    _contextGraph = _ui->hierarchiesTree->selectionModel()
                        ->selectedRows(0)[0]
                        .data(tlp::Model::GraphRole)
                        .value<tlp::Graph *>();
  }

  if (_contextGraph == nullptr) {
    return;
  }

  TalipotMainWindow::instance().closePanelsForGraph(_contextGraph);
  _contextGraph->push();

  if (_contextGraph->getRoot() == _contextGraph) {
    delete _contextGraph;
    _model->setCurrentGraph(nullptr);
  } else {
    tlp::Graph *sg = _contextGraph->getSuperGraph();
    _contextGraph->getSuperGraph()->delSubGraph(_contextGraph);
    _model->setCurrentGraph(sg);
  }

  _contextGraph = nullptr;
}

void GraphHierarchiesEditor::delAllGraph() {
  if (_contextGraph == nullptr &&
      !_ui->hierarchiesTree->selectionModel()->selectedRows(0).empty()) {
    _contextGraph = _ui->hierarchiesTree->selectionModel()
                        ->selectedRows(0)[0]
                        .data(tlp::Model::GraphRole)
                        .value<tlp::Graph *>();
  }

  if (_contextGraph == nullptr) {
    return;
  }

  if (_contextGraph->getRoot() == _contextGraph) {

    if (QMessageBox::question(parentWidget(), "Delete a whole hierarchy",
                              "You are going to delete a complete graph hierarchy. This operation "
                              "cannot be undone. Do you really want to continue?",
                              QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {
      TalipotMainWindow::instance().closePanelsForGraph(_contextGraph);
      delete _contextGraph;
      _model->setCurrentGraph(nullptr);

      if (_model->empty()) {
        TalipotMainWindow::instance().setWindowModified(false);
        TalipotMainWindow::instance().resetTitle();
      }
    }
  } else {
    _contextGraph->push();
    TalipotMainWindow::instance().closePanelsForGraph(_contextGraph);
    tlp::Graph *sg = _contextGraph->getSuperGraph();
    _contextGraph->getSuperGraph()->delAllSubGraphs(_contextGraph);
    _model->setCurrentGraph(sg);
  }

  _contextGraph = nullptr;
}

void GraphHierarchiesEditor::delAllNodes() {
  if (_contextGraph == nullptr) {
    return;
  }

  _contextGraph->push();
  Observable::holdObservers();
  _contextGraph->clear();
  Observable::unholdObservers();
}

void GraphHierarchiesEditor::delAllEdges() {
  if (_contextGraph == nullptr) {
    return;
  }

  _contextGraph->push();
  Observable::holdObservers();
  std::vector<edge> edges = _contextGraph->edges();
  _contextGraph->delEdges(edges);
  Observable::unholdObservers();
}

void GraphHierarchiesEditor::delSelection(bool fromRoot) {
  Observable::holdObservers();
  tlp::BooleanProperty *selection = _contextGraph->getBooleanProperty("viewSelection");

  std::vector<tlp::edge> edgesToDelete =
      iteratorVector(selection->getEdgesEqualTo(true, _contextGraph));
  bool hasPush = !edgesToDelete.empty();

  if (hasPush) {
    _contextGraph->push();
    _contextGraph->delEdges(edgesToDelete, fromRoot);
  }

  std::vector<tlp::node> nodesToDelete =
      iteratorVector(selection->getNodesEqualTo(true, _contextGraph));

  if (!hasPush && !nodesToDelete.empty())
    _contextGraph->push();

  _contextGraph->delNodes(nodesToDelete, fromRoot);

  Observable::unholdObservers();
}

void GraphHierarchiesEditor::delSelectionFromRoot() {
  delSelection(true);
}

void GraphHierarchiesEditor::createPanel() {
  tlp::Graph *g = _contextGraph;

  if (g == nullptr) {
    g = _model->currentGraph();

    if (g == nullptr) {
      return;
    }
  }

  TalipotMainWindow::instance().createPanel(g);
}

void GraphHierarchiesEditor::exportGraph() {
  TalipotMainWindow::instance().exportGraph(_contextGraph);
}

void GraphHierarchiesEditor::renameGraph() {
  if (_contextIndex.isValid() &&
      _ui->hierarchiesTree->selectionModel()->selectedRows(0).size() == 1) {
    _ui->hierarchiesTree->edit(_ui->hierarchiesTree->selectionModel()->selectedRows(0)[0]);
  }
}

void GraphHierarchiesEditor::saveGraphHierarchyInTlpFile() {
  TalipotMainWindow::instance().saveGraphHierarchyInTlpFile(_contextGraph);
}

void GraphHierarchiesEditor::toggleSynchronization(bool f) {
  if (f) {
    _linkButton->setIcon(FontIconManager::icon(MaterialDesignIcons::LinkVariant, Qt::white, 0.8));
    _linkButton->setToolTip("Click here to disable the synchronization with workspace active "
                            "panel.\nWhen synchronization is enabled, the graph currently "
                            "displayed\nin the active panel, becomes the current one in the Graphs "
                            "panel.");
  } else {
    _linkButton->setIcon(
        FontIconManager::icon(MaterialDesignIcons::LinkVariantOff, Qt::white, 0.8));
    _linkButton->setToolTip("Click here to enable the synchronization with workspace active "
                            "panel.\nWhen synchronization is enabled, the graph currently "
                            "displayed\nin the active panel, becomes the current one in the Graphs "
                            "panel.");
  }

  emit changeSynchronization(f);
}

void GraphHierarchiesEditor::setSynchronizeButtonVisible(bool f) {
  _linkButton->setVisible(f);
}

void GraphHierarchiesEditor::collapseGraphHierarchy() {
  _ui->hierarchiesTree->setAllHierarchyVisible(_contextIndex, false);
}

void GraphHierarchiesEditor::expandGraphHierarchy() {
  _ui->hierarchiesTree->setAllHierarchyVisible(_contextIndex, true);
}
