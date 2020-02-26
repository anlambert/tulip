INCLUDE_DIRECTORIES(
  ${TalipotCoreBuildInclude}
  ${TalipotCoreInclude}
  ${TalipotOGLInclude}
  ${TalipotGUIInclude}
  ${TalipotUIGUIInclude}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GLEW_INCLUDE_DIR})

QTX_SET_INCLUDES_AND_DEFINITIONS()
QTXWEB_SET_INCLUDES_AND_DEFINITIONS()

SET(LIB_SRCS
    GeographicView.cpp
    GeographicViewConfigWidget.cpp
    GeolocalisationConfigWidget.cpp
    ProgressWidgetGraphicsProxy.cpp
    GeographicViewInteractors.cpp
    AddressSelectionDialog.cpp
    LeafletMaps.cpp
    GeographicViewGraphicsView.cpp
    GeographicViewShowElementInfo.cpp
    NominatimGeocoder.cpp)

QTX_WRAP_UI(
  LIB_UI_HEADERS GeolocalisationConfigWidget.ui GeographicViewConfigWidget.ui
  ProgressWidget.ui AddressSelectionDialog.ui)

QTX_ADD_RESOURCES(LIB_RCC_SRCS GeographicViewResource.qrc)

QTX_WRAP_CPP(
  LIB_MOC_SRCS
  GeographicView.h
  GeographicViewGraphicsView.h
  ProgressWidgetGraphicsProxy.h
  GeographicViewConfigWidget.h
  GeographicViewShowElementInfo.h
  GeolocalisationConfigWidget.h
  LeafletMaps.h)

IF(WIN32)
  SET(GEOVIEW_DLL "GeographicView-${TalipotVersion}.dll")
  IF(NOT MSVC)
    SET(GEOVIEW_DLL "lib${GEOVIEW_DLL}")
  ENDIF(NOT MSVC)
  # update the list of bundle libs
  SET_PROPERTY(
    GLOBAL APPEND
    PROPERTY FIXUP_BUNDLE_LIBS
             "${CMAKE_INSTALL_PREFIX}/lib/talipot/view/${GEOVIEW_DLL}")
ENDIF(WIN32)

ADD_LIBRARY(GeographicView-${TalipotVersion} SHARED
            ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})

TARGET_LINK_LIBRARIES(
  GeographicView-${TalipotVersion}
  ${LibTalipotCoreName}
  ${LibTalipotOGLName}
  ${LibTalipotGUIName}
  ${OPENGL_gl_LIBRARY}
  ${QT_LIBRARIES}
  ${QT_WEB_LIBRARIES})

TALIPOT_INSTALL_PLUGIN(GeographicView-${TalipotVersion}
                       ${TalipotViewPluginsInstallDir})
