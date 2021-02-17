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

%Module(name=_talipotgui, keyword_arguments="Optional")

%PreInitialisationCode

bool initQt = true;

// test if there is an X server running on linux
// special case for PPA build, otherwise Python documentation is not generated
#if defined(__linux)
char *display = getenv("DISPLAY");
initQt = (display != NULL);
#endif

// If there is no QApplication instance, it means that we are working from the classical Python shell
// or we are executing a script with the interpreter.
// Instantiate a QApplication but don't run its event loop continually through the exec method.
// Instead use the same hack as in PyQt4 in order to process qt events when the Python shell
// is waiting for input.
if (!QApplication::instance() && initQt) {
  installQtInputHook();
#ifdef __APPLE__
  // Hack for MacOS to avoid importing the qt_menu.nib file as it is
  // not needed for our purpose and it forbids the talipotgui module to
  // be imported when it has been installed through pip
  QApplication::setAttribute(Qt::AA_MacPluginApplication);
#endif
  new QApplication(argc, argv);

  tlp::initTalipotSoftware();
}

%End

%PostInitialisationCode
// rename tlp namespace into tlpgui
PyObject *moduleDict = PyModule_GetDict(sipModule);
PyObject *tlpClass = PyDict_GetItemString(moduleDict, "tlp");
PyDict_DelItemString(moduleDict, "tlp");
PyDict_SetItemString(moduleDict, "tlpgui", tlpClass);
%End

%ModuleHeaderCode
#include <string>
#include <cstdlib>
#include <QApplication>

#include <talipot/TlpTools.h>
#include <talipot/TlpQtTools.h>
#include <talipot/PluginsManager.h>
#include <talipot/GlyphManager.h>
#include <talipot/EdgeExtremityGlyphManager.h>
#include <talipot/Interactor.h>
#include <talipot/PluginLibraryLoader.h>
#include <talipot/Workspace.h>

static int argc = 1;
static char *argv[1] = {const_cast<char*>("talipotgui")};
extern void printErrorMessage(const std::string &errMsg);
extern void releaseSIPWrapper(void *wrappedCppObject, const sipTypeDef *td);

extern void installQtInputHook();
extern void removeQtInputHook();

namespace tlp {
extern void initInteractorsDependencies();
}

%End

%ModuleCode

void printErrorMessage(const std::string &errMsg) {
    std::string pythonCode = "import sys\n"
            "sys.stderr.write(\"";
    pythonCode += errMsg;
    pythonCode += "\\n\")";
    PyRun_SimpleString(pythonCode.c_str());
}

void releaseSIPWrapper(void *wrappedCppObject, const sipTypeDef *td) {
    PyObject *pyObj = sipGetPyObject(wrappedCppObject, td);
    if (pyObj) {
        sipTransferTo(pyObj, pyObj);
        sipSimpleWrapper *wrapper = reinterpret_cast<sipSimpleWrapper *>(pyObj);
        sipInstanceDestroyed(wrapper);
    }
}

namespace tlp {

void initInteractorsDependencies() {
  tlp::InteractorLister::initInteractorsDependencies();
}

}

#include <QCoreApplication>
#include <QThread>

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN)
#include <conio.h>
#include <QTimer>
#else
#include <QSocketNotifier>
#endif

// This is the input hook that will process events while the interpreter is
// waiting for interactive input (from PyQt).
extern "C" {static int qtInputHook();}

static int qtInputHook() {
    QCoreApplication *app = QCoreApplication::instance();

    if (app && app->thread() == QThread::currentThread()) {
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN)
        QTimer timer;
        QMetaObject::Connection connection;
        connection = QObject::connect(&timer, &QTimer::timeout, app, &QCoreApplication::quit);

        while (!_kbhit()) {
            timer.start(35);
            QCoreApplication::exec();
            timer.stop();
        }

        QObject::disconnect(connection);
#else
        QSocketNotifier notifier(0, QSocketNotifier::Read, 0);
        QMetaObject::Connection connection;
        connection = QObject::connect(&notifier, &QSocketNotifier::activated, app, &QCoreApplication::quit);
        QCoreApplication::exec();
        QObject::disconnect(connection);
#endif
    }

    return 0;
}

void installQtInputHook() {
    PyOS_InputHook = qtInputHook;
}

void removeQtInputHook() {
    PyOS_InputHook = 0;
}

%End

namespace tlp {
  void initInteractorsDependencies();
};

%Include Interactor.sip
%Include View.sip
%Include GlView.sip
%Include GlGraphRenderingParameters.sip
%Include GlGraphInputData.sip
%Include NodeLinkDiagramView.sip
%Include TalipotGui.sip
%Include ColorScalesManager.sip


%Import ../talipot-core/Module.sip




