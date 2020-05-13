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

%ModuleHeaderCode
#include <ViewsUtils.h>
#include <talipot/GlOffscreenRenderer.h>
extern ViewsManager tvm;
%End

%ModuleCode
ViewsManager tvm;
%End

namespace tlp {

std::list<std::string> getAvailableViews() const;
%Docstring
tlpgui.getAvailableViews()

Returns a list of the views names currently available in Talipot.

:rtype: list of string
%End

%MethodCode
    sipRes = new std::list<std::string>(tvm.getViews());
%End

// =========================================================================================

std::vector<tlp::View *> getOpenedViews() const;
%Docstring
tlpgui.getOpenedViews()

Returns a list of the views currently opened.

:rtype: list of :class:`tlpgui.View`
%End

%MethodCode
    sipRes = new std::vector<tlp::View *>(tvm.getOpenedViews());
%End

// =========================================================================================

std::vector<tlp::View *> getOpenedViewsWithName(const std::string &viewName) const;
%Docstring
tlpgui.getOpenedViewsWithName(viewName)

Returns a list of the views named viewName currently opened. To get the names of
the available views, use function :func:`tlpgui.getAvailableViews`.

:param viewName: the view name (e.g. "Node Link Diagram")
:type viewName: string
:rtype: list of :class:`tlpgui.View`
%End

%MethodCode
    sipRes = new std::vector<tlp::View *>(tvm.getOpenedViewsWithName(*a0));
%End

// =========================================================================================

            tlp::View *createView(const std::string &viewName, tlp::Graph *graph, tlp::DataSet dataSet = tlp::DataSet(), bool show = true);
%Docstring
tlpgui.createView(viewName, graph, dataSet={}, show=True)

Creates and adds a new Talipot view. If called through a Python Script editor in the main Talipot GUI, the
view will be added in the Talipot workspace. If called through the classical Python shell,
a dialog containing the view will be created and displayed.
Returns an instance of the newly created view.

:param viewName: the name of the view to create (use :func:`tlpgui.getAvailableViews` to know which ones are installed)
:type viewName: string
:param graph: the graph to set on the new view
:type graph: :class:`talipot.tlp.Graph`
:param dataSet: the optional parameters of the view
:type dataSet: :class:`talipot.tlp.DataSet`
:param show: show the view if :const:`True`
:type show: boolean
:rtype: :class:`tlpgui.View`
%End

%MethodCode
    sipRes = NULL;
    std::list<std::string> tlpViews = tvm.getViews();
    if (std::find(tlpViews.begin(), tlpViews.end(), *a0) != tlpViews.end()) {
        if (tvm.tlpWorkspace()) {
            PyObject *pyObj = sipGetPyObject(a1, sipFindType("tlp::Graph"));
            if (pyObj) {
                sipTransferTo(pyObj, pyObj);
            }
        }
        sipRes = tvm.addView(*a0, a1, *a2, a3);
    } else {
        sipIsErr = 1;
        std::ostringstream oss;
        oss << "Error : No Talipot view named \"" << *a0 << "\".";
        PyErr_SetString(PyExc_Exception, oss.str().c_str());
    }
%End

// =========================================================================================

tlp::NodeLinkDiagramView *createNodeLinkDiagramView(tlp::Graph *graph, tlp::DataSet dataSet = tlp::DataSet(), bool show = true);
%Docstring
tlpgui.createNodeLinkDiagramView(graph, dataSet={}, show=True)

Convenient function for creating the principal view offered by Talipot : a node link diagram.
Its call is equivalent to::

    nodeLinkView = tlp.createView("Node Link Diagram", graph)

:param graph: the graph to visualize
:type graph: :class:`talipot.tlp.Graph`
:param dataSet: the optional parameters of the view
:type dataSet: :class:`talipot.tlp.DataSet`
:param show: show the view if :const:`True`
:type show: boolean
:rtype: :class:`tlpgui.NodeLinkDiagramView`
%End

%MethodCode
    sipRes = static_cast<tlp::NodeLinkDiagramView *>(tvm.addView("Node Link Diagram", a0, *a1, a2));
%End

// =========================================================================================

    void closeView(tlp::View *view);
%Docstring
tlpgui.closeView(view)

Closes and destroys an opened view.

:param view: the view to close
:type view: :class:`tlpgui.View`
%End

%MethodCode
    tvm.closeView(a0);
%End

// =========================================================================================

    std::vector<tlp::View*> getViewsOfGraph(tlp::Graph *graph);
%Docstring
tlpgui.getViewsOfGraph(graph)

Returns a list of views associated to a particular graph.

:param graph: the graph on which to find views
:type graph: :class:`talipot.tlp.Graph`
:rtype: list of :class:`tlpgui.View`
%End

%MethodCode
    sipRes = new std::vector<tlp::View*>(tvm.getViewsOfGraph(a0));
%End

// =========================================================================================

    void closeAllViews();
%Docstring
tlpgui.closeAllViews()

Closes and destroys all opened views (except the Python Script ones).
%End

%MethodCode
    tvm.closeAllViews();
%End

// =========================================================================================

    void closeViewsRelatedToGraph(tlp::Graph* graph);
%Docstring
tlpgui.closeViewsRelatedToGraph(graph)

Closes and destroys all the views associated to a particular graph.

:param graph: the graph on which to close views
:type graph: :class:`talipot.tlp.Graph`
%End

%MethodCode
    tvm.closeViewsRelatedToGraph(a0);
%End

// =========================================================================================

    void runQtMainLoop();
%MethodCode
    if (tvm.areViewsVisible()) {
      QApplication::exec();
    }
    tlp::GlOffscreenRenderer::destroyInstance();
%End

};
