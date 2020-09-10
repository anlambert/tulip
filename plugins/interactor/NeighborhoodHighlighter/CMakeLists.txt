INCLUDE_DIRECTORIES(BEFORE ${TalipotCoreBuildInclude})

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${OPENGL_INCLUDE_DIR}
  ${TalipotCoreInclude} ${TalipotOGLInclude} ${TalipotGUIInclude})

SET(LIB_SRCS NeighborhoodHighlighterConfigWidget.cpp
             NeighborhoodHighlighterInteractor.cpp NodeNeighborhoodView.cpp)

QTX_WRAP_UI(LIB_UI_HEADERS NeighborhoodHighlighterConfigWidget.ui)

QTX_WRAP_CPP(LIB_MOC_SRCS NeighborhoodHighlighterInteractor.h
             NeighborhoodHighlighterConfigWidget.h)

TALIPOT_ADD_PLUGIN(
  NAME
  NeighborhoodHighlighter
  SRCS
  ${LIB_SRCS}
  ${LIB_UI_HEADERS}
  ${LIB_MOC_SRCS}
  LINKS
  ${LibTalipotCoreName}
  ${LibTalipotOGLName}
  ${LibTalipotGUIName}
  ${OPENGL_gl_LIBRARY}
  INSTALL_DIR
  ${TalipotInteractorPluginsInstallDir})
