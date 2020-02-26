ADD_SUBDIRECTORY(SocialNetwork)
ADD_SUBDIRECTORY(BibTeX)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TalipotCoreBuildInclude} ${TalipotCoreInclude}
                    ${TalipotGUIInclude} ${UTF8CppInclude})

# ------------------------------------------------------------------------------
SET(ERImport_SRCS ERRandomGraph.cpp)
ADD_LIBRARY(ERImport-${TalipotVersion} SHARED ${ERImport_SRCS})
TARGET_LINK_LIBRARIES(ERImport-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(PlanarGraph_SRCS PlanarGraph.cpp)
ADD_LIBRARY(PlanarGraph-${TalipotVersion} SHARED ${PlanarGraph_SRCS})
TARGET_LINK_LIBRARIES(PlanarGraph-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(DotImport_SRCS dotImport.cpp)
ADD_LIBRARY(DotImport-${TalipotVersion} SHARED ${DotImport_SRCS})
TARGET_LINK_LIBRARIES(DotImport-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(Grid_SRCS Grid.cpp)
ADD_LIBRARY(Grid-${TalipotVersion} SHARED ${Grid_SRCS})
TARGET_LINK_LIBRARIES(Grid-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(GMLImport_SRCS GMLImport.cpp)
ADD_LIBRARY(GMLImport-${TalipotVersion} SHARED ${GMLImport_SRCS})
TARGET_LINK_LIBRARIES(GMLImport-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(RandomGraph_SRCS RandomGraph.cpp)
ADD_LIBRARY(RandomGraph-${TalipotVersion} SHARED ${RandomGraph_SRCS})
TARGET_LINK_LIBRARIES(RandomGraph-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(AdjacencyMatrixImport_SRCS AdjacencyMatrixImport.cpp)
ADD_LIBRARY(AdjacencyMatrixImport-${TalipotVersion} SHARED
            ${AdjacencyMatrixImport_SRCS})
TARGET_LINK_LIBRARIES(AdjacencyMatrixImport-${TalipotVersion}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(CompleteGraph_SRCS CompleteGraph.cpp)
ADD_LIBRARY(CompleteGraph-${TalipotVersion} SHARED ${CompleteGraph_SRCS})
TARGET_LINK_LIBRARIES(CompleteGraph-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(RandomTree_SRCS RandomTree.cpp)
ADD_LIBRARY(RandomTree-${TalipotVersion} SHARED ${RandomTree_SRCS})
TARGET_LINK_LIBRARIES(RandomTree-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(RandomTreeGeneral_SRCS RandomTreeGeneral.cpp)
ADD_LIBRARY(RandomTreeGeneral-${TalipotVersion} SHARED
            ${RandomTreeGeneral_SRCS})
TARGET_LINK_LIBRARIES(RandomTreeGeneral-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(CompleteTree_SRCS CompleteTree.cpp)
ADD_LIBRARY(CompleteTree-${TalipotVersion} SHARED ${CompleteTree_SRCS})
TARGET_LINK_LIBRARIES(CompleteTree-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(SmallWorldGraph_SRCS SmallWorldGraph.cpp)
ADD_LIBRARY(SmallWorldGraph-${TalipotVersion} SHARED ${SmallWorldGraph_SRCS})
TARGET_LINK_LIBRARIES(SmallWorldGraph-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(ImportPajek_SRCS ImportPajek.cpp)
ADD_LIBRARY(ImportPajek-${TalipotVersion} SHARED ${ImportPajek_SRCS})
TARGET_LINK_LIBRARIES(ImportPajek-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(ImportUCINET_SRCS ImportUCINET.cpp)
ADD_LIBRARY(ImportUCINET-${TalipotVersion} SHARED ${ImportUCINET_SRCS})
TARGET_LINK_LIBRARIES(ImportUCINET-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(EmptyGraph-${TalipotVersion} SHARED EmptyGraph.cpp)
TARGET_LINK_LIBRARIES(EmptyGraph-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
IF(NOT TALIPOT_BUILD_CORE_ONLY)
  QTX_SET_INCLUDES_AND_DEFINITIONS()
  SET(FileSystem_SRCS FileSystem.cpp)
  ADD_LIBRARY(FileSystem-${TalipotVersion} SHARED ${FileSystem_SRCS})
  TARGET_LINK_LIBRARIES(FileSystem-${TalipotVersion} ${LibTalipotCoreName}
                        ${QT_LIBRARIES})

  # ----------------------------------------------------------------------------
  SET(WebImport_SRCS WebImport.cpp)
  QTX_WRAP_CPP(WebImport_MOC_SRCS WebImport.h)
  ADD_LIBRARY(WebImport-${TalipotVersion} SHARED ${WebImport_SRCS}
                                                 ${WebImport_MOC_SRCS})
  TARGET_LINK_LIBRARIES(WebImport-${TalipotVersion} ${LibTalipotCoreName}
                        ${LibTalipotGUIName} ${QT_LIBRARIES})
  # ----------------------------------------------------------------------------
  ADD_LIBRARY(GEXFImport-${TalipotVersion} SHARED GEXFImport.cpp)
  TARGET_LINK_LIBRARIES(GEXFImport-${TalipotVersion} ${LibTalipotCoreName}
                        ${QT_LIBRARIES})

  # ----------------------------------------------------------------------------
ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)

# ------------------------------------------------------------------------------
TALIPOT_INSTALL_PLUGIN(PlanarGraph-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(DotImport-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(Grid-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(GMLImport-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(RandomGraph-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(AdjacencyMatrixImport-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(CompleteGraph-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(RandomTree-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(RandomTreeGeneral-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(CompleteTree-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(SmallWorldGraph-${TalipotVersion}
                       ${TalipotPluginsInstallDir})

IF(NOT TALIPOT_BUILD_CORE_ONLY)
  TALIPOT_INSTALL_PLUGIN(FileSystem-${TalipotVersion}
                         ${TalipotPluginsInstallDir})
  TALIPOT_INSTALL_PLUGIN(WebImport-${TalipotVersion}
                         ${TalipotPluginsInstallDir})
  TALIPOT_INSTALL_PLUGIN(GEXFImport-${TalipotVersion}
                         ${TalipotPluginsInstallDir})
ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)

TALIPOT_INSTALL_PLUGIN(ImportPajek-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(ImportUCINET-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(EmptyGraph-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(ERImport-${TalipotVersion} ${TalipotPluginsInstallDir})
