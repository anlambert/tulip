QTX_SET_INCLUDES_AND_DEFINITIONS()

ADD_SUBDIRECTORY(FishEye)
ADD_SUBDIRECTORY(MouseMagnifyingGlass)
ADD_SUBDIRECTORY(MouseLassoNodesSelector)
ADD_SUBDIRECTORY(NeighborhoodHighlighter)
ADD_SUBDIRECTORY(PathFinder)

INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude}
  ${TalipotGUIInclude} ${OPENGL_INCLUDE_DIR})

# ------------------------------------------------------------------------------
ADD_LIBRARY(InteractorNavigation-${TalipotVersion} SHARED
            InteractorNavigation.cpp)
TARGET_LINK_LIBRARIES(
  InteractorNavigation-${TalipotVersion} ${LibTalipotCoreName}
  ${LibTalipotOGLName} ${LibTalipotGUIName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(InteractorGetInformation-${TalipotVersion} SHARED
            InteractorGetInformation.cpp)
TARGET_LINK_LIBRARIES(
  InteractorGetInformation-${TalipotVersion} ${LibTalipotCoreName}
  ${LibTalipotOGLName} ${LibTalipotGUIName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(InteractorSelection-${TalipotVersion} SHARED
            InteractorSelection.cpp)
TARGET_LINK_LIBRARIES(
  InteractorSelection-${TalipotVersion} ${LibTalipotCoreName}
  ${LibTalipotOGLName} ${LibTalipotGUIName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(InteractorSelectionModifier-${TalipotVersion} SHARED
            InteractorSelectionModifier.cpp)
TARGET_LINK_LIBRARIES(
  InteractorSelectionModifier-${TalipotVersion} ${LibTalipotCoreName}
  ${LibTalipotOGLName} ${LibTalipotGUIName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(InteractorRectangleZoom-${TalipotVersion} SHARED
            InteractorRectangleZoom.cpp)
TARGET_LINK_LIBRARIES(
  InteractorRectangleZoom-${TalipotVersion} ${LibTalipotCoreName}
  ${LibTalipotOGLName} ${LibTalipotGUIName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(InteractorAddNodeEdge-${TalipotVersion} SHARED
            InteractorAddEdge.cpp)
TARGET_LINK_LIBRARIES(
  InteractorAddNodeEdge-${TalipotVersion} ${LibTalipotCoreName}
  ${LibTalipotOGLName} ${LibTalipotGUIName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(InteractorEditEdgeBends-${TalipotVersion} SHARED
            InteractorEditEdgeBends.cpp)
TARGET_LINK_LIBRARIES(
  InteractorEditEdgeBends-${TalipotVersion} ${LibTalipotCoreName}
  ${LibTalipotOGLName} ${LibTalipotGUIName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(InteractorDeleteElement-${TalipotVersion} SHARED
            InteractorDeleteElement.cpp)
TARGET_LINK_LIBRARIES(
  InteractorDeleteElement-${TalipotVersion} ${LibTalipotCoreName}
  ${LibTalipotOGLName} ${LibTalipotGUIName})

# ------------------------------------------------------------------------------
TALIPOT_INSTALL_PLUGIN(InteractorNavigation-${TalipotVersion}
                       ${TalipotInteractorPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(InteractorGetInformation-${TalipotVersion}
                       ${TalipotInteractorPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(InteractorSelection-${TalipotVersion}
                       ${TalipotInteractorPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(InteractorSelectionModifier-${TalipotVersion}
                       ${TalipotInteractorPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(InteractorRectangleZoom-${TalipotVersion}
                       ${TalipotInteractorPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(InteractorAddNodeEdge-${TalipotVersion}
                       ${TalipotInteractorPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(InteractorEditEdgeBends-${TalipotVersion}
                       ${TalipotInteractorPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(InteractorDeleteElement-${TalipotVersion}
                       ${TalipotInteractorPluginsInstallDir})