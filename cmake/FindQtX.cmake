# CMake module to find required Qt5 or Qt6 modules for building Talipot. Qt5 is
# used by default, set TALIPOT_USE_QT6 CMake variable to ON to use Qt6.

# If CMake does not automatically find QtX, the root directory of the QtX
# installation must be provided in the CMAKE_PREFIX_PATH variable.

SET(QT_MAJOR_VERSION 5)
SET(QT_MIN_VERSION 5.7)
IF(TALIPOT_USE_QT6)
  SET(QT_MAJOR_VERSION 6)
  SET(QT_MIN_VERSION 6.2)
ENDIF(TALIPOT_USE_QT6)

# Unset related CMake variables in order to change the QtX version (by modifying
# the root QtX directory through the CMAKE_PREFIX_PATH variable) without having
# to delete the current CMake cache
UNSET(Qt${QT_MAJOR_VERSION}Core_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Gui_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Widgets_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Xml_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}XmlPatterns_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Network_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}WebKit_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}WebKitWidgets_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Multimedia_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}MultimediaWidgets_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Positioning_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}PrintSupport_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Qml_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Quick_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Sensors_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}DBus_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}WebEngineWidgets_FOUND CACHE)
UNSET(Qt${QT_MAJOR_VERSION}WebChannel_FOUND CACHE)
IF(QT_MAJOR_VERSION EQUAL 6)
  UNSET(Qt6OpenGLWidgets_FOUND CACHE)
ENDIF(QT_MAJOR_VERSION EQUAL 6)

UNSET(Qt${QT_MAJOR_VERSION}Core_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Gui_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Widgets_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Xml_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}XmlPatterns_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Network_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}WebKit_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}WebKitWidgets_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Multimedia_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}MultimediaWidgets_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Positioning_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}PrintSupport_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Qml_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Quick_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}Sensors_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}DBus_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}WebEngineWidgets_DIR CACHE)
UNSET(Qt${QT_MAJOR_VERSION}WebChannel_DIR CACHE)
IF(QT_MAJOR_VERSION EQUAL 6)
  UNSET(Qt6OpenGLWidgets_DIR CACHE)
ENDIF(QT_MAJOR_VERSION EQUAL 6)

SET(QT_HAS_WEBKIT FALSE)
SET(QT_HAS_WEBENGINE FALSE)

# Macro used to workaround a small issue with QtWebkit components on MSYS2: when
# compiling in RelWithDebInfo mode, Qt debug libraries are selected instead of
# the release one (this should only happen when compiling in Debug mode)
MACRO(SETUP_QT_LIBRARIES QtModule LIBRARIES)
  IF(MINGW)
    GET_TARGET_PROPERTY(
      Qt${QT_MAJOR_VERSION}${QtModule}_INCLUDE_DIRS
      Qt${QT_MAJOR_VERSION}::${QtModule} INTERFACE_INCLUDE_DIRECTORIES)
    IF(CMAKE_DEBUG_MODE)
      GET_TARGET_PROPERTY(Qt${QT_MAJOR_VERSION}${QtModule}_LIBRARIES
                          Qt${QT_MAJOR_VERSION}::${QtModule} LOCATION_DEBUG)
    ELSE(CMAKE_DEBUG_MODE)
      GET_TARGET_PROPERTY(Qt${QT_MAJOR_VERSION}${QtModule}_LIBRARIES
                          Qt${QT_MAJOR_VERSION}::${QtModule} LOCATION_RELEASE)
    ENDIF(CMAKE_DEBUG_MODE)
  ENDIF(MINGW)
  SET(${LIBRARIES}
      ${${LIBRARIES}} ${Qt${QT_MAJOR_VERSION}${QtModule}_LIBRARIES}
      ${Qt${QT_MAJOR_VERSION}${QtModule}_LIBRARIES})
ENDMACRO(SETUP_QT_LIBRARIES)

FIND_PACKAGE(Qt${QT_MAJOR_VERSION}Widgets ${QT_MIN_VERSION} REQUIRED)
FIND_PACKAGE(Qt${QT_MAJOR_VERSION}Network ${QT_MIN_VERSION} REQUIRED)
IF(QT_MAJOR_VERSION EQUAL 6)
  FIND_PACKAGE(Qt6OpenGLWidgets ${QT_MIN_VERSION} REQUIRED)
ENDIF(QT_MAJOR_VERSION EQUAL 6)

