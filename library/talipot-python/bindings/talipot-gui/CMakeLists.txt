SET(TALIPOT_GUI_SIP_FILES
    Interactor.sip
    Module.sip
    View.sip
    GlView.sip
    NodeLinkDiagramView.sip
    TalipotGui.sip
    ColorScalesManager.sip)

SET(TALIPOT_GUI_PYTHON_BINDINGS_SRC
    sip_talipotguipart0.cpp sip_talipotguipart1.cpp sip_talipotguipart2.cpp)

ADD_CUSTOM_COMMAND(
  OUTPUT ${TALIPOT_GUI_PYTHON_BINDINGS_SRC}
  COMMAND
    ${SIP_EXE} -n ${SIP_MODULE} -I ${CMAKE_CURRENT_BINARY_DIR} -I
    ${CMAKE_CURRENT_BINARY_DIR}/../talipot-core -I
    ${CMAKE_CURRENT_SOURCE_DIR}/../talipot-core -c ${CMAKE_CURRENT_BINARY_DIR}
    -j3 ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
  COMMENT "Generating Python bindings for libtalipot-gui"
  DEPENDS ${TALIPOT_GUI_SIP_FILES} ${TALIPOT_SIP_FILES} ${STL_SIP_FILES}
  VERBATIM)

QT_WRAP_CPP(talipotguiutils_MOC ViewsUtils.h)
QT_ADD_RESOURCES(talipotgui_RCC_SRCS TalipotGui.qrc)

SET(TALIPOT_GUI_PYTHON_BINDINGS_SRC
    ${TALIPOT_GUI_PYTHON_BINDINGS_SRC} ViewsUtils.cpp ${talipotguiutils_MOC}
    ${talipotgui_RCC_SRCS})

DISABLE_COMPILER_WARNINGS()

# Force to use SIP headers located in thirdparty instead of those installed in
# the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR} ${TalipotCoreBuildInclude}
                    ${TalipotGUIBuildInclude})

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${PYTHON_INCLUDE_DIR}
  ${PYTHON_INCLUDE_PATH}
  ${OPENGL_INCLUDE_DIR}
  ${TalipotCoreInclude}
  ${TalipotOGLInclude}
  ${TalipotGUIInclude})

QTX_SET_INCLUDES_AND_DEFINITIONS()

ADD_LIBRARY(${LibTalipotGUIPythonBindingsName} SHARED
            ${TALIPOT_GUI_PYTHON_BINDINGS_SRC})

ADD_DEPENDENCIES(${LibTalipotGUIPythonBindingsName}
                 ${LibTalipotCorePythonBindingsName})

SET_TARGET_PROPERTIES(
  ${LibTalipotGUIPythonBindingsName}
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TALIPOTGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(
  ${LibTalipotGUIPythonBindingsName}
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TALIPOTGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(
  ${LibTalipotGUIPythonBindingsName}
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE
             ${TALIPOTGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(
  ${LibTalipotGUIPythonBindingsName}
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL
             ${TALIPOTGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(
  ${LibTalipotGUIPythonBindingsName}
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO
             ${TALIPOTGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(
  ${LibTalipotGUIPythonBindingsName}
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${TALIPOTGUI_PYTHON_NATIVE_FOLDER})

# Ensure bindings build output folder exists
ADD_CUSTOM_TARGET(
  create-talipotgui-python-native-folder ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory ${TALIPOTGUI_PYTHON_NATIVE_FOLDER})
ADD_DEPENDENCIES(${LibTalipotGUIPythonBindingsName}
                 create-talipotgui-python-native-folder)

ADD_CUSTOM_TARGET(
  copyTalipotGuiInitPy ALL
  ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
  ${TALIPOTGUI_PYTHON_FOLDER}/__init__.py)

IF(NOT SYSTEM_SIP)
  ADD_DEPENDENCIES(${LibTalipotGUIPythonBindingsName} ${SIP_LIB})
ENDIF(NOT SYSTEM_SIP)

SET_TARGET_PROPERTIES(${LibTalipotGUIPythonBindingsName}
                      PROPERTIES OUTPUT_NAME talipotgui PREFIX "_")

IF(WIN32)
  SET_TARGET_PROPERTIES(${LibTalipotGUIPythonBindingsName} PROPERTIES SUFFIX
                                                                      ".pyd")
ELSE(WIN32)
  SET_TARGET_PROPERTIES(${LibTalipotGUIPythonBindingsName} PROPERTIES SUFFIX
                                                                      ".so")
ENDIF(WIN32)

FILE(RELATIVE_PATH TalipotLibsInstallRelPath
     ${TalipotPythonModulesInstallDir}/talipotgui/native
     ${CMAKE_INSTALL_PREFIX}/${TalipotLibInstallDir})

# On MacOS, add the paths of dependencies dylibs in install rpaths of the
# _talipotgui.so binary That way, the talipotgui module can be imported in a
# classical Python shell without having to modify the content of the
# DYLD_LIBRARY_PATH environment variable
IF(APPLE)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTalipotGUIPythonBindingsName}
                      INSTALL_RPATH)
  SET_TARGET_PROPERTIES(
    ${LibTalipotGUIPythonBindingsName}
    PROPERTIES
      INSTALL_RPATH
      "${CURRENT_INSTALL_RPATHS};@loader_path/${TalipotLibsInstallRelPath}/;@loader_path/../../../../Frameworks"
  )
ELSEIF(LINUX)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTalipotGUIPythonBindingsName}
                      INSTALL_RPATH)
  SET_TARGET_PROPERTIES(
    ${LibTalipotGUIPythonBindingsName}
    PROPERTIES INSTALL_RPATH
               "${CURRENT_INSTALL_RPATHS}:$ORIGIN/${TalipotLibsInstallRelPath}")
ENDIF(APPLE)

TARGET_LINK_LIBRARIES(
  ${LibTalipotGUIPythonBindingsName} ${LibTalipotCoreName} ${LibTalipotOGLName}
  ${LibTalipotGUIName} ${QT_LIBRARIES} ${PYTHON_LIBRARY})

TALIPOT_INSTALL_PYTHON_FILES(talipotgui ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py)
TALIPOT_INSTALL_PYTHON_FILES(talipotgui/native
                             ${LibTalipotGUIPythonBindingsName})

ADD_CUSTOM_COMMAND(
  TARGET ${LibTalipotGUIPythonBindingsName}
  POST_BUILD
  COMMAND
    ${SIP_EXE} -n ${SIP_MODULE} -I ${CMAKE_CURRENT_BINARY_DIR}/../talipot-core
    -I ${CMAKE_CURRENT_SOURCE_DIR}/../talipot-core -a
    ${CMAKE_CURRENT_BINARY_DIR}/talipotgui.api
    ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
  COMMENT "Generating API file for Talipot GUI Python bindings"
  VERBATIM)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/talipotgui.api
        DESTINATION ${TalipotShareInstallDir}/apiFiles)

SET_DIRECTORY_PROPERTIES(
  PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
             "talipotgui.api;sipAPI_talipotgui.h;talipotgui_module")
