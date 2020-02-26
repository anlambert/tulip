INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude})

ADD_LIBRARY(AttractAndIntroduce-${TalipotVersion} SHARED
            AttractAndIntroduce.cpp)
TARGET_LINK_LIBRARIES(AttractAndIntroduce-${TalipotVersion}
                      ${LibTalipotCoreName})
TALIPOT_INSTALL_PLUGIN(AttractAndIntroduce-${TalipotVersion}
                       ${TalipotPluginsInstallDir})

ADD_LIBRARY(Bollobas-${TalipotVersion} SHARED BollobasModel.cpp)
TARGET_LINK_LIBRARIES(Bollobas-${TalipotVersion} ${LibTalipotCoreName})
TALIPOT_INSTALL_PLUGIN(Bollobas-${TalipotVersion} ${TalipotPluginsInstallDir})

ADD_LIBRARY(BuWangZhou-${TalipotVersion} SHARED BuWangZhouModel.cpp)
TARGET_LINK_LIBRARIES(BuWangZhou-${TalipotVersion} ${LibTalipotCoreName})
TALIPOT_INSTALL_PLUGIN(BuWangZhou-${TalipotVersion} ${TalipotPluginsInstallDir})

ADD_LIBRARY(Cantazaro-${TalipotVersion} SHARED Catanzaro.cpp)
TARGET_LINK_LIBRARIES(Cantazaro-${TalipotVersion} ${LibTalipotCoreName})
TALIPOT_INSTALL_PLUGIN(Cantazaro-${TalipotVersion} ${TalipotPluginsInstallDir})

ADD_LIBRARY(Fuliao-${TalipotVersion} SHARED FuLiao.cpp)
TARGET_LINK_LIBRARIES(Fuliao-${TalipotVersion} ${LibTalipotCoreName})
TALIPOT_INSTALL_PLUGIN(Fuliao-${TalipotVersion} ${TalipotPluginsInstallDir})

ADD_LIBRARY(GuillaumeLatapy-${TalipotVersion} SHARED GuillaumeLatapyModel.cpp)
TARGET_LINK_LIBRARIES(GuillaumeLatapy-${TalipotVersion} ${LibTalipotCoreName})
TALIPOT_INSTALL_PLUGIN(GuillaumeLatapy-${TalipotVersion}
                       ${TalipotPluginsInstallDir})

ADD_LIBRARY(HolmeKim-${TalipotVersion} SHARED HolmeKim.cpp)
TARGET_LINK_LIBRARIES(HolmeKim-${TalipotVersion} ${LibTalipotCoreName})
TALIPOT_INSTALL_PLUGIN(HolmeKim-${TalipotVersion} ${TalipotPluginsInstallDir})

ADD_LIBRARY(KlemmEguiluz-${TalipotVersion} SHARED KlemmEguiluzModel.cpp)
TARGET_LINK_LIBRARIES(KlemmEguiluz-${TalipotVersion} ${LibTalipotCoreName})
TALIPOT_INSTALL_PLUGIN(KlemmEguiluz-${TalipotVersion}
                       ${TalipotPluginsInstallDir})

ADD_LIBRARY(LiuEtAl-${TalipotVersion} SHARED LiuEtAl.cpp)
TARGET_LINK_LIBRARIES(LiuEtAl-${TalipotVersion} ${LibTalipotCoreName})
TALIPOT_INSTALL_PLUGIN(LiuEtAl-${TalipotVersion} ${TalipotPluginsInstallDir})

ADD_LIBRARY(WangEtAl-${TalipotVersion} SHARED WangEtAl.cpp)
TARGET_LINK_LIBRARIES(WangEtAl-${TalipotVersion} ${LibTalipotCoreName})
TALIPOT_INSTALL_PLUGIN(WangEtAl-${TalipotVersion} ${TalipotPluginsInstallDir})

ADD_LIBRARY(WangRong-${TalipotVersion} SHARED WangRong.cpp)
TARGET_LINK_LIBRARIES(WangRong-${TalipotVersion} ${LibTalipotCoreName})
TALIPOT_INSTALL_PLUGIN(WangRong-${TalipotVersion} ${TalipotPluginsInstallDir})

ADD_LIBRARY(WattsStrotgatz-${TalipotVersion} SHARED WattsStrotgatzModel.cpp)
TARGET_LINK_LIBRARIES(WattsStrotgatz-${TalipotVersion} ${LibTalipotCoreName})
TALIPOT_INSTALL_PLUGIN(WattsStrotgatz-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
