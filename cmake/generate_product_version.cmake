# CMakeHelpers library, distributed under MIT license
#
# Copyright (C) 2015, by halex2005
#
# Report bugs and download new versions at
# https://github.com/halex2005/CMakeHelpers

INCLUDE(CMakeParseArguments)

SET(GenerateProductVersionCurrentDir ${CMAKE_CURRENT_LIST_DIR})

# generate_product_version() function
#
# This function uses VersionInfo.in template file and VersionResource.rc file to
# generate WIN32 resource with version information and general resource strings.
#
# Usage: generate_product_version( SomeOutputResourceVariable NAME
# MyGreatProject ICON ${PATH_TO_APP_ICON} VERSION_MAJOR 2 VERSION_MINOR 3
# VERSION_PATH ${BUILD_COUNTER} VERSION_REVISION ${BUILD_REVISION} ) where
# BUILD_COUNTER and BUILD_REVISION could be values from your CI server.
#
# You can use generated resource for your executable targets:
# add_executable(target-name ${target-files} ${SomeOutputResourceVariable})
#
# You can specify resource strings in arguments: NAME               - name of
# executable (no defaults, ex: Microsoft Word) BUNDLE             - bundle
# (${NAME} is default, ex: Microsoft Office) ICON               - path to
# application icon (${CMAKE_SOURCE_DIR}/product.ico by default) VERSION_MAJOR -
# 1 is default VERSION_MINOR      - 0 is default VERSION_PATCH      - 0 is
# default VERSION_REVISION   - 0 is default COMPANY_NAME       - your company
# name (no defaults) COMPANY_COPYRIGHT  - ${COMPANY_NAME} (C) Copyright
# ${CURRENT_YEAR} is default COMMENTS           - ${NAME}
# v${VERSION_MAJOR}.${VERSION_MINOR} is default ORIGINAL_FILENAME  - ${NAME} is
# default INTERNAL_NAME      - ${NAME} is default FILE_DESCRIPTION   - ${NAME}
# is default
FUNCTION(generate_product_version outfiles)
  SET(options)
  SET(oneValueArgs
      NAME
      BUNDLE
      ICON
      VERSION_MAJOR
      VERSION_MINOR
      VERSION_PATCH
      VERSION_REVISION
      COMPANY_NAME
      COMPANY_COPYRIGHT
      COMMENTS
      ORIGINAL_FILENAME
      INTERNAL_NAME
      FILE_DESCRIPTION)
  SET(multiValueArgs)
  CMAKE_PARSE_ARGUMENTS(PRODUCT "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  IF(NOT PRODUCT_BUNDLE OR "${PRODUCT_BUNDLE}" STREQUAL "")
    SET(PRODUCT_BUNDLE "${PRODUCT_NAME}")
  ENDIF()

  IF(NOT PRODUCT_ICON OR "${PRODUCT_ICON}" STREQUAL "")
    SET(APP_HAS_ICON 0)
  ELSE()
    SET(APP_HAS_ICON 1)
  ENDIF()

  IF(NOT PRODUCT_VERSION_MAJOR
     OR "${PRODUCT_VERSION_MAJOR}" STREQUAL ""
     OR NOT "${PRODUCT_VERSION_MAJOR}" MATCHES "^[0-9]+$")
    SET(PRODUCT_VERSION_MAJOR 1)
  ENDIF()
  IF(NOT PRODUCT_VERSION_MINOR
     OR "${PRODUCT_VERSION_MINOR}" STREQUAL ""
     OR NOT "${PRODUCT_VERSION_MINOR}" MATCHES "^[0-9]+$")
    SET(PRODUCT_VERSION_MINOR 0)
  ENDIF()
  IF(NOT PRODUCT_VERSION_PATCH
     OR "${PRODUCT_VERSION_PATCH}" STREQUAL ""
     OR NOT "${PRODUCT_VERSION_PATCH}" MATCHES "^[0-9]+$")
    SET(PRODUCT_VERSION_PATCH 0)
  ENDIF()
  IF(NOT PRODUCT_VERSION_REVISION
     OR "${PRODUCT_VERSION_REVISION}" STREQUAL ""
     OR NOT "${PRODUCT_VERSION_REVISION}" MATCHES "^[0-9]+$")
    SET(PRODUCT_VERSION_REVISION 0)
  ENDIF()

  IF(NOT PRODUCT_COMPANY_COPYRIGHT OR "${PRODUCT_COMPANY_COPYRIGHT}" STREQUAL
                                      "")
    STRING(TIMESTAMP PRODUCT_CURRENT_YEAR "%Y")
    SET(PRODUCT_COMPANY_COPYRIGHT
        "${PRODUCT_COMPANY_NAME} (C) Copyright ${PRODUCT_CURRENT_YEAR}")
  ENDIF()
  IF(NOT PRODUCT_COMMENTS OR "${PRODUCT_COMMENTS}" STREQUAL "")
    SET(PRODUCT_COMMENTS
        "${PRODUCT_NAME} v${PRODUCT_VERSION_MAJOR}.${PRODUCT_VERSION_MINOR}")
  ENDIF()
  IF(NOT PRODUCT_ORIGINAL_FILENAME OR "${PRODUCT_ORIGINAL_FILENAME}" STREQUAL
                                      "")
    SET(PRODUCT_ORIGINAL_FILENAME "${PRODUCT_NAME}")
  ENDIF()
  IF(NOT PRODUCT_INTERNAL_NAME OR "${PRODUCT_INTERNAL_NAME}" STREQUAL "")
    SET(PRODUCT_INTERNAL_NAME "${PRODUCT_NAME}")
  ENDIF()
  IF(NOT PRODUCT_FILE_DESCRIPTION OR "${PRODUCT_FILE_DESCRIPTION}" STREQUAL "")
    SET(PRODUCT_FILE_DESCRIPTION "${PRODUCT_NAME}")
  ENDIF()

  SET(VERSION_HEADER "${PRODUCT_NAME}VersionInfo.h")

  SET(_VersionInfoFile ${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT_NAME}VersionInfo.h)
  SET(_VersionResourceFile
      ${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT_NAME}VersionResource.rc)
  CONFIGURE_FILE(${GenerateProductVersionCurrentDir}/VersionInfo.in
                 ${_VersionInfoFile} @ONLY)
  CONFIGURE_FILE(${GenerateProductVersionCurrentDir}/VersionResource.rc
                 ${_VersionResourceFile})
  LIST(APPEND ${outfiles} ${_VersionInfoFile} ${_VersionResourceFile})
  SET(${outfiles}
      ${${outfiles}}
      PARENT_SCOPE)
ENDFUNCTION()
