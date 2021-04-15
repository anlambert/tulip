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
  :noindex:

.. autofunction:: talipot::tlp.saveGraph

.. autofunction:: talipot::tlp.saveGraph
  :noindex:

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
  :noindex:

.. autofunction:: talipot::tlp.dfs

.. autofunction:: talipot::tlp.dfs
  :noindex:

.. autofunction:: talipot::tlp.selectShortestPaths

Geometry
^^^^^^^^

.. autofunction:: talipot::tlp.computeBoundingBox

.. autofunction:: talipot::tlp.computeBoundingBox
  :noindex:

.. autofunction:: talipot::tlp.computeBoundingRadius

.. autofunction:: talipot::tlp.computeBoundingRadius
  :noindex:

.. autofunction:: talipot::tlp.computeConvexHull

.. autofunction:: talipot::tlp.computeConvexHull
  :noindex:

.. autofunction:: talipot::tlp.computeConvexHull
  :noindex:

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
  :noindex:

.. automethod:: talipot::tlp.Graph.addNodes

.. automethod:: talipot::tlp.Graph.addNodes
  :noindex:

.. automethod:: talipot::tlp.Graph.addNodes
  :noindex:

.. automethod:: talipot::tlp.Graph.delNode

.. automethod:: talipot::tlp.Graph.delNodes

.. automethod:: talipot::tlp.Graph.delNodes
  :noindex:

.. automethod:: talipot::tlp.Graph.addEdge

.. automethod:: talipot::tlp.Graph.addEdge
  :noindex:

.. automethod:: talipot::tlp.Graph.addEdges

.. automethod:: talipot::tlp.Graph.addEdges
  :noindex:

.. automethod:: talipot::tlp.Graph.addEdges
  :noindex:

.. automethod:: talipot::tlp.Graph.delEdge

.. automethod:: talipot::tlp.Graph.delEdges
  :noindex:

.. automethod:: talipot::tlp.Graph.delEdges
  :noindex:

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
  :noindex:

.. automethod:: talipot::tlp.Graph.addCloneSubGraph

.. automethod:: talipot::tlp.Graph.inducedSubGraph

.. automethod:: talipot::tlp.Graph.inducedSubGraph
  :noindex:

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
  :noindex:

.. automethod:: talipot::tlp.Graph.getDescendantGraph

.. automethod:: talipot::tlp.Graph.getDescendantGraph
  :noindex:

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

.. automethod:: talipot::tlp.Graph.incidence

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
  :noindex:

.. automethod:: talipot::tlp.Graph.isElement
  :noindex:

.. automethod:: talipot::tlp.Graph.isMetaEdge

.. automethod:: talipot::tlp.Graph.hasEdge

.. automethod:: talipot::tlp.Graph.existEdge

.. automethod:: talipot::tlp.Graph.getEdges
  :noindex:

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
  :noindex:

.. automethod:: talipot::tlp.Graph.applyColorAlgorithm

.. automethod:: talipot::tlp.Graph.applyColorAlgorithm
  :noindex:

.. automethod:: talipot::tlp.Graph.applyDoubleAlgorithm

.. automethod:: talipot::tlp.Graph.applyDoubleAlgorithm
  :noindex:

.. automethod:: talipot::tlp.Graph.applyIntegerAlgorithm

.. automethod:: talipot::tlp.Graph.applyLayoutAlgorithm

.. automethod:: talipot::tlp.Graph.applyLayoutAlgorithm
  :noindex:

.. automethod:: talipot::tlp.Graph.applySizeAlgorithm

.. automethod:: talipot::tlp.Graph.applySizeAlgorithm
  :noindex:

.. automethod:: talipot::tlp.Graph.applyStringAlgorithm

