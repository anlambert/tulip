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
#include <talipot/APIDataBase.h>
#include <talipot/PythonIDE.h>
#include <talipot/PythonCodeEditor.h>
#include <talipot/SimplePluginProgressWidget.h>
#include "PythonPanel.h"

#include "TalipotMainWindow.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QCloseEvent>
#include <QClipboard>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QTimer>
#include <QVBoxLayout>
#include <QDialog>
#include <QByteArray>
#include <QStatusBar>
#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>

#include <talipot/TlpTools.h>
#include <talipot/ImportModule.h>
#include <talipot/Graph.h>
#include <talipot/ExportModule.h>
#include <talipot/View.h>
#include <talipot/SimplePluginProgressWidget.h>
#include <talipot/GraphHierarchiesModel.h>
#include <talipot/CSVImportWizard.h>
#include <talipot/GraphModel.h>
#include <talipot/GraphTableItemDelegate.h>
#include <talipot/GraphPropertiesModel.h>
#include <talipot/GlMainView.h>
#include <talipot/GlMainWidget.h>
#include <talipot/GlGraphComposite.h>
#include <talipot/Settings.h>
#include <talipot/PluginsManager.h>
#include <talipot/TlpQtTools.h>
#include <talipot/Project.h>
#include <talipot/GraphTools.h>
#include <talipot/ColorScaleConfigDialog.h>
#include <talipot/AboutPage.h>
#include <talipot/ColorScalesManager.h>
#include <talipot/StableIterator.h>

#include "ui_TalipotMainWindow.h"

#include "TalipotLogger.h"
#include "ImportWizard.h"
#include "ExportWizard.h"
#include "PanelSelectionWizard.h"
#include "GraphHierarchiesEditor.h"
#include "PreferencesDialog.h"
#include "PluginsCenter.h"

#include <QDebug>

using namespace tlp;
using namespace std;

// checks if it exists a Talipot import plugin that can load the provided file based on its
// extension
static bool talipotCanOpenFile(const QString &path) {
  // Talipot project file does not use import / export plugin
  if (path.endsWith(".tlpx")) {
    return true;
  }

  std::list<std::string> imports = PluginsManager::availablePlugins<ImportModule>();

  for (auto &import : imports) {
    ImportModule *m = PluginsManager::getPluginObject<ImportModule>(import);
    std::list<std::string> fileExtensions(m->allFileExtensions());

    for (auto &ext : fileExtensions) {
      if (path.endsWith(tlpStringToQString(ext))) {
        delete m;
        return true;
      }
    }

    delete m;
  }

  return false;
}

static void logMsgToStdErr(const QString &msg) {
  if (msg.startsWith("[Python")) {
    // remove quotes around message added by Qt
    QString msgClean = msg.mid(14).mid(2, msg.length() - 17);

    if (msg.startsWith("[PythonStdOut]")) {
      std::cout << QStringToTlpString(msgClean) << std::endl;
    } else {
      std::cerr << QStringToTlpString(msgClean) << std::endl;
    }
  } else {
    std::cerr << QStringToTlpString(msg) << std::endl;
  }
}

static void talipotLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  logMsgToStdErr(msg);
  TalipotMainWindow::instance().log(type, context, msg);
}

class PythonIDEDialog : public QDialog {

  QByteArray _windowGeometry;

public:
  PythonIDEDialog(QWidget *parent, Qt::WindowFlags flags) : QDialog(parent, flags) {}

protected:
  void showEvent(QShowEvent *e) override {
    QDialog::showEvent(e);

    if (!_windowGeometry.isEmpty()) {
      restoreGeometry(_windowGeometry);
    }
  }

  void closeEvent(QCloseEvent *e) override {
    _windowGeometry = saveGeometry();
    QDialog::closeEvent(e);
  }
};

