INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR} ${TalipotCoreBuildInclude} ${TalipotCoreInclude}
  ${TalipotOGLInclude} ${TalipotGUIInclude} ${GLEW_INCLUDE_DIR})

SET(LIB_SRCS FisheyeInteractor.cpp FisheyeConfigWidget.cpp)

QT_WRAP_UI(LIB_UI_HEADERS FisheyeConfigWidget.ui)

QT_WRAP_CPP(LIB_MOC_SRCS)

TALIPOT_ADD_PLUGIN(
  NAME
  FisheyeInteractor
  SRCS
  ${LIB_SRCS}
  ${LIB_UI_HEADERS}
  ${LIB_MOC_SRCS}
  LINKS
  ${LibTalipotCoreName}
  ${LibTalipotOGLName}
  ${LibTalipotGUIName}
  INSTALL_DIR
  ${TalipotInteractorPluginsInstallDir})
