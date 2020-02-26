INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude}
                    ${CMAKE_CURRENT_BINARY_DIR})

ADD_SUBDIRECTORY(Delaunay)

# ------------------------------------------------------------------------------
SET(StrengthClustering_SRCS StrengthClustering.h StrengthClustering.cpp)

ADD_LIBRARY(StrengthClustering-${TalipotVersion} SHARED
            ${StrengthClustering_SRCS})
TARGET_LINK_LIBRARIES(StrengthClustering-${TalipotVersion}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(HierarchicalClustering_SRCS HierarchicalClustering.cpp
                                HierarchicalClustering.h)

ADD_LIBRARY(HierarchicalClustering-${TalipotVersion} SHARED
            ${HierarchicalClustering_SRCS})
TARGET_LINK_LIBRARIES(HierarchicalClustering-${TalipotVersion}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(CliqueEnumeration_SRCS CliqueEnumeration.cpp CliqueEnumeration.h)

ADD_LIBRARY(CliqueEnumeration-${TalipotVersion} SHARED
            ${CliqueEnumeration_SRCS})
TARGET_LINK_LIBRARIES(CliqueEnumeration-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
IF(NOT TALIPOT_BUILD_CORE_ONLY)
  QTX_SET_INCLUDES_AND_DEFINITIONS()

  QTX_WRAP_UI(ConvolutionClustering_UI_SRCS ConvolutionClusteringSetup.ui)

  QTX_WRAP_CPP(ConvolutionClustering_MOC_SRCS ConvolutionClusteringSetup.h)

  SET(ConvolutionClustering_SRCS
      ConvolutionClustering.h ConvolutionClustering.cpp
      ConvolutionClusteringSetup.cpp ConvolutionClusteringSetup.h)

  ADD_LIBRARY(
    ConvolutionClustering-${TalipotVersion} SHARED
    ${ConvolutionClustering_SRCS} ${ConvolutionClustering_UI_SRCS}
    ${ConvolutionClustering_MOC_SRCS})
  TARGET_LINK_LIBRARIES(ConvolutionClustering-${TalipotVersion}
                        ${LibTalipotCoreName} ${QT_LIBRARIES})
ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)

# ------------------------------------------------------------------------------
SET(EqualValueClustering_SRCS EqualValueClustering.h EqualValueClustering.cpp)

ADD_LIBRARY(EqualValueClustering-${TalipotVersion} SHARED
            ${EqualValueClustering_SRCS})
TARGET_LINK_LIBRARIES(EqualValueClustering-${TalipotVersion}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
SET(QuotientClustering_SRCS QuotientClustering.cpp)

ADD_LIBRARY(QuotientClustering-${TalipotVersion} SHARED
            ${QuotientClustering_SRCS})
TARGET_LINK_LIBRARIES(QuotientClustering-${TalipotVersion}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
TALIPOT_INSTALL_PLUGIN(StrengthClustering-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(HierarchicalClustering-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
IF(NOT TALIPOT_BUILD_CORE_ONLY)
  TALIPOT_INSTALL_PLUGIN(ConvolutionClustering-${TalipotVersion}
                         ${TalipotPluginsInstallDir})
ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)
TALIPOT_INSTALL_PLUGIN(EqualValueClustering-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(QuotientClustering-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(CliqueEnumeration-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
