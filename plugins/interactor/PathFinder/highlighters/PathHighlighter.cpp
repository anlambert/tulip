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

#include "PathHighlighter.h"

#include <talipot/GlMainWidget.h>
#include <talipot/GlMainView.h>
#include <talipot/GlGraph.h>

using namespace tlp;
using namespace std;

PathHighlighter::PathHighlighter(const string &name)
    : name(name), backupScene(nullptr), entityId(0) {}

PathHighlighter::~PathHighlighter() {
  if (backupScene) {
    backupScene->removeLayer(getWorkingLayer(backupScene), true);
  }
}

GlGraphInputData *PathHighlighter::getInputData(GlMainWidget *glMainWidget) {
  return glMainWidget->getGlGraphInputData();
}

GlLayer *PathHighlighter::getWorkingLayer(GlScene *scene) {
  GlLayer *result(scene->getLayer(getName()));

  if (result) {
    return result;
  }

  result = new GlLayer(getName(), false);
  result->setSharedCamera(&scene->getLayer("Main")->getCamera());
  scene->addExistingLayer(result);
  return result;
}

void PathHighlighter::clear() {
  if (backupScene) {
    GlLayer *layer = getWorkingLayer(backupScene);

    for (const auto &it : entities) {
      string entityName(it.first);
      GlEntity *entity(layer->findGlEntity(entityName));

      if (entity) {
        layer->deleteGlEntity(entity);

        if (it.second) {
          delete entity;
        }
      }
    }

    entities.clear();
  }
}

void PathHighlighter::addGlEntity(GlScene *scene, GlEntity *entity, bool deleteOnExit,
                                  const string &name) {
  string realName = name;

  if (realName.compare("") == 0) {
    stringstream ss;
    ss << getName() << entityId++;
    realName = ss.str();
  }

  if (backupScene) {
    backupScene->removeListener(this);
  }

  backupScene = scene;
  backupScene->addListener(this);

  GlLayer *layer = getWorkingLayer(backupScene);
  entities[realName] = deleteOnExit;
  layer->addGlEntity(entity, realName);
}

void PathHighlighter::treatEvent(const Event &ev) {
  if (ev.type() == Event::TLP_DELETE) {
    if (ev.sender() == backupScene) {
      // reset backupScene to avoid segfault
      // on destructor invocation
      backupScene = nullptr;
    }
  }
}
