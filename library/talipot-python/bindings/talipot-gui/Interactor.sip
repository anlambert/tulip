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

namespace tlp {

  tlp::Interactor *createInteractor(const std::string &interactorName);
%Docstring
tlpgui.createInteractor(interactorName)

Creates a Interactor. An interactor enables to add user
interaction (usually through the use of mouse and keyboards) to a Talipot view.
Use function :func:`tlpgui.getInteractorsCompatibleWithView` to get the names
of the interactors compatible with a particular view.
Use method :meth:`tlpgui.View.setInteractors` to modify the default interactors
associated to a view.

:param interactorName: the name of the interactor to create
:type interactorName: string
:rtype: :class:`tlpgui.Interactor`
:throws: an exception if the interactor name does not exist
%End

%MethodCode
    if (tlp::PluginsManager::pluginExists<tlp::Interactor>(*a0)) {
        sipRes = tlp::PluginsManager::getPluginObject<tlp::Interactor>(*a0, NULL);
    } else {
        sipIsErr = 1;
        std::ostringstream oss;
        oss << "Error : No Interactor named \"" << *a0 << "\".";
        PyErr_SetString(PyExc_Exception, oss.str().c_str());
    }
%End

  std::list<std::string> getInteractorsCompatibleWithView(const std::string &viewName);
%Docstring
tlpgui.getInteractorsCompatibleWithView(viewName)

Returns a list of interactors names compatible with a specific view.

:param viewName: the name of the view for which to retrieve the compatible interactors
:type viewName: string
:rtype: list of string
:throws: an exception if the view name does not exist
%End

%MethodCode
    if (tlp::PluginsManager::pluginExists<tlp::View>(*a0)) {
        QList<std::string> ret = tlp::InteractorLister::compatibleInteractors(*a0);
        sipRes = new std::list<std::string>(ret.toStdList());
    } else {
        sipIsErr = 1;
        std::ostringstream oss;
        oss << "Error : No Talipot view named \"" << *a0 << "\".";
        PyErr_SetString(PyExc_Exception, oss.str().c_str());
    }
%End

class Interactor : tlp::Plugin /Abstract/ {
%TypeHeaderCode
#include <talipot/Interactor.h>
%End

%Docstring
Bases: :class:`talipot.tlp.Plugin`

An interactor enables to add user interaction (usually through the use of mouse and keyboards)
to a Talipot view.
Use function :func:`tlpgui.getInteractorsCompatibleWithView` to get the names
of the interactors compatible with a particular view.
Use function :func:`tlpgui.createInteractor` to instantiate a specific interactor.
Use method :meth:`tlpgui.View.setInteractors` to modify the default interactors
associated to a view.
%End

public:

  virtual bool isCompatible(const std::string& viewName) const=0;
%Docstring
tlpgui.Interactor.isCompatible(viewName)

Returns if the interactor is compatible with a specific view.
%End

%MethodCode
    if (tlp::PluginsManager::pluginExists<tlp::View>(*a0)) {
        QList<std::string> ret = tlp::InteractorLister::compatibleInteractors(*a0);
        sipRes = sipCpp->isCompatible(*a0);
    } else {
        sipIsErr = 1;
        std::ostringstream oss;
        oss << "Error : No Talipot view named \"" << *a0 << "\".";
        PyErr_SetString(PyExc_Exception, oss.str().c_str());
    }
%End

  virtual tlp::View* view() const=0;


};

};
