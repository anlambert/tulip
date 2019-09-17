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

#ifndef _GRAPHTEST_H
#define _GRAPHTEST_H

#include <tulip/Algorithm.h>

namespace tlp {
class GraphTest : public tlp::Algorithm {
public:
  GraphTest(const tlp::PluginContext *context) : Algorithm(context) {
    addOutParameter<bool>("result", "Whether the test succeeded or not.");
  }

  bool run() override {
    bool result = test();

    if (dataSet) {
      dataSet->set("result", result);
    }

    return true;
  }

  virtual bool test() = 0;
};
} // namespace tlp
#endif //_GRAPHTEST_H
///@endcond
