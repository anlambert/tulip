/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_GLVERTEXARRAYVISITOR_H
#define Tulip_GLVERTEXARRAYVISITOR_H
#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/GlSceneVisitor.h>

namespace tlp {

class GlGraphInputData;

/** \brief Visitor to collect edges/nodes vertex array data
 *
 * Visitor to collect edges/nodes vertex array data
 */
class TLP_GL_SCOPE GlVertexArrayVisitor : public GlSceneVisitor {

public:
  /**
   * Constructor
   */
  GlVertexArrayVisitor(const GlGraphInputData *inputData) : inputData(inputData) {
    threadSafe = true;
  }

  /**
   * Method used for GlSimpleEntity
   */
  void visit(GlSimpleEntity *) override {}
  /**
   * Method used for GlNodes (and GlMetaNodes)
   */
  void visit(GlNode *glNode) override;
  /**
   * Method used for GlEdges
   */
  void visit(GlEdge *glEdge) override;

private:
  const GlGraphInputData *inputData;
};
}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLVERTEXARRAYVISITOR_H
///@endcond
