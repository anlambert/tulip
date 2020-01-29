INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR} ${TalipotCoreBuildInclude} ${TalipotCoreInclude}
  ${TalipotOGLInclude} ${TalipotGUIInclude} ${GLEW_INCLUDE_DIR})

SET(LIB_SRCS MouseMagnifyingGlass.cpp)

ADD_LIBRARY(MouseMagnifyingGlass-${TalipotVersion} SHARED ${LIB_SRCS})

TARGET_LINK_LIBRARIES(
  MouseMagnifyingGlass-${TalipotVersion} ${LibTalipotCoreName}
  ${LibTalipotOGLName} ${LibTalipotGUIName} ${OPENGL_gl_LIBRARY}
  ${GLEW_LIBRARY})

TALIPOT_INSTALL_PLUGIN(MouseMagnifyingGlass-${TalipotVersion}
                       ${TalipotInteractorPluginsInstallDir})