STRING(REGEX MATCH "[0-9]\\.[0-9]+\\.[0-9]+" QT_VERSION
             "${Qt${QT_MAJOR_VERSION}Widgets_VERSION_STRING}")
# QtXWidgets_VERSION_STRING has been deprecated in favor of QtXWidgets_VERSION
# since a few releases of Qt5 and seems to have been removed in some Linux
# distributions (experienced on KDE Neon Developer edition that now uses Qt 5.9)
IF(NOT QT_VERSION MATCHES "[0-9]\\.[0-9]+\\.[0-9]+")
  STRING(REGEX MATCH "[0-9]\\.[0-9]+" QT_VERSION
               "${Qt${QT_MAJOR_VERSION}Widgets_VERSION}")
ENDIF(NOT QT_VERSION MATCHES "[0-9]\\.[0-9]+\\.[0-9]+")

IF(NOT "${QT_VERSION}" STREQUAL "${LAST_FOUND_QT_VERSION}")
  MESSAGE(STATUS "Found Qt${QT_MAJOR_VERSION} (version ${QT_VERSION})")
ENDIF(NOT "${QT_VERSION}" STREQUAL "${LAST_FOUND_QT_VERSION}")

SET(LAST_FOUND_QT_VERSION
    "${QT_VERSION}"
    CACHE INTERNAL "")

IF(WIN32)
  GET_TARGET_PROPERTY(QtCore_location Qt${QT_MAJOR_VERSION}::Core LOCATION)
  GET_FILENAME_COMPONENT(QT_BINARY_DIR ${QtCore_location} PATH)
  SET(QT_BINARY_DIR ${QT_BINARY_DIR}/../bin)
  # Standard QtX installation
  IF(EXISTS ${QT_BINARY_DIR}/../plugins)
    SET(QT_PLUGINS_DIR ${QT_BINARY_DIR}/../plugins)
    # MSYS2 special case
  ELSEIF(EXISTS ${QT_BINARY_DIR}/../share/qt5/plugins)
    SET(QT_PLUGINS_DIR ${QT_BINARY_DIR}/../share/qt5/plugins)
  ENDIF()
ELSEIF(APPLE)
  GET_TARGET_PROPERTY(QtCore_location Qt${QT_MAJOR_VERSION}::Core LOCATION)
  GET_FILENAME_COMPONENT(QT_FRAMEWORKS_DIR ${QtCore_location} PATH)
  SET(QT_FRAMEWORKS_DIR ${QT_FRAMEWORKS_DIR}/..)
  SET(QT_BINARY_DIR ${QT_FRAMEWORKS_DIR}/../bin)
ELSE()
  IF(EXISTS /usr/lib/x86_64-linux-gnu/qt5/bin)
    SET(QT_BINARY_DIR /usr/lib/x86_64-linux-gnu/qt5/bin)
  ELSEIF(EXISTS /usr/lib/i386-linux-gnu/qt5/bin)
    SET(QT_BINARY_DIR /usr/lib/i386-linux-gnu/qt5/bin)
  ELSEIF(EXISTS /usr/lib64/qt5/bin)
    SET(QT_BINARY_DIR /usr/lib64/qt5/bin)
  ELSE()
    SET(QT_BINARY_DIR "${_qt5Gui_install_prefix}/bin")
  ENDIF()
  IF(EXISTS ${QT_BINARY_DIR}/qmake)
    SET(QT_QMAKE_EXECUTABLE ${QT_BINARY_DIR}/qmake)
  ELSEIF(EXISTS ${QT_BINARY_DIR}/qmake-qt5)
    SET(QT_QMAKE_EXECUTABLE ${QT_BINARY_DIR}/qmake-qt5)
  ENDIF()
  # Standard QtX installation
  IF(EXISTS ${QT_BINARY_DIR}/../plugins)
    SET(QT_PLUGINS_DIR ${QT_BINARY_DIR}/../plugins)
  ENDIF()
ENDIF()

SET(QT_LIBRARIES ${Qt${QT_MAJOR_VERSION}Widgets_LIBRARIES}
                 ${Qt${QT_MAJOR_VERSION}Network_LIBRARIES})
IF(QT_MAJOR_VERSION EQUAL 6)
  SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt6OpenGLWidgets_LIBRARIES})
ENDIF(QT_MAJOR_VERSION EQUAL 6)

GET_FILENAME_COMPONENT(QT_CMAKE_DIR "${Qt${QT_MAJOR_VERSION}Core_DIR}"
                       DIRECTORY)

