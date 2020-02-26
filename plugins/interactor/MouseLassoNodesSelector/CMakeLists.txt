INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${TalipotCoreBuildInclude}
  ${TalipotCoreInclude}
  ${TalipotOGLInclude}
  ${TalipotGUIInclude}
  ${OPENGL_INCLUDE_DIR})

SET(LIB_SRCS MouseLassoNodesSelector.cpp)

QTX_ADD_RESOURCES(LIB_RCC_SRCS MouseLassoNodesSelectorResource.qrc)

ADD_LIBRARY(MouseLassoNodesSelector-${TalipotVersion} SHARED ${LIB_SRCS}
                                                             ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(
  MouseLassoNodesSelector-${TalipotVersion} ${LibTalipotCoreName}
  ${LibTalipotOGLName} ${LibTalipotGUIName} ${OPENGL_gl_LIBRARY})

TALIPOT_INSTALL_PLUGIN(MouseLassoNodesSelector-${TalipotVersion}
                       ${TalipotInteractorPluginsInstallDir})
