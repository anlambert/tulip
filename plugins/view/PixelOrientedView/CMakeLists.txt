INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude}
  ${TalipotCoreInclude}
  ${TalipotOGLInclude}
  ${TalipotGUIInclude}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GLEW_INCLUDE_DIR})

SET(LIB_SRCS
    PixelOrientedView.cpp
    PixelOrientedOverview.cpp
    PixelOrientedViewNavigator.cpp
    PixelOrientedInteractors.cpp
    PixelOrientedOptionsWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.cpp
    PixelOrientedViewQuickAccessBar.cpp
    POLIB/FishEyesScreen.cpp
    POLIB/FishEyesScreenFurnas.cpp
    POLIB/HilbertLayout.cpp
    POLIB/HSIColorMapping.cpp
    POLIB/HSIColorSpace.cpp
    POLIB/LinearMappingColor.cpp
    POLIB/LogarithmicMappingColor.cpp
    POLIB/PixelOrientedMediator.cpp
    POLIB/SpiralLayout.cpp
    POLIB/SquareLayout.cpp
    POLIB/GraphDimension.cpp
    POLIB/NodeColorMapping.cpp
    POLIB/NodeMetricSorter.cpp
    POLIB/UniformDeformationScreen.cpp
    POLIB/ZOrderLayout.cpp)

QT_WRAP_UI(
  LIB_UI_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.ui
  PixelOrientedOptionsWidget.ui)

QT_WRAP_CPP(
  LIB_MOC_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.h
  PixelOrientedView.h
  PixelOrientedOptionsWidget.h
  PixelOrientedViewQuickAccessBar.h)

QT_ADD_RESOURCES(LIB_RCC_SRCS PixelOrientedView.qrc)

TALIPOT_ADD_PLUGIN(
  NAME
  PixelOrientedView
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