.. automethod:: talipot::tlp.Graph.applyStringAlgorithm
  :noindex:

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
  :noindex:

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
    :noindex:

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

  .. automethod:: talipot::tlp.StringCollection.at

  .. automethod:: talipot::tlp.StringCollection.empty

  .. automethod:: talipot::tlp.StringCollection.getCurrent

  .. automethod:: talipot::tlp.StringCollection.getCurrentString

  .. automethod:: talipot::tlp.StringCollection.push_back

  .. automethod:: talipot::tlp.StringCollection.setCurrent

  .. automethod:: talipot::tlp.StringCollection.setCurrent
    :noindex:

  .. automethod:: talipot::tlp.StringCollection.size

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

  .. automethod:: talipot::tlp.BooleanProperty.copy

  .. automethod:: talipot::tlp.BooleanProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.BooleanProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.BooleanProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.BooleanProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.BooleanProperty.reverse

  .. automethod:: talipot::tlp.BooleanProperty.reverseEdgeDirection

  .. automethod:: talipot::tlp.BooleanProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.BooleanProperty.getEdgeValue

  .. automethod:: talipot::tlp.BooleanProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.BooleanProperty.getNodeValue

  .. automethod:: talipot::tlp.BooleanProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.BooleanProperty.setAllNodeValue

  .. automethod:: talipot::tlp.BooleanProperty.setEdgeValue

  .. automethod:: talipot::tlp.BooleanProperty.setNodeValue

  .. automethod:: talipot::tlp.BooleanProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.BooleanProperty.setEdgeDefaultValue

tlp.ColorProperty
^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.ColorProperty

  .. automethod:: talipot::tlp.ColorProperty.copy

  .. automethod:: talipot::tlp.ColorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.ColorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.ColorProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.ColorProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.ColorProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.ColorProperty.getEdgeValue

  .. automethod:: talipot::tlp.ColorProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.ColorProperty.getNodeValue

  .. automethod:: talipot::tlp.ColorProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.ColorProperty.setAllNodeValue

  .. automethod:: talipot::tlp.ColorProperty.setEdgeValue

  .. automethod:: talipot::tlp.ColorProperty.setNodeValue

  .. automethod:: talipot::tlp.ColorProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.ColorProperty.setEdgeDefaultValue

tlp.NumericProperty
^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.NumericProperty

  .. automethod:: talipot::tlp.NumericProperty.copyProperty

  .. automethod:: talipot::tlp.NumericProperty.uniformQuantification

  .. automethod:: talipot::tlp.NumericProperty.nodesUniformQuantification

  .. automethod:: talipot::tlp.NumericProperty.edgesUniformQuantification

  .. automethod:: talipot::tlp.NumericProperty.getEdgeDoubleMin

  .. automethod:: talipot::tlp.NumericProperty.getEdgeDoubleMax

  .. automethod:: talipot::tlp.NumericProperty.getNodeDoubleMin

  .. automethod:: talipot::tlp.NumericProperty.getNodeDoubleMax

  .. automethod:: talipot::tlp.NumericProperty.getEdgeDoubleDefaultValue

  .. automethod:: talipot::tlp.NumericProperty.getEdgeDoubleValue

  .. automethod:: talipot::tlp.NumericProperty.getNodeDoubleDefaultValue

  .. automethod:: talipot::tlp.NumericProperty.getNodeDoubleValue

  .. automethod:: talipot::tlp.NumericProperty.getSortedNodes

  .. automethod:: talipot::tlp.NumericProperty.getSortedEdges

  .. automethod:: talipot::tlp.NumericProperty.getSortedEdgesBySourceValue

  .. automethod:: talipot::tlp.NumericProperty.getSortedEdgesByTargetValue

  .. automethod:: talipot::tlp.NumericProperty.getSortedEdgesByExtremitiesValues

tlp.DoubleProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.DoubleProperty

  .. automethod:: talipot::tlp.DoubleProperty.copy

  .. automethod:: talipot::tlp.DoubleProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.DoubleProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.DoubleProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.DoubleProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.DoubleProperty.getNodeMin

  .. automethod:: talipot::tlp.DoubleProperty.getNodeMax

  .. automethod:: talipot::tlp.DoubleProperty.getEdgeMin

  .. automethod:: talipot::tlp.DoubleProperty.getEdgeMax

  .. automethod:: talipot::tlp.DoubleProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.DoubleProperty.getEdgeValue

  .. automethod:: talipot::tlp.DoubleProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.DoubleProperty.getNodeValue

  .. automethod:: talipot::tlp.DoubleProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.DoubleProperty.setAllNodeValue

  .. automethod:: talipot::tlp.DoubleProperty.setEdgeValue

  .. automethod:: talipot::tlp.DoubleProperty.setNodeValue

  .. automethod:: talipot::tlp.DoubleProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.DoubleProperty.setEdgeDefaultValue

