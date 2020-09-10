INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

SET(LIB_SRCS Distances.cpp Grip.cpp MISFiltering.cpp)

TALIPOT_ADD_PLUGIN(
  NAME
  Grip
  SRCS
  ${LIB_SRCS}
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
