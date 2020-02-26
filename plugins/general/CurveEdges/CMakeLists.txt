SET(PLUGIN_NAME CurveEdges)

INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

SET(PLUGIN_SRCS CurveEdges.cpp)

ADD_LIBRARY(${PLUGIN_NAME}-${TalipotVersion} SHARED ${PLUGIN_SRCS})
TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(${PLUGIN_NAME}-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
