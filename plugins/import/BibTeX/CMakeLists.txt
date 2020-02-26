INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty ${TalipotCoreBuildInclude}
                    ${TalipotCoreInclude})

FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(BibTeXImport-${TalipotVersion} SHARED ${LIB_SRCS} ${RCC_SRCS})
TARGET_LINK_LIBRARIES(BibTeXImport-${TalipotVersion} ${LibTalipotCoreName}
                      xdkbibtex)

TALIPOT_INSTALL_PLUGIN(BibTeXImport-${TalipotVersion}
                       ${TalipotPluginsInstallDir})
