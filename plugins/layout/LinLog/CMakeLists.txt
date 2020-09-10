INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

SET(LIB_SRCS LinLogAlgorithm.cpp LinLogLayout.cpp OctTree.cpp)

TALIPOT_ADD_PLUGIN(
  NAME
  LinLog
  SRCS
  ${LIB_SRCS}
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
