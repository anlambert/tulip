INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude}
                    ${TalipotOGLInclude})

# ------------------------------------------------------------------------------
ADD_LIBRARY(MetricSizeMapping-${TalipotVersion} SHARED SizeMapping.cpp)
TARGET_LINK_LIBRARIES(MetricSizeMapping-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(AutoSize-${TalipotVersion} SHARED AutoSize.cpp)
TARGET_LINK_LIBRARIES(AutoSize-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
TALIPOT_INSTALL_PLUGIN(MetricSizeMapping-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(AutoSize-${TalipotVersion} ${TalipotPluginsInstallDir})
