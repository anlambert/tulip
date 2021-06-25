INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude}
  ${TalipotGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

SET(LIB_SRCS TableView.cpp PropertiesEditor.cpp NavigableTableView.cpp)

QT_WRAP_UI(LIB_UI_HEADERS TableView.ui PropertiesEditor.ui)

QT_WRAP_CPP(LIB_MOC_SRCS TableView.h PropertiesEditor.h NavigableTableView.h)

QT_ADD_RESOURCES(LIB_RCC_SRCS TableView.qrc)

TALIPOT_ADD_PLUGIN(
  NAME
  TableView
  SRCS
  ${LIB_SRCS}
  ${LIB_UI_HEADERS}
  ${LIB_MOC_SRCS}
  ${LIB_RCC_SRCS}
  LINKS
  ${LibTalipotCoreName}
  ${LibTalipotOGLName}
  ${LibTalipotGUIName}
  INSTALL_DIR
  ${TalipotViewPluginsInstallDir})
