/**
 *
 * Copyright (C) 2019  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#ifndef TALIPOT_MAIN_WINDOW_H
#define TALIPOT_MAIN_WINDOW_H

#include <talipot/Observable.h>

#include <QModelIndex>
#include <QPoint>
#include <QMainWindow>

class QAction;
class QHeaderView;
class QDialog;

class GraphHierarchiesEditor;
class TalipotLogger;
class PluginsCenter;

namespace tlp {
class BooleanProperty;
class ColorScaleConfigDialog;
class DataSet;
class Graph;
class GraphHierarchiesModel;
class PluginProgress;
class Project;
class PropertyInterface;
class View;
}

namespace Ui {
class TalipotMainWindowData;
}

class PythonPanel;
namespace tlp {
class PythonIDE;
}

class TalipotMainWindow : public QMainWindow, tlp::Observable {
  Q_OBJECT
  Ui::TalipotMainWindowData *_ui;
  tlp::GraphHierarchiesModel *_graphs;
  tlp::ColorScaleConfigDialog *_colorScalesDialog;
  tlp::Project *_project;
  PluginsCenter *_pluginsCenter;
  PythonPanel *_pythonPanel;
  tlp::PythonIDE *_pythonIDE;
  QDialog *_pythonIDEDialog;

  QString _lastOpenLocation;
  QString _recentDocumentsSettingsKey;
  QString _title;

  bool _maximized;

  void buildRecentDocumentsMenu();
  void addRecentDocument(const QString &path);

  void showStartPanels(tlp::Graph *);
  void applyRandomLayout(tlp::Graph *);

  static TalipotMainWindow *_instance;

public:
  TalipotLogger *_logger;

  TalipotMainWindow();
  ~TalipotMainWindow() override;
  void start(const QString &inputFilePath);
  tlp::GraphHierarchiesModel *model() const;
  void copy(tlp::Graph *, bool deleteAfter = false);
  tlp::Graph *createSubGraph(tlp::Graph *);

  void treatEvent(const tlp::Event &) override;

  void log(QtMsgType, const QMessageLogContext &, const QString &);

  bool terminated();

  PluginsCenter *pluginsCenter() const {
    return _pluginsCenter;
  }

  enum ProgressOption {
    NoProgressOption = 0x0,
    IsPreviewable = 0x1,
    IsCancellable = 0x2,
    IsStoppable = 0x4
  };
  Q_DECLARE_FLAGS(ProgressOptions, ProgressOption)
  tlp::PluginProgress *
  progress(ProgressOptions options = ProgressOptions(IsPreviewable | IsStoppable | IsCancellable));

  void resetTitle() {
    emit resetWindowTitle();
  }

  static TalipotMainWindow *getInstance();

public slots:
  void importGraph();
  void exportGraph(tlp::Graph *g = nullptr);
  void saveGraphHierarchyInTlpFile(tlp::Graph *g = nullptr);
  void createPanel(tlp::Graph *g = nullptr);
  bool save();
  bool saveAs(const QString &path = "");
  void open(QString fileName = "");
  void openProjectFile(const QString &path);

  void showLogger();
  void showAPIDocumentation();
  void showUserDocumentation();
  void showPythonDocumentation();

  void redrawPanels(bool center = false);
  void centerPanelsForGraph(tlp::Graph *, bool graphChanged, bool onlyGlMainView);
  void centerPanelsForGraph(tlp::Graph *g) {
    centerPanelsForGraph(g, false, false);
  }
  void closePanelsForGraph(tlp::Graph *g = nullptr);
  bool
  setGlMainViewPropertiesForGraph(tlp::Graph *g,
                                  const std::map<std::string, tlp::PropertyInterface *> &propsMap);
  void setSearchOutput(bool);
  void setPythonPanel(bool);
  void openPreferences();

  void setAutoCenterPanelsOnDraw(bool f);

  void pluginsListChanged();

  void showPythonIDE();

  void displayColorScalesDialog();

  void showAboutPage();

protected slots:
  void currentGraphChanged(tlp::Graph *graph);
  void refreshDockExpandControls();
  void panelFocused(tlp::View *);
  void focusedPanelGraphSet(tlp::Graph *);
  void focusedPanelSynchronized();
  void clearGraph();
  void deleteSelectedElements(bool fromRoot = false);
  void deleteSelectedElementsFromRootGraph();
  void invertSelection();
  void reverseSelectedEdges();
  void cancelSelection();
  void make_graph();
  void selectAll(bool nodes = true, bool edges = true);
  void selectAllNodes();
  void selectAllEdges();
  void undo();
  void redo();
  void cut();
  void paste();
  void copy();
  void group();
  void createSubGraph();
  void cloneSubGraph();
  void addEmptySubGraph();
  void CSVImport();
  void logCleared();
  void findPlugins();
  void addNewGraph();
  void newProject();
  void openRecentFile();
  void changeSynchronization(bool);
  void showHideSideBar();
  void workspaceButtonClicked();
  void resetLoggerDialogPosition();
  void showHideLogger();
  void showHideMenuBar();
  void initPythonIDE();
  void anchoredPythonIDE(bool anchored);
  void projectFileChanged(const QString &projectFile = "");
  void showFullScreen(bool f);
  void showPluginsCenter();

signals:
  void resetWindowTitle();

protected:
  bool eventFilter(QObject *, QEvent *) override;
  void closeEvent(QCloseEvent *event) override;
  void importGraph(const std::string &module, tlp::DataSet &data);
  void updateLogIconsAndCounters();
  void cleanProject();
};

#endif // TALIPOT_MAIN_WINDOW_H