tlp.IntegerProperty
^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.IntegerProperty

  .. automethod:: talipot::tlp.IntegerProperty.copy

  .. automethod:: talipot::tlp.IntegerProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.IntegerProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.IntegerProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.IntegerProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.IntegerProperty.getNodeMin

  .. automethod:: talipot::tlp.IntegerProperty.getNodeMax

  .. automethod:: talipot::tlp.IntegerProperty.getEdgeMin

  .. automethod:: talipot::tlp.IntegerProperty.getEdgeMax

  .. automethod:: talipot::tlp.IntegerProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.IntegerProperty.getEdgeValue

  .. automethod:: talipot::tlp.IntegerProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.IntegerProperty.getNodeValue

  .. automethod:: talipot::tlp.IntegerProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.IntegerProperty.setAllNodeValue

  .. automethod:: talipot::tlp.IntegerProperty.setEdgeValue

  .. automethod:: talipot::tlp.IntegerProperty.setNodeValue

  .. automethod:: talipot::tlp.IntegerProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.IntegerProperty.setEdgeDefaultValue

tlp.LayoutProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.LayoutProperty

  .. automethod:: talipot::tlp.LayoutProperty.angularResolutions

  .. automethod:: talipot::tlp.LayoutProperty.averageAngularResolution

  .. automethod:: talipot::tlp.LayoutProperty.averageAngularResolution
    :noindex:

  .. automethod:: talipot::tlp.LayoutProperty.averageEdgeLength

  .. automethod:: talipot::tlp.LayoutProperty.center

  .. automethod:: talipot::tlp.LayoutProperty.center
    :noindex:

  .. automethod:: talipot::tlp.LayoutProperty.computeEmbedding

  .. automethod:: talipot::tlp.LayoutProperty.computeEmbedding
    :noindex:

  .. automethod:: talipot::tlp.LayoutProperty.copy

  .. automethod:: talipot::tlp.LayoutProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.LayoutProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.LayoutProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.LayoutProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.LayoutProperty.edgeLength

  .. automethod:: talipot::tlp.LayoutProperty.getMin

  .. automethod:: talipot::tlp.LayoutProperty.getMax

  .. automethod:: talipot::tlp.LayoutProperty.normalize

  .. automethod:: talipot::tlp.LayoutProperty.perfectAspectRatio

  .. automethod:: talipot::tlp.LayoutProperty.rotateX

  .. automethod:: talipot::tlp.LayoutProperty.rotateX
    :noindex:

  .. automethod:: talipot::tlp.LayoutProperty.rotateY

  .. automethod:: talipot::tlp.LayoutProperty.rotateY
    :noindex:

  .. automethod:: talipot::tlp.LayoutProperty.rotateZ

  .. automethod:: talipot::tlp.LayoutProperty.rotateZ
    :noindex:

  .. automethod:: talipot::tlp.LayoutProperty.scale

  .. automethod:: talipot::tlp.LayoutProperty.scale
    :noindex:

  .. automethod:: talipot::tlp.LayoutProperty.translate

  .. automethod:: talipot::tlp.LayoutProperty.translate
    :noindex:

  .. automethod:: talipot::tlp.LayoutProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.LayoutProperty.getEdgeValue

  .. automethod:: talipot::tlp.LayoutProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.LayoutProperty.getNodeValue

  .. automethod:: talipot::tlp.LayoutProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.LayoutProperty.setAllNodeValue

  .. automethod:: talipot::tlp.LayoutProperty.setEdgeValue

  .. automethod:: talipot::tlp.LayoutProperty.setNodeValue

  .. automethod:: talipot::tlp.LayoutProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.LayoutProperty.setEdgeDefaultValue

tlp.SizeProperty
^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.SizeProperty

  .. automethod:: talipot::tlp.SizeProperty.copy

  .. automethod:: talipot::tlp.SizeProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.SizeProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.SizeProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.SizeProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.SizeProperty.getMin

  .. automethod:: talipot::tlp.SizeProperty.getMax

  .. automethod:: talipot::tlp.SizeProperty.scale

  .. automethod:: talipot::tlp.SizeProperty.scale
    :noindex:

  .. automethod:: talipot::tlp.SizeProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.SizeProperty.getEdgeValue

  .. automethod:: talipot::tlp.SizeProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.SizeProperty.getNodeValue

  .. automethod:: talipot::tlp.SizeProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.SizeProperty.setAllNodeValue

  .. automethod:: talipot::tlp.SizeProperty.setEdgeValue

  .. automethod:: talipot::tlp.SizeProperty.setNodeValue

  .. automethod:: talipot::tlp.SizeProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.SizeProperty.setEdgeDefaultValue

