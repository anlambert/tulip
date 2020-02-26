INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude}
                    ${CMAKE_CURRENT_BINARY_DIR})

# ------------------------------------------------------------------------------
SET(Planarity_SRCS Planarity.cpp)

ADD_LIBRARY(Planarity-${TalipotVersion} SHARED ${Planarity_SRCS})
TARGET_LINK_LIBRARIES(Planarity-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(Planarity-${TalipotVersion} ${TalipotPluginsInstallDir})

# ------------------------------------------------------------------------------
SET(Simple_SRCS Simple.cpp)

ADD_LIBRARY(Simple-${TalipotVersion} SHARED ${Simple_SRCS})
TARGET_LINK_LIBRARIES(Simple-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(Simple-${TalipotVersion} ${TalipotPluginsInstallDir})

# ------------------------------------------------------------------------------
SET(Tree_SRCS Tree.cpp)

ADD_LIBRARY(Tree-${TalipotVersion} SHARED ${Tree_SRCS})
TARGET_LINK_LIBRARIES(Tree-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(Tree-${TalipotVersion} ${TalipotPluginsInstallDir})

# ------------------------------------------------------------------------------
SET(Connected_SRCS Connected.cpp)

ADD_LIBRARY(Connected-${TalipotVersion} SHARED ${Connected_SRCS})
TARGET_LINK_LIBRARIES(Connected-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(Connected-${TalipotVersion} ${TalipotPluginsInstallDir})

# ------------------------------------------------------------------------------
SET(Biconnected_SRCS Biconnected.cpp)

ADD_LIBRARY(Biconnected-${TalipotVersion} SHARED ${Biconnected_SRCS})
TARGET_LINK_LIBRARIES(Biconnected-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(Biconnected-${TalipotVersion}
                       ${TalipotPluginsInstallDir})

# ------------------------------------------------------------------------------
SET(Triconnected_SRCS Triconnected.cpp)

ADD_LIBRARY(Triconnected-${TalipotVersion} SHARED ${Triconnected_SRCS})
TARGET_LINK_LIBRARIES(Triconnected-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(Triconnected-${TalipotVersion}
                       ${TalipotPluginsInstallDir})

# ------------------------------------------------------------------------------
SET(Outerplanar_SRCS Outerplanar.cpp)

ADD_LIBRARY(Outerplanar-${TalipotVersion} SHARED ${Outerplanar_SRCS})
TARGET_LINK_LIBRARIES(Outerplanar-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(Outerplanar-${TalipotVersion}
                       ${TalipotPluginsInstallDir})

# ------------------------------------------------------------------------------
SET(Acyclic_SRCS Acyclic.cpp)

ADD_LIBRARY(Acyclic-${TalipotVersion} SHARED ${Acyclic_SRCS})
TARGET_LINK_LIBRARIES(Acyclic-${TalipotVersion} ${LibTalipotCoreName})

TALIPOT_INSTALL_PLUGIN(Acyclic-${TalipotVersion} ${TalipotPluginsInstallDir})
