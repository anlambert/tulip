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

#ifndef TALIPOT_EDGE_H
#define TALIPOT_EDGE_H

#include <talipot/IndexElement.h>

namespace tlp {

/**
 * @ingroup Graph
 * @brief The edge struct represents an edge in a Graph object.
 *
 * This structure only contains an identifier, and a function to check if the edge is valid.
 * A edge is considered invalid when its id has the UINT_MAX value.
 *
 * Most operations performed on an edge (getting the source, target, etc) are available into the
 * tlp::Graph object.
 *
 * @see tlp::node
 * @see tlp::Graph
 */
TLP_INDEX_ELEMENT(edge)
}
TLP_INDEX_ELEMENT_HASH(edge)

#endif // TALIPOT_EDGE_H
