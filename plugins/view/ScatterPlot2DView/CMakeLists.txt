INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude}
  ${TalipotCoreInclude}
  ${TalipotOGLInclude}
  ${TalipotGUIInclude}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GLEW_INCLUDE_DIR})

SET(LIB_SRCS
    ScatterPlot2DView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.cpp
    ScatterPlot2D.cpp
    ScatterPlot2DViewNavigator.cpp
    ScatterPlotTrendLine.cpp
    ScatterPlot2DOptionsWidget.cpp
    ScatterPlot2DInteractors.cpp
    ScatterPlotCorrelCoeffSelector.cpp
    ScatterPlotCorrelCoeffSelectorOptionsWidget.cpp
    ScatterPlotQuickAccessBar.cpp)

QT_WRAP_UI(
  LIB_UI_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.ui
  ScatterPlot2DOptionsWidget.ui ScatterPlotCorrelCoeffSelectorOptionsWidget.ui)

QT_WRAP_CPP(
  LIB_MOC_SRCS ScatterPlot2DView.h ScatterPlot2DOptionsWidget.h
  ScatterPlotCorrelCoeffSelectorOptionsWidget.h ScatterPlotQuickAccessBar.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.h)

QT_ADD_RESOURCES(LIB_RCC_SRCS ScatterPlotResource.qrc)

TALIPOT_ADD_PLUGIN(
  NAME
  ScatterPlot2DView
  SRCS
  ${LIB_SRCS}
  ${LIB_UI_HEADERS}
  ${LIB_MOC_SRCS}
  ${LIB_RCC_SRCS}
  LINKS
  ${LibTalipotCoreName}
  ${LibTalipotOGLName}
  ${LibTalipotGUIName}
  ${OPENGL_gl_LIBRARY}
  INSTALL_DIR
  ${TalipotViewPluginsInstallDir})
