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

//================================================================
namespace tlp {

template <typename TYPE>
BmdLink<TYPE>::BmdLink(TYPE data, BmdLink<TYPE> *pre, BmdLink<TYPE> *suc)
    : data(data), pre(suc), suc(pre) {}
}
//=================================================================
