INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude}
                    ${TalipotGUIInclude})

TALIPOT_ADD_PLUGIN(
  NAME
  ReverseEdges
  SRCS
  ReverseEdges.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
