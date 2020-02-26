
SET(PLUGIN_NAME ReverseEdges)

INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude}
                    ${TalipotGUIInclude})

SET(PLUGIN_SRCS ReverseEdges.cpp)

ADD_LIBRARY(${PLUGIN_NAME}-${TalipotVersion} SHARED ${PLUGIN_SRCS})

TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(${PLUGIN_NAME}-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