TalipotMainWindow::TalipotMainWindow()
    : _ui(new Ui::TalipotMainWindow), _graphs(new GraphHierarchiesModel(this)),
      _project(Project::newProject()), _pluginsCenter(new PluginsCenter()),
      _recentDocumentsSettingsKey("talipot/recent_files"), _maximized(false) {
  Q_INIT_RESOURCE(TalipotApp);
  _ui->setupUi(this);

  _title = "Talipot ";
  QString iconPath;

  // show patch number only if needed
  if (TALIPOT_INT_VERSION % 10) {
    _title += TALIPOT_VERSION;
  } else {
    _title += TALIPOT_MM_VERSION;
  }

  setWindowTitle(_title);

  _pythonIDE = new PythonIDE();
  QVBoxLayout *dialogLayout = new QVBoxLayout();
  dialogLayout->addWidget(_pythonIDE);
  dialogLayout->setContentsMargins(0, 0, 0, 0);
  _pythonIDEDialog = new PythonIDEDialog(nullptr, Qt::Window);
  _pythonIDEDialog->setWindowIcon(windowIcon());
  _pythonIDEDialog->setLayout(dialogLayout);
  _pythonIDEDialog->resize(800, 600);
  _pythonIDEDialog->setWindowTitle("Talipot Python IDE");

  currentGraphChanged(nullptr);
  // set win/Mac dependent tooltips with ctrl shortcut
  setToolTipWithCtrlShortcut(_ui->exposeModeButton, "Toggle the Expose mode", "E");
  setToolTipWithCtrlShortcut(_ui->searchButton, "Show/hide the graph's elements search panel", "F");
  setToolTipWithCtrlShortcut(_ui->pythonButton,
                             "Show/hide the Python interpreter (Read Eval Print Loop) panel",
                             "Shift+P");
  setToolTipWithCtrlShortcut(_ui->previousPageButton, "Show previous panel", "Shift+Left");
  setToolTipWithCtrlShortcut(_ui->nextPageButton, "Show next panel", "Shift+Right");
  setToolTipWithCtrlShortcut(_ui->actionNewProject, "Start a new empty Talipot project", "Shift+N");
  setToolTipWithCtrlShortcut(
      _ui->actionSave_Project,
      "Save the current project (current graphs with current views) in the attached file", "S");
  setToolTipWithCtrlShortcut(_ui->actionSave_Project_as, "Save Project as a new file name",
                             "Shift+S");
  setToolTipWithCtrlShortcut(_ui->actionImport, "Display the Graph importing wizard", "Shift+O");
  setToolTipWithCtrlShortcut(_ui->actionExit, "Exit from Talipot", "Q");
  setToolTipWithCtrlShortcut(_ui->actionUndo, "Undo the latest update of the current graph", "Z");
  setToolTipWithCtrlShortcut(_ui->actionRedo, "Redo the latest update of the current graph", "Y");
  setToolTipWithCtrlShortcut(_ui->actionCut,
                             "Move the selected elements of the current graph into the "
                             "clipboard (the selected edges ends are selected too)",
                             "X");
  setToolTipWithCtrlShortcut(_ui->actionCopy,
                             "Copy the selected elements of the current graph into the "
                             "clipboard (the selected edges ends are selected too)",
                             "C");
  setToolTipWithCtrlShortcut(_ui->actionPaste,
                             "Paste the clipboard elements into the current graph", "V");
  setToolTipWithCtrlShortcut(_ui->actionSelect_All, "Select all elements of the current graph",
                             "A");
  setToolTipWithCtrlShortcut(_ui->actionInvert_selection,
                             "Invert the selection of the current "
                             "graph elements, deselect if "
                             "selected and select if not selected",
                             "I");
  setToolTipWithCtrlShortcut(_ui->actionCancel_selection,
                             "Deselect all selected elements of the current grap", "Shift+A");
  setToolTipWithCtrlShortcut(_ui->actionGroup_elements,
                             "Create a meta-node representing a newly created subgraph "
                             "containing all selected elements of the current graph",
                             "G");
  setToolTipWithCtrlShortcut(
      _ui->actionCreate_sub_graph,
      "Create a subgraph containing all selected elements of the current graph", "Shift+G");
  setToolTipWithCtrlShortcut(_ui->actionPreferences, "Show Talipot preferences dialog", ",");
  setToolTipWithCtrlShortcut(_ui->actionShowUserDocumentation,
                             "Display the User handbook in a navigator", "?");
  setToolTipWithCtrlShortcut(_ui->actionShowPythonDocumentation,
                             "Display the Talipot python documentation in a navigator", "P");
  setToolTipWithCtrlShortcut(_ui->actionMessages_log, "Show the message log", "M");
  setToolTipWithCtrlShortcut(_ui->actionPython_IDE, "Show the Python IDE", "Alt+P");
  setToolTipWithCtrlShortcut(_ui->actionExport, "Show the Graph exporting wizard", "E");
  setToolTipWithCtrlShortcut(_ui->actionOpen_Project, "Open a graph file", "O");
  setToolTipWithCtrlShortcut(_ui->actionFind_plugins,
                             "Allow to find algorithms in typing text in a search field", "Alt+H");
  setToolTipWithCtrlShortcut(_ui->actionNew_graph, "Create a new empty graph", "N");
  // set portable tooltips
  _ui->undoButton->setToolTip("Undo the latest update of the current graph");
  _ui->redoButton->setToolTip("Redo the latest undone update of the current graph");
  _ui->workspaceButton->setToolTip("Display the existing graph views");
  _ui->developButton->setToolTip(
      "Display the Talipot Python IDE for developing scripts and plugins to "
      "execute on the loaded graphs");
  _ui->loggerMessageInfo->setToolTip("Show/Hide the Messages log panel");
  _ui->loggerMessagePython->setToolTip(_ui->loggerMessageInfo->toolTip());
  _ui->loggerMessageWarning->setToolTip(_ui->loggerMessageInfo->toolTip());
  _ui->loggerMessageError->setToolTip(_ui->loggerMessageInfo->toolTip());
  _ui->exportButton->setToolTip("Display the Graph exporting wizard");
  _ui->csvImportButton->setToolTip("Import data in the current graph using a csv formatted file");
  _ui->importButton->setToolTip("Display the Graph importing wizard");
  _ui->sidebarButton->setToolTip("Hide Sidebar");
  _ui->menubarButton->setToolTip("Hide Menubar");
  _ui->addPanelButton->setToolTip("Open a new visualization panel on the current graph");
  _ui->singleModeButton->setToolTip("Switch to 1 panel mode");
  _ui->splitModeButton->setToolTip("Switch to 2 panels mode");
  _ui->splitHorizontalModeButton->setToolTip("Switch to 2 panels mode");
  _ui->split3ModeButton->setToolTip("Switch to 2-top 1-bottom panels mode");
  _ui->split32ModeButton->setToolTip("Switch to 1-left 2-right panels mode");
  _ui->split33ModeButton->setToolTip("Switch to 2-left 1-right panels mode");
  _ui->gridModeButton->setToolTip("Switch to 4 panels mode");
  _ui->sixModeButton->setToolTip("Switch to 6 panels mode");
  _ui->menuDelete->setToolTip("Delete elements of the current graph");
  _ui->menuOpen_recent_file->setToolTip(
      "Choose a file to open among the recently opened/saved graphs or projects");
  _ui->actionDelete->setToolTip("Delete the selected elements from the current graph [Del]");
  _ui->actionFull_screen->setToolTip("Display Talipot in full screen mode [F11]");
  _ui->actionShow_Menubar->setToolTip("Show/Hide the main menu bar [Ctrl+Shift+M]");
  _ui->actionAbout_us->setToolTip("Display the 'About Talipot' information dialog [F1]");
  _ui->actionPlugins_Center->setToolTip("Display the Plugin center");
  _ui->actionImport_CSV->setToolTip(_ui->csvImportButton->toolTip());
  _ui->actionSave_graph_to_file->setToolTip("Write the current graph into a file");
  _ui->actionCreate_empty_sub_graph->setToolTip("Create an empty subgraph of the current graph");
  _ui->actionClone_sub_graph->setToolTip(
      "Create a subgraph containing the same elements as the current graph");
  _ui->action_Close_All->setToolTip("Close all opened workspace views");
  _ui->actionColor_scales_management->setToolTip("Manage Talipot color scales");
  _ui->actionMake_selection_a_graph->setToolTip(
      "Add the non selected ends of the selected edges to the current graph selection");
  _ui->actionDelete_from_the_root_graph->setToolTip("Delete the selected elements from the whole "
                                                    "graph hierarchy (not only from the current "
                                                    "graph) [Shift+Del]");
  _ui->actionReverse_selected_edges->setToolTip(
      "Reverse the source and target nodes of the selected edges");
  _ui->actionDelete_all->setToolTip("Delete all elements of the current graph");
  _ui->actionSelect_All_Nodes->setToolTip("Select all nodes of the current graph");
  _ui->actionSelect_All_Edges->setToolTip("Select all edges of the current graph");

  _ui->singleModeButton->setEnabled(false);
  _ui->singleModeButton->hide();
  _ui->workspace->setSingleModeSwitch(_ui->singleModeButton);
  _ui->workspace->setFocusedPanelHighlighting(true);
  _ui->splitModeButton->setEnabled(false);
  _ui->splitModeButton->hide();
  _ui->workspace->setSplitModeSwitch(_ui->splitModeButton);
  _ui->splitHorizontalModeButton->setEnabled(false);
  _ui->splitHorizontalModeButton->hide();
  _ui->workspace->setSplitHorizontalModeSwitch(_ui->splitHorizontalModeButton);
  _ui->split3ModeButton->setEnabled(false);
  _ui->split3ModeButton->hide();
  _ui->workspace->setSplit3ModeSwitch(_ui->split3ModeButton);
  _ui->split32ModeButton->setEnabled(false);
  _ui->split32ModeButton->hide();
  _ui->workspace->setSplit32ModeSwitch(_ui->split32ModeButton);
  _ui->split33ModeButton->setEnabled(false);
  _ui->split33ModeButton->hide();
  _ui->workspace->setSplit33ModeSwitch(_ui->split33ModeButton);
  _ui->gridModeButton->setEnabled(false);
  _ui->gridModeButton->hide();
  _ui->workspace->setGridModeSwitch(_ui->gridModeButton);
  _ui->sixModeButton->setEnabled(false);
  _ui->sixModeButton->hide();
  _ui->workspace->setSixModeSwitch(_ui->sixModeButton);
  _ui->workspace->setPageCountLabel(_ui->pageCountLabel);
  _ui->workspace->setExposeModeSwitch(_ui->exposeModeButton);
  _ui->outputFrame->hide();
  _logger = new TalipotLogger(this);
  _ui->loggerFrame->installEventFilter(this);
  installEventFilter(this);
  setAcceptDrops(true);

  connect(_logger, &TalipotLogger::cleared, this, &TalipotMainWindow::logCleared);
  connect(_logger, &TalipotLogger::resetLoggerPosition, this,
          &TalipotMainWindow::resetLoggerDialogPosition);

  _colorScalesDialog = new ColorScaleConfigDialog(ColorScalesManager::getLatestColorScale(), this);

  // redirection of various output
  redirectDebugOutputToQDebug();
  redirectWarningOutputToQWarning();
  redirectErrorOutputToQCritical();

  _ui->menuFile->setToolTipsVisible(true);
  _ui->menuEdit->setToolTipsVisible(true);
  _ui->menuHelp->setToolTipsVisible(true);
  _ui->menuWindow->setToolTipsVisible(true);

  Settings::instance().synchronizeViewSettings();

  qInstallMessageHandler(talipotLogger);

  connect(_ui->workspaceButton, &QAbstractButton::clicked, this,
          &TalipotMainWindow::workspaceButtonClicked);
  connect(_ui->workspace, &Workspace::addPanelRequest, this, &TalipotMainWindow::createPanel);
  connect(_ui->workspace, &Workspace::focusedPanelSynchronized, this,
          &TalipotMainWindow::focusedPanelSynchronized);
  connect(_graphs, &GraphHierarchiesModel::currentGraphChanged, this,
          &TalipotMainWindow::currentGraphChanged);
  connect(_graphs, &GraphHierarchiesModel::currentGraphChanged, _ui->algorithmRunner,
          &AlgorithmRunner::setGraph);
  connect(_ui->graphHierarchiesEditor, &GraphHierarchiesEditor::changeSynchronization, this,
          &TalipotMainWindow::changeSynchronization);

  // Connect actions
  connect(_ui->actionMessages_log, &QAction::triggered, this, &TalipotMainWindow::showLogger);
  connect(_ui->actionPython_IDE, &QAction::triggered, this, &TalipotMainWindow::showPythonIDE);
  connect(_ui->actionFull_screen, &QAction::triggered, this, &TalipotMainWindow::showFullScreen);
  connect(_ui->actionImport, &QAction::triggered, [this] { importGraph(); });
  connect(_ui->actionExport, &QAction::triggered, [this] { exportGraph(); });
  connect(_ui->actionSave_graph_to_file, &QAction::triggered,
          [this] { saveGraphHierarchyInTlpFile(); });
  connect(_ui->workspace, &Workspace::panelFocused, this, &TalipotMainWindow::panelFocused);
  connect(_ui->actionSave_Project, &QAction::triggered, this, &TalipotMainWindow::save);
  connect(_ui->actionSave_Project_as, &QAction::triggered, [this] { saveAs(); });
  connect(_ui->actionOpen_Project, &QAction::triggered, [this] { open(); });
  connect(_ui->actionDelete, &QAction::triggered, this, &TalipotMainWindow::deleteSelectedElements);
  connect(_ui->actionDelete_from_the_root_graph, &QAction::triggered, this,
          &TalipotMainWindow::deleteSelectedElementsFromRootGraph);
  connect(_ui->actionDelete_all, &QAction::triggered, this, &TalipotMainWindow::clearGraph);
  connect(_ui->actionInvert_selection, &QAction::triggered, this,
          &TalipotMainWindow::invertSelection);
  connect(_ui->actionCancel_selection, &QAction::triggered, this,
          &TalipotMainWindow::cancelSelection);
  connect(_ui->actionReverse_selected_edges, &QAction::triggered, this,
          &TalipotMainWindow::reverseSelectedEdges);
  connect(_ui->actionMake_selection_a_graph, &QAction::triggered, this,
          &TalipotMainWindow::make_graph);
  connect(_ui->actionSelect_All, &QAction::triggered, [this] { selectAll(); });
  connect(_ui->actionSelect_All_Nodes, &QAction::triggered, this,
          &TalipotMainWindow::selectAllNodes);
  connect(_ui->actionSelect_All_Edges, &QAction::triggered, this,
          &TalipotMainWindow::selectAllEdges);
  connect(_ui->actionUndo, &QAction::triggered, this, &TalipotMainWindow::undo);
  connect(_ui->actionRedo, &QAction::triggered, this, &TalipotMainWindow::redo);
  connect(_ui->actionCut, &QAction::triggered, this, &TalipotMainWindow::cut);
  connect(_ui->actionPaste, &QAction::triggered, this, &TalipotMainWindow::paste);
  connect(_ui->actionCopy, &QAction::triggered, [this] { copy(); });
  connect(_ui->actionGroup_elements, &QAction::triggered, this, &TalipotMainWindow::group);
  connect(_ui->actionCreate_sub_graph, &QAction::triggered, [this] { createSubGraph(); });
  connect(_ui->actionClone_sub_graph, &QAction::triggered, this, &TalipotMainWindow::cloneSubGraph);
  connect(_ui->actionCreate_empty_sub_graph, &QAction::triggered, this,
          &TalipotMainWindow::addEmptySubGraph);
  connect(_ui->actionImport_CSV, &QAction::triggered, this, &TalipotMainWindow::CSVImport);
  connect(_ui->actionFind_plugins, &QAction::triggered, this, &TalipotMainWindow::findPlugins);
  connect(_ui->actionNew_graph, &QAction::triggered, this, &TalipotMainWindow::addNewGraph);
  connect(_ui->actionNewProject, &QAction::triggered, this, &TalipotMainWindow::newProject);
  connect(_ui->actionPreferences, &QAction::triggered, this, &TalipotMainWindow::openPreferences);
  connect(_ui->searchButton, &QAbstractButton::clicked, this, &TalipotMainWindow::setSearchOutput);
  connect(_ui->workspace, &Workspace::importGraphRequest, [this] { importGraph(); });
  connect(_ui->action_Close_All, &QAction::triggered, _ui->workspace, &Workspace::closeAll);
  connect(_ui->addPanelButton, &QAbstractButton::clicked, [this] { createPanel(); });
  connect(_ui->actionColor_scales_management, &QAction::triggered, this,
          &TalipotMainWindow::displayColorScalesDialog);
  connect(_ui->exportButton, &QAbstractButton::clicked, [this] { exportGraph(); });

  connect(_ui->actionPlugins_Center, &QAction::triggered, this,
          &TalipotMainWindow::showPluginsCenter);
  connect(_ui->actionAbout_us, &QAction::triggered, this, &TalipotMainWindow::showAboutPage);

  if (QFile(tlpStringToQString(tlp::TalipotShareDir) +
            "../doc/talipot/talipot-user/html/index.html")
          .exists()) {
    connect(_ui->actionShowUserDocumentation, &QAction::triggered, this,
            &TalipotMainWindow::showUserDocumentation);
    connect(_ui->actionShowPythonDocumentation, &QAction::triggered, this,
            &TalipotMainWindow::showPythonDocumentation);
  } else {
    _ui->actionShowUserDocumentation->setVisible(false);
    _ui->actionShowPythonDocumentation->setVisible(false);
  }

  if (QFile(tlpStringToQString(tlp::TalipotShareDir) + "../doc/talipot/doxygen/html/index.html")
          .exists()) {
    connect(_ui->actionShowAPIDocumentation, &QAction::triggered, this,
            &TalipotMainWindow::showAPIDocumentation);
  } else {
    _ui->actionShowAPIDocumentation->setVisible(false);
  }

  _ui->mainSplitter->setCollapsible(0, false);
  _ui->mainSplitter->setCollapsible(1, false);

  connect(_project, &Project::projectFileChanged, this, &TalipotMainWindow::projectFileChanged);
  connect(this, &TalipotMainWindow::resetWindowTitle, [this] { projectFileChanged(); });

  // set a default position and size that works seamlessly on all platforms
  move(0, 0);
  resize(QDesktopWidget().availableGeometry(this).size() * 0.9);
}

