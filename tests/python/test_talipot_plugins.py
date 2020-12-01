# Copyright (C) 2020  The Talipot developers
#
# Talipot is a fork of Tulip, created by David Auber
# and the Tulip development Team from LaBRI, University of Bordeaux
#
# See the AUTHORS file at the top-level directory of this distribution
# License: GNU General Public License version 3, or any later version
# See top-level LICENSE file for more information

import os
import unittest

from talipot import tlp


class TestTalipotPlugins(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        if 'TALIPOT_BUILD_DIR' in os.environ:
            talipot_build_dir = os.environ['TALIPOT_BUILD_DIR']
            if talipot_build_dir:
                for pluginsPath in (
                    'plugins/clustering', 'plugins/general',
                    'plugins/import', 'library/talipot-python'
                ):
                    tlp.loadPluginsFromDir(
                        os.path.join(talipot_build_dir, pluginsPath))

    def test_cpp_plugins_loaded(self):
        self.assertTrue('Edge bundling' in tlp.getAlgorithmPluginsList())

    def test_pythons_plugins_loaded(self):
        self.assertTrue('H3' in tlp.getLayoutAlgorithmPluginsList())

    def test_edge_bundling_3d_layout(self):
        # import a grid graph drawn in the plane
        graph = tlp.importGraph('Grid')
        # create a clone subgraph
        clone = graph.addCloneSubGraph()
        # add two nodes with z coordinate different from 0
        graph.addNode({'viewLayout': (-100, -100, -10)})
        graph.addNode({'viewLayout': (100, 100, 10)})
        # apply edge bundling on the clone graph
        params = tlp.getDefaultPluginParameters('Edge bundling', clone)
        ret = clone.applyAlgorithm('Edge bundling', params)
        # check that the resulting layout is in the plane
        self.assertTrue(ret[0])
        edge_bends = [graph['viewLayout'][e] for e in graph.getEdges()]
        for bends in edge_bends:
            for c in bends:
                self.assertEqual(c[2], 0)
        # apply edge bundling on the root graph
        ret = graph.applyAlgorithm('Edge bundling', params)
        # check that the layout failed as the 3D_layout parameter
        # has not been set to true
        self.assertFalse(ret[0])
