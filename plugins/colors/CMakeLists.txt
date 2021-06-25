INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude}
  ${TalipotGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} .)

IF(NOT TALIPOT_BUILD_CORE_ONLY)
  QTX_SET_INCLUDES_AND_DEFINITIONS()

  QT_WRAP_UI(ColorMapping_UI_SRCS DoubleStringsListRelationDialog.ui)

  QT_WRAP_CPP(ColorMapping_MOC_SRCS DoubleStringsListRelationDialog.h)

  SET(ColorMapping_SRCS ColorMapping.cpp DoubleStringsListRelationDialog.cpp
                        ${ColorMapping_UI_SRCS} ${ColorMapping_MOC_SRCS})

  SET(ColorMapping_LINKS ${LibTalipotCoreName} ${LibTalipotGUIName})
ELSE(NOT TALIPOT_BUILD_CORE_ONLY)
  SET(ColorMapping_SRCS ColorMapping.cpp)

  SET_SOURCE_FILES_PROPERTIES(
    ColorMapping.cpp PROPERTIES COMPILE_DEFINITIONS TALIPOT_BUILD_CORE_ONLY)

  SET(ColorMapping_LINKS ${LibTalipotCoreName})
ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)

TALIPOT_ADD_PLUGIN(
  NAME
  ColorMapping
  SRCS
  ${ColorMapping_SRCS}
  LINKS
  ${ColorMapping_LINKS}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
