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

    def setUp(self):
        if 'TALIPOT_BUILD_DIR' in os.environ:
            talipot_build_dir = os.environ['TALIPOT_BUILD_DIR']
            if talipot_build_dir:
                tlp.loadPluginsFromDir(os.path.join(talipot_build_dir,
                                                    'plugins'))
                tlp.loadPluginsFromDir(os.path.join(talipot_build_dir,
                                                    'library/talipot-python'))

    def test_cpp_plugins_loaded(self):
        self.assertTrue('Edge bundling' in tlp.getAlgorithmPluginsList())

    def test_pythons_plugins_loaded(self):
        self.assertTrue('H3' in tlp.getLayoutAlgorithmPluginsList())
