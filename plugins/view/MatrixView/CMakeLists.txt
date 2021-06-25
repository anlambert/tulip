INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude}
  ${TalipotCoreInclude}
  ${TalipotOGLInclude}
  ${TalipotGUIInclude}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

SET(LIB_SRCS
    MatrixView.cpp PropertyValuesDispatcher.cpp
    MatrixViewConfigurationWidget.cpp GlMatrixBackgroundGrid.cpp
    MatrixViewQuickAccessBar.cpp)

QT_WRAP_UI(LIB_UI_HEADERS MatrixViewConfigurationWidget.ui)

QT_WRAP_CPP(LIB_MOC_SRCS MatrixView.h MatrixViewConfigurationWidget.h
            MatrixViewQuickAccessBar.h)

QT_ADD_RESOURCES(LIB_RCC_SRCS MatrixView.qrc)

TALIPOT_ADD_PLUGIN(
  NAME
  MatrixView
  SRCS
  ${LIB_SRCS}
  ${LIB_UI_HEADERS}
  ${LIB_MOC_SRCS}
  ${LIB_RCC_SRCS}
  LINKS
  ${LibTalipotCoreName}
  ${LibTalipotOGLName}
  ${LibTalipotGUIName}
  ${OPENGL_gl_LIBRARY}
  INSTALL_DIR
  ${TalipotViewPluginsInstallDir})
