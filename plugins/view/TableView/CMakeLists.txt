INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude}
  ${TalipotGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

SET(LIB_SRCS TableView.cpp PropertiesEditor.cpp NavigableTableView.cpp)

QTX_WRAP_UI(UI_SRCS TableView.ui PropertiesEditor.ui)

QTX_WRAP_CPP(MOC_SRCS TableView.h PropertiesEditor.h NavigableTableView.h)

QTX_ADD_RESOURCES(RCC_SRCS TableView.qrc)

ADD_LIBRARY(TableView-${TalipotVersion} SHARED ${LIB_SRCS} ${MOC_SRCS}
                                               ${UI_SRCS} ${RCC_SRCS})

TARGET_LINK_LIBRARIES(TableView-${TalipotVersion} ${LibTalipotCoreName}
                      ${LibTalipotOGLName} ${LibTalipotGUIName})

TALIPOT_INSTALL_PLUGIN(TableView-${TalipotVersion}
                       ${TalipotViewPluginsInstallDir})