# On Apple platform, we need to link against QtXDBus and QtXPrintSupport when
# using the official QtX bundle provided by qt.io (dylibs dependencies side
# effect). However, those modules are not necessarily present when using Qt5
# from Homebrew or MacPorts, so handle those special cases here.
IF(APPLE)
  SET(QT_DBUS_CMAKE_DIR "${QT_CMAKE_DIR}/Qt${QT_MAJOR_VERSION}DBus")
  SET(QT_PRINTSUPPORT_CMAKE_DIR
      "${QT_CMAKE_DIR}/Qt${QT_MAJOR_VERSION}PrintSupport")
  IF(EXISTS ${QT_DBUS_CMAKE_DIR} AND EXISTS ${QT_PRINTSUPPORT_CMAKE_DIR})
    FIND_PACKAGE(Qt${QT_MAJOR_VERSION}DBus ${QT_MIN_VERSION})
    FIND_PACKAGE(Qt${QT_MAJOR_VERSION}PrintSupport ${QT_MIN_VERSION})
    IF(${Qt${QT_MAJOR_VERSION}DBus_FOUND}
       AND ${Qt${QT_MAJOR_VERSION}PrintSupport_FOUND})
      SET(QT_LIBRARIES
          ${QT_LIBRARIES} ${Qt${QT_MAJOR_VERSION}PrintSupport_LIBRARIES}
          ${Qt${QT_MAJOR_VERSION}DBus_LIBRARIES})
    ENDIF(${Qt${QT_MAJOR_VERSION}DBus_FOUND}
          AND ${Qt${QT_MAJOR_VERSION}PrintSupport_FOUND})
  ENDIF(EXISTS ${QT_DBUS_CMAKE_DIR} AND EXISTS ${QT_PRINTSUPPORT_CMAKE_DIR})
ENDIF(APPLE)

# Check if the QtX installation is bundled with WebKit (deprecated since Qt 5.5)
# and setup its use if it is the case.
SET(QT_WEBKIT_WIDGETS_CMAKE_DIR
    "${QT_CMAKE_DIR}/Qt${QT_MAJOR_VERSION}WebKitWidgets")
IF(EXISTS ${QT_WEBKIT_WIDGETS_CMAKE_DIR})
  FIND_PACKAGE(Qt${QT_MAJOR_VERSION}WebKit)
  FIND_PACKAGE(Qt${QT_MAJOR_VERSION}WebKitWidgets)
  IF(${Qt${QT_MAJOR_VERSION}WebKit_FOUND}
     AND ${Qt${QT_MAJOR_VERSION}WebKitWidgets_FOUND})
    SET(QT_HAS_WEBKIT TRUE)
    SET(QT_WEB_COMPONENT "QtWebKit")
    SETUP_QT_LIBRARIES(WebKit QT_WEB_LIBRARIES)
    SETUP_QT_LIBRARIES(WebKitWidgets QT_WEB_LIBRARIES)
  ENDIF(${Qt${QT_MAJOR_VERSION}WebKit_FOUND}
        AND ${Qt${QT_MAJOR_VERSION}WebKitWidgets_FOUND})
ENDIF(EXISTS ${QT_WEBKIT_WIDGETS_CMAKE_DIR})

# If QtX is not bundled with WebKit then check if its installation provides
# WebEngine (new web module since Qt 5.4) and setup its use.
SET(QT_WEBENGINE_WIDGETS_CMAKE_DIR
    "${QT_CMAKE_DIR}/Qt${QT_MAJOR_VERSION}WebEngineWidgets")
IF(NOT QT_HAS_WEBKIT AND EXISTS ${QT_WEBENGINE_WIDGETS_CMAKE_DIR})
  FIND_PACKAGE(Qt${QT_MAJOR_VERSION}WebEngineWidgets ${QT_MIN_VERSION})
  FIND_PACKAGE(Qt${QT_MAJOR_VERSION}WebChannel ${QT_MIN_VERSION})
  IF(${Qt${QT_MAJOR_VERSION}WebEngineWidgets_FOUND}
     AND ${Qt${QT_MAJOR_VERSION}WebChannel_FOUND})
    SET(QT_HAS_WEBENGINE TRUE)
    SET(QT_WEB_COMPONENT "QtWebEngine")
    SET(QT_WEB_LIBRARIES ${Qt${QT_MAJOR_VERSION}WebEngineWidgets_LIBRARIES}
                         ${Qt${QT_MAJOR_VERSION}WebChannel_LIBRARIES})
  ENDIF(${Qt${QT_MAJOR_VERSION}WebEngineWidgets_FOUND}
        AND ${Qt${QT_MAJOR_VERSION}WebChannel_FOUND})
