INCLUDE_DIRECTORIES(BEFORE ${TalipotCoreBuildInclude})

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${OPENGL_INCLUDE_DIR}
  ${TalipotCoreInclude} ${TalipotOGLInclude} ${TalipotGUIInclude})

SET(LIB_SRCS
    PathFinder.cpp
    PathFinderComponent.cpp
    PathFinderConfigurationWidget.cpp
    PathFinderTools.cpp
    PathFinding/PathAlgorithm.cpp
    PathFinding/DFS/DFS.cpp
    highlighters/EnclosingCircleHighlighter.cpp
    highlighters/PathHighlighter.cpp
    highlighters/ZoomAndPanHighlighter.cpp)

QTX_WRAP_UI(LIB_UI_HEADERS designer/EnclosingCircleConfiguration.ui
            designer/PathFinderConfiguration.ui)

QTX_WRAP_CPP(LIB_MOC_SRCS PathFinderConfigurationWidget.h PathFinder.h
             highlighters/EnclosingCircleHighlighter.h)

ADD_LIBRARY(PathFinder-${TalipotVersion} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS}
                                                ${LIB_MOC_SRCS})

TARGET_LINK_LIBRARIES(PathFinder-${TalipotVersion} ${LibTalipotCoreName}
                      ${LibTalipotOGLName} ${LibTalipotGUIName})

TALIPOT_INSTALL_PLUGIN(PathFinder-${TalipotVersion}
                       ${TalipotInteractorPluginsInstallDir})
