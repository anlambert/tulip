ADD_SUBDIRECTORY(SOMLIB)

INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude}
  ${TalipotCoreInclude}
  ${TalipotOGLInclude}
  ${TalipotGUIInclude}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/SOMLIB/)

SET(LIB_SRCS
    ColorScalePreview.cpp
    ComputeSOMWidget.cpp
    EditColorScaleInteractor.cpp
    GlLabelledColorScale.cpp
    GradientManager.cpp
    ThresholdInteractor.cpp
    SOMMapElement.cpp
    SOMPreviewComposite.cpp
    SOMPropertiesWidget.cpp
    SOMView.cpp
    SOMWidget.cpp
    SOMViewInteractor.cpp
    ZoomUtils.cpp)

QT_WRAP_UI(LIB_UI_HEADERS ComputeSOMWidget.ui SOMPropertiesWidget.ui
           SOMWidget.ui)

QT_WRAP_CPP(LIB_MOC_SRCS ColorScalePreview.h ComputeSOMWidget.h
            SOMPropertiesWidget.h SOMWidget.h SOMView.h)

QT_ADD_RESOURCES(LIB_RCC_SRCS SOMViewResource.qrc)

TALIPOT_ADD_PLUGIN(
  NAME
  SOMView
  SRCS
  ${LIB_SRCS}
  ${LIB_UI_HEADERS}
  ${LIB_MOC_SRCS}
  ${LIB_RCC_SRCS}
  LINKS
  som
  ${LibTalipotCoreName}
  ${LibTalipotOGLName}
  ${LibTalipotGUIName}
  ${OPENGL_gl_LIBRARY}
  INSTALL_DIR
  ${TalipotViewPluginsInstallDir})
