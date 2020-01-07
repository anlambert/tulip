.. py:currentmodule:: talipot

:mod:`talipot` module API
=========================

Utility functions
-------------------

Graph management
^^^^^^^^^^^^^^^^

.. autofunction:: talipot::tlp.newGraph

.. autofunction:: talipot::tlp.loadGraph

.. autofunction:: talipot::tlp.loadGraph

.. autofunction:: talipot::tlp.saveGraph

.. autofunction:: talipot::tlp.saveGraph

.. autofunction:: talipot::tlp.importGraph

.. autofunction:: talipot::tlp.exportGraph

.. autofunction:: talipot::tlp.copyToGraph

.. autofunction:: talipot::tlp.removeFromGraph

.. autofunction:: talipot::tlp.getRootGraphs

Graph measures and functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: talipot::tlp.averagePathLength

.. autofunction:: talipot::tlp.averageClusteringCoefficient

.. autofunction:: talipot::tlp.clusteringCoefficient

.. autofunction:: talipot::tlp.dagLevel

.. autofunction:: talipot::tlp.minDegree

.. autofunction:: talipot::tlp.maxDegree

.. autofunction:: talipot::tlp.maxDistance

.. autofunction:: talipot::tlp.maxDistanceWeighted

.. autofunction:: talipot::tlp.reachableNodes

.. autofunction:: talipot::tlp.bfs

.. autofunction:: talipot::tlp.bfs

.. autofunction:: talipot::tlp.dfs

.. autofunction:: talipot::tlp.dfs

.. autofunction:: talipot::tlp.selectShortestPaths

Geometry
^^^^^^^^

.. autofunction:: talipot::tlp.computeBoundingBox

.. autofunction:: talipot::tlp.computeBoundingBox

.. autofunction:: talipot::tlp.computeBoundingRadius

.. autofunction:: talipot::tlp.computeBoundingRadius

.. autofunction:: talipot::tlp.computeConvexHull

.. autofunction:: talipot::tlp.computeConvexHull

.. autofunction:: talipot::tlp.computeConvexHull

.. autofunction:: talipot::tlp.convexHull2D

.. autofunction:: talipot::tlp.convexHull

.. autofunction:: talipot::tlp.computeLinesIntersection

.. autofunction:: talipot::tlp.computePolygonCentroid

.. autofunction:: talipot::tlp.delaunayTriangulation

.. autoclass:: talipot::tlp.VoronoiDiagram
  :members:

.. autofunction:: talipot::tlp.voronoiDiagram

.. _parametric-curves:

Parametric curves computation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: talipot::tlp.computeBezierPoint

.. autofunction:: talipot::tlp.computeBezierPoints

.. autofunction:: talipot::tlp.computeCatmullRomPoint

.. autofunction:: talipot::tlp.computeCatmullRomPoints

.. autofunction:: talipot::tlp.computeOpenUniformBsplinePoint

.. autofunction:: talipot::tlp.computeOpenUniformBsplinePoints

Plugins Management
^^^^^^^^^^^^^^^^^^

.. autofunction:: talipot::tlp.getRelease

.. autofunction:: talipot::tlp.initTalipotLib

.. autofunction:: talipot::tlp.loadPlugins

.. autofunction:: talipot::tlp.loadPluginsFromDir

.. autofunction:: talipot::tlp.loadPlugin

.. autofunction:: talipot::tlp.loadPluginsCheckDependencies

.. autofunction:: talipot::tlp.getDefaultPluginParameters

.. autofunction:: talipot::tlp.getImportPluginsList

.. autofunction:: talipot::tlp.getExportPluginsList

.. autofunction:: talipot::tlp.getAlgorithmPluginsList

.. autofunction:: talipot::tlp.getBooleanAlgorithmPluginsList

.. autofunction:: talipot::tlp.getColorAlgorithmPluginsList

.. autofunction:: talipot::tlp.getDoubleAlgorithmPluginsList

.. autofunction:: talipot::tlp.getIntegerAlgorithmPluginsList

.. autofunction:: talipot::tlp.getLayoutAlgorithmPluginsList

