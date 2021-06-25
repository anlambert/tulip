INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude}
                    ${CMAKE_CURRENT_BINARY_DIR})

ADD_SUBDIRECTORY(Delaunay)

# ------------------------------------------------------------------------------
TALIPOT_ADD_PLUGIN(
  NAME
  StrengthClustering
  SRCS
  StrengthClustering.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
# ------------------------------------------------------------------------------
TALIPOT_ADD_PLUGIN(
  NAME
  NAMEHierarchicalClustering
  SRCS
  HierarchicalClustering.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
# ------------------------------------------------------------------------------
TALIPOT_ADD_PLUGIN(
  NAME
  CliqueEnumeration
  SRCS
  CliqueEnumeration.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
# ------------------------------------------------------------------------------
IF(NOT TALIPOT_BUILD_CORE_ONLY)
  QTX_SET_INCLUDES_AND_DEFINITIONS()

  QT_WRAP_UI(ConvolutionClustering_UI_SRCS ConvolutionClusteringSetup.ui)

  QT_WRAP_CPP(ConvolutionClustering_MOC_SRCS ConvolutionClusteringSetup.h)

  SET(ConvolutionClustering_SRCS ConvolutionClustering.cpp
                                 ConvolutionClusteringSetup.cpp)

  TALIPOT_ADD_PLUGIN(
    NAME
    ConvolutionClustering
    SRCS
    ${ConvolutionClustering_SRCS}
    ${ConvolutionClustering_UI_SRCS}
    ${ConvolutionClustering_MOC_SRCS}
    LINKS
    ${LibTalipotCoreName}
    ${QT_LIBRARIES}
    INSTALL_DIR
    ${TalipotPluginsInstallDir})
ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)
# ------------------------------------------------------------------------------
TALIPOT_ADD_PLUGIN(
  NAME
  EqualValueClustering
  SRCS
  EqualValueClustering.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
# ------------------------------------------------------------------------------
TALIPOT_ADD_PLUGIN(
  NAME
  QuotientClustering
  SRCS
  QuotientClustering.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
# ------------------------------------------------------------------------------
