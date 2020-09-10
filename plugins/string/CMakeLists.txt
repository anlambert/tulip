INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

TALIPOT_ADD_PLUGIN(
  NAME
  ToLabels
  SRCS
  ToLabels.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