void TalipotMainWindow::buildRecentDocumentsMenu() {
  _ui->menuOpen_recent_file->clear();

  for (const QString &s : Settings::instance().recentDocuments()) {
    if (!QFileInfo(s).exists() || !talipotCanOpenFile(s)) {
      continue;
    }

    QAction *action = _ui->menuOpen_recent_file->addAction(
        QIcon(":/talipot/app/icons/16/archive.png"), s, this, &TalipotMainWindow::openRecentFile);
    action->setData(s);
  }

  _ui->menuOpen_recent_file->addSeparator();

  for (const QString &s : Settings::instance().value(_recentDocumentsSettingsKey).toStringList()) {
    if (!QFileInfo(s).exists() || !talipotCanOpenFile(s)) {
      continue;
    }

    QAction *action =
        _ui->menuOpen_recent_file->addAction(QIcon(":/talipot/app/icons/16/empty-file.png"), s,
                                             this, &TalipotMainWindow::openRecentFile);
    action->setData(s);
  }
  _ui->menuOpen_recent_file->setEnabled(!_ui->menuOpen_recent_file->isEmpty());
}

void TalipotMainWindow::addRecentDocument(const QString &path) {
  QStringList recents = Settings::instance().value(_recentDocumentsSettingsKey).toStringList();

  if (recents.contains(path) || !talipotCanOpenFile(path)) {
    return;
  }

  recents += path;

  if (recents.size() > 10) {
    recents.pop_front();
  }

  Settings::instance().setValue(_recentDocumentsSettingsKey, recents);
  Settings::instance().sync();
  buildRecentDocumentsMenu();
}