.. autofunction:: talipot::tlp.getSizeAlgorithmPluginsList

.. autofunction:: talipot::tlp.getStringAlgorithmPluginsList

Graph elements classes
----------------------

tlp.node
^^^^^^^^

.. autoclass:: talipot::tlp.node
	:members:


tlp.edge
^^^^^^^^

.. autoclass:: talipot::tlp.edge
	:members:


.. _graph-class:

The tlp.Graph class
----------------------------

.. autoclass:: talipot::tlp.Graph

.. _graph-class.structure-modif:

Modification of the graph structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. automethod:: talipot::tlp.Graph.clear

.. automethod:: talipot::tlp.Graph.addNode

.. automethod:: talipot::tlp.Graph.addNode

.. automethod:: talipot::tlp.Graph.addNodes

.. automethod:: talipot::tlp.Graph.addNodes

.. automethod:: talipot::tlp.Graph.addNodes

.. automethod:: talipot::tlp.Graph.delNode

.. automethod:: talipot::tlp.Graph.delNodes

.. automethod:: talipot::tlp.Graph.delNodes

.. automethod:: talipot::tlp.Graph.addEdge

.. automethod:: talipot::tlp.Graph.addEdge

.. automethod:: talipot::tlp.Graph.addEdges

.. automethod:: talipot::tlp.Graph.addEdges

.. automethod:: talipot::tlp.Graph.addEdges

.. automethod:: talipot::tlp.Graph.delEdge

.. automethod:: talipot::tlp.Graph.delEdges

.. automethod:: talipot::tlp.Graph.delEdges

.. automethod:: talipot::tlp.Graph.setEdgeOrder

.. automethod:: talipot::tlp.Graph.swapEdgeOrder

.. automethod:: talipot::tlp.Graph.setSource

.. automethod:: talipot::tlp.Graph.setTarget

.. automethod:: talipot::tlp.Graph.setEnds

.. automethod:: talipot::tlp.Graph.reverse

.. _graph-class.subgraphs-hierarchy:

Creation and Modification of the subgraph hierarchy
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. automethod:: talipot::tlp.Graph.addSubGraph

.. automethod:: talipot::tlp.Graph.addSubGraph

.. automethod:: talipot::tlp.Graph.addCloneSubGraph

.. automethod:: talipot::tlp.Graph.inducedSubGraph

.. automethod:: talipot::tlp.Graph.inducedSubGraph

.. automethod:: talipot::tlp.Graph.delSubGraph

.. automethod:: talipot::tlp.Graph.delAllSubGraphs

.. automethod:: talipot::tlp.Graph.getSuperGraph

.. automethod:: talipot::tlp.Graph.getRoot

.. automethod:: talipot::tlp.Graph.setSuperGraph

.. automethod:: talipot::tlp.Graph.getSubGraphs

.. automethod:: talipot::tlp.Graph.isSubGraph

.. automethod:: talipot::tlp.Graph.isDescendantGraph

.. automethod:: talipot::tlp.Graph.getSubGraph

.. automethod:: talipot::tlp.Graph.getSubGraph

.. automethod:: talipot::tlp.Graph.getDescendantGraph

.. automethod:: talipot::tlp.Graph.getDescendantGraph

.. automethod:: talipot::tlp.Graph.getNthSubGraph

.. automethod:: talipot::tlp.Graph.numberOfSubGraphs

.. automethod:: talipot::tlp.Graph.numberOfDescendantGraphs

.. automethod:: talipot::tlp.Graph.getDescendantGraphs

.. _graph-class.iterators:

Iterators on the graph structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

All the provided iterators are stable, meaning you can safely delete a node/edge/subgraph
from the graph while iterating on its nodes/edges/subgraphs.

.. automethod:: talipot::tlp.Graph.getOneNode

.. automethod:: talipot::tlp.Graph.getRandomNode

.. automethod:: talipot::tlp.Graph.getNodes

.. automethod:: talipot::tlp.Graph.getInNode

.. automethod:: talipot::tlp.Graph.getInNodes

