INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TalipotCoreBuildInclude}
                    ${TalipotCoreInclude})

TALIPOT_ADD_PLUGIN(
  NAME
  Delaunay
  SRCS
  DelaunayTriangulation.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})

TALIPOT_ADD_PLUGIN(
  NAME
  Voronoi
  SRCS
  VoronoiDiagram.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
