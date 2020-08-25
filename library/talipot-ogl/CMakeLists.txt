IF(WIN32)
  TALIPOT_SET_CXX_FLAGS("-DDLL_TALIPOT_GL")
ENDIF(WIN32)

ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(textures)

INSTALL(DIRECTORY fonts DESTINATION ${TalipotShareInstallDir})

IF(WIN32 OR APPLE)
  # Install fontconfig configuration files on Windows / MacOS
  IF(EXISTS ${FONTCONFIG_INCLUDE_DIR}/../etc/fonts)
    # MSYS2 / MacPorts
    INSTALL(DIRECTORY ${FONTCONFIG_INCLUDE_DIR}/../etc/fonts
            DESTINATION ${TalipotBinInstallDir}/../etc)
  ELSEIF(EXISTS ${FONTCONFIG_INCLUDE_DIR}/../tools/fontconfig)
    # MSVC vcpkg
    INSTALL(DIRECTORY ${FONTCONFIG_INCLUDE_DIR}/../tools/fontconfig/
            DESTINATION ${TalipotBinInstallDir}/fonts)
  ENDIF(EXISTS ${FONTCONFIG_INCLUDE_DIR}/../etc/fonts)
ENDIF(WIN32 OR APPLE)
