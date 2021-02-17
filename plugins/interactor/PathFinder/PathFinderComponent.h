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

#ifndef PATH_FINDER_COMPONENT_H
#define PATH_FINDER_COMPONENT_H

#include <QSet>

#include <talipot/GLInteractor.h>
#include <talipot/Node.h>

namespace tlp {
class Graph;
class GlMainView;
class BooleanProperty;
class DoubleProperty;

class PathHighlighter;
class PathFinder;

/**
 * @brief The main component of the PathFinder interactor. Runs the path finding algorithm when two
 * nodes have been selected and
 * triggers the highlighters if a path has been found.
 */
class PathFinderComponent : public GLInteractorComponent {
public:
  PathFinderComponent(PathFinder *parent);
  ~PathFinderComponent() override;
  bool eventFilter(QObject *, QEvent *) override;

  /**
   * Install a path highlighter. This highlighter is not always activated. The component will ask
   * its interactor parent before triggering an highlighter.
   * @param highlighter The highlighter to install.
   */
  void addHighlighter(PathHighlighter *highlighter);

  /**
   * @return A list of all the installed highlighters.
   */
  QSet<PathHighlighter *> getHighlighters();

  void clear() override;

private:
  tlp::node src;
  tlp::node tgt;
  tlp::node tmp;
  PathFinder *parent;
  bool graphPopable;

  QSet<PathHighlighter *> highlighters;
  void runHighlighters(tlp::GlWidget *glWidget, tlp::BooleanProperty *selection, tlp::node src,
                       tlp::node tgt);
  void clearHighlighters(tlp::GlWidget *glWidget);

  void selectPath(GlWidget *glWidget, tlp::Graph *graph);
  PathHighlighter *findHighlighter(const std::string &name);
};
}

#endif // PATH_FINDER_COMPONENT_H
