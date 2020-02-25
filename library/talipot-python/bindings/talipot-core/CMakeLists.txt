IF(WIN32)
  TALIPOT_SET_CXX_FLAGS("-DDLL_TALIPOT_PYTHON")
ENDIF(WIN32)

# Visual Studio has a limit for string constants length
# so we define the sip feature NO_LIMIT_DOC in Module.sip
# to avoid the compiler error (fatal error C1091)
IF(MSVC)
  SET(SIP_FLAGS -x NO_LIMIT_DOC)
ENDIF()

SET(TALIPOT_SIP_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/AcyclicTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/AbstractProperty.sip.in
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/BiconnectedTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/BooleanPropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/BoundingBox.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ColorScale.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Color.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ConnectedTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ConvexHull.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Coord.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/DataSet.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Delaunay.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/DoublePropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/DrawingTools.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Edge.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ExportModule.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/FontAwesome.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Graph.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphEvent.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphProperty.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphMeasure.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphTools.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ImportModule.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/IntegerPropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Iterator.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/LayoutPropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/MapIterator.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/MaterialDesignIcons.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Matrix.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Node.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/NumericProperty.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Observable.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/OuterPlanarTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PlanarityTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginContext.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginLoader.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginProgress.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Plugin.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginsManager.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyAlgorithm.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyEvent.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyInterface.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyProxy.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/SimpleTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/SizePropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Size.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/StringCollection.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TlpTools.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TriconnectedTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ViewSettings.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Vector.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/WithDependency.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/WithParameter.sip
    CACHE INTERNAL ""
)

SET(TALIPOT_PYTHON_BINDINGS_SRC
    sip_talipotpart0.cpp
    sip_talipotpart1.cpp
    sip_talipotpart2.cpp
    sip_talipotpart3.cpp
    sip_talipotpart4.cpp
    sip_talipotpart5.cpp
    sip_talipotpart6.cpp
    sip_talipotpart7.cpp
)

ADD_CUSTOM_COMMAND(OUTPUT ${TALIPOT_PYTHON_BINDINGS_SRC}
                   COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -DBINARY_DIR=${CMAKE_CURRENT_BINARY_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/generatePropertiesBindings.cmake
                   COMMAND ${SIP_EXE} -n ${SIP_MODULE} -y ${CMAKE_CURRENT_BINARY_DIR}/talipot.pyi -o -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} -j8 ${SIP_FLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                   COMMENT "Generating Python bindings for libtalipot-core"
                   DEPENDS ${TALIPOT_SIP_FILES} ${STL_SIP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/../../include/talipot/PythonCppTypesConverter.h VERBATIM)

DISABLE_COMPILER_WARNINGS()

# Force to use SIP and headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotPythonInclude})

SET(TALIPOT_PYTHON_BINDINGS_SRC
    ${TALIPOT_PYTHON_BINDINGS_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/PythonCppTypesConverter.cpp)

ADD_LIBRARY(${LibTalipotCorePythonBindingsName} SHARED ${TALIPOT_PYTHON_BINDINGS_SRC})

SET_TARGET_PROPERTIES(${LibTalipotCorePythonBindingsName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TALIPOT_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTalipotCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TALIPOT_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTalipotCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${TALIPOT_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTalipotCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${TALIPOT_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTalipotCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${TALIPOT_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTalipotCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${TALIPOT_PYTHON_NATIVE_FOLDER})

# Ensure bindings build output folder exists
ADD_CUSTOM_TARGET(create-talipot-python-native-folder ALL
                  COMMAND ${CMAKE_COMMAND} -E make_directory ${TALIPOT_PYTHON_NATIVE_FOLDER})
ADD_DEPENDENCIES(${LibTalipotCorePythonBindingsName} create-talipot-python-native-folder)

ADD_CUSTOM_TARGET(copyTalipotInitPy ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${TALIPOT_PYTHON_FOLDER}/__init__.py)

IF(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_DEPENDENCIES(wheel copyTalipotInitPy)

  CONFIGURE_FILE(packaging/setup.cfg.in ${TALIPOT_PYTHON_FOLDER}/../setup.cfg)
  CONFIGURE_FILE(packaging/setup.py.in ${TALIPOT_PYTHON_FOLDER}/../setup.py)
  IF(TALIPOT_GENERATE_TESTPYPI_WHEEL)
    SET(TALIPOT_PYTHON_WHEEL_VERSION ${TALIPOT_PYTHON_TEST_WHEEL_VERSION})
    CONFIGURE_FILE(packaging/setup.py.in ${TALIPOT_PYTHON_FOLDER}/../setuptest.py)
  ENDIF(TALIPOT_GENERATE_TESTPYPI_WHEEL)

  ADD_CUSTOM_TARGET(copyTalipotPythonPackagingFiles ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/MANIFEST.in ${TALIPOT_PYTHON_FOLDER}/../MANIFEST.in
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/README.rst ${TALIPOT_PYTHON_FOLDER}/../README.rst
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/copyTalipotCoreDllDependencies.cmake ${TALIPOT_PYTHON_FOLDER}/../copyTalipotCoreDllDependencies.cmake
                    DEPENDS ${TALIPOT_PYTHON_FOLDER}/../setup.py ${TALIPOT_PYTHON_FOLDER}/../setup.cfg)
  ADD_DEPENDENCIES(wheel copyTalipotPythonPackagingFiles)
  IF(TALIPOT_GENERATE_TESTPYPI_WHEEL)
    ADD_DEPENDENCIES(wheel copyTalipotPythonPackagingFiles)
  ENDIF(TALIPOT_GENERATE_TESTPYPI_WHEEL)
ENDIF(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)

IF(NOT SYSTEM_SIP)
  ADD_DEPENDENCIES(${LibTalipotCorePythonBindingsName} ${SIP_LIB})
ENDIF(NOT SYSTEM_SIP)

SET_TARGET_PROPERTIES(${LibTalipotCorePythonBindingsName}
                      PROPERTIES OUTPUT_NAME talipot PREFIX "_")

IF(WIN32)
  SET_TARGET_PROPERTIES(${LibTalipotCorePythonBindingsName}
                        PROPERTIES SUFFIX ".pyd")
ELSE(WIN32)
  SET_TARGET_PROPERTIES(${LibTalipotCorePythonBindingsName}
                        PROPERTIES SUFFIX ".so")
ENDIF(WIN32)

# On MacOS, add the paths of dependencies dylibs in install rpaths of the _talipot.so binary
# That way, the talipot module can be imported in a classical Python shell without having to
# modify the content of the DYLD_LIBRARY_PATH environment variable

FILE(RELATIVE_PATH TalipotLibsInstallRelPath ${TalipotPythonModulesInstallDir}/talipot/native ${CMAKE_INSTALL_PREFIX}/${TalipotLibInstallDir})

IF(APPLE AND NOT TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTalipotCorePythonBindingsName} INSTALL_RPATH)
  SET_TARGET_PROPERTIES(${LibTalipotCorePythonBindingsName} PROPERTIES INSTALL_RPATH "${CURRENT_INSTALL_RPATHS};@loader_path/${TalipotLibsInstallRelPath};@loader_path/../../../../Frameworks")
ELSEIF(LINUX AND NOT TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTalipotCorePythonBindingsName} INSTALL_RPATH)
  SET_TARGET_PROPERTIES(${LibTalipotCorePythonBindingsName} PROPERTIES INSTALL_RPATH "${CURRENT_INSTALL_RPATHS}:$ORIGIN/${TalipotLibsInstallRelPath}")
ENDIF(APPLE AND NOT TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)

TARGET_LINK_LIBRARIES(${LibTalipotCorePythonBindingsName} ${LibTalipotCoreName})
# When building Python wheel for MacOS, don't link the C extension module with the Python library
# and use dynamic lookup for retrieving its symbols.
# That way, we can produce a C extension module that can be imported through the Python interpreter
# provided by Apple with the System and the one provided by Python.org
IF(APPLE AND TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
  SET_TARGET_PROPERTIES(${LibTalipotCorePythonBindingsName} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
ELSE(APPLE AND TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
  IF(NOT LINUX OR NOT TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
    TARGET_LINK_LIBRARIES(${LibTalipotCorePythonBindingsName} ${PYTHON_LIBRARY})
  ENDIF(NOT LINUX OR NOT TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
ENDIF(APPLE AND TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)

TALIPOT_INSTALL_PYTHON_FILES(talipot ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py)
TALIPOT_INSTALL_PYTHON_FILES(talipot/native ${LibTalipotCorePythonBindingsName})

IF(NOT TALIPOT_BUILD_CORE_ONLY)
  ADD_CUSTOM_COMMAND(TARGET ${LibTalipotCorePythonBindingsName}
                     POST_BUILD
                     COMMAND ${SIP_EXE} -n ${SIP_MODULE} -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR} -a ${CMAKE_CURRENT_BINARY_DIR}/talipot.api ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                     COMMENT "Generating API file for Talipot Python bindings"
                     VERBATIM)
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/talipot.api DESTINATION ${TalipotShareInstallDir}/apiFiles)
ENDIF(NOT TALIPOT_BUILD_CORE_ONLY)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "talipot.api;sipAPI_talipot.h;talipot_module;BooleanProperty.sip;ColorProperty.sip;DoubleProperty.sip;IntegerProperty.sip;LayoutProperty.sip;SizeProperty.sip;StringProperty.sip")
