QTX_SET_INCLUDES_AND_DEFINITIONS()

INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude}
  ${TalipotGUIInclude} ${CMAKE_CURRENT_SOURCE_DIR})

SET(LIB_SRCS ExportInterface.cpp ExportSvg.cpp ReadGraph.cpp Shape.cpp
             SvgExportPlugin.cpp)

ADD_LIBRARY(SVGExport-${TalipotVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(SVGExport-${TalipotVersion} ${LibTalipotCoreName}
                      ${LibTalipotOGLName} ${QT_LIBRARIES})

TALIPOT_INSTALL_PLUGIN(SVGExport-${TalipotVersion} ${TalipotPluginsInstallDir})
