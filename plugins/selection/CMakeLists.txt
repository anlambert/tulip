INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

# ------------------------------------------------------------------------------
ADD_LIBRARY(SpanningDagSelection-${TalipotVersion} SHARED
            SpanningDagSelection.cpp SpanningDagSelection.h)
TARGET_LINK_LIBRARIES(SpanningDagSelection-${TalipotVersion}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(ReachableSubGraphSelection-${TalipotVersion} SHARED
            ReachableSubGraphSelection.h ReachableSubGraphSelection.cpp)
TARGET_LINK_LIBRARIES(ReachableSubGraphSelection-${TalipotVersion}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(SpanningTreeSelection-${TalipotVersion} SHARED
            SpanningTreeSelection.cpp SpanningTreeSelection.h)
TARGET_LINK_LIBRARIES(SpanningTreeSelection-${TalipotVersion}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(InducedSubGraphSelection-${TalipotVersion} SHARED
            InducedSubGraphSelection.cpp InducedSubGraphSelection.h)
TARGET_LINK_LIBRARIES(InducedSubGraphSelection-${TalipotVersion}
                      ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(LoopSelection-${TalipotVersion} SHARED LoopSelection.cpp
                                                   LoopSelection.h)
TARGET_LINK_LIBRARIES(LoopSelection-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(MultipleSelection-${TalipotVersion} SHARED MultipleEdgeSelection.cpp
                                                       MultipleEdgeSelection.h)
TARGET_LINK_LIBRARIES(MultipleSelection-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(Kruskal-${TalipotVersion} SHARED Kruskal.cpp Kruskal.h)
TARGET_LINK_LIBRARIES(Kruskal-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
ADD_LIBRARY(MakeGraph-${TalipotVersion} SHARED MakeSelectionGraph.cpp
                                               MakeSelectionGraph.h)
TARGET_LINK_LIBRARIES(MakeGraph-${TalipotVersion} ${LibTalipotCoreName})

# ------------------------------------------------------------------------------
TALIPOT_INSTALL_PLUGIN(SpanningDagSelection-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(ReachableSubGraphSelection-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(SpanningTreeSelection-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(InducedSubGraphSelection-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(LoopSelection-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(MultipleSelection-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(Kruskal-${TalipotVersion} ${TalipotPluginsInstallDir})
TALIPOT_INSTALL_PLUGIN(MakeGraph-${TalipotVersion} ${TalipotPluginsInstallDir})
