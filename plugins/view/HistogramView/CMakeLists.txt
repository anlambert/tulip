INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude}
  ${TalipotCoreInclude}
  ${TalipotOGLInclude}
  ${TalipotGUIInclude}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GLEW_INCLUDE_DIR})

SET(LIB_SRCS
    HistogramView.cpp
    HistoStatsConfigWidget.cpp
    HistogramMetricMapping.cpp
    HistogramStatistics.cpp
    Histogram.cpp
    HistogramViewNavigator.cpp
    HistogramInteractors.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.cpp
    HistoOptionsWidget.cpp
    SizeScaleConfigDialog.cpp
    GlyphScaleConfigDialog.cpp)

QTX_WRAP_UI(
  LIB_UI_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.ui
  HistoStatsConfigWidget.ui
  HistoOptionsWidget.ui
  SizeScaleConfigDialog.ui
  GlyphScaleConfigDialog.ui)

QTX_WRAP_CPP(
  LIB_MOC_SRCS
  HistogramView.h
  HistogramStatistics.h
  HistoOptionsWidget.h
  SizeScaleConfigDialog.h
  GlyphScaleConfigDialog.h
  HistoStatsConfigWidget.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.h)

QTX_ADD_RESOURCES(LIB_RCC_SRCS HistogramResource.qrc)

TALIPOT_ADD_PLUGIN(
  NAME
  HistogramView
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