void TalipotMainWindow::updateLogIconsAndCounters() {
  TalipotLogger::LogType logType = _logger->getLastLogType();
  QFrame *logIconCounterFrame = nullptr;
  QLabel *logIconLabel = nullptr;
  QLabel *logCounterLabel = nullptr;

  if (logType == TalipotLogger::Info) {
    logIconCounterFrame = _ui->loggerFrameInfo;
    logIconLabel = _ui->loggerIconInfo;
    logCounterLabel = _ui->loggerMessageInfo;
  } else if (logType == TalipotLogger::Warning) {
    logIconCounterFrame = _ui->loggerFrameWarning;
    logIconLabel = _ui->loggerIconWarning;
    logCounterLabel = _ui->loggerMessageWarning;
  } else if (logType == TalipotLogger::Error) {
    logIconCounterFrame = _ui->loggerFrameError;
    logIconLabel = _ui->loggerIconError;
    logCounterLabel = _ui->loggerMessageError;
  } else {
    logIconCounterFrame = _ui->loggerFramePython;
    logIconLabel = _ui->loggerIconPython;
    logCounterLabel = _ui->loggerMessagePython;
  }

  logIconCounterFrame->setVisible(true);
  logIconLabel->setPixmap(_logger->icon(logType));
  logIconLabel->setToolTip("Click here to show/hide the message log window");
  logCounterLabel->setText(QString::number(_logger->countByType(logType)));
  logCounterLabel->setToolTip("Click here to show/hide the message log window");
}

void TalipotMainWindow::log(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  _logger->log(type, context, msg);
  updateLogIconsAndCounters();
}

TalipotMainWindow::~TalipotMainWindow() {

  qInstallMessageHandler(nullptr);

  // ensure all loaded graphs are deleted
  for (auto graph : _graphs->graphs()) {
    delete graph;
  }

  delete _pythonIDEDialog;

  PythonCodeEditor::deleteStaticResources();

  delete _project;
  delete _ui;
}

bool TalipotMainWindow::terminated() {

  _pythonIDE->savePythonFilesAndWriteToProject(true);
  _pythonIDEDialog->hide();

  if (_graphs->needsSaving() || isWindowModified()) {
    QString message("The project has been modified (loaded graphs or Python files opened in the "
                    "IDE).\nDo you want to save your changes?");
    QMessageBox::StandardButton answer = QMessageBox::question(
        this, "Save", message,
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Escape);

    if ((answer == QMessageBox::Yes && !save()) || (answer == QMessageBox::Cancel)) {
      return false;
    }
  }

  // force workspace and views destruction here to avoid hanging on exit
  // when linking against QtWebEngine binaries provided by qt.io
  _ui->workspace->closeAll();
  return true;
}

void TalipotMainWindow::logCleared() {
  _ui->loggerMessageInfo->clear();
  _ui->loggerIconInfo->clear();
  _ui->loggerFrameInfo->setVisible(false);
  _ui->loggerFrameWarning->setVisible(false);
  _ui->loggerFrameError->setVisible(false);
  _ui->loggerFramePython->setVisible(false);
}

void TalipotMainWindow::findPlugins() {
  _ui->algorithmRunner->findPlugins();
}

bool TalipotMainWindow::eventFilter(QObject *obj, QEvent *ev) {
  if (ev->type() == QEvent::DragEnter) {
    QDragEnterEvent *dragEvent = static_cast<QDragEnterEvent *>(ev);

    if (dragEvent->mimeData()->hasUrls()) {
      dragEvent->accept();
    }
  }

  if (ev->type() == QEvent::Drop) {
    QDropEvent *dropEvent = static_cast<QDropEvent *>(ev);

    for (const QUrl &url : dropEvent->mimeData()->urls()) {
      open(url.toLocalFile());
    }
  }

  if (obj == _ui->loggerFrame && ev->type() == QEvent::MouseButtonPress)
    showHideLogger();

  if (obj == this && _logger &&
      (ev->type() == QEvent::Resize || ev->type() == QEvent::Move ||
       ev->type() == QEvent::WindowStateChange)) {

    if (_logger->anchored()) {
      resetLoggerDialogPosition();
    }
  }

  return false;
}

void TalipotMainWindow::showLogger() {
  _logger->show();

  static bool firstTime = true;

  if (firstTime) {
    resetLoggerDialogPosition();
    firstTime = false;
  }
}

void TalipotMainWindow::showHideLogger() {

  if (!_logger->isVisible()) {
    showLogger();
  } else {
    _logger->hide();
  }
}

void TalipotMainWindow::redrawPanels(bool center) {
  _ui->workspace->redrawPanels(center);
}

void TalipotMainWindow::start(const QString &inputFilePath) {

  PluginProgress *pluginProgress = progress(ProgressOptions(IsStoppable | IsCancellable));

  if (!inputFilePath.isEmpty() && inputFilePath.endsWith(".tlpx")) {
    if (!_project->openProjectFile(inputFilePath, pluginProgress)) {
      _project->clearProject();
    }
  }

  // Open project with model
  QMap<QString, tlp::Graph *> rootIds;

  if (!_project->projectFile().isEmpty()) {
    rootIds = _graphs->readProject(_project, pluginProgress);

    if (rootIds.empty())
      QMessageBox::critical(this,
                            QString("Error while loading project ").append(_project->projectFile()),
                            QString("The Talipot project file is probably corrupted.<br>") +
                                tlpStringToQString(pluginProgress->getError()));
  }

  // these ui initializations are needed here
  // in case of a call to showStartPanels in the open method
  _ui->graphHierarchiesEditor->setModel(_graphs);
  _ui->workspace->setModel(_graphs);

  if (!rootIds.empty())
    _ui->workspace->readProject(_project, rootIds, pluginProgress);

  _ui->searchPanel->setModel(_graphs);

  delete pluginProgress;

  connect(_ui->pythonButton, &QAbstractButton::clicked, this, &TalipotMainWindow::setPythonPanel);
  connect(_ui->developButton, &QAbstractButton::clicked, this, &TalipotMainWindow::showPythonIDE);
  _ui->pythonPanel->setModel(_graphs);
  _pythonIDE->setGraphsModel(_graphs);
  connect(_pythonIDE, &PythonIDE::anchoredRequest, this, &TalipotMainWindow::anchoredPythonIDE);
  tlp::PluginsManager::instance().addListener(this);
  QTimer::singleShot(100, this, &TalipotMainWindow::initPythonIDE);

  if (!inputFilePath.isEmpty() && QFileInfo(inputFilePath).exists()) {
    open(inputFilePath);
  }

  for (auto h : _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    connect(h, &HeaderFrame::expanded, this, &TalipotMainWindow::refreshDockExpandControls);
  }

  connect(_ui->sidebarButton, &QAbstractButton::clicked, this, &TalipotMainWindow::showHideSideBar);
  connect(_ui->menubarButton, &QAbstractButton::clicked, this, &TalipotMainWindow::showHideMenuBar);

  // fill menu with recent documents
  buildRecentDocumentsMenu();

  logCleared();

  projectFileChanged();
}

tlp::GraphHierarchiesModel *TalipotMainWindow::model() const {
  return _graphs;
}

void TalipotMainWindow::refreshDockExpandControls() {
  QList<HeaderFrame *> expandedHeaders, collapsedHeaders;

  for (auto h : _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    h->expandControl()->setEnabled(true);

    if (h->isExpanded())
      expandedHeaders.push_back(h);
    else
      collapsedHeaders.push_back(h);
  }

  if (expandedHeaders.size() == 1)
    expandedHeaders[0]->expandControl()->setEnabled(false);
}

void TalipotMainWindow::exportGraph(Graph *g) {
  if (g == nullptr) {
    g = _graphs->currentGraph();
  }

  if (g == nullptr) {
    return;
  }

  static QString exportFile;
  ExportWizard wizard(g, exportFile, this);
  wizard.setWindowTitle(QString("Exporting graph \"") + tlpStringToQString(g->getName()) + '"');

  if (wizard.exec() != QDialog::Accepted || wizard.algorithm().isEmpty() ||
      wizard.outputFile().isEmpty()) {
    return;
  }

  std::string filename = QStringToTlpString(exportFile = wizard.outputFile());
  std::string exportPluginName = QStringToTlpString(wizard.algorithm());

  DataSet data = wizard.parameters();
  PluginProgress *prg = progress(NoProgressOption);
  prg->setTitle(exportPluginName);
  // take time before run
  QTime start = QTime::currentTime();
  bool result = tlp::saveGraph(g, filename, prg, &data);

  if (!result) {
    QMessageBox::critical(this, "Export error",
                          QString("<i>") + wizard.algorithm() +
                              "</i> failed to export graph.<br/><br/><b>" +
                              tlp::tlpStringToQString(prg->getError()) + "</b>");
  } else {
    // log export plugin call
    if (Settings::instance().logPluginCall() != Settings::NoLog) {
      std::stringstream log;
      log << exportPluginName.c_str() << " - " << data.toString().c_str();

      if (Settings::instance().logPluginCall() == Settings::LogCallWithExecutionTime) {
        log << ": " << start.msecsTo(QTime::currentTime()) << "ms";
      }

      qDebug() << log.str().c_str();
    }

    addRecentDocument(wizard.outputFile());
  }

  delete prg;
}

