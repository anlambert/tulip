INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty ${TalipotCoreBuildInclude}
                    ${TalipotCoreInclude})

TALIPOT_ADD_PLUGIN(
  NAME
  BibTeXImport
  SRCS
  ImportBibTeX.cpp
  LINKS
  ${LibTalipotCoreName}
  xdkbibtex
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
