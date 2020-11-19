IF(PYTHONLIBS_FOUND)
  IF(SIP_OK)

    IF(WIN32)
      TALIPOT_SET_CXX_FLAGS("-DDLL_TALIPOT_PYTHON")
    ELSE(WIN32)
      # to disable deprecated register warnings with Python 2.7.13 headers
      TALIPOT_SET_CXX_FLAGS("-Wno-register")
      IF(APPLE)
        TALIPOT_SET_CXX_FLAGS("-Wno-deprecated-declarations")
      ENDIF(APPLE)
    ENDIF(WIN32)

    INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})
    INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH}
                        ${OPENGL_INCLUDE_DIR})
    INCLUDE_DIRECTORIES(BEFORE ${TalipotCoreBuildInclude})
    INCLUDE_DIRECTORIES(
      ${TalipotCoreInclude} ${TalipotOGLInclude} ${TalipotGUIInclude}
      ${TalipotPythonInclude} ${CMAKE_CURRENT_BINARY_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR})

    QTX_SET_INCLUDES_AND_DEFINITIONS()

    SET(${LibTalipotPythonName}_LIB_SRCS
        ConsoleUtilsModule.cpp
        PythonCodeEditor.cpp
        PythonCodeHighlighter.cpp
        ParenMatcherHighlighter.cpp
        PythonInterpreter.cpp
        PythonShellWidget.cpp
        TalipotUtilsModule.cpp
        APIDataBase.cpp
        AutoCompletionDataBase.cpp
        ConsoleHandlers.h
        PythonEditorsTabWidget.cpp
        PythonCppTypesConverter.cpp
        PythonIDE.cpp
        PythonPluginCreationDialog.cpp
        PythonREPLWidget.cpp)

    SET(${LibTalipotPythonName}_UI
        ../designer/FindReplaceDialog.ui
        ../designer/PythonPluginCreationDialog.ui ../designer/PythonIDE.ui
        ../designer/PythonREPLWidget.ui)

    SET(${LibTalipotPythonName}_MOC
        ConsoleHandlers.h
        ../include/talipot/PythonShellWidget.h
        ../include/talipot/PythonCodeEditor.h
        ../include/talipot/PythonEditorsTabWidget.h
        ../include/talipot/PythonInterpreter.h
        ../include/talipot/PythonIDE.h
        ../include/talipot/PythonPluginCreationDialog.h
        ../include/talipot/PythonREPLWidget.h)

    QTX_WRAP_UI(${LibTalipotPythonName}_UI_SRCS ${${LibTalipotPythonName}_UI})
    QTX_WRAP_CPP(${LibTalipotPythonName}_MOC_SRCS
                 ${${LibTalipotPythonName}_MOC})

    ADD_LIBRARY(
      ${LibTalipotPythonName} SHARED
      ${${LibTalipotPythonName}_LIB_SRCS} ${${LibTalipotPythonName}_MOC_SRCS}
      ${${LibTalipotPythonName}_UI_SRCS})

    TARGET_LINK_LIBRARIES(${LibTalipotPythonName} ${LibTalipotCoreName})
    TARGET_LINK_LIBRARIES(${LibTalipotPythonName} ${LibTalipotGUIName})
    TARGET_LINK_LIBRARIES(${LibTalipotPythonName} ${QT_LIBRARIES})
    TARGET_LINK_LIBRARIES(${LibTalipotPythonName} ${PYTHON_LIBRARY})

    INSTALL(
      TARGETS ${LibTalipotPythonName}
      RUNTIME DESTINATION ${TalipotBinInstallDir}
      LIBRARY DESTINATION ${TalipotLibInstallDir}
      ARCHIVE DESTINATION ${TalipotLibInstallDir})

  ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
