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

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <talipot/TlpTools.h>

#include <CrashHandler.h>

using namespace std;
using namespace tlp;

int main() {

  CrashHandler::install();

  initTalipotLib();

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
