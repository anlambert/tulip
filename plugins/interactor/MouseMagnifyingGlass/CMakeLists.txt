INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR} ${TalipotCoreBuildInclude} ${TalipotCoreInclude}
  ${TalipotOGLInclude} ${TalipotGUIInclude} ${GLEW_INCLUDE_DIR})

TALIPOT_ADD_PLUGIN(
  NAME
  MouseMagnifyingGlass
  SRCS
  MouseMagnifyingGlass.cpp
  LINKS
  ${LibTalipotCoreName}
  ${LibTalipotOGLName}
  ${LibTalipotGUIName}
  ${OPENGL_gl_LIBRARY}
  ${GLEW_LIBRARY}
  INSTALL_DIR
  ${TalipotInteractorPluginsInstallDir})
