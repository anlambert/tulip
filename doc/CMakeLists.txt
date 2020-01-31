SET(TALIPOT_BUILD_DOC ON CACHE BOOL "Do you want to build the documentation ? [ON|OFF]")

IF(TALIPOT_BUILD_DOC)
  # Unset the previous values of the CMake cache variable SPHINX_EXECUTABLE
  # in case the value of PYTHON_EXECUTABLE CMake variable changed
  # but save it before into D_SPHINX_EXECUTABLE to be able to restore it
  # in case there will be no other available
  SET(D_SPHINX_EXECUTABLE ${SPHINX_EXECUTABLE})
  UNSET(SPHINX_EXECUTABLE CACHE)
  IF(EXISTS ${D_SPHINX_EXECUTABLE})
    FIND_PACKAGE(Sphinx QUIET)
  ELSE(EXISTS ${D_SPHINX_EXECUTABLE})
    IF(D_SPHINX_EXECUTABLE)
      MESSAGE(FATAL_ERROR " SPHINX_EXECUTABLE ${D_SPHINX_EXECUTABLE} does not exist")
    ENDIF(D_SPHINX_EXECUTABLE)
    FIND_PACKAGE(Sphinx)
  ENDIF(EXISTS ${D_SPHINX_EXECUTABLE})
  IF(NOT SPHINX_FOUND AND (EXISTS ${D_SPHINX_EXECUTABLE}))
    # restore the previously defined value
    SET(SPHINX_FOUND TRUE)
    SET(SPHINX_EXECUTABLE ${D_SPHINX_EXECUTABLE} CACHE FILEPATH "Sphinx document generator" FORCE)
    MESSAGE("-- Found Sphinx: ${SPHINX_EXECUTABLE}")
  ENDIF(NOT SPHINX_FOUND AND (EXISTS ${D_SPHINX_EXECUTABLE}))
  IF(SPHINX_FOUND)
    ADD_SUBDIRECTORY(python)
  ELSE(SPHINX_FOUND)
    MESSAGE(AUTHOR_WARNING "Sphinx not found: unable to generate the documentation.")
  ENDIF(SPHINX_FOUND)

  IF(NOT TALIPOT_BUILD_FOR_APPIMAGE)
    FIND_PACKAGE(Doxygen)
    IF(DOXYGEN_FOUND)
      ADD_SUBDIRECTORY(doxygen)
    ELSE(DOXYGEN_FOUND)
      MESSAGE(AUTHOR_WARNING "doxygen not found: unable to generate the Talipot API documentation.")
    ENDIF(DOXYGEN_FOUND)
  ENDIF(NOT TALIPOT_BUILD_FOR_APPIMAGE)
ENDIF(TALIPOT_BUILD_DOC)
