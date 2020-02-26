INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TalipotCoreBuildInclude}
                    ${TalipotCoreInclude})

ADD_LIBRARY(Delaunay-${TalipotVersion} SHARED DelaunayTriangulation.cpp)
TARGET_LINK_LIBRARIES(Delaunay-${TalipotVersion} ${LibTalipotCoreName})

ADD_LIBRARY(Voronoi-${TalipotVersion} SHARED VoronoiDiagram.cpp)
TARGET_LINK_LIBRARIES(Voronoi-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(Delaunay-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(Voronoi-${TalipotVersion} ${TalipotPluginsInstallDir})
