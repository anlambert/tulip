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

#include <iostream>
#include <cstdlib>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <talipot/TlpTools.h>
#ifndef NDEBUG
#include <talipot/PluginLoaderTxt.h>
#endif
#include <talipot/PluginLibraryLoader.h>

static void loadTalipotPluginsFromDir(const std::string &pluginsDir,
                                      tlp::PluginLoader *pluginLoader = nullptr) {
  std::string oldTlpPluginsPath = tlp::TalipotPluginsPath;
  tlp::TalipotPluginsPath = pluginsDir;
  tlp::PluginLibraryLoader::loadPlugins(pluginLoader);
  tlp::TalipotPluginsPath = oldTlpPluginsPath;
}

int main(int /* arg */, char ** /* argv */) {

  std::string talipotBuildDir = TALIPOT_BUILD_DIR;

  tlp::initTalipotLib();
  tlp::PluginLoader *pLoader = nullptr;
#ifndef NDEBUG
  tlp::PluginLoaderTxt loader;
  pLoader = &loader;
#endif

  loadTalipotPluginsFromDir(talipotBuildDir + "/plugins/clustering", pLoader);
  loadTalipotPluginsFromDir(talipotBuildDir + "/plugins/colors", pLoader);
  loadTalipotPluginsFromDir(talipotBuildDir + "/plugins/export", pLoader);
  loadTalipotPluginsFromDir(talipotBuildDir + "/plugins/import", pLoader);
  loadTalipotPluginsFromDir(talipotBuildDir + "/plugins/import/Graphviz", pLoader);
  loadTalipotPluginsFromDir(talipotBuildDir + "/plugins/layout", pLoader);
  loadTalipotPluginsFromDir(talipotBuildDir + "/plugins/layout/FastOverlapRemoval", pLoader);
  loadTalipotPluginsFromDir(talipotBuildDir + "/plugins/metric", pLoader);
  loadTalipotPluginsFromDir(talipotBuildDir + "/plugins/selection", pLoader);
  loadTalipotPluginsFromDir(talipotBuildDir + "/plugins/sizes", pLoader);

  // CPPUNIT_NS::QtUi::TestRunner runner;
  CPPUNIT_NS::TestResult controller;
  CPPUNIT_NS::TestResultCollector result;
  controller.addListener(&result);

  CPPUNIT_NS::TextUi::TestRunner runner;
  runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
  runner.run(controller);

  std::ofstream xmlFileOut("cpptestresults.xml");
  CPPUNIT_NS::XmlOutputter xmlOut(&result, xmlFileOut);
  xmlOut.write();

  CPPUNIT_NS::TextOutputter stdOut(&result, std::cout);
  stdOut.write();

  return result.wasSuccessful() ? EXIT_SUCCESS : EXIT_FAILURE;
}
