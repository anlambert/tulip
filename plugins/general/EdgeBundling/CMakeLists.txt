INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

SET(LIB_SRCS BendsTools.cpp Dijkstra.cpp EdgeBundling.cpp OctreeBundle.cpp
             QuadTree.cpp SphereUtils.cpp)

TALIPOT_ADD_PLUGIN(
  NAME
  EdgeBundling
  SRCS
  ${LIB_SRCS}
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