void TalipotMainWindow::saveGraphHierarchyInTlpFile(Graph *g) {
  if (g == nullptr) {
    g = _graphs->currentGraph();
  }

  if (g == nullptr) {
    return;
  }

  static QString savedFile;
  QString filter("TLP format (*.tlp *.tlp.gz *.tlpz);;TLPB format (*.tlpb *.tlpb.gz *.tlpbz)");
  QString filename = QFileDialog::getSaveFileName(this, tr("Save graph hierarchy in tlp/tlpb file"),
                                                  savedFile, filter);

  if (!filename.isEmpty()) {
    bool result = tlp::saveGraph(g, tlp::QStringToTlpString(filename));

    if (!result) {
      QMessageBox::critical(this, "Save error", "Failed to save graph hierarchy");
    } else {
      savedFile = filename;
      addRecentDocument(filename);
    }
  }
}

void TalipotMainWindow::importGraph(const std::string &module, DataSet &data) {
  Graph *g;

  if (!module.empty()) {
    PluginProgress *prg = progress(ProgressOptions(IsStoppable | IsCancellable));
    prg->setTitle(module);
    // take time before run
    QTime start = QTime::currentTime();
    g = tlp::importGraph(module, data, prg);

    if (g == nullptr) {
      QMessageBox::critical(this, "Import error",
                            QString("<i>") + tlp::tlpStringToQString(module) +
                                "</i> failed to import data.<br/><br/><b>" +
                                tlp::tlpStringToQString(prg->getError()) + "</b>");
      delete prg;
      return;
    }

    delete prg;

    // log import plugin call
    if (Settings::instance().logPluginCall() != Settings::NoLog) {
      std::stringstream log;
      log << module.c_str() << " import - " << data.toString().c_str();

      if (Settings::instance().logPluginCall() == Settings::LogCallWithExecutionTime) {
        log << ": " << start.msecsTo(QTime::currentTime()) << "ms";
      }

      qDebug() << log.str().c_str();
    }

    if (g->getName().empty()) {
      g->setName(module);
      g->setAttribute("Import algorithm", module);
      g->setAttribute("Import parameters", data.toString());
    }
  } else {
    g = tlp::newGraph();
  }

  _graphs->addGraph(g);
  std::string fileName;

  if (data.get("file::filename", fileName))
    // set current directory to the directory of the loaded file
    // to ensure a correct loading of the associated texture files if any
    QDir::setCurrent(QFileInfo(tlpStringToQString(fileName)).absolutePath());

  applyRandomLayout(g);
  showStartPanels(g);
}

void TalipotMainWindow::importGraph() {
  ImportWizard wizard(this);

  if (wizard.exec() == QDialog::Accepted) {
    DataSet data = wizard.parameters();
    importGraph(tlp::QStringToTlpString(wizard.algorithm()), data);
  }
}

void TalipotMainWindow::createPanel(tlp::Graph *g) {
  if (_graphs->empty()) {
    return;
  }

  PanelSelectionWizard wizard(_graphs, this);

  if (g != nullptr) {
    wizard.setSelectedGraph(g);
  } else {
    wizard.setSelectedGraph(_graphs->currentGraph());
  }

  int result = wizard.exec();

  if (result == QDialog::Accepted && wizard.panel() != nullptr) {
    // expose mode is not safe to add a new panel
    // so hide it if needed
    _ui->workspace->hideExposeMode();
    _ui->workspace->addPanel(wizard.panel());
    _ui->workspace->setActivePanel(wizard.panel());
    wizard.panel()->applySettings();
  }
}

void TalipotMainWindow::panelFocused(tlp::View *view) {
  disconnect(view, &View::graphSet, this, &TalipotMainWindow::focusedPanelGraphSet);

  if (!_ui->graphHierarchiesEditor->synchronized())
    return;

  connect(view, &View::graphSet, this, &TalipotMainWindow::focusedPanelGraphSet);
  focusedPanelGraphSet(view->graph());
}

void TalipotMainWindow::changeSynchronization(bool s) {
  _ui->workspace->setFocusedPanelHighlighting(s);
}

void TalipotMainWindow::focusedPanelGraphSet(Graph *g) {
  _graphs->setCurrentGraph(g);
}

void TalipotMainWindow::focusedPanelSynchronized() {
  _ui->workspace->setGraphForFocusedPanel(_graphs->currentGraph());
}

bool TalipotMainWindow::save() {
  return saveAs(_project->projectFile());
}

bool TalipotMainWindow::saveAs(const QString &path) {
  if (_graphs->empty()) {
    return false;
  }

  if (path.isEmpty()) {
    QString path =
        QFileDialog::getSaveFileName(this, "Save project", QString(), "Talipot Project (*.tlpx)");

    if (!path.isEmpty()) {
      if (!path.endsWith(".tlpx")) {
        path += ".tlpx";
      }

      _project->setProjectFile(path);
      return saveAs(path);
    }

    return false;
  }

  SimplePluginProgressDialog progress(this);
  progress.showPreview(false);
  progress.show();
  QMap<Graph *, QString> rootIds = _graphs->writeProject(_project, &progress);
  _ui->workspace->writeProject(_project, rootIds, &progress);
  _pythonIDE->savePythonFilesAndWriteToProject();
  bool ret = _project->write(path, &progress);

  if (ret) {
    Settings::instance().addToRecentDocuments(path);
  }

  return ret;
}

void TalipotMainWindow::open(QString fileName) {
  QMap<std::string, std::string> modules;
  std::list<std::string> imports = PluginsManager::availablePlugins<ImportModule>();

  std::string filters("Talipot project (*.tlpx);;");
  std::string filterAny("Any supported format (");

  for (auto &import : imports) {
    ImportModule *m = PluginsManager::getPluginObject<ImportModule>(import);
    std::list<std::string> fileExtension(m->allFileExtensions());

    std::string currentFilter;

    for (auto &ext : fileExtension) {

      if (ext.empty())
        continue;

      filterAny += "*." + ext + " ";
      currentFilter += "*." + ext + " ";

      modules[ext] = import;
    }

    if (!currentFilter.empty())
      filters += import + "(" + currentFilter + ");;";

    delete m;
  }

  filterAny += " *.tlpx);;";

  filters += "All files (*)";
  filters.insert(0, filterAny);

  if (fileName.isEmpty()) { // If open() was called without a parameter, open the file dialog
    fileName =
        QFileDialog::getOpenFileName(this, tr("Open graph"), _lastOpenLocation, filters.c_str());
  }

  if (!fileName.isEmpty()) {
    QFileInfo fileInfo(fileName);

    _lastOpenLocation = fileInfo.absolutePath();

    for (const std::string &extension : modules.keys()) {
      if (fileName.endsWith(".tlpx")) {
        openProjectFile(fileName);
        Settings::instance().addToRecentDocuments(fileInfo.absoluteFilePath());
        break;
      } else if (fileName.endsWith(QString::fromStdString(extension))) {
        DataSet params;
        params.set("file::filename", QStringToTlpString(fileName));
        addRecentDocument(fileName);
        importGraph(modules[extension], params);
        break;
      }
    }
  }
}

