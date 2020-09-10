INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

TALIPOT_ADD_PLUGIN(
  NAME
  CurveEdges
  SRCS
  CurveEdges.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
