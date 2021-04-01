INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty ${TalipotCoreBuildInclude}
                    ${TalipotCoreInclude})

TALIPOT_ADD_PLUGIN(
  NAME
  BibTeXImport
  SRCS
  ImportBibTeX.cpp
  ForceUtf8String.cpp
  NormalizeString.cpp
  LINKS
  ${LibTalipotCoreName}
  xdkbibtex
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
