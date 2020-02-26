INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude}
  ${TalipotGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} .)

# ------------------------------------------------------------------------------
IF(NOT TALIPOT_BUILD_CORE_ONLY)

  QTX_SET_INCLUDES_AND_DEFINITIONS()

  SET(ColorMapping_SRCS ColorMapping.cpp DoubleStringsListRelationDialog.cpp)

  QTX_WRAP_UI(ColorMapping_UI_SRCS DoubleStringsListRelationDialog.ui)

  QTX_WRAP_CPP(ColorMapping_MOC_SRCS DoubleStringsListRelationDialog.h)

  ADD_LIBRARY(
    ColorMapping-${TalipotVersion} SHARED
    ${ColorMapping_SRCS} ${ColorMapping_UI_SRCS} ${ColorMapping_MOC_SRCS})
  TARGET_LINK_LIBRARIES(ColorMapping-${TalipotVersion} ${LibTalipotCoreName}
                        ${LibTalipotGUIName})

ELSE(NOT TALIPOT_BUILD_CORE_ONLY)

  SET(ColorMapping_SRCS ColorMapping.cpp)

  SET_SOURCE_FILES_PROPERTIES(
    ColorMapping.cpp PROPERTIES COMPILE_DEFINITIONS TALIPOT_BUILD_CORE_ONLY)

  ADD_LIBRARY(ColorMapping-${TalipotVersion} SHARED ${ColorMapping_SRCS})
  TARGET_LINK_LIBRARIES(ColorMapping-${TalipotVersion} ${LibTalipotCoreName})

ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)

# ------------------------------------------------------------------------------
TALIPOT_INSTALL_PLUGIN(ColorMapping-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