ENDIF(NOT QT_HAS_WEBKIT AND EXISTS ${QT_WEBENGINE_WIDGETS_CMAKE_DIR})

IF(NOT "${QT_WEB_COMPONENT}" STREQUAL "${LAST_FOUND_QT_WEB_COMPONENT}")
  MESSAGE(STATUS "Found ${QT_WEB_COMPONENT}")
ENDIF(NOT "${QT_WEB_COMPONENT}" STREQUAL "${LAST_FOUND_QT_WEB_COMPONENT}")

SET(LAST_FOUND_QT_WEB_COMPONENT
    "${QT_WEB_COMPONENT}"
    CACHE INTERNAL "")

MACRO(QTX_SET_INCLUDES_AND_DEFINITIONS)
  INCLUDE_DIRECTORIES(${Qt${QT_MAJOR_VERSION}Widgets_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${Qt${QT_MAJOR_VERSION}Network_INCLUDE_DIRS})
  ADD_DEFINITIONS(${Qt${QT_MAJOR_VERSION}Widgets_DEFINITIONS})
  ADD_DEFINITIONS(${Qt${QT_MAJOR_VERSION}Network_DEFINITIONS})
  IF(QT_MAJOR_VERSION EQUAL 6)
    INCLUDE_DIRECTORIES(${Qt6OpenGLWidgets_INCLUDE_DIRS})
    ADD_DEFINITIONS(${Qt6OpenGLWidgets_DEFINITIONS})
  ENDIF(QT_MAJOR_VERSION EQUAL 6)
ENDMACRO()

MACRO(QTXWEB_SET_INCLUDES_AND_DEFINITIONS)
  IF(QT_HAS_WEBKIT)
    INCLUDE_DIRECTORIES(${Qt${QT_MAJOR_VERSION}WebKit_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${Qt${QT_MAJOR_VERSION}WebKitWidgets_INCLUDE_DIRS})
    ADD_DEFINITIONS(${Qt${QT_MAJOR_VERSION}WebKit_DEFINITIONS})
    ADD_DEFINITIONS(${Qt${QT_MAJOR_VERSION}WebKitWidgets_DEFINITIONS})
    ADD_DEFINITIONS(-DQT_HAS_WEBKIT)
  ENDIF()
  IF(QT_HAS_WEBENGINE)
    INCLUDE_DIRECTORIES(${Qt${QT_MAJOR_VERSION}WebEngineWidgets_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${Qt${QT_MAJOR_VERSION}WebChannel_INCLUDE_DIRS})
    ADD_DEFINITIONS(${Qt${QT_MAJOR_VERSION}WebEngineWidgets_DEFINITIONS})
    ADD_DEFINITIONS(${Qt${QT_MAJOR_VERSION}WebChannel_DEFINITIONS})
    ADD_DEFINITIONS(-DQT_HAS_WEBENGINE)
  ENDIF()
ENDMACRO(QTXWEB_SET_INCLUDES_AND_DEFINITIONS)

# Define aliases for Qt macros in order to build the project
IF(QT_MAJOR_VERSION EQUAL 5)
  MACRO(QT_WRAP_CPP outfiles)
    QT5_WRAP_CPP(${outfiles} ${ARGN})
  ENDMACRO(QT_WRAP_CPP outfiles)
ENDIF(QT_MAJOR_VERSION EQUAL 5)

IF(QT_MAJOR_VERSION EQUAL 5)
  MACRO(QT_WRAP_UI outfiles)
    QT5_WRAP_UI(${outfiles} ${ARGN})
  ENDMACRO()
ENDIF(QT_MAJOR_VERSION EQUAL 5)

IF(QT_MAJOR_VERSION EQUAL 5)
  MACRO(QT_ADD_RESOURCES outfiles)
    QT5_ADD_RESOURCES(${outfiles} ${ARGN})
  ENDMACRO()
ENDIF(QT_MAJOR_VERSION EQUAL 5)

# With MinGW, remove the -fPIC compiler option as it is not needed and generates
# a lot of warnings
IF(MINGW)
  STRING(REPLACE "-fPIC" ""
                 Qt${QT_MAJOR_VERSION}Widgets_EXECUTABLE_COMPILE_FLAGS
                 "${Qt${QT_MAJOR_VERSION}Widgets_EXECUTABLE_COMPILE_FLAGS}")
ENDIF(MINGW)
