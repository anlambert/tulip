# Returns a version string from Git
#
# These functions force a re-configure on each git commit so that you can trust
# the values of the variables in your build system.
#
# get_git_head_revision(<refspecvar> <hashvar> [<additional arguments to git
# describe> ...])
#
# Returns the refspec and sha hash of the current head revision
#
# git_describe(<var> [<additional arguments to git describe> ...])
#
# Returns the results of git describe on the source tree, and adjusting the
# output so that it tests false if an error occurs.
#
# git_get_exact_tag(<var> [<additional arguments to git describe> ...])
#
# Returns the results of git describe --exact-match on the source tree, and
# adjusting the output so that it tests false if there was no exact matching
# tag.
#
# git_local_changes(<var>)
#
# Returns either "CLEAN" or "DIRTY" with respect to uncommitted changes. Uses
# the return code of "git diff-index --quiet HEAD --". Does not regard untracked
# files.
#
# Requires CMake 2.6 or newer (uses the 'function' command)
#
# Original Author: 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# <abiryan@ryand.net> http://academic.cleardefinition.com Iowa State University
# HCI Graduate Program/VRAC
#
# Copyright Iowa State University 2009-2010. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
# at http://www.boost.org/LICENSE_1_0.txt)

IF(__get_git_revision_description)
  RETURN()
ENDIF()
SET(__get_git_revision_description YES)

# We must run the following at "include" time, not at function call time, to
# find the path to this module rather than the path to a calling list file
GET_FILENAME_COMPONENT(_gitdescmoddir ${CMAKE_CURRENT_LIST_FILE} PATH)

FUNCTION(get_git_head_revision _refspecvar _hashvar)
  SET(GIT_PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  SET(GIT_DIR "${GIT_PARENT_DIR}/.git")
  WHILE(NOT EXISTS "${GIT_DIR}") # .git dir not found, search parent directories
    SET(GIT_PREVIOUS_PARENT "${GIT_PARENT_DIR}")
    GET_FILENAME_COMPONENT(GIT_PARENT_DIR ${GIT_PARENT_DIR} PATH)
    IF(GIT_PARENT_DIR STREQUAL GIT_PREVIOUS_PARENT)
      # We have reached the root directory, we are not in git
      SET(${_refspecvar}
          "GITDIR-NOTFOUND"
          PARENT_SCOPE)
      SET(${_hashvar}
          "GITDIR-NOTFOUND"
          PARENT_SCOPE)
      RETURN()
    ENDIF()
    SET(GIT_DIR "${GIT_PARENT_DIR}/.git")
  ENDWHILE()
  # check if this is a submodule
  IF(NOT IS_DIRECTORY ${GIT_DIR})
    FILE(READ ${GIT_DIR} submodule)
    STRING(REGEX REPLACE "gitdir: (.*)\n$" "\\1" GIT_DIR_RELATIVE ${submodule})
    GET_FILENAME_COMPONENT(SUBMODULE_DIR ${GIT_DIR} PATH)
    GET_FILENAME_COMPONENT(GIT_DIR ${SUBMODULE_DIR}/${GIT_DIR_RELATIVE}
                           ABSOLUTE)
  ENDIF()
  SET(GIT_DATA "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/git-data")
  IF(NOT EXISTS "${GIT_DATA}")
    FILE(MAKE_DIRECTORY "${GIT_DATA}")
  ENDIF()

  IF(NOT EXISTS "${GIT_DIR}/HEAD")
    RETURN()
  ENDIF()
  SET(HEAD_FILE "${GIT_DATA}/HEAD")
  CONFIGURE_FILE("${GIT_DIR}/HEAD" "${HEAD_FILE}" COPYONLY)

  CONFIGURE_FILE("${_gitdescmoddir}/GetGitRevisionDescription.cmake.in"
                 "${GIT_DATA}/grabRef.cmake" @ONLY)
  INCLUDE("${GIT_DATA}/grabRef.cmake")

  SET(${_refspecvar}
      "${HEAD_REF}"
      PARENT_SCOPE)
  SET(${_hashvar}
      "${HEAD_HASH}"
      PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(git_describe _var)
  IF(NOT GIT_FOUND)
    FIND_PACKAGE(Git QUIET)
  ENDIF()
  GET_GIT_HEAD_REVISION(refspec hash)
  IF(NOT GIT_FOUND)
    SET(${_var}
        "GIT-NOTFOUND"
        PARENT_SCOPE)
    RETURN()
  ENDIF()
  IF(NOT hash)
    SET(${_var}
        "HEAD-HASH-NOTFOUND"
        PARENT_SCOPE)
    RETURN()
  ENDIF()

  # TODO sanitize if((${ARGN}" MATCHES "&&") OR (ARGN MATCHES "||") OR (ARGN
  # MATCHES "\\;")) message("Please report the following error to the project!")
  # message(FATAL_ERROR "Looks like someone's doing something nefarious with
  # git_describe! Passed arguments ${ARGN}") endif()

  # message(STATUS "Arguments to execute_process: ${ARGN}")

  EXECUTE_PROCESS(
    COMMAND "${GIT_EXECUTABLE}" describe ${hash} ${ARGN}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RESULT_VARIABLE res
    OUTPUT_VARIABLE out
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
  IF(NOT res EQUAL 0)
    SET(out "${out}-${res}-NOTFOUND")
  ENDIF()

  SET(${_var}
      "${out}"
      PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(git_get_exact_tag _var)
  GIT_DESCRIBE(out --exact-match ${ARGN})
  SET(${_var}
      "${out}"
      PARENT_SCOPE)
ENDFUNCTION()

FUNCTION(git_local_changes _var)
  IF(NOT GIT_FOUND)
    FIND_PACKAGE(Git QUIET)
  ENDIF()
  GET_GIT_HEAD_REVISION(refspec hash)
  IF(NOT GIT_FOUND)
    SET(${_var}
        "GIT-NOTFOUND"
        PARENT_SCOPE)
    RETURN()
  ENDIF()
  IF(NOT hash)
    SET(${_var}
        "HEAD-HASH-NOTFOUND"
        PARENT_SCOPE)
    RETURN()
  ENDIF()

  EXECUTE_PROCESS(
    COMMAND "${GIT_EXECUTABLE}" diff-index --quiet HEAD --
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RESULT_VARIABLE res
    OUTPUT_VARIABLE out ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
  IF(res EQUAL 0)
    SET(${_var}
        "CLEAN"
        PARENT_SCOPE)
  ELSE()
    SET(${_var}
        "DIRTY"
        PARENT_SCOPE)
  ENDIF()
ENDFUNCTION()
