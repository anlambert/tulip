IF(NOT TALIPOT_BUILD_CORE_ONLY)
  ADD_SUBDIRECTORY(SVGExport)
ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)

INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

# ------------------------------------------------------------------------------
SET(GMLExport_SRCS GMLExport.cpp)

SET(CSVExport_SRCS CSVExport.cpp)

ADD_LIBRARY(GMLExport-${TalipotVersion} SHARED ${GMLExport_SRCS})
TARGET_LINK_LIBRARIES(GMLExport-${TalipotVersion} ${LibTalipotCoreName})

ADD_LIBRARY(CSVExport-${TalipotVersion} SHARED ${CSVExport_SRCS})
TARGET_LINK_LIBRARIES(CSVExport-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
TALIPOT_INSTALL_PLUGIN(GMLExport-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(CSVExport-${TalipotVersion} ${TalipotPluginsInstallDir})
