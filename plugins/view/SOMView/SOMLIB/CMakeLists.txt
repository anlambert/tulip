SET(LIB_SRCS
    DiffusionRateFunction.cpp
    DiffusionRateFunctionSimple.cpp
    DiffusionRateFunctionGaussian.cpp
    InputSample.cpp
    SOMAlgorithm.cpp
    SOMMap.cpp
    TimeDecreasingFunctionSimple.cpp)

INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude}
                    ${TalipotOGLInclude} ${CMAKE_CURRENT_SOURCE_DIR})
IF(UNIX)
  TALIPOT_SET_CXX_FLAGS("-fPIC")
ENDIF(UNIX)

ADD_LIBRARY(som STATIC ${LIB_SRCS})

TARGET_LINK_LIBRARIES(som ${LibTalipotCoreName} ${LibTalipotOGLName})