void TalipotMainWindow::openProjectFile(const QString &path) {
  cleanProject();
  PluginProgress *prg = progress(NoProgressOption);
  if (_project->openProjectFile(path, prg)) {
    QMap<QString, tlp::Graph *> rootIds = _graphs->readProject(_project, prg);
    _ui->workspace->readProject(_project, rootIds, prg);
    QTimer::singleShot(100, this, &TalipotMainWindow::initPythonIDE);
  } else {
    QMessageBox::critical(this,
                          QString("Error while loading project ").append(_project->projectFile()),
                          QString("The Talipot project file is probably corrupted:<br>") +
                              tlpStringToQString(prg->getError()));
  }
  delete prg;
}

void TalipotMainWindow::initPythonIDE() {
  _pythonIDE->setProject(_project);
  _pythonIDE->setAnchored(Settings::instance().pythonIDEAnchored());
  if (Settings::instance().pythonIDEAnchored()) {
    _pythonIDE->setVisible(false);
    _pythonIDE->setParent(nullptr);
    _ui->mainSplitter->addWidget(_pythonIDE);
    _ui->developButton->setCheckable(true);
  }
}

void TalipotMainWindow::anchoredPythonIDE(bool anchored) {
  _ui->developButton->setCheckable(anchored);
  Settings::instance().setPythonIDEAnchored(anchored);
  if (anchored) {
    _pythonIDEDialog->hide();
    _pythonIDE->setParent(nullptr);
    _ui->mainSplitter->addWidget(_pythonIDE);
    _ui->mainSplitter->setCollapsible(1, true);
    _ui->mainSplitter->setCollapsible(2, true);
    _ui->developButton->setChecked(anchored);
  } else {
    _pythonIDE->setParent(nullptr);
    _pythonIDEDialog->layout()->addWidget(_pythonIDE);
    _pythonIDEDialog->show();
    _ui->developButton->setChecked(false);
    _ui->mainSplitter->setCollapsible(1, false);
  }
  _pythonIDE->setAnchored(anchored);
}

void TalipotMainWindow::deleteSelectedElementsFromRootGraph() {
  deleteSelectedElements(true);
}

