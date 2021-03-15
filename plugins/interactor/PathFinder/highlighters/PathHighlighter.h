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

#ifndef PATH_HIGHLIGHTER_H
#define PATH_HIGHLIGHTER_H

#include <string>
#include <unordered_map>

#include <talipot/Node.h>
#include <talipot/Observable.h>

class QWidget;

namespace tlp {
class BooleanProperty;
class GlGraphInputData;
class GlWidget;
class GlLayer;
class GlScene;
class GlEntity;

class PathFinder;

class PathHighlighter : public Observable {
public:
  PathHighlighter(const std::string &name);
  ~PathHighlighter() override;

  std::string getName() const {
    return this->name;
  }
  virtual void highlight(const PathFinder *parent, GlWidget *glWidget, BooleanProperty *selection,
                         node src, node tgt) = 0;
  virtual void draw(tlp::GlWidget *glWidget) = 0;
  void clear();
  virtual bool isConfigurable() const = 0;
  virtual QWidget *getConfigurationWidget() = 0;

protected:
  tlp::GlLayer *getWorkingLayer(tlp::GlScene *scene) const;
  tlp::GlGraphInputData *getInputData(tlp::GlWidget *glWidget);
  void addGlEntity(tlp::GlScene *scene, tlp::GlEntity *entity, bool deleteOnExit = true,
                   const std::string &name = "");
  void treatEvent(const Event &ev) override;

private:
  std::string name;
  tlp::GlScene *backupScene;
  std::unordered_map<std::string, bool> entities;
  int entityId;
};
}
#endif // PATH_HIGHLIGHTER_H
