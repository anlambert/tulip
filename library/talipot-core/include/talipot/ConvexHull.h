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

#ifndef TALIPOT_CONVEX_HULL_H
#define TALIPOT_CONVEX_HULL_H

#include <vector>
#include <talipot/Coord.h>

namespace tlp {

/**
 * \brief function for computing a 2D convex hull
 *
 * Computes a 2D convex hull using the Qhull library (www.qhull.org)
 *
 * Computes the 2D convex hull and returns a list of indexes for the
 * points on the convex hull in counterclockwise order.
 *
 * The convexHull vector is automatically cleared.
 *
 */
TLP_SCOPE void convexHull(const std::vector<Coord> &points, std::vector<unsigned int> &convexHull);

/**
 * \brief function for computing a 2D/3D convex hull
 *
 * Computes a 2D/3D convex hull using the Qhull library (www.qhull.org)
 *
 * Computes a 2D/3D convex hull and returns a list of the hull facets (segments for 2D, triangles
 * for 3D)
 * and a list of neighbors facets for each facet. A facet is defined by a list of indexes in the
 * points vector.
 * The neighbors facets of a facet are defined by a list of indexes in the convexHullFacets vector
 * (facetNeighbors[i] stores the neighbors of facet convexHullFacets[i]).
 *
 * The convexHullFacets and facetNeighbors vectors are automatically cleared.
 */
TLP_SCOPE void convexHull(const std::vector<Coord> &points,
                          std::vector<std::vector<unsigned int>> &convexHullFacets,
                          std::vector<std::vector<unsigned int>> &facetNeighbors);

}
#endif // TALIPOT_CONVEX_HULL_H
