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
#include <talipot/GlMainWidget.h>
%End

namespace tlp {
class NodeLinkDiagramView : tlp::GlMainView  /NoDefaultCtors/ {
%TypeHeaderCode
#include <talipot/NodeLinkDiagramView.h>
#include <talipot/GlGraph.h>
%End

%Docstring

Bases: :class:`tlpgui.GlMainView`

That class represents the main type of view offered by Talipot : a node link diagram.
%End

public:

  NodeLinkDiagramView(const tlp::PluginContext* context = NULL);
  virtual ~NodeLinkDiagramView();

// =======================================================================================

    tlp::GlGraphInputData *getInputData();
%Docstring
tlpgui.NodeLinkDiagramView.getInputData()

Returns the current rendering input data for the node link diagram view.

:rtype: :class:`tlpgui.GlGraphInputData`
%End

%MethodCode
    sipRes = sipCpp->getGlMainWidget()->getScene()->getGlGraph()->getInputData();
%End

// =======================================================================================

    tlp::GlGraphRenderingParameters getRenderingParameters();
%Docstring
tlpgui.NodeLinkDiagramView.getRenderingParameters()

Returns a copy of the current rendering parameters for the node link diagram view.

:rtype: :class:`tlpgui.GlGraphRenderingParameters`
%End

%MethodCode
    sipRes = new tlp::GlGraphRenderingParameters(sipCpp->getGlMainWidget()->getScene()->getGlGraph()->getRenderingParameters());
%End

// =======================================================================================

    void setRenderingParameters(const tlp::GlGraphRenderingParameters &parameters);
%Docstring
tlpgui.NodeLinkDiagramView.setRenderingParameters(parameters)

Sets the rendering parameters for the node link diagram view.

:param parameters: the new rendering parameters
:type parameters: :class:`tlpgui.GlGraphRenderingParameters`
%End

%MethodCode
    sipCpp->getGlMainWidget()->getScene()->getGlGraph()->setRenderingParameters(*a0);
    sipCpp->draw();
    QApplication::processEvents();
%End

// =======================================================================================

protected :

    void setQuickAccessBarVisible(bool);
%Docstring
tlpgui.NodeLinkDiagramView.setQuickAccessBarVisible(state)

Sets the visibility state of the quick access bar.

:param state: the visibility state of the quick access bar
:type state: boolean
%End

// =======================================================================================

    bool quickAccessBarVisible() const;
%Docstring
tlpgui.NodeLinkDiagramView.quickAccessBarVisible()

Returns the visibility state of the quick access bar.

:rtype: boolean
%End


};

};
