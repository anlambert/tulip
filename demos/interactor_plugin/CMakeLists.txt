FIND_PACKAGE(Talipot REQUIRED)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

QT_ADD_RESOURCES(LIB_RCC_SRCS interactor_plugin_resource.qrc)

INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude}
                    ${TalipotOGLInclude} ${TalipotGUIInclude})

ADD_LIBRARY(interactor_plugin-${TALIPOT_VERSION} SHARED interactor_plugin.cpp
                                                        ${LIB_RCC_SRCS})

TARGET_LINK_LIBRARIES(interactor_plugin-${TALIPOT_VERSION} ${TALIPOT_LIBRARIES}
                      ${QT_LIBRARIES})

INSTALL(TARGETS interactor_plugin-${TALIPOT_VERSION}
        DESTINATION ${TALIPOT_PLUGINS_DIR})
