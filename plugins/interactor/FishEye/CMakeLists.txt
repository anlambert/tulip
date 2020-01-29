INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR} ${TalipotCoreBuildInclude} ${TalipotCoreInclude}
  ${TalipotOGLInclude} ${TalipotGUIInclude} ${GLEW_INCLUDE_DIR})

SET(LIB_SRCS FishEyeInteractor.cpp FishEyeConfigWidget.cpp)

QTX_WRAP_UI(LIB_UI_HEADERS FishEyeConfigWidget.ui)

QTX_WRAP_CPP(LIB_MOC_SRCS)

ADD_LIBRARY(FishEyeInteractor-${TalipotVersion} SHARED
            ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS})

TARGET_LINK_LIBRARIES(FishEyeInteractor-${TalipotVersion} ${LibTalipotCoreName}
                      ${LibTalipotOGLName} ${LibTalipotGUIName})

TALIPOT_INSTALL_PLUGIN(FishEyeInteractor-${TalipotVersion}
                       ${TalipotInteractorPluginsInstallDir})
