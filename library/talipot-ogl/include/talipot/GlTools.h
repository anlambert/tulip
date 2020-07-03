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

#ifndef TALIPOT_GL_TOOLS_H
#define TALIPOT_GL_TOOLS_H

#include <talipot/config.h>
#include <talipot/Color.h>
#include <talipot/Coord.h>
#include <talipot/Size.h>
#include <talipot/Matrix.h>

#include <talipot/OpenGlIncludes.h>

namespace tlp {

struct BoundingBox;

typedef Matrix<float, 4> MatrixGL;

TLP_GL_SCOPE void setColor(const Color &c);
TLP_GL_SCOPE void setColor(GLfloat *);
TLP_GL_SCOPE void setMaterial(const Color &c);
TLP_GL_SCOPE bool cameraIs3D();
/*
 * Project point on screen according to the transformation matrix (modelview * projection)
 * and the viewport (x, y, width, height) given in parameter.
 */
TLP_GL_SCOPE Coord projectPoint(const Coord &obj, const MatrixGL &, const Vec4i &viewport);
/*
 * UnProject point from screen to 3D world according to the inverse transformation matrix (modelview
 * * projection)^-1
 * and the viewport (x, y, width, height) given in parameter.
 */
TLP_GL_SCOPE Coord unprojectPoint(const Coord &obj, const MatrixGL &, const tlp::Vec4i &viewport);
TLP_GL_SCOPE GLfloat projectSize(const BoundingBox &bb, const MatrixGL &, const MatrixGL &,
                                 const Vec4i &viewport);
TLP_GL_SCOPE GLfloat projectSize(const Coord &position, const Size &size, const MatrixGL &,
                                 const MatrixGL &, const Vec4i &viewport);
TLP_GL_SCOPE float calculateAABBSize(const BoundingBox &bb, const Coord &eye,
                                     const MatrixGL &transformMatrix, const Vec4i &globalViewport,
                                     const Vec4i &currentViewport);
TLP_GL_SCOPE float calculate2DLod(const BoundingBox &bb, const Vec4i &globalViewport,
                                  const Vec4i &currentViewport);

/**
 * Computes the normals associated to the vertices of a triangulated mesh.
 *
 * @param vertices a vector containing the vertices to compute associated normals
 * @param facesIndices a vector containing the unsigned short indices of the triangles faces (its
 * size must be a multiple of 3)
 * @return a vector, with the same size as the one holding vertices, filled with the computed
 * normals
 *
 */
TLP_GL_SCOPE std::vector<Coord> computeNormals(const std::vector<Coord> &vertices,
                                               const std::vector<unsigned short> &facesIndices);

/**
 * Computes the normals associated to the vertices of a triangulated mesh.
 *
 * @param vertices a vector containing the vertices to compute associated normals
 * @param facesIndices a vector containing the unsigned int indices of the triangles faces (its size
 * must be a multiple of 3)
 * @return a vector, with the same size as the one holding vertices, filled with the computed
 * normals
 *
 */
TLP_GL_SCOPE std::vector<Coord> computeNormals(const std::vector<Coord> &vertices,
                                               const std::vector<unsigned int> &facesIndices);
}

#endif // TALIPOT_GL_TOOLS_H