.. automethod:: talipot::tlp.Graph.getOutNode

.. automethod:: talipot::tlp.Graph.getOutNodes

.. automethod:: talipot::tlp.Graph.getInOutNodes

.. automethod:: talipot::tlp.Graph.bfs

.. automethod:: talipot::tlp.Graph.dfs

.. automethod:: talipot::tlp.Graph.getNodeMetaInfo

.. automethod:: talipot::tlp.Graph.getOneEdge

.. automethod:: talipot::tlp.Graph.getRandomEdge

.. automethod:: talipot::tlp.Graph.getEdges

.. automethod:: talipot::tlp.Graph.getOutEdges

.. automethod:: talipot::tlp.Graph.getInOutEdges

.. automethod:: talipot::tlp.Graph.getInEdges

.. automethod:: talipot::tlp.Graph.getEdgeMetaInfo

.. _graph-class.information:

Information about the graph structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. automethod:: talipot::tlp.Graph.nodes

.. automethod:: talipot::tlp.Graph.nodePos

.. automethod:: talipot::tlp.Graph.edges

.. automethod:: talipot::tlp.Graph.edgePos

.. automethod:: talipot::tlp.Graph.allEdges

.. automethod:: talipot::tlp.Graph.subGraphs

.. automethod:: talipot::tlp.Graph.getId

.. automethod:: talipot::tlp.Graph.getName

.. automethod:: talipot::tlp.Graph.setName

.. automethod:: talipot::tlp.Graph.getAttributes

.. automethod:: talipot::tlp.Graph.getAttribute

.. automethod:: talipot::tlp.Graph.setAttribute

.. automethod:: talipot::tlp.Graph.removeAttribute

.. automethod:: talipot::tlp.Graph.existAttribute

.. automethod:: talipot::tlp.Graph.isEmpty

.. automethod:: talipot::tlp.Graph.numberOfNodes

.. automethod:: talipot::tlp.Graph.numberOfEdges

.. automethod:: talipot::tlp.Graph.deg

.. automethod:: talipot::tlp.Graph.indeg

.. automethod:: talipot::tlp.Graph.outdeg

.. automethod:: talipot::tlp.Graph.source

.. automethod:: talipot::tlp.Graph.target

.. automethod:: talipot::tlp.Graph.ends

.. automethod:: talipot::tlp.Graph.opposite

.. automethod:: talipot::tlp.Graph.isElement

.. automethod:: talipot::tlp.Graph.isMetaNode

.. automethod:: talipot::tlp.Graph.isElement

.. automethod:: talipot::tlp.Graph.isElement

.. automethod:: talipot::tlp.Graph.isMetaEdge

.. automethod:: talipot::tlp.Graph.existEdge

.. automethod:: talipot::tlp.Graph.getSource

.. automethod:: talipot::tlp.Graph.getSink

.. _graph-class.properties:

Create/Access graph properties
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. automethod:: talipot::tlp.Graph.getBooleanProperty

.. automethod:: talipot::tlp.Graph.getLocalBooleanProperty

.. automethod:: talipot::tlp.Graph.getBooleanVectorProperty

.. automethod:: talipot::tlp.Graph.getLocalBooleanVectorProperty

.. automethod:: talipot::tlp.Graph.getColorProperty

.. automethod:: talipot::tlp.Graph.getLocalColorProperty

.. automethod:: talipot::tlp.Graph.getColorVectorProperty

.. automethod:: talipot::tlp.Graph.getLocalColorVectorProperty

.. automethod:: talipot::tlp.Graph.getDoubleProperty

.. automethod:: talipot::tlp.Graph.getLocalDoubleProperty

.. automethod:: talipot::tlp.Graph.getDoubleVectorProperty

.. automethod:: talipot::tlp.Graph.getLocalDoubleVectorProperty

.. automethod:: talipot::tlp.Graph.getIntegerProperty

.. automethod:: talipot::tlp.Graph.getLocalIntegerProperty

.. automethod:: talipot::tlp.Graph.getIntegerVectorProperty

