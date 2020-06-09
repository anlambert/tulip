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

ADD_LIBRARY(som STATIC ${LIB_SRCS})
SET_TARGET_PROPERTIES(som PROPERTIES POSITION_INDEPENDENT_CODE ON)

TARGET_LINK_LIBRARIES(som ${LibTalipotCoreName} ${LibTalipotOGLName})