void TalipotMainWindow::clearGraph() {
  if (QMessageBox::question(this, "Clear graph content",
                            "Do you really want to remove all nodes and edges from the current "
                            "graph. This action cannot be undone",
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
    _graphs->currentGraph()->clear();
  }
}

void TalipotMainWindow::deleteSelectedElements(bool fromRoot) {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();
  tlp::BooleanProperty *selection = graph->getBooleanProperty("viewSelection");

  vector<tlp::edge> edgesToDelete = iteratorVector(selection->getEdgesEqualTo(true, graph));
  bool hasPush = !edgesToDelete.empty();

  if (hasPush) {
    graph->push();
    graph->delEdges(edgesToDelete, fromRoot);
  }

  vector<tlp::node> nodesToDelete = iteratorVector(selection->getNodesEqualTo(true, graph));

  if (!hasPush && !nodesToDelete.empty())
    graph->push();

  graph->delNodes(nodesToDelete, fromRoot);

  Observable::unholdObservers();
}

void TalipotMainWindow::invertSelection() {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();
  tlp::BooleanProperty *selection = graph->getBooleanProperty("viewSelection");
  graph->push();
  selection->reverse(graph);
  Observable::unholdObservers();
}

void TalipotMainWindow::reverseSelectedEdges() {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();
  tlp::BooleanProperty *selection = graph->getBooleanProperty("viewSelection");
  graph->push();
  selection->reverseEdgeDirection(graph);
  graph->popIfNoUpdates();
  Observable::unholdObservers();
}

void TalipotMainWindow::cancelSelection() {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();
  tlp::BooleanProperty *selection = graph->getBooleanProperty("viewSelection");
  graph->push();
  selection->setAllNodeValue(false, graph);
  selection->setAllEdgeValue(false, graph);
  graph->popIfNoUpdates();
  Observable::unholdObservers();
}

void TalipotMainWindow::selectAll(bool nodes, bool edges) {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();
  tlp::BooleanProperty *selection = graph->getBooleanProperty("viewSelection");
  graph->push();
  selection->setAllNodeValue(false);
  selection->setAllEdgeValue(false);

  if (nodes) {
    selection->setAllNodeValue(true, graph);
  }

  if (edges) {
    selection->setAllEdgeValue(true, graph);
  }

  Observable::unholdObservers();
}

void TalipotMainWindow::selectAllEdges() {
  selectAll(false, true);
}

void TalipotMainWindow::selectAllNodes() {
  selectAll(true, false);
}

void TalipotMainWindow::undo() {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();

  if (graph != nullptr) {
    graph->pop();
  }

  Observable::unholdObservers();

  for (auto v : _ui->workspace->panels()) {
    if (v->graph() == graph)
      v->undoCallback();
  }
}

void TalipotMainWindow::redo() {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();

  if (graph != nullptr) {
    graph->unpop();
  }

  Observable::unholdObservers();

  for (auto v : _ui->workspace->panels()) {
    if (v->graph() == graph)
      v->undoCallback();
  }
}

void TalipotMainWindow::cut() {
  copy(_graphs->currentGraph(), true);
}

void TalipotMainWindow::paste() {
  if (_graphs->currentGraph() == nullptr) {
    return;
  }

  Graph *outGraph = _graphs->currentGraph();
  std::stringstream ss;
  ss << QStringToTlpString(QApplication::clipboard()->text());

  Observable::holdObservers();
  outGraph->push();
  DataSet data;
  data.set("file::data", ss.str());
  Graph *inGraph = tlp::importGraph("TLP Import", data);
  tlp::copyToGraph(outGraph, inGraph);
  outGraph->popIfNoUpdates();
  delete inGraph;
  Observable::unholdObservers();
  centerPanelsForGraph(outGraph);
}

void TalipotMainWindow::copy() {
  copy(_graphs->currentGraph());
}

void TalipotMainWindow::copy(Graph *g, bool deleteAfter) {
  if (g == nullptr) {
    return;
  }

  BooleanProperty *selection = g->getBooleanProperty("viewSelection");

  Graph *copyGraph = tlp::newGraph();
  tlp::copyToGraph(copyGraph, g, selection);

  if (!copyGraph->isEmpty()) {
    std::stringstream ss;
    DataSet data;
    tlp::exportGraph(copyGraph, ss, "TLP Export", data);
    delete copyGraph;
    QApplication::clipboard()->setText(tlpStringToQString(ss.str()));

    if (deleteAfter) {
      Observable::holdObservers();
      g->push();

      for (auto n : stableIterator(selection->getNonDefaultValuatedNodes(g))) {
        g->delNode(n);
      }
      Observable::unholdObservers();
    }
  }
}

void TalipotMainWindow::group() {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();
  tlp::BooleanProperty *selection = graph->getBooleanProperty("viewSelection");
  std::vector<node> groupedNodes;
  for (auto n : selection->getNodesEqualTo(true, graph)) {
    groupedNodes.push_back(n);
  }

  if (groupedNodes.empty()) {
    Observable::unholdObservers();
    qCritical() << "[Group] Cannot create meta-nodes from empty selection";
    return;
  }

  graph->push();

  bool changeGraph = false;

  if (graph == graph->getRoot()) {
    qWarning() << "[Group] Grouping can not be done on the root graph. A subgraph has "
                  "automatically been created";
    graph = graph->addCloneSubGraph("groups");
    changeGraph = true;
  }

  graph->createMetaNode(groupedNodes, false);

  selection->setAllNodeValue(false);
  selection->setAllEdgeValue(false);

  Observable::unholdObservers();

  if (!changeGraph) {
    return;
  }

  for (auto v : _ui->workspace->panels()) {
    if (v->graph() == graph->getRoot())
      v->setGraph(graph);
  }
}

void TalipotMainWindow::make_graph() {
  Graph *graph = _graphs->currentGraph();
  makeSelectionGraph(_graphs->currentGraph(), graph->getBooleanProperty("viewSelection"));
}

Graph *TalipotMainWindow::createSubGraph(Graph *graph) {
  if (graph == nullptr) {
    return nullptr;
  }

  graph->push();
  Observable::holdObservers();
  BooleanProperty *selection = graph->getBooleanProperty("viewSelection");
  makeSelectionGraph(graph, selection);
  Graph *result = graph->addSubGraph(selection, "selection subgraph");
  Observable::unholdObservers();
  return result;
}

void TalipotMainWindow::createSubGraph() {
  createSubGraph(_graphs->currentGraph());
}

void TalipotMainWindow::cloneSubGraph() {
  if (_graphs->currentGraph() == nullptr) {
    return;
  }

  tlp::BooleanProperty prop(_graphs->currentGraph());
  prop.setAllNodeValue(true);
  prop.setAllEdgeValue(true);
  _graphs->currentGraph()->push();
  _graphs->currentGraph()->addSubGraph(&prop, "clone subgraph");
}

void TalipotMainWindow::addEmptySubGraph() {
  if (_graphs->currentGraph() == nullptr) {
    return;
  }

  _graphs->currentGraph()->push();
  _graphs->currentGraph()->addSubGraph(nullptr, "empty subgraph");
}

void TalipotMainWindow::currentGraphChanged(Graph *graph) {
  bool enabled(graph != nullptr);
  _ui->actionUndo->setEnabled(enabled);
  _ui->actionRedo->setEnabled(enabled);
  _ui->actionCut->setEnabled(enabled);
  _ui->actionCopy->setEnabled(enabled);
  _ui->actionPaste->setEnabled(enabled);
  _ui->actionDelete->setEnabled(enabled);
  _ui->actionDelete_from_the_root_graph->setEnabled(enabled && (graph != graph->getRoot()));
  _ui->actionDelete_all->setEnabled(enabled);
  _ui->actionInvert_selection->setEnabled(enabled);
  _ui->actionReverse_selected_edges->setEnabled(enabled);
  _ui->actionSelect_All->setEnabled(enabled);
  _ui->actionSelect_All_Nodes->setEnabled(enabled);
  _ui->actionSelect_All_Edges->setEnabled(enabled);
  _ui->actionCancel_selection->setEnabled(enabled);
  _ui->actionMake_selection_a_graph->setEnabled(enabled);
  _ui->actionGroup_elements->setEnabled(enabled);
  _ui->actionCreate_sub_graph->setEnabled(enabled);
  _ui->actionCreate_empty_sub_graph->setEnabled(enabled);
  _ui->actionClone_sub_graph->setEnabled(enabled);
  _ui->actionExport->setEnabled(enabled);
  _ui->singleModeButton->setEnabled(enabled);
  _ui->splitModeButton->setEnabled(enabled);
  _ui->splitHorizontalModeButton->setEnabled(enabled);
  _ui->split3ModeButton->setEnabled(enabled);
  _ui->split32ModeButton->setEnabled(enabled);
  _ui->split33ModeButton->setEnabled(enabled);
  _ui->gridModeButton->setEnabled(enabled);
  _ui->sixModeButton->setEnabled(enabled);
  _ui->exposeModeButton->setEnabled(enabled);
  _ui->searchButton->setEnabled(enabled);
  _ui->pythonButton->setEnabled(enabled);
  _ui->exportButton->setEnabled(enabled);
  _ui->previousPageButton->setVisible(enabled);
  _ui->pageCountLabel->setVisible(enabled);
  _ui->nextPageButton->setVisible(enabled);
  _ui->actionSave_Project->setEnabled(enabled);
  _ui->actionSave_Project_as->setEnabled(enabled);

  if (graph == nullptr) {
    _ui->workspace->switchToStartupMode();
    _ui->exposeModeButton->setChecked(false);
    _ui->searchButton->setChecked(false);
    _ui->pythonButton->setChecked(false);
    setSearchOutput(false);
    _ui->actionSave_Project->setEnabled(false);
    _ui->actionSave_Project_as->setEnabled(false);
  } else {
    _ui->workspace->setGraphForFocusedPanel(graph);
  }

  if (_graphs->empty()) {
    _pythonIDE->clearPythonCodeEditors();
    _pythonIDEDialog->hide();
    _ui->developButton->setEnabled(false);
    _ui->actionPython_IDE->setEnabled(false);
  } else {
    _ui->developButton->setEnabled(true);
    _ui->actionPython_IDE->setEnabled(true);
  }
}

void TalipotMainWindow::CSVImport() {
  bool mustDeleteGraph = false;

  if (_graphs->empty()) {
    _graphs->addGraph(tlp::newGraph());
    mustDeleteGraph = true;
  }

  Graph *g = _graphs->currentGraph();

  if (g == nullptr) {
    return;
  }

  CSVImportWizard wizard(this);

  if (mustDeleteGraph) {
    wizard.setWindowTitle("Import CSV data into a new graph");
    wizard.setButtonText(QWizard::FinishButton, QString("Import into a new graph"));
  } else {
    wizard.setWindowTitle(QString("Import CSV data into current graph: ") + g->getName().c_str());
    wizard.setButtonText(QWizard::FinishButton, QString("Import into current graph"));
  }

  // get the number of line displayed in the logger
  unsigned int nbLogsBefore = _logger->countByType(TalipotLogger::Error);
  nbLogsBefore += _logger->countByType(TalipotLogger::Warning);

  wizard.setGraph(g);

  tlp::ObserverHolder oh;

  g->push();
  int result = wizard.exec();

  if (result == QDialog::Rejected) {
    if (mustDeleteGraph) {
      _graphs->removeGraph(g);
      delete g;
    } else {
      g->pop();
    }

    return;
  } else {
    unsigned int nbLogsAfter = _logger->countByType(TalipotLogger::Error);
    nbLogsAfter += _logger->countByType(TalipotLogger::Warning);
    applyRandomLayout(g);
    bool openPanels = true;

    for (auto v : _ui->workspace->panels()) {
      if (v->graph() == g) {
        openPanels = false;
        break;
      }
    }

    if (openPanels) {
      showStartPanels(g);
    } else {
      centerPanelsForGraph(g);
    }

    unsigned nb_error = nbLogsAfter - nbLogsBefore;
    if ((nb_error == 1) &&
        (QMessageBox::question(this, "CSV parse error",
                               "When parsing your CSV file,<br/> one error has been "
                               "encountered.<br/>Do you want to see it?",
                               QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)) {
      showLogger();
    }
    if ((nb_error > 1) &&
        (QMessageBox::question(this, "CSV parse errors",
                               QString("When parsing your CSV file,<br/> %1 errors have been "
                                       "encountered.<br/>Do you want to see them?")
                                   .arg(nb_error),
                               QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)) {
      showLogger();
    }

    g->popIfNoUpdates();
  }
}

void TalipotMainWindow::showStartPanels(Graph *g) {
  if (Settings::instance().displayDefaultViews() == false) {
    return;
  }

  // expose mode is not safe to add a new panel
  // so hide it if needed
  _ui->workspace->hideExposeMode();
  View *firstPanel = nullptr;

  for (auto panelName : {"Spreadsheet", "Node Link Diagram"}) {
    View *view = PluginsManager::getPluginObject<View>(panelName);

    if (firstPanel == nullptr) {
      firstPanel = view;
    }

    view->setupUi();
    view->setGraph(g);
    view->setState(DataSet());
    _ui->workspace->addPanel(view);
  }

  _ui->workspace->setActivePanel(firstPanel);
  _ui->workspace->switchToSplitHorizontalMode();
}

void TalipotMainWindow::applyRandomLayout(Graph *g) {
  Observable::holdObservers();
  LayoutProperty *viewLayout = g->getLayoutProperty("viewLayout");

  if (!viewLayout->hasNonDefaultValuatedNodes(g)) {
    std::string str;
    g->applyPropertyAlgorithm("Random layout", viewLayout, str);
  }

  Observable::unholdObservers();
}

void TalipotMainWindow::centerPanelsForGraph(tlp::Graph *g, bool graphChanged,
                                             bool onlyGlMainView) {
  for (auto v : _ui->workspace->panels()) {
    if ((v->graph() == g) && (!onlyGlMainView || dynamic_cast<tlp::GlMainView *>(v)))
      v->centerView(graphChanged);
  }
}

void TalipotMainWindow::closePanelsForGraph(tlp::Graph *g) {
  list<View *> viewsToDelete;

  for (auto v : _ui->workspace->panels()) {
    if (v->graph() == g || g->isDescendantGraph(v->graph()))
      viewsToDelete.push_back(v);
  }

  if (!viewsToDelete.empty()) {
    // expose mode is not safe when deleting a panel
    // so hide it first
    _ui->workspace->hideExposeMode();
    for (auto v : viewsToDelete) {
      _ui->workspace->delView(v);
    }
  }
}

bool TalipotMainWindow::setGlMainViewPropertiesForGraph(
    tlp::Graph *g, const std::map<std::string, tlp::PropertyInterface *> &propsMap) {
  bool result = false;

  for (auto v : _ui->workspace->panels()) {
    GlMainView *glMainView = dynamic_cast<tlp::GlMainView *>(v);

    if (v->graph() == g && glMainView != nullptr) {
      if (glMainView->getGlMainWidget()
              ->getScene()
              ->getGlGraphComposite()
              ->getInputData()
              ->installProperties(propsMap))
        result = true;
    }
  }

  return result;
}

void TalipotMainWindow::setSearchOutput(bool f) {
  if (f) {
    _ui->outputFrame->setCurrentWidget(_ui->searchPanel);
    _ui->pythonButton->setChecked(false);
  }

  _ui->outputFrame->setVisible(f);
}

void TalipotMainWindow::setPythonPanel(bool f) {
  if (f) {
    _ui->outputFrame->setCurrentWidget(_ui->pythonPanel);
    _ui->searchButton->setChecked(false);
  }

  _ui->outputFrame->setVisible(f);
}

void TalipotMainWindow::openPreferences() {
  PreferencesDialog dlg(_ui->mainWidget);
  dlg.readSettings();

  if (dlg.exec() == QDialog::Accepted) {
    dlg.writeSettings();

    for (auto v : _ui->workspace->panels()) {
      GlMainView *glMainView = dynamic_cast<tlp::GlMainView *>(v);

      if (glMainView != nullptr) {
        if (glMainView->getGlMainWidget() != nullptr) {
          glMainView->getGlMainWidget()
              ->getScene()
              ->getGlGraphComposite()
              ->getRenderingParametersPointer()
              ->setSelectionColor(Settings::instance().defaultSelectionColor());
          glMainView->redraw();
        }
      }
    }
  }
}

void TalipotMainWindow::setAutoCenterPanelsOnDraw(bool f) {
  _ui->workspace->setAutoCenterPanelsOnDraw(f);
}

void TalipotMainWindow::pluginsListChanged() {
  _ui->algorithmRunner->refreshPluginsList();
}

void TalipotMainWindow::addNewGraph() {
  Graph *g = tlp::newGraph();
  _graphs->addGraph(g);
  showStartPanels(g);
}

void TalipotMainWindow::newProject() {
  cleanProject();
  _project->setProjectFile("");
}

void TalipotMainWindow::openRecentFile() {
  QAction *action = static_cast<QAction *>(sender());
  open(action->data().toString());
}

void TalipotMainWindow::treatEvent(const tlp::Event &ev) {
  if (dynamic_cast<const tlp::PluginEvent *>(&ev)) {
    pluginsListChanged();
  }
}

void TalipotMainWindow::showPythonIDE() {
  if (!_pythonIDE->isAnchored()) {
    _pythonIDEDialog->raise();
    _pythonIDEDialog->show();
    _pythonIDEDialog->raise();
  } else {
    _pythonIDE->setVisible(!_pythonIDE->isVisible());
    _ui->developButton->setChecked(_pythonIDE->isVisible());
    _ui->mainSplitter->setCollapsible(1, _pythonIDE->isVisible());
    _ui->mainSplitter->setCollapsible(2, _pythonIDE->isVisible());
  }
}

void TalipotMainWindow::showUserDocumentation() {
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TalipotShareDir) +
                                                "../doc/talipot/talipot-user/html/index.html"));
}

