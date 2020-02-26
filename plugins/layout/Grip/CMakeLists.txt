INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

SET(LIB_SRCS Distances.cpp Grip.cpp MISFiltering.cpp)

ADD_LIBRARY(Grip-${TalipotVersion} SHARED ${LIB_SRCS})

TARGET_LINK_LIBRARIES(Grip-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(Grip-${TalipotVersion} ${TalipotPluginsInstallDir})
