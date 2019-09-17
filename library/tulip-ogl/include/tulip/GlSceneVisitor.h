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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_GLSCENEVISITOR_H
#define Tulip_GLSCENEVISITOR_H

#include <tulip/tulipconf.h>

namespace tlp {

class GlSimpleEntity;
class GlNode;
class GlEdge;
class GlLayer;

class TLP_GL_SCOPE GlSceneVisitor {

public:
  GlSceneVisitor() : threadSafe(false) {}
  virtual ~GlSceneVisitor() {}

  virtual void visit(GlSimpleEntity *) {}
  virtual void visit(GlNode *) {}
  virtual void visit(GlEdge *) {}
  virtual void visit(GlLayer *) {}
  virtual void endOfVisit() {}
  virtual void reserveMemoryForGraphElts(unsigned int /*nbNodes*/, unsigned int /*nbEdges*/) {}

  bool isThreadSafe() {
    return threadSafe;
  }

protected:
  bool threadSafe;
};
} // namespace tlp

#endif // Tulip_GLSCENEVISITOR_H
///@endcond
