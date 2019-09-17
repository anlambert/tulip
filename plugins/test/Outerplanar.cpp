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

#include <talipot/OuterPlanarTest.h>
#include <talipot/GraphTest.h>

class OuterplanarTest : public tlp::GraphTest {
public:
  PLUGININFORMATION("Outer Planar", "Tulip team", "18/04/2012",
                    "Tests whether a graph is outer planar or not.", "1.0", "Topological Test")
  OuterplanarTest(const tlp::PluginContext *context) : tlp::GraphTest(context) {}

  bool test() override {
    return tlp::OuterPlanarTest::isOuterPlanar(graph);
  }
};
PLUGIN(OuterplanarTest)
