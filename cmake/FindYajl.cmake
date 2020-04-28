# Try to find libyajl
#
# Once done this will define YAJL_FOUND - System has YAJL YAJL_INCLUDE_DIRS -
# The YAJL include directories YAJL_LIBRARIES - The libraries needed to use YAJL

FIND_PATH(
  YAJL_INCLUDE_DIR yajl/yajl_version.h
  HINTS ${PC_YAJL_INCLUDEDIR} ${PC_YAJL_INCLUDE_DIRS}
  PATH_SUFFIXES libyajl)

FIND_LIBRARY(
  YAJL_LIBRARY
  NAMES yajl libyajl
  HINTS ${PC_YAJL_LIBDIR} ${PC_YAJL_LIBRARY_DIRS})

SET(YAJL_LIBRARIES ${YAJL_LIBRARY})
SET(YAJL_INCLUDE_DIRS ${YAJL_INCLUDE_DIR})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Yajl DEFAULT_MSG YAJL_LIBRARY
                                  YAJL_INCLUDE_DIR)

MARK_AS_ADVANCED(YAJL_INCLUDE_DIR YAJL_LIBRARY)
