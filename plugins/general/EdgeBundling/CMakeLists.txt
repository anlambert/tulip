INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

SET(LIB_SRCS BendsTools.cpp Dijkstra.cpp EdgeBundling.cpp OctreeBundle.cpp
             QuadTree.cpp SphereUtils.cpp)

ADD_LIBRARY(EdgeBundling-${TalipotVersion} SHARED ${LIB_SRCS})

TARGET_LINK_LIBRARIES(EdgeBundling-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(EdgeBundling-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