void TalipotMainWindow::showPythonDocumentation() {
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TalipotShareDir) +
                                                "../doc/talipot/talipot-python/html/index.html"));
}

void TalipotMainWindow::showAPIDocumentation() {
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TalipotShareDir) +
                                                "../doc/talipot/doxygen/html/index.html"));
}

void TalipotMainWindow::showHideSideBar() {
  if (_ui->docksWidget->isVisible()) {
    _ui->docksWidget->setVisible(false);
    _ui->sidebarButton->setToolTip("Show Sidebar");
  } else {
    _ui->docksWidget->setVisible(true);
    _ui->sidebarButton->setToolTip("Hide Sidebar");
  }

  if (_logger->anchored()) {
    resetLoggerDialogPosition();
  }
}

void TalipotMainWindow::showHideMenuBar() {
  if (menuBar()->isVisible()) {
    menuBar()->setVisible(false);
    _ui->menubarButton->setToolTip("Show Menubar");
  } else {
    menuBar()->setVisible(true);
    _ui->menubarButton->setToolTip("Hide Menubar");
  }
}

void TalipotMainWindow::displayColorScalesDialog() {
  _colorScalesDialog->show();
}

void TalipotMainWindow::showAboutPage() {
  tlp::AboutPage *aboutPage = new tlp::AboutPage;
  QDialog aboutDialog(this, Qt::Window);
  aboutDialog.setWindowTitle("About Talipot");
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(aboutPage);
  layout->setContentsMargins(0, 0, 0, 0);
  aboutDialog.setLayout(layout);
  aboutDialog.resize(800, 600);
  aboutDialog.exec();
}

void TalipotMainWindow::workspaceButtonClicked() {
  _ui->workspaceButton->setChecked(true);
}

void TalipotMainWindow::resetLoggerDialogPosition() {
  QPoint pos = mapToGlobal(_ui->exportButton->pos());
  pos.setX(pos.x() + _ui->loggerFrame->width());

  if (_logger->isVisible()) {
    _logger->showNormal();
  }

  // extend the logger frame width until reaching the right side of the main window
  _logger->setGeometry(pos.x(), pos.y(), width() - _ui->loggerFrame->width(),
                       mapToGlobal(QPoint(0, 0)).y() + height() - pos.y() - 2);
}

PluginProgress *TalipotMainWindow::progress(ProgressOptions options) {
  SimplePluginProgressDialog *dlg = new SimplePluginProgressDialog(this);
  dlg->setWindowIcon(windowIcon());
  dlg->showPreview(options.testFlag(IsPreviewable));
  dlg->setCancelButtonVisible(options.testFlag(IsCancellable));
  dlg->setStopButtonVisible(options.testFlag(IsStoppable));
  dlg->show();
  QApplication::processEvents();
  return dlg;
}

void TalipotMainWindow::closeEvent(QCloseEvent *event) {
  if (terminated()) {
    QMainWindow::closeEvent(event);
  } else {
    event->ignore();
  }
}

void TalipotMainWindow::projectFileChanged(const QString &projectFile) {
  QString wTitle(_title);

  if (!_project->name().isEmpty()) {
    wTitle += QString(" - ") + _project->name();
  } else if (!projectFile.isEmpty()) {
    wTitle += QString(" - ") + projectFile;
  } else { // All graphs are deleted, so clear project.
    _project->clearProject();
    wTitle += QString(" - unsaved project");
  }

  wTitle += "[*]"; // placeholder for window modification
#ifndef NDEBUG
  wTitle += " - [ Debug mode ]";
#endif
  setWindowTitle(wTitle);
  setWindowModified(false);
}

void TalipotMainWindow::cleanProject() {
  if (!_graphs->empty()) {
    terminated();
    // ensure the opened views and interactors get deleted before the loaded graphs
    // to avoid possible segfaults
    _ui->workspace->closeAll();

    // ensure all loaded graphs are deleted
    for (Graph *graph : _graphs->graphs()) {
      delete graph;
    }

    _pythonIDE->clearPythonCodeEditors();
    _project->clearProject();
    _graphs->clear();
  }
}

void TalipotMainWindow::showFullScreen(bool f) {
  if (f) {
    _maximized = isMaximized();
    QMainWindow::showFullScreen();
  } else {
    showNormal();

    if (_maximized) {
      showMaximized();
    }
  }
}

void TalipotMainWindow::showPluginsCenter() {
  _pluginsCenter->setWindowIcon(windowIcon());
  _pluginsCenter->show();
}