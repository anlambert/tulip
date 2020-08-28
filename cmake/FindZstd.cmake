# Find the zstd compression library and includes

FIND_PATH(
  zstd_INCLUDE_DIRS
  NAMES zstd.h
  HINTS ${zstd_ROOT_DIR}/include)

FIND_LIBRARY(
  zstd_LIBRARIES
  NAMES zstd
  HINTS ${zstd_ROOT_DIR}/lib)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(zstd DEFAULT_MSG zstd_LIBRARIES
                                  zstd_INCLUDE_DIRS)

MARK_AS_ADVANCED(zstd_LIBRARIES zstd_INCLUDE_DIRS)

IF(zstd_FOUND AND NOT (TARGET zstd::zstd))
  ADD_LIBRARY(zstd::zstd UNKNOWN IMPORTED)
  SET_TARGET_PROPERTIES(
    zstd::zstd PROPERTIES IMPORTED_LOCATION ${zstd_LIBRARIES}
                          INTERFACE_INCLUDE_DIRECTORIES ${zstd_INCLUDE_DIRS})
ENDIF()