tlp.StringProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.StringProperty

  .. automethod:: talipot::tlp.StringProperty.copy

  .. automethod:: talipot::tlp.StringProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.StringProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.StringProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.StringProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.StringProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.StringProperty.getEdgeValue

  .. automethod:: talipot::tlp.StringProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.StringProperty.getNodeValue

  .. automethod:: talipot::tlp.StringProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.StringProperty.setAllNodeValue

  .. automethod:: talipot::tlp.StringProperty.setEdgeValue

  .. automethod:: talipot::tlp.StringProperty.setNodeValue

  .. automethod:: talipot::tlp.StringProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.StringProperty.setEdgeDefaultValue

tlp.BooleanVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.BooleanVectorProperty

  .. automethod:: talipot::tlp.BooleanVectorProperty.copy

  .. automethod:: talipot::tlp.BooleanVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.BooleanVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.BooleanVectorProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.BooleanVectorProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.BooleanVectorProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.getEdgeValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.getNodeValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.setAllNodeValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.setEdgeValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.setNodeValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.setEdgeDefaultValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.getNodeEltValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.setNodeEltValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.pushBackNodeEltValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.popBackNodeEltValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.resizeNodeValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: talipot::tlp.BooleanVectorProperty.getEdgeEltValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.setEdgeEltValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.pushBackEdgeEltValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.popBackEdgeEltValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.resizeEdgeValue

  .. automethod:: talipot::tlp.BooleanVectorProperty.resizeEdgeValue
    :noindex:

tlp.CoordVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.CoordVectorProperty

  .. automethod:: talipot::tlp.CoordVectorProperty.copy

  .. automethod:: talipot::tlp.CoordVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.CoordVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.CoordVectorProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.CoordVectorProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.CoordVectorProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.CoordVectorProperty.getEdgeValue

  .. automethod:: talipot::tlp.CoordVectorProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.CoordVectorProperty.getNodeValue

  .. automethod:: talipot::tlp.CoordVectorProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.CoordVectorProperty.setAllNodeValue

  .. automethod:: talipot::tlp.CoordVectorProperty.setEdgeValue

  .. automethod:: talipot::tlp.CoordVectorProperty.setNodeValue

  .. automethod:: talipot::tlp.CoordVectorProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.CoordVectorProperty.setEdgeDefaultValue

  .. automethod:: talipot::tlp.CoordVectorProperty.getNodeEltValue

  .. automethod:: talipot::tlp.CoordVectorProperty.setNodeEltValue

  .. automethod:: talipot::tlp.CoordVectorProperty.pushBackNodeEltValue

  .. automethod:: talipot::tlp.CoordVectorProperty.popBackNodeEltValue

  .. automethod:: talipot::tlp.CoordVectorProperty.resizeNodeValue

  .. automethod:: talipot::tlp.CoordVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: talipot::tlp.CoordVectorProperty.getEdgeEltValue

  .. automethod:: talipot::tlp.CoordVectorProperty.setEdgeEltValue

  .. automethod:: talipot::tlp.CoordVectorProperty.pushBackEdgeEltValue

  .. automethod:: talipot::tlp.CoordVectorProperty.popBackEdgeEltValue

  .. automethod:: talipot::tlp.CoordVectorProperty.resizeEdgeValue

  .. automethod:: talipot::tlp.CoordVectorProperty.resizeEdgeValue
    :noindex:

tlp.ColorVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.ColorVectorProperty

  .. automethod:: talipot::tlp.ColorVectorProperty.copy

  .. automethod:: talipot::tlp.ColorVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.ColorVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.ColorVectorProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.ColorVectorProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.ColorVectorProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.ColorVectorProperty.getEdgeValue

  .. automethod:: talipot::tlp.ColorVectorProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.ColorVectorProperty.getNodeValue

  .. automethod:: talipot::tlp.ColorVectorProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.ColorVectorProperty.setAllNodeValue

  .. automethod:: talipot::tlp.ColorVectorProperty.setEdgeValue

  .. automethod:: talipot::tlp.ColorVectorProperty.setNodeValue

  .. automethod:: talipot::tlp.ColorVectorProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.ColorVectorProperty.setEdgeDefaultValue

  .. automethod:: talipot::tlp.ColorVectorProperty.getNodeEltValue

  .. automethod:: talipot::tlp.ColorVectorProperty.setNodeEltValue

  .. automethod:: talipot::tlp.ColorVectorProperty.pushBackNodeEltValue

  .. automethod:: talipot::tlp.ColorVectorProperty.popBackNodeEltValue

  .. automethod:: talipot::tlp.ColorVectorProperty.resizeNodeValue

  .. automethod:: talipot::tlp.ColorVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: talipot::tlp.ColorVectorProperty.getEdgeEltValue

  .. automethod:: talipot::tlp.ColorVectorProperty.setEdgeEltValue

  .. automethod:: talipot::tlp.ColorVectorProperty.pushBackEdgeEltValue

  .. automethod:: talipot::tlp.ColorVectorProperty.popBackEdgeEltValue

  .. automethod:: talipot::tlp.ColorVectorProperty.resizeEdgeValue

  .. automethod:: talipot::tlp.ColorVectorProperty.resizeEdgeValue
    :noindex:

tlp.DoubleVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.DoubleVectorProperty

  .. automethod:: talipot::tlp.DoubleVectorProperty.copy

  .. automethod:: talipot::tlp.DoubleVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.DoubleVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.DoubleVectorProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.DoubleVectorProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.DoubleVectorProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.getEdgeValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.getNodeValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.setAllNodeValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.setEdgeValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.setNodeValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.setEdgeDefaultValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.getNodeEltValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.setNodeEltValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.pushBackNodeEltValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.popBackNodeEltValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.resizeNodeValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: talipot::tlp.DoubleVectorProperty.getEdgeEltValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.setEdgeEltValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.pushBackEdgeEltValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.popBackEdgeEltValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.resizeEdgeValue

  .. automethod:: talipot::tlp.DoubleVectorProperty.resizeEdgeValue
    :noindex:

tlp.IntegerVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.IntegerVectorProperty

  .. automethod:: talipot::tlp.IntegerVectorProperty.copy

  .. automethod:: talipot::tlp.IntegerVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.IntegerVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.IntegerVectorProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.IntegerVectorProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.IntegerVectorProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.getEdgeValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.getNodeValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.setAllNodeValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.setEdgeValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.setNodeValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.setEdgeDefaultValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.getNodeEltValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.setNodeEltValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.pushBackNodeEltValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.popBackNodeEltValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.resizeNodeValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: talipot::tlp.IntegerVectorProperty.getEdgeEltValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.setEdgeEltValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.pushBackEdgeEltValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.popBackEdgeEltValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.resizeEdgeValue

  .. automethod:: talipot::tlp.IntegerVectorProperty.resizeEdgeValue
    :noindex:

tlp.SizeVectorProperty
^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.SizeVectorProperty

  .. automethod:: talipot::tlp.SizeVectorProperty.copy

  .. automethod:: talipot::tlp.SizeVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.SizeVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.SizeVectorProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.SizeVectorProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.SizeVectorProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.SizeVectorProperty.getEdgeValue

  .. automethod:: talipot::tlp.SizeVectorProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.SizeVectorProperty.getNodeValue

  .. automethod:: talipot::tlp.SizeVectorProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.SizeVectorProperty.setAllNodeValue

  .. automethod:: talipot::tlp.SizeVectorProperty.setEdgeValue

  .. automethod:: talipot::tlp.SizeVectorProperty.setNodeValue

  .. automethod:: talipot::tlp.SizeVectorProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.SizeVectorProperty.setEdgeDefaultValue

  .. automethod:: talipot::tlp.SizeVectorProperty.getNodeEltValue

  .. automethod:: talipot::tlp.SizeVectorProperty.setNodeEltValue

  .. automethod:: talipot::tlp.SizeVectorProperty.pushBackNodeEltValue

  .. automethod:: talipot::tlp.SizeVectorProperty.popBackNodeEltValue

  .. automethod:: talipot::tlp.SizeVectorProperty.resizeNodeValue

  .. automethod:: talipot::tlp.SizeVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: talipot::tlp.SizeVectorProperty.getEdgeEltValue

  .. automethod:: talipot::tlp.SizeVectorProperty.setEdgeEltValue

  .. automethod:: talipot::tlp.SizeVectorProperty.pushBackEdgeEltValue

  .. automethod:: talipot::tlp.SizeVectorProperty.popBackEdgeEltValue

  .. automethod:: talipot::tlp.SizeVectorProperty.resizeEdgeValue

  .. automethod:: talipot::tlp.SizeVectorProperty.resizeEdgeValue
    :noindex:

tlp.StringVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.StringVectorProperty

  .. automethod:: talipot::tlp.StringVectorProperty.copy

  .. automethod:: talipot::tlp.StringVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.StringVectorProperty.copy
    :noindex:

  .. automethod:: talipot::tlp.StringVectorProperty.getNodesEqualTo

  .. automethod:: talipot::tlp.StringVectorProperty.getEdgesEqualTo

  .. automethod:: talipot::tlp.StringVectorProperty.getEdgeDefaultValue

  .. automethod:: talipot::tlp.StringVectorProperty.getEdgeValue

  .. automethod:: talipot::tlp.StringVectorProperty.getNodeDefaultValue

  .. automethod:: talipot::tlp.StringVectorProperty.getNodeValue

  .. automethod:: talipot::tlp.StringVectorProperty.setAllEdgeValue

  .. automethod:: talipot::tlp.StringVectorProperty.setAllNodeValue

  .. automethod:: talipot::tlp.StringVectorProperty.setEdgeValue

  .. automethod:: talipot::tlp.StringVectorProperty.setNodeValue

  .. automethod:: talipot::tlp.StringVectorProperty.setNodeDefaultValue

  .. automethod:: talipot::tlp.StringVectorProperty.setEdgeDefaultValue

  .. automethod:: talipot::tlp.StringVectorProperty.getNodeEltValue

  .. automethod:: talipot::tlp.StringVectorProperty.setNodeEltValue

  .. automethod:: talipot::tlp.StringVectorProperty.pushBackNodeEltValue

  .. automethod:: talipot::tlp.StringVectorProperty.popBackNodeEltValue

  .. automethod:: talipot::tlp.StringVectorProperty.resizeNodeValue

  .. automethod:: talipot::tlp.StringVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: talipot::tlp.StringVectorProperty.getEdgeEltValue

  .. automethod:: talipot::tlp.StringVectorProperty.setEdgeEltValue

  .. automethod:: talipot::tlp.StringVectorProperty.pushBackEdgeEltValue

  .. automethod:: talipot::tlp.StringVectorProperty.popBackEdgeEltValue

  .. automethod:: talipot::tlp.StringVectorProperty.resizeEdgeValue

  .. automethod:: talipot::tlp.StringVectorProperty.resizeEdgeValue
    :noindex:

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

  .. automethod:: talipot::tlp.Algorithm.check

  .. automethod:: talipot::tlp.Algorithm.run


tlp.BooleanAlgorithm
^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.BooleanAlgorithm

tlp.ColorAlgorithm
^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.ColorAlgorithm

tlp.DoubleAlgorithm
^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.DoubleAlgorithm

tlp.IntegerAlgorithm
^^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.IntegerAlgorithm

tlp.LayoutAlgorithm
^^^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.LayoutAlgorithm

tlp.SizeAlgorithm
^^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.SizeAlgorithm

tlp.ImportModule
^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.ImportModule

  .. automethod:: talipot::tlp.ImportModule.fileExtensions

  .. automethod:: talipot::tlp.ImportModule.gzipFileExtensions

  .. automethod:: talipot::tlp.ImportModule.allFileExtensions

  .. automethod:: talipot::tlp.ImportModule.importGraph

tlp.ExportModule
^^^^^^^^^^^^^^^^

.. autoclass:: talipot::tlp.ExportModule

  .. automethod:: talipot::tlp.ExportModule.fileExtension

  .. automethod:: talipot::tlp.ExportModule.gzipFileExtensions

  .. automethod:: talipot::tlp.ExportModule.allFileExtensions

  .. automethod:: talipot::tlp.ExportModule.exportGraph


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