.. automethod:: talipot::tlp.Graph.getLocalIntegerVectorProperty

.. automethod:: talipot::tlp.Graph.getGraphProperty

.. automethod:: talipot::tlp.Graph.getLocalGraphProperty

.. automethod:: talipot::tlp.Graph.getLayoutProperty

.. automethod:: talipot::tlp.Graph.getLocalLayoutProperty

.. automethod:: talipot::tlp.Graph.getCoordVectorProperty

.. automethod:: talipot::tlp.Graph.getLocalCoordVectorProperty

.. automethod:: talipot::tlp.Graph.getSizeProperty

.. automethod:: talipot::tlp.Graph.getLocalSizeProperty

.. automethod:: talipot::tlp.Graph.getSizeVectorProperty

.. automethod:: talipot::tlp.Graph.getLocalSizeVectorProperty

.. automethod:: talipot::tlp.Graph.getStringProperty

.. automethod:: talipot::tlp.Graph.getLocalStringProperty

.. automethod:: talipot::tlp.Graph.getStringVectorProperty

.. automethod:: talipot::tlp.Graph.getLocalStringVectorProperty

.. automethod:: talipot::tlp.Graph.getProperty

.. automethod:: talipot::tlp.Graph.existProperty

.. automethod:: talipot::tlp.Graph.existLocalProperty

.. automethod:: talipot::tlp.Graph.delLocalProperty

.. automethod:: talipot::tlp.Graph.getLocalProperties

.. automethod:: talipot::tlp.Graph.getInheritedProperties

.. automethod:: talipot::tlp.Graph.getProperties

.. automethod:: talipot::tlp.Graph.getLocalObjectProperties

.. automethod:: talipot::tlp.Graph.getInheritedObjectProperties

.. automethod:: talipot::tlp.Graph.getObjectProperties

.. automethod:: talipot::tlp.Graph.getNodePropertiesValues

.. automethod:: talipot::tlp.Graph.getEdgePropertiesValues

.. automethod:: talipot::tlp.Graph.setNodePropertiesValues

.. automethod:: talipot::tlp.Graph.setEdgePropertiesValues

Application of algorithms
^^^^^^^^^^^^^^^^^^^^^^^^^

.. automethod:: talipot::tlp.Graph.applyAlgorithm

.. automethod:: talipot::tlp.Graph.applyBooleanAlgorithm

.. automethod:: talipot::tlp.Graph.applyBooleanAlgorithm

.. automethod:: talipot::tlp.Graph.applyColorAlgorithm

.. automethod:: talipot::tlp.Graph.applyColorAlgorithm

.. automethod:: talipot::tlp.Graph.applyDoubleAlgorithm

.. automethod:: talipot::tlp.Graph.applyDoubleAlgorithm

.. automethod:: talipot::tlp.Graph.applyIntegerAlgorithm

.. automethod:: talipot::tlp.Graph.applyLayoutAlgorithm

.. automethod:: talipot::tlp.Graph.applyLayoutAlgorithm

.. automethod:: talipot::tlp.Graph.applySizeAlgorithm

.. automethod:: talipot::tlp.Graph.applySizeAlgorithm

.. automethod:: talipot::tlp.Graph.applyStringAlgorithm

.. automethod:: talipot::tlp.Graph.applyStringAlgorithm

Update management
^^^^^^^^^^^^^^^^^

.. automethod:: talipot::tlp.Graph.push

.. automethod:: talipot::tlp.Graph.pop

.. automethod:: talipot::tlp.Graph.unpop

.. automethod:: talipot::tlp.Graph.canPop

.. automethod:: talipot::tlp.Graph.canUnpop

.. automethod:: talipot::tlp.Graph.canPopThenUnpop

.. automethod:: talipot::tlp.Graph.popIfNoUpdates

Meta-nodes management
^^^^^^^^^^^^^^^^^^^^^

.. automethod:: talipot::tlp.Graph.createMetaNode

.. automethod:: talipot::tlp.Graph.createMetaNode

.. automethod:: talipot::tlp.Graph.createMetaNodes

.. automethod:: talipot::tlp.Graph.openMetaNode

