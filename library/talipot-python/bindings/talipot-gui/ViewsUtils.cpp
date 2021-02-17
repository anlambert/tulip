/**
 *
 * Copyright (C) 2019-2021  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include <sipAPI_talipotgui.h>

#include <QApplication>

#include <talipot/PluginsManager.h>
#include <talipot/WorkspacePanel.h>
#include <talipot/GlMainView.h>
#include <talipot/GlWidget.h>

#include "ViewsUtils.h"

using namespace std;
using namespace tlp;

ViewMainWindow::ViewMainWindow() : QMainWindow() {
  setAttribute(Qt::WA_DeleteOnClose, true);
  setWindowIcon(QIcon(":/software/bitmaps/logo32x32.ico"));
  resize(800, 600);
}

std::list<std::string> ViewsManager::getViews() {
  return PluginsManager::availablePlugins<View>();
}

tlp::Workspace *ViewsManager::tlpWorkspace() {
  QMainWindow *mainWindow = tlp::getMainWindow();

  if (mainWindow) {
    return mainWindow->findChild<tlp::Workspace *>();
  }

  return nullptr;
}

ViewsManager::ViewsManager() {
  model = new GraphHierarchiesModel(this);
}

std::vector<tlp::View *> ViewsManager::getOpenedViews() {
  tlp::Workspace *workspace = tlpWorkspace();

  if (workspace) {
    QList<tlp::View *> views = workspace->panels();
    return views.toVector().toStdVector();
  } else {
    return openedViews;
  }
}

std::vector<tlp::View *> ViewsManager::getOpenedViewsWithName(const std::string &viewName) {
  std::vector<tlp::View *> views = getOpenedViews();
  std::vector<tlp::View *> ret;

  for (size_t i = 0; i < views.size(); ++i) {
    if (views[i]->name() == viewName) {
      ret.push_back(views[i]);
    }
  }

  return ret;
}

tlp::View *ViewsManager::addView(const std::string &viewName, tlp::Graph *graph,
                                 const DataSet &dataSet, bool show) {
  tlp::Workspace *workspace = tlpWorkspace();
  tlp::View *view = PluginsManager::getPluginObject<View>(viewName);
  view->setupUi();
  view->setGraph(graph);
  view->setState(dataSet);

  if (workspace) {
    workspace->graphModel()->addGraph(graph);
    workspace->addPanel(view);
  } else {

    graph->addListener(this);

    model->addGraph(graph);

    WorkspacePanel *panel = new WorkspacePanel(view);
    panel->setGraphsModel(model);
    panel->viewGraphSet(graph);

    connect(view, &View::drawNeeded, view, &View::draw);

    openedViews.push_back(view);
    viewToPanel[view] = panel;

    setViewVisible(view, show);
    view->draw();
    QApplication::processEvents();
  }

  connect(view, &QObject::destroyed, this, &ViewsManager::viewDestroyed);

  return view;
}

void ViewsManager::closeView(tlp::View *view) {
  tlp::Workspace *workspace = tlpWorkspace();

  if (workspace) {
    workspace->delView(view);
  } else {
    if (viewToWindow.find(view) != viewToWindow.end()) {
      delete viewToWindow[view];
      viewToWindow.erase(view);
      viewToPanel.erase(view);
    }

    if (viewToPanel.find(view) != viewToPanel.end()) {
      delete viewToPanel[view];
      viewToPanel.erase(view);
    }
  }
}

std::vector<tlp::View *> ViewsManager::getViewsOfGraph(tlp::Graph *graph) {
  tlp::Workspace *workspace = tlpWorkspace();
  std::vector<tlp::View *> ret;

  if (workspace) {
    QList<tlp::View *> views = workspace->panels();

    for (int i = 0; i < views.count(); ++i) {
      if (views.at(i)->graph() == graph) {
        ret.push_back(views.at(i));
      }
    }
  } else {
    for (size_t i = 0; i < openedViews.size(); ++i) {
      if (openedViews[i]->graph() == graph) {
        ret.push_back(openedViews[i]);
      }
    }
  }

  return ret;
}

void ViewsManager::closeAllViews() {
  tlp::Workspace *workspace = tlpWorkspace();

  if (workspace) {
    QList<tlp::View *> views = workspace->panels();

    for (int i = 0; i < views.size(); ++i) {
      workspace->delView(views.at(i));
    }
  } else {
    std::vector<tlp::View *> openedViewsCp(openedViews);

    for (size_t i = 0; i < openedViewsCp.size(); ++i) {
      closeView(openedViewsCp[i]);
    }
  }
}

void ViewsManager::closeViewsRelatedToGraph(tlp::Graph *graph) {
  tlp::Workspace *workspace = tlpWorkspace();

  if (workspace) {
    QList<tlp::View *> views = workspace->panels();

    for (int i = 0; i < views.count(); ++i) {
      if (views.at(i)->graph() == graph) {
        workspace->delView(views.at(i));
      }
    }
  } else {
    std::vector<tlp::View *> openedViewsCp(openedViews);

    for (size_t i = 0; i < openedViewsCp.size(); ++i) {
      if (openedViewsCp[i]->graph() == graph) {
        closeView(openedViewsCp[i]);
      }
    }
  }
}

void ViewsManager::viewDestroyed(QObject *obj) {
  tlp::Workspace *workspace = tlpWorkspace();
  tlp::View *view = static_cast<tlp::View *>(obj);
  releaseSIPWrapper(view, sipFindType("tlp::View"));

  if (!workspace) {
    viewToWindow.erase(view);
    viewToPanel.erase(view);
    openedViews.erase(std::remove(openedViews.begin(), openedViews.end(), view), openedViews.end());
  }
}

void ViewsManager::setViewVisible(tlp::View *view, const bool visible) {
  tlp::Workspace *workspace = tlpWorkspace();

  if (!workspace) {
    if (visible) {
      viewToWindow[view] = new ViewMainWindow();
      viewToWindow[view]->setWindowTitle(
          ("Talipot : " + view->name() + " : " + view->graph()->getName()).c_str());
      viewToWindow[view]->setCentralWidget(viewToPanel[view]);
      viewToWindow[view]->setVisible(true);
    } else {
      viewToPanel[view]->setParent(nullptr);

      if (viewToWindow.find(view) != viewToWindow.end()) {
        delete viewToWindow[view];
        viewToWindow.erase(view);
      }
    }

    if (visible) {
      viewToWindow[view]->raise();
    }

    QApplication::processEvents();
  }
}

bool ViewsManager::areViewsVisible() {
  bool ret = false;

  tlp::Workspace *workspace = tlpWorkspace();

  if (!workspace) {
    for (size_t i = 0; i < openedViews.size(); ++i) {
      ret = ret || (viewToWindow.find(openedViews[i]) != viewToWindow.end() &&
                    viewToWindow[openedViews[i]]->isVisible());
    }
  }

  return ret;
}

void ViewsManager::resizeView(tlp::View *view, int width, int height) {
  tlp::Workspace *workspace = tlpWorkspace();

  if (!workspace) {
    viewToWindow[view]->resize(width, height);
    tlp::GlMainView *glView = dynamic_cast<tlp::GlMainView *>(view);
    if (glView) {
      glView->getGlWidget()->resize(width, height);
      glView->getGlWidget()->resizeGL(width, height);
    }
    QApplication::processEvents();
  }
}

void ViewsManager::setViewPos(tlp::View *view, int x, int y) {
  tlp::Workspace *workspace = tlpWorkspace();

  if (!workspace) {
    viewToWindow[view]->move(x, y);
    QApplication::processEvents();
  }
}

void ViewsManager::treatEvent(const tlp::Event &ev) {
  if (ev.type() == Event::TLP_DELETE) {
    Graph *g = static_cast<Graph *>(ev.sender());
    closeViewsRelatedToGraph(g);
  }
}
