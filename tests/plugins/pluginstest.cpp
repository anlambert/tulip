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

#include <CrashHandler.h>

using namespace std;
using namespace tlp;

int main() {

  CrashHandler::install();

  string talipotBuildDir = TALIPOT_BUILD_DIR;

  initTalipotLib();
  PluginLoader *pLoader = nullptr;
#ifndef NDEBUG
  PluginLoaderTxt loader;
  pLoader = &loader;
#endif

  PluginLibraryLoader::loadPluginsFromDir(talipotBuildDir + "/plugins/clustering", pLoader);
  PluginLibraryLoader::loadPluginsFromDir(talipotBuildDir + "/plugins/colors", pLoader);
  PluginLibraryLoader::loadPluginsFromDir(talipotBuildDir + "/plugins/export", pLoader);
  PluginLibraryLoader::loadPluginsFromDir(talipotBuildDir + "/plugins/import", pLoader);
  PluginLibraryLoader::loadPluginsFromDir(talipotBuildDir + "/plugins/import/Graphviz", pLoader);
  PluginLibraryLoader::loadPluginsFromDir(talipotBuildDir + "/plugins/layout", pLoader);
  PluginLibraryLoader::loadPluginsFromDir(talipotBuildDir + "/plugins/layout/FastOverlapRemoval",
                                          pLoader);
  PluginLibraryLoader::loadPluginsFromDir(talipotBuildDir + "/plugins/metric", pLoader);
  PluginLibraryLoader::loadPluginsFromDir(talipotBuildDir + "/plugins/selection", pLoader);
  PluginLibraryLoader::loadPluginsFromDir(talipotBuildDir + "/plugins/sizes", pLoader);

  CPPUNIT_NS::TestResult controller;
  CPPUNIT_NS::TestResultCollector result;
  controller.addListener(&result);

  CPPUNIT_NS::TextUi::TestRunner runner;
  runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
  runner.run(controller);

  CPPUNIT_NS::TextOutputter stdOut(&result, cout);
  stdOut.write();

  return result.wasSuccessful() ? EXIT_SUCCESS : EXIT_FAILURE;
}
