IF(NOT TALIPOT_BUILD_CORE_ONLY)
  ADD_SUBDIRECTORY(SVGExport)
ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)

INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

# ------------------------------------------------------------------------------
TALIPOT_ADD_PLUGIN(
  NAME
  GMLExport
  SRCS
  GMLExport.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
# ------------------------------------------------------------------------------
TALIPOT_ADD_PLUGIN(
  NAME
  CSVExport
  SRCS
  CSVExport.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
