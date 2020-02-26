INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

ADD_LIBRARY(ToLabels-${TalipotVersion} SHARED ToLabels.cpp)

TARGET_LINK_LIBRARIES(ToLabels-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(ToLabels-${TalipotVersion} ${TalipotPluginsInstallDir})
