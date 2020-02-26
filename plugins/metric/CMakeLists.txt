INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

# ------------------------------------------------------------------------------
ADD_LIBRARY(Eccentricity-${TalipotVersion} SHARED Eccentricity.h
                                                  Eccentricity.cpp)
TARGET_LINK_LIBRARIES(Eccentricity-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(DegreeMetric-${TalipotVersion} SHARED DegreeMetric.cpp
                                                  DegreeMetric.h)
TARGET_LINK_LIBRARIES(DegreeMetric-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(ClusterMetric-${TalipotVersion} SHARED ClusterMetric.cpp
                                                   ClusterMetric.h)
TARGET_LINK_LIBRARIES(ClusterMetric-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(StrengthMetric-${TalipotVersion} SHARED StrengthMetric.cpp
                                                    StrengthMetric.h)
TARGET_LINK_LIBRARIES(StrengthMetric-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(BiconnectedComponent-${TalipotVersion} SHARED
            BiconnectedComponent.cpp)
TARGET_LINK_LIBRARIES(BiconnectedComponent-${TalipotVersion}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(ConnectedComponent-${TalipotVersion} SHARED ConnectedComponent.cpp
                                                        ConnectedComponent.h)
TARGET_LINK_LIBRARIES(ConnectedComponent-${TalipotVersion}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(StrongComponent-${TalipotVersion} SHARED StrongComponent.cpp
                                                     StrongComponent.h)
TARGET_LINK_LIBRARIES(StrongComponent-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(DagLevelMetric-${TalipotVersion} SHARED DagLevelMetric.cpp
                                                    DagLevelMetric.h)
TARGET_LINK_LIBRARIES(DagLevelMetric-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(IdMetric-${TalipotVersion} SHARED IdMetric.cpp IdMetric.h)
TARGET_LINK_LIBRARIES(IdMetric-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(LeafMetric-${TalipotVersion} SHARED LeafMetric.cpp LeafMetric.h)
TARGET_LINK_LIBRARIES(LeafMetric-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(NodeMetric-${TalipotVersion} SHARED NodeMetric.cpp NodeMetric.h)
TARGET_LINK_LIBRARIES(NodeMetric-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(DepthMetric-${TalipotVersion} SHARED DepthMetric.cpp DepthMetric.h)
TARGET_LINK_LIBRARIES(DepthMetric-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(PathLengthMetric-${TalipotVersion} SHARED PathLengthMetric.cpp
                                                      PathLengthMetric.h)
TARGET_LINK_LIBRARIES(PathLengthMetric-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(StrahlerMetric-${TalipotVersion} SHARED StrahlerMetric.cpp
                                                    StrahlerMetric.h)
TARGET_LINK_LIBRARIES(StrahlerMetric-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(RandomMetric-${TalipotVersion} SHARED Random.cpp Random.h)
TARGET_LINK_LIBRARIES(RandomMetric-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(BetweennessCentrality-${TalipotVersion} SHARED
            BetweennessCentrality.cpp)
TARGET_LINK_LIBRARIES(BetweennessCentrality-${TalipotVersion}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(KCores-${TalipotVersion} SHARED KCores.cpp)
TARGET_LINK_LIBRARIES(KCores-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(LouvainClustering-${TalipotVersion} SHARED LouvainClustering.cpp)
TARGET_LINK_LIBRARIES(LouvainClustering-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(LinkCommunities-${TalipotVersion} SHARED LinkCommunities.cpp)
TARGET_LINK_LIBRARIES(LinkCommunities-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(MCLClustering-${TalipotVersion} SHARED MCLClustering.cpp)
TARGET_LINK_LIBRARIES(MCLClustering-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(PageRank-${TalipotVersion} SHARED PageRank.cpp)
TARGET_LINK_LIBRARIES(PageRank-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(WelshPowell-${TalipotVersion} SHARED WelshPowell.cpp)
TARGET_LINK_LIBRARIES(WelshPowell-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
TALIPOT_INSTALL_PLUGIN(Eccentricity-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(DegreeMetric-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(ClusterMetric-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(StrengthMetric-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(BiconnectedComponent-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(ConnectedComponent-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(StrongComponent-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(DagLevelMetric-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(IdMetric-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(LeafMetric-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(NodeMetric-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(DepthMetric-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(PathLengthMetric-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(StrahlerMetric-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(RandomMetric-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(BetweennessCentrality-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(KCores-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(LouvainClustering-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(LinkCommunities-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(MCLClustering-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(PageRank-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(WelshPowell-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
