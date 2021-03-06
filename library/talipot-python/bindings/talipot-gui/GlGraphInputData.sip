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

class GlGraphInputData /Abstract/ {
%TypeHeaderCode
#include <talipot/GlGraphInputData.h>
%End

%Docstring
That class defines all the visual properties used to render a graph (layout, colors, sizes, ...).
By default, Talipot creates a pool of default properties whose name is prefixed by "view".
Through the use of that class, you can set other properties as rendering input data.

Use :meth:`talipotgui.tlpgui.NodeLinkDiagramView.getInputData` to get an instance on a such object.
%End

     public:

    GlGraphInputData(tlp::Graph* graph, tlp::GlGraphRenderingParameters* parameters);

    ~GlGraphInputData();

// =============================================================================================

    tlp::Graph* getGraph();
%Docstring
tlpgui.GlGraphInputData.getGraph()

Returns the graph associated to these data.

:rtype: :class:`talipot.tlp.Graph`
%End

// =============================================================================================

    tlp::ColorProperty * getElementColor() const;
%Docstring
tlpgui.GlGraphInputData.getElementColor()

Returns the color property that defines the current colors of graph elements (see :ref:`graph-colors`).

:rtype: :class:`talipot.tlp.ColorProperty`
%End

// =============================================================================================

    void setElementColor(tlp::ColorProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementColor(property)

Sets the color property that defines the current colors of graph elements (see :ref:`graph-colors`).

:param property: a color property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.ColorProperty`
%End

// =============================================================================================

    tlp::ColorProperty *getElementLabelColor() const;
%Docstring
tlpgui.GlGraphInputData.getElementLabelColor()

Returns the color property that defines the current labels colors of graph elements (see :ref:`graph-colors`).

:rtype: :class:`talipot.tlp.ColorProperty`
%End

// =============================================================================================

    void setElementLabelColor(tlp::ColorProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementLabelColor(property)

Sets the color property that defines the current labels colors of graph elements (see :ref:`graph-colors`).

:param property: a color property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.ColorProperty`
%End

// =============================================================================================


    tlp::ColorProperty *getElementLabelBorderColor() const;
%Docstring
tlpgui.GlGraphInputData.getElementLabelBorderColor()

Returns the color property that defines the current labels border colors of graph elements (see :ref:`graph-colors`).

:rtype: :class:`talipot.tlp.ColorProperty`
%End

// =============================================================================================

    void setElementLabelBorderColor(tlp::ColorProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementBorderColor(property)

Sets the color property that defines the current labels borders colors of graph elements (see :ref:`graph-colors`).

:param property: a color property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.ColorProperty`
%End

// =============================================================================================


    tlp::DoubleProperty *getElementLabelBorderWidth() const;
%Docstring
tlpgui.GlGraphInputData.getElementLabelBorderWidth()

Returns the double property that defines the current labels borders widths of graph elements (see :ref:`borders-widths`).

:rtype: :class:`talipot.tlp.DoubleProperty`
%End

// =============================================================================================

    void setElementLabelBorderWidth(tlp::DoubleProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementLabelBorderWidth(property)

Sets the double property that defines the current labels borders widths of graph elements (see :ref:`borders-widths`).

:param property: a double property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.DoubleProperty`
%End

// =============================================================================================

    tlp::SizeProperty *getElementSize() const;
%Docstring
tlpgui.GlGraphInputData.getElementSize()

Returns the size property that defines the current sizes of graph elements (see :ref:`graph-sizes`).

:rtype: :class:`talipot.tlp.SizeProperty`
%End

// =============================================================================================

    void setElementSize(tlp::SizeProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementSize(property)

Sets the size property that defines the current sizes of graph elements (see :ref:`graph-sizes`).

:param property: a size property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.SizeProperty`
%End

// =============================================================================================

    tlp::IntegerProperty *getElementLabelPosition() const;
%Docstring
tlpgui.GlGraphInputData.getElementLabelPosition()

Returns the integer property that defines the current labels positions of graph elements (see :ref:`graph-labels`).

:rtype: :class:`talipot.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementLabelPosition(tlp::IntegerProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementLabelPosition(property)

Sets the integer property that defines the current labels positions of graph elements (see :ref:`graph-labels`).

:param property: an integer property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::IntegerProperty *getElementShape() const;
%Docstring
tlpgui.GlGraphInputData.getElementShape()

Returns the integer property that defines the current shapes of graph elements (see :ref:`graph-shapes`).

:rtype: :class:`talipot.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementShape(tlp::IntegerProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementShape(property)

Sets the integer property that defines the current shapes of graph elements (see :ref:`graph-shapes`).

:param property: an integer property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::DoubleProperty *getElementRotation() const;
%Docstring
tlpgui.GlGraphInputData.getElementRotation()

Returns the double property that defines the current rotations of graph nodes (see :ref:`nodes-rotations`).

:rtype: :class:`talipot.tlp.DoubleProperty`
%End

// =============================================================================================

    void setElementRotation(tlp::DoubleProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementRotation(property)

Sets the double property that defines the current rotations graph nodes (see :ref:`nodes-rotations`).

:param property: a double property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.DoubleProperty`
%End

// =============================================================================================

    tlp::BooleanProperty *getElementSelected() const;
%Docstring
tlpgui.GlGraphInputData.getElementSelected()

Returns the boolean property that defines the current selection of graph elements (see :ref:`graph-selection`).

:rtype: :class:`talipot.tlp.BooleanProperty`
%End

// =============================================================================================

    void setElementSelected(tlp::BooleanProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementSelected(property)

Sets the boolean property that defines the current selection of graph elements (see :ref:`graph-selection`).

:param property: a boolean property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.BooleanProperty`
%End

// =============================================================================================

    tlp::StringProperty *getElementFont() const;
%Docstring
tlpgui.GlGraphInputData.getElementFont()

Returns the string property that defines the current font files used to render the labels of graph elements (see :ref:`graph-labels`).

:rtype: :class:`talipot.tlp.StringProperty`
%End

// =============================================================================================

    void setElementFont(tlp::StringProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementFont(property)

Sets the string property that defines the current font files used to render the labels of graph elements (see :ref:`graph-labels`).

:param property: a string property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.StringProperty`
%End

// =============================================================================================

    tlp::IntegerProperty  *getElementFontSize() const;
%Docstring
tlpgui.GlGraphInputData.getElementFontSize()

Returns the integer property that defines the current font size for labels of graph elements (see :ref:`graph-labels`).

:rtype: :class:`talipot.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementFontSize(tlp::IntegerProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementFontSize(property)

Sets the integer property that defines the current font size for labels of graph elements (see :ref:`graph-labels`).

:param property: an integer property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::StringProperty *getElementLabel() const;
%Docstring
tlpgui.GlGraphInputData.getElementLabel()

Returns the string property that defines the current labels of graph elements (see :ref:`graph-labels`).

:rtype: :class:`talipot.tlp.StringProperty`
%End

// =============================================================================================

    void setElementLabel(tlp::StringProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementLabel(property)

Sets the string property that defines the current labels of graph elements (see :ref:`graph-labels`).

:param property: a string property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.StringProperty`
%End

// =============================================================================================

    tlp::LayoutProperty *getElementLayout() const;
%Docstring
tlpgui.GlGraphInputData.getElementLayout()

Returns the layout property that defines the current layout of graph elements (see :ref:`graph-layout`).

:rtype: :class:`talipot.tlp.LayoutProperty`
%End

// =============================================================================================

    void setElementLayout(tlp::LayoutProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementLayout(property)

Sets the layout property that defines the current layout of graph elements (see :ref:`graph-layout`).

:param property: a layout property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.LayoutProperty`
%End

// =============================================================================================

    tlp::StringProperty *getElementTexture() const;
%Docstring
tlpgui.GlGraphInputData.getElementTexture()

Returns the string property that defines the current image files used to texture the shapes of graph elements (see :ref:`graph-textures`).

:rtype: :class:`talipot.tlp.StringProperty`
%End

// =============================================================================================

    void setElementTexture(tlp::StringProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementTexture(property)

Sets the string property that defines the current image files used to texture the shapes of graph elements (see :ref:`graph-textures`).

:param property: a string property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.StringProperty`
%End

// =============================================================================================

    tlp::ColorProperty *getElementBorderColor() const;
%Docstring
tlpgui.GlGraphInputData.getElementBorderColor()

Returns the color property that defines the current borders colors of graph elements (see :ref:`graph-colors`).

:rtype: :class:`talipot.tlp.LayoutProperty`
%End

// =============================================================================================

    void setElementBorderColor(tlp::ColorProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementBorderColor(property)

Sets the color property that defines the current borders colors of graph elements (see :ref:`graph-colors`).

:param property: a color property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.ColorProperty`
%End

// =============================================================================================

    tlp::DoubleProperty *getElementBorderWidth() const;
%Docstring
tlpgui.GlGraphInputData.getElementBorderWidth()

Returns the double property that defines the current borders widths of graph elements (see :ref:`borders-widths`).

:rtype: :class:`talipot.tlp.DoubleProperty`
%End

// =============================================================================================

    void setElementBorderWidth(tlp::DoubleProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementBorderWidth(property)

Sets the double property that defines the current borders widths of graph elements (see :ref:`borders-widths`).

:param property: a double property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.DoubleProperty`
%End

// =============================================================================================

    tlp::IntegerProperty *getElementSrcAnchorShape() const;
%Docstring
tlpgui.GlGraphInputData.getElementSrcAnchorShape()

Returns the integer property that defines the current shapes of the source edges extremities (see :ref:`graph-shapes`).

:rtype: :class:`talipot.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementSrcAnchorShape(tlp::IntegerProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementSrcAnchorShape(property)

Sets the integer property that defines the current shapes of the source edges extremities (see :ref:`graph-shapes`).

:param property: an integer property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::SizeProperty *getElementSrcAnchorSize() const;
%Docstring
tlpgui.GlGraphInputData.getElementSrcAnchorSize()

Returns the size property that defines the current sizes of the source edges extremities (see :ref:`graph-sizes`).

:rtype: :class:`talipot.tlp.SizeProperty`
%End

// =============================================================================================

    void setElementSrcAnchorSize(tlp::SizeProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementSrcAnchorSize(property)

Sets the size property that defines the current sizes of the source edges extremities (see :ref:`graph-shapes`).

:param property: a size property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.SizeProperty`
%End

// =============================================================================================

    tlp::IntegerProperty *getElementTgtAnchorShape() const;
%Docstring
tlpgui.GlGraphInputData.getElementTgtAnchorShape()

Returns the integer property that defines the current shapes of the target edges extremities (see :ref:`graph-shapes`).

:rtype: :class:`talipot.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementTgtAnchorShape(tlp::IntegerProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementTgtAnchorShape(property)

Sets the integer property that defines the current shapes of the target edges extremities (see :ref:`graph-shapes`).

:param property: an integer property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::SizeProperty *getElementTgtAnchorSize() const;
%Docstring
tlpgui.GlGraphInputData.getElementTgtAnchorSize()

Returns the size property that defines the current sizes of the target edges extremities (see :ref:`graph-sizes`).

:rtype: :class:`talipot.tlp.SizeProperty`
%End

// =============================================================================================

    void setElementTgtAnchorSize(tlp::SizeProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementTgtAnchorSize(property)

Sets the size property that defines the current sizes of the target edges extremities (see :ref:`graph-shapes`).

:param property: a size property attached to the graph associated to these data.
:type property: :class:`talipot.tlp.SizeProperty`
%End

// =============================================================================================

    void reloadGraphProperties();
%Docstring
tlpgui.GlGraphInputData.reloadGraphProperties()

Restores all default graph visual properties (whose name is prefixed by "view") as rendering input data.
%End

// =============================================================================================

};
};