Talipot datatypes
-----------------

tlp.Vec3f
^^^^^^^^^

.. autoclass:: talipot::tlp.Vec3f
	:members:


tlp.Vec4f
^^^^^^^^^

.. autoclass:: talipot::tlp.Vec4f

tlp.Vec4i
^^^^^^^^^

.. autoclass:: talipot::tlp.Vec4i

tlp.Coord
^^^^^^^^^

.. autoclass:: talipot::tlp.Coord
  :members:


tlp.BoundingBox
^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.BoundingBox

  .. automethod:: talipot::tlp.BoundingBox.center

  .. automethod:: talipot::tlp.BoundingBox.contains

  .. automethod:: talipot::tlp.BoundingBox.depth

  .. automethod:: talipot::tlp.BoundingBox.expand

  .. automethod:: talipot::tlp.BoundingBox.height

  .. automethod:: talipot::tlp.BoundingBox.intersect

  .. automethod:: talipot::tlp.BoundingBox.intersect

  .. automethod:: talipot::tlp.BoundingBox.isValid

  .. automethod:: talipot::tlp.BoundingBox.translate

  .. automethod:: talipot::tlp.BoundingBox.width

tlp.Size
^^^^^^^^

.. autoclass:: talipot::tlp.Size

  alias of :class:`tlp.Vec3f`


tlp.Color
^^^^^^^^^

.. autoclass:: talipot::tlp.Color
	:members:


tlp.ColorScale
^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.ColorScale
	:members:


tlp.StringCollection
^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.StringCollection
	:members: at, empty, getCurrent, getCurrentString, push_back, setCurrent, setCurrent, size

tlp.DataSet
^^^^^^^^^^^

.. autoclass:: talipot::tlp.DataSet
	:members:


tlp.PluginProgress
^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.PluginProgress
	:members:


Talipot observation mechanism
-----------------------------

tlp.Event
^^^^^^^^^

.. autoclass:: talipot::tlp.Event
  :members:

tlp.GraphEvent
^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.GraphEvent
  :members:

tlp.PropertyEvent
^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.PropertyEvent
  :members:

tlp.Observable
^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.Observable
  :members:

Graph properties classes
------------------------

tlp.PropertyInterface
^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.PropertyInterface
	:members:


tlp.BooleanProperty
^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.BooleanProperty
  :members:
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, reverse, reverseEdgeDirection,
    getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
    setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue, setNodeDefaultValue,
    setEdgeDefaultValue

tlp.ColorProperty
^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.ColorProperty
  :members:
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue,
    getEdgeValue, getNodeDefaultValue, getNodeValue, setAllEdgeValue, setAllNodeValue,
    setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue

tlp.NumericProperty
^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.NumericProperty
  :members:
    copyProperty, uniformQuantification, nodesUniformQuantification,
    edgesUniformQuantification, getEdgeDoubleMin, getEdgeDoubleMax,
    getNodeDoubleMin, getNodeDoubleMax, getEdgeDoubleDefaultValue,
    getEdgeDoubleValue, getNodeDoubleDefaultValue, getNodeDoubleValue,
    getSortedNodes, getSortedEdges, getSortedEdgesBySourceValue,
    getSortedEdgesByTargetValue, getSortedEdgesByExtremitiesValues

tlp.DoubleProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.DoubleProperty
  :members:
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getNodeMin,
    getNodeMax, getEdgeMin, getEdgeMax, getEdgeDefaultValue, getEdgeValue,
    getNodeDefaultValue, getNodeValue, setAllEdgeValue, setAllNodeValue,
    setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue

tlp.IntegerProperty
^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.IntegerProperty
  :members:
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getNodeMin,
    getNodeMax, getEdgeMin, getEdgeMax, getEdgeDefaultValue, getEdgeValue,
    getNodeDefaultValue, getNodeValue, setAllEdgeValue, setAllNodeValue,
    setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue

tlp.LayoutProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.LayoutProperty
  :members:
    angularResolutions, averageAngularResolution, averageAngularResolution,
    averageEdgeLength, center, center, computeEmbedding, computeEmbedding,
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, edgeLength, getMin,
    getMax, normalize, perfectAspectRatio, rotateX, rotateX, rotateY, rotateY,
    rotateZ, rotateZ, scale, scale, translate, translate, getEdgeDefaultValue,
    getEdgeValue, getNodeDefaultValue, getNodeValue, setAllEdgeValue, setAllNodeValue,
    setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue

tlp.SizeProperty
^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.SizeProperty
  :members:
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getMin, getMax, scale, scale,
    getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
    setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
    setNodeDefaultValue, setEdgeDefaultValue

tlp.StringProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.StringProperty
  :members:
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue,
    getEdgeValue, getNodeDefaultValue, getNodeValue, setAllEdgeValue, setAllNodeValue,
    setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue

tlp.BooleanVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.BooleanVectorProperty
  :members:
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue,
    getEdgeValue, getNodeDefaultValue, getNodeValue, setAllEdgeValue,
    setAllNodeValue, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
    getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
    resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
    pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.CoordVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.CoordVectorProperty
  :members:
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue,
    getEdgeValue, getNodeDefaultValue, getNodeValue, setAllEdgeValue,
    setAllNodeValue, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
    getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
    resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
    pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.ColorVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.ColorVectorProperty
  :members:
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue,
    getEdgeValue, getNodeDefaultValue, getNodeValue, setAllEdgeValue, setAllNodeValue,
    setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
    getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
    resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
    pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.DoubleVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.DoubleVectorProperty
  :members:
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue,
    getEdgeValue, getNodeDefaultValue, getNodeValue, setAllEdgeValue, setAllNodeValue,
    setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
    getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
    resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
    pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.IntegerVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.IntegerVectorProperty
  :members:
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue,
    getEdgeValue, getNodeDefaultValue, getNodeValue, setAllEdgeValue, setAllNodeValue,
    setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
    getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
    resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
    pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.SizeVectorProperty
^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.SizeVectorProperty
  :members:
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue,
    getEdgeValue, getNodeDefaultValue, getNodeValue, setAllEdgeValue, setAllNodeValue,
    setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
    getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
    resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
    pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.StringVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.StringVectorProperty
  :members:
    copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue,
    getEdgeValue, getNodeDefaultValue, getNodeValue, setAllEdgeValue, setAllNodeValue,
    setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
    getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
    resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
    pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

Talipot plugins classes
-----------------------

tlp.WithParameter
^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.WithParameter
	:members:

tlp.Plugin
^^^^^^^^^^

.. autoclass:: talipot::tlp.Plugin
  :members:

tlp.Algorithm
^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.Algorithm
	:members:


tlp.BooleanAlgorithm
^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.BooleanAlgorithm
	:members:


tlp.ColorAlgorithm
^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.ColorAlgorithm
	:members:

tlp.DoubleAlgorithm
^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.DoubleAlgorithm
  :members:

tlp.IntegerAlgorithm
^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.IntegerAlgorithm
	:members:


tlp.LayoutAlgorithm
^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.LayoutAlgorithm
	:members:


tlp.SizeAlgorithm
^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.SizeAlgorithm
	:members:


tlp.ImportModule
^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.ImportModule
	:members:


tlp.ExportModule
^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.ExportModule
	:members:


Graph test classes
-------------------

tlp.SelfLoops
^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.SelfLoops
	:members:


tlp.AcyclicTest
^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.AcyclicTest
	:members:


tlp.BiconnectedTest
^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.BiconnectedTest
	:members:


tlp.ConnectedTest
^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.ConnectedTest
	:members:


tlp.OuterPlanarTest
^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.OuterPlanarTest
	:members:


tlp.PlanarityTest
^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.PlanarityTest
	:members:


tlp.SimpleTest
^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.SimpleTest
	:members:


tlp.TreeTest
^^^^^^^^^^^^

.. autoclass:: talipot::tlp.TreeTest
	:members:


tlp.TriconnectedTest
^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.TriconnectedTest
	:members:
