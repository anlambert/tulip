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

#ifndef TALIPOT_FACE_H
#define TALIPOT_FACE_H

#include <climits>
#include <functional>

namespace tlp {

struct Face {
  unsigned int id;
  Face() : id(UINT_MAX) {}
  explicit Face(unsigned int j) : id(j) {}
  bool operator!=(const Face f) const {
    return id != f.id;
  }
  bool operator==(const Face f) const {
    return id == f.id;
  }
  bool isValid() const {
    return id != UINT_MAX;
  }
};
}

namespace std {
template <>
struct hash<tlp::Face> {
  size_t operator()(const tlp::Face f) const {
    return f.id;
  }
};
template <>
struct equal_to<tlp::Face> {
  size_t operator()(const tlp::Face f, const tlp::Face f2) const {
    return f.id == f2.id;
  }
};
template <>
struct less<tlp::Face> {
  size_t operator()(const tlp::Face f, const tlp::Face f2) const {
    return f.id < f2.id;
  }
};
} // namespace std

#endif // TALIPOT_FACE_H
