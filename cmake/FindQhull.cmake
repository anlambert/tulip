# ##############################################################################
# File is taken and adapted from the Point Cloud Library (PCL) -
# www.pointclouds.org (BSD License)
# https://github.com/otherlab/pcl/blob/master/cmake/Modules/FindQhull.cmake
# ##############################################################################
# Find QHULL
#
# This sets the following variables: QHULL_FOUND - True if QHULL was found.
# QHULL_INCLUDE_DIRS - Directories containing the QHULL include files.
# QHULL_LIBRARIES - Libraries needed to use QHULL.
# If QHULL_USE_STATIC is specified then look for static libraries ONLY else
# look for shared ones

FIND_PATH(
  QHULL_INCLUDE_DIR
  NAMES qhull_ra.h qhull_a.h
  HINTS "${QHULL_ROOT}" "$ENV{QHULL_ROOT}" "${QHULL_INCLUDE_DIR}"
  PATHS "$ENV{PROGRAMFILES}/QHull" "$ENV{PROGRAMW6432}/QHull"
  PATH_SUFFIXES qhull_r src/libqhull_r libqhull_r qhull src/libqhull libqhull)

FIND_LIBRARY(
  QHULL_LIBRARY
  NAMES qhull_r qhull
  HINTS "${QHULL_ROOT}" "$ENV{QHULL_ROOT}"
  PATHS "$ENV{PROGRAMFILES}/QHull" "$ENV{PROGRAMW6432}/QHull"
  PATH_SUFFIXES project build bin lib)

FIND_LIBRARY(
  QHULL_LIBRARY_DEBUG
  NAMES qhull_rd qhulld
  HINTS "${QHULL_ROOT}" "$ENV{QHULL_ROOT}"
  PATHS "$ENV{PROGRAMFILES}/QHull" "$ENV{PROGRAMW6432}/QHull"
  PATH_SUFFIXES project build bin lib)

IF(NOT QHULL_LIBRARY_DEBUG)
  SET(QHULL_LIBRARY_DEBUG ${QHULL_LIBRARY})
ENDIF(NOT QHULL_LIBRARY_DEBUG)

SET(QHULL_INCLUDE_DIRS ${QHULL_INCLUDE_DIR})
SET(QHULL_LIBRARIES optimized ${QHULL_LIBRARY} debug ${QHULL_LIBRARY_DEBUG})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Qhull DEFAULT_MSG QHULL_LIBRARY
                                  QHULL_INCLUDE_DIR)

MARK_AS_ADVANCED(QHULL_LIBRARY QHULL_LIBRARY_DEBUG QHULL_INCLUDE_DIR)

IF("${QHULL_LIBRARY}" MATCHES ".*qhull_r.*")
  SET(HAVE_REENTRANT_QHULL TRUE)
ENDIF("${QHULL_LIBRARY}" MATCHES ".*qhull_r.*")