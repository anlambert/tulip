# Copyright (C) 2019-2020  The Talipot developers
#
# Talipot is a fork of Tulip, created by David Auber
# and the Tulip development Team from LaBRI, University of Bordeaux
#
# See the AUTHORS file at the top-level directory of this distribution
# License: GNU General Public License version 3, or any later version
# See top-level LICENSE file for more information

# -*- coding: utf-8 -*-

import os
import os.path
import sys
import platform

from talipot import tlp

# fix an ubuntu bug when using the nvidia driver,
# when the python interpreter dynamically loads the GL library,
# it ends up loading the mesa one instead of the nvidia one.
# The use of the talipotgui module through the classical python intrepreter
# was quite unusable because of that ...
# Apply the workaround proposed here :
# https://bugs.launchpad.net/ubuntu/+source/python-qt4/+bug/941826
if hasattr(os, 'uname'):
    if os.uname()[3].lower().find('ubuntu') != -1:
        import ctypes.util
        ctypes.CDLL(ctypes.util.find_library('GL'), ctypes.RTLD_GLOBAL)

_talipotGuiNativeLibsPath = '%s/native/' % os.path.dirname(__file__)
_talipotGuiNativePluginsPath = '%splugins' % _talipotGuiNativeLibsPath
_talipotGuiPluginsPath = '%s/plugins/' % os.path.dirname(__file__)

sys.path.append(_talipotGuiNativeLibsPath)

if platform.system() == 'Windows':
    os.environ['PATH'] = '%s;%s../../../;%s' % (
        _talipotGuiNativeLibsPath, _talipotGuiNativeLibsPath,
        os.environ['PATH'])


import _talipotgui # noqa

sys.path.pop()


class tlpgui(_talipotgui.tlpgui):
    pass


def talipotguiExitFunc():
    import talipotgui
    talipotgui.tlpgui.runQtMainLoop()


# fix loading of Talipot plugins when the talipotgui module has been installed
# with the pip tool
if (platform.system() == 'Linux' and
        os.path.exists(_talipotGuiNativePluginsPath)):
    dlOpenFlagsBackup = sys.getdlopenflags()
    if sys.version_info < (3, 6):
        import DLFCN
        dlOpenFlags = DLFCN.RTLD_NOW | DLFCN.RTLD_GLOBAL
    else:
        dlOpenFlags = os.RTLD_NOW | os.RTLD_GLOBAL
    sys.setdlopenflags(dlOpenFlags)

tlp.loadTalipotPluginsFromDir(_talipotGuiPluginsPath)

if (platform.system() == 'Linux'
        and os.path.exists(_talipotGuiNativePluginsPath)):
    sys.setdlopenflags(dlOpenFlagsBackup)

# Check if we are in script execution mode
# (sys.ps1 is not defined in that case)
# If so, register an exit callback that will run the Qt event loop
# if some widgets are still in a visible state
if not hasattr(sys, 'ps1') and not sys.flags.interactive:
    import atexit
    atexit.register(talipotguiExitFunc)

import signal # noqa
signal.signal(signal.SIGINT, signal.SIG_DFL)

__all__ = ['tlpgui']
__author__ = 'The Talipot developers'
__license__ = 'GPLv3'
__version__ = tlp.getRelease()
__status__ = 'Production'
