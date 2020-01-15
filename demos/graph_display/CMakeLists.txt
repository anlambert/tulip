INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude} ${TalipotGUIInclude})

TALIPOT_SET_CXX_FLAGS("${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

ADD_EXECUTABLE(graph_display graph_display.cpp)
TARGET_LINK_LIBRARIES(graph_display ${TalipotLibs} ${QT_LIBRARIES})

INSTALL(TARGETS graph_display DESTINATION demos)
