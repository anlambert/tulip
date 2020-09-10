QTX_SET_INCLUDES_AND_DEFINITIONS()

INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude}
  ${TalipotGUIInclude} ${CMAKE_CURRENT_SOURCE_DIR})

SET(LIB_SRCS ExportInterface.cpp ExportSvg.cpp ReadGraph.cpp Shape.cpp
             SvgExportPlugin.cpp)

TALIPOT_ADD_PLUGIN(
  NAME
  SVGExport
  SRCS
  ${LIB_SRCS}
  LINKS
  ${LibTalipotCoreName}
  ${LibTalipotOGLName}
  ${QT_LIBRARIES}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
