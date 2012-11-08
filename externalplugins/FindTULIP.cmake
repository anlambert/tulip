SET(TULIP_DIR ${CMAKE_INSTALL_PREFIX})
SET(TULIP_FOUND true)
SET(TULIP_INCLUDE_DIR ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipOGDFInclude} ${TulipGUIInclude})
SET(TULIP_LIBRARIES ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName} ${LibTulipOGDFName})
IF(WIN32)
  SET(TULIP_LIBRARIES_DIR ${CMAKE_INSTALL_PREFIX}/bin)
ELSEIF(APPLE)
  SET(TULIP_LIBRARIES_DIR ${CMAKE_INSTALL_PREFIX}/Frameworks)
ELSE()
  SET(TULIP_LIBRARIES_DIR ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()

SET(TULIP_PLUGINS_DIR ${TulipPluginsInstallDir})
SET(TULIP_GLYPH_PLUGINS_DIR ${TulipGlyphPluginsInstallDir})
SET(TULIP_INTERACTOR_PLUGINS_DIR ${TulipInteractorPluginsInstallDir})
SET(TULIP_VIEW_PLUGINS_DIR ${TulipViewPluginsInstallDir})
SET(TULIP_PERSPECTIVE_PLUGINS_DIR ${TulipPerspectivePluginsInstallDir})

SET(TULIP_CORE_LIBRARY ${LibTulipCoreName})
SET(TULIP_OGL_LIBRARY ${LibTulipOglName})
SET(TULIP_GUI_LIBRARY ${LibTulipGUIName})
SET(TULIP_OGDF_LIBRARY ${LibTulipOGDFName})
SET(TULIP_VERSION ${TulipVersion})
SET(TULIP_MAJOR_VERSION ${TulipMajorVersion})
SET(TULIP_MINOR_VERSION ${TulipMinorVersion})
SET(TULIP_PATCH_VERSION ${TulipReleaseVersion})
set(TULIP_SHARE_DIR ${TulipShareInstallDir})
SET(TULIP_USE_FILE ${CMAKE_SOURCE_DIR}/TulipUseFile.cmake)
