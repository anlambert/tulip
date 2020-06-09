ADD_SUBDIRECTORY(OGDF)
ADD_SUBDIRECTORY(Grip)
ADD_SUBDIRECTORY(LinLog)

INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

SET(LayoutUtils_SRCS DatasetTools.cpp OrientableCoord.cpp OrientableLayout.cpp
                     OrientableSize.cpp OrientableSizeProxy.cpp Orientation.cpp)

IF(NOT EMSCRIPTEN)
  SET(LayoutUtilsLibraryName LayoutUtils)
  ADD_LIBRARY(${LayoutUtilsLibraryName} STATIC ${LayoutUtils_SRCS})
  SET_TARGET_PROPERTIES(${LayoutUtilsLibraryName}
                        PROPERTIES POSITION_INDEPENDENT_CODE ON)
  ADD_DEPENDENCIES(${LayoutUtilsLibraryName} ${LibTalipotCoreName})
ELSE(NOT EMSCRIPTEN)
  SET(LayoutUtilsLibraryName "")
ENDIF(NOT EMSCRIPTEN)

# ------------------------------------------------------------------------------
ADD_LIBRARY(MixedModel-${TalipotVersion} SHARED MixedModel.cpp)
TARGET_LINK_LIBRARIES(MixedModel-${TalipotVersion} ${LayoutUtilsLibraryName}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(ConnectedComponentPacking_SRCS
    ConnectedComponentPacking.cpp Number.cpp RectanglePacking.cpp
    rectanglePackingFonctions.cpp RectangleRelativePositionList.cpp)
ADD_LIBRARY(ConnectedComponentPacking-${TalipotVersion} SHARED
            ${ConnectedComponentPacking_SRCS})
TARGET_LINK_LIBRARIES(ConnectedComponentPacking-${TalipotVersion}
                      ${LayoutUtilsLibraryName} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(Random-${TalipotVersion} SHARED Random.cpp)
TARGET_LINK_LIBRARIES(Random-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(GemLayout-${TalipotVersion} SHARED GEMLayout.cpp)
TARGET_LINK_LIBRARIES(GemLayout-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(TreeReingoldAndTilforExtended-${TalipotVersion} SHARED
            TreeReingoldAndTilfordExtended.cpp)
TARGET_LINK_LIBRARIES(TreeReingoldAndTilforExtended-${TalipotVersion}
                      ${LayoutUtilsLibraryName} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(ConeTreeExtended-${TalipotVersion} SHARED ConeTreeExtended.cpp)
TARGET_LINK_LIBRARIES(ConeTreeExtended-${TalipotVersion}
                      ${LayoutUtilsLibraryName} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(TreeRadial-${TalipotVersion} SHARED TreeRadial.cpp)
TARGET_LINK_LIBRARIES(TreeRadial-${TalipotVersion} ${LayoutUtilsLibraryName}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(TreeLeaf-${TalipotVersion} SHARED TreeLeaf.cpp)
TARGET_LINK_LIBRARIES(TreeLeaf-${TalipotVersion} ${LayoutUtilsLibraryName}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(BubblePack-${TalipotVersion} SHARED BubblePack.cpp)
TARGET_LINK_LIBRARIES(BubblePack-${TalipotVersion} ${LayoutUtilsLibraryName}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(BubbleTree-${TalipotVersion} SHARED BubbleTree.cpp)
TARGET_LINK_LIBRARIES(BubbleTree-${TalipotVersion} ${LayoutUtilsLibraryName}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(Circular-${TalipotVersion} SHARED Circular.cpp)
TARGET_LINK_LIBRARIES(Circular-${TalipotVersion} ${LayoutUtilsLibraryName}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(HierarchicalGraph-${TalipotVersion} SHARED HierarchicalGraph.cpp)
TARGET_LINK_LIBRARIES(HierarchicalGraph-${TalipotVersion}
                      ${LayoutUtilsLibraryName} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(Tutte-${TalipotVersion} SHARED Tutte.cpp)
TARGET_LINK_LIBRARIES(Tutte-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(Dendrogram-${TalipotVersion} SHARED Dendrogram.cpp)
TARGET_LINK_LIBRARIES(Dendrogram-${TalipotVersion} ${LayoutUtilsLibraryName}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(ImprovedWalker-${TalipotVersion} SHARED ImprovedWalker.cpp)
TARGET_LINK_LIBRARIES(ImprovedWalker-${TalipotVersion}
                      ${LayoutUtilsLibraryName} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(SquarifiedTreeMap-${TalipotVersion} SHARED SquarifiedTreeMap.cpp)
TARGET_LINK_LIBRARIES(SquarifiedTreeMap-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(PerfectLayout-${TalipotVersion} SHARED PerfectLayout.cpp)
TARGET_LINK_LIBRARIES(PerfectLayout-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(OrthoTree-${TalipotVersion} SHARED OrthoTree.cpp)
TARGET_LINK_LIBRARIES(OrthoTree-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(PolyominoPacking-${TalipotVersion} SHARED PolyominoPacking.cpp)
TARGET_LINK_LIBRARIES(PolyominoPacking-${TalipotVersion}
                      ${LayoutUtilsLibraryName} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(${VPSCInclude})
ADD_LIBRARY(FastOverlapRemoval-${TalipotVersion} SHARED FastOverlapRemoval.cpp)
TARGET_LINK_LIBRARIES(
  FastOverlapRemoval-${TalipotVersion} ${LayoutUtilsLibraryName}
  ${LibTalipotCoreName} ${VPSCLibrary})

# ------------------------------------------------------------------------------
TALIPOT_INSTALL_PLUGIN(MixedModel-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(ConnectedComponentPacking-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(Random-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(GemLayout-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(TreeReingoldAndTilforExtended-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(ConeTreeExtended-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(TreeRadial-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(TreeLeaf-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(BubblePack-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(BubbleTree-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(Circular-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(HierarchicalGraph-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(Tutte-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(Dendrogram-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(ImprovedWalker-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(SquarifiedTreeMap-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(OrthoTree-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(PerfectLayout-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(PolyominoPacking-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(FastOverlapRemoval-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
