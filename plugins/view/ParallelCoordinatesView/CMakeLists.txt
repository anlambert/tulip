INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude}
  ${TalipotCoreInclude}
  ${TalipotOGLInclude}
  ${TalipotGUIInclude}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  include)

SET(LIB_SRCS
    src/ParallelCoordinatesView.cpp
    src/ParallelCoordinatesDrawing.cpp
    src/ParallelCoordinatesGraphProxy.cpp
    src/ParallelAxis.cpp
    src/NominalParallelAxis.cpp
    src/AxisConfigDialogs.cpp
    src/QuantitativeParallelAxis.cpp
    src/ParallelCoordsElementShowInfo.cpp
    src/ParallelCoordsElementsSelector.cpp
    src/ParallelCoordsGlEntitiesSelector.cpp
    src/ParallelCoordsElementDeleter.cpp
    src/ParallelCoordsElementHighlighter.cpp
    src/ParallelCoordsAxisSwapper.cpp
    src/ParallelCoordsAxisSliders.cpp
    src/ParallelCoordsAxisBoxPlot.cpp
    src/ParallelCoordsAxisSpacer.cpp
    src/ParallelCoordinatesInteractors.cpp
    src/ParallelTools.cpp
    src/ParallelCoordsDrawConfigWidget.cpp
    src/ParallelCoordinatesViewQuickAccessbar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.cpp)

QT_WRAP_UI(
  LIB_UI_HEADERS designer/ParallelCoordsDrawConfigWidget.ui
  designer/AxisSlidersOptions.ui
  ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.ui)

QT_WRAP_CPP(
  LIB_MOC_SRCS
  include/ParallelCoordinatesView.h
  include/AxisConfigDialogs.h
  include/ParallelCoordsDrawConfigWidget.h
  include/ParallelCoordinatesViewQuickAccessbar.h
  include/ParallelCoordinatesInteractors.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.h)

QT_ADD_RESOURCES(LIB_RCC_SRCS resources/ParallelResource.qrc)

TALIPOT_ADD_PLUGIN(
  NAME
  ParallelCoordinatesView
  SRCS
  ${LIB_SRCS}
  ${LIB_UI_HEADERS}
  ${LIB_MOC_SRCS}
  ${LIB_RCC_SRCS}
  LINKS
  ${LibTalipotCoreName}
  ${LibTalipotOGLName}
  ${LibTalipotGUIName}
  INSTALL_DIR
  ${TalipotViewPluginsInstallDir})
