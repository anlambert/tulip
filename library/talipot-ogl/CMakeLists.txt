IF(WIN32)
  TALIPOT_SET_CXX_FLAGS("-DDLL_TALIPOT_GL")
ENDIF(WIN32)

ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(textures)

INSTALL(DIRECTORY fonts DESTINATION ${TalipotShareInstallDir})

# Install fontconfig configuration files on Windows / MacOS
IF(WIN32 OR APPLE)
  # MSYS2 / Apple
  IF(EXISTS ${FONTCONFIG_INCLUDE_DIR}/../etc/fonts)
    INSTALL(DIRECTORY ${FONTCONFIG_INCLUDE_DIR}/../etc/fonts
            DESTINATION ${TalipotBinInstallDir}/../etc)
    # MSVC
  ELSEIF(EXISTS ${FONTCONFIG_INCLUDE_DIR}/../tools/fontconfig)
    INSTALL(DIRECTORY ${FONTCONFIG_INCLUDE_DIR}/../tools/fontconfig/
            DESTINATION ${TalipotBinInstallDir}/fonts)
  ENDIF(EXISTS ${FONTCONFIG_INCLUDE_DIR}/../etc/fonts)
ENDIF(WIN32 OR APPLE)
