INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

IF(UNIX)
  TALIPOT_SET_CXX_FLAGS("-fPIC")
ENDIF(UNIX)

SET(LIB_SRCS
    block.cpp
    blocks.cpp
    constraint.cpp
    generate-constraints.cpp
    PairingHeap.cpp
    remove_rectangle_overlap.cpp
    solve_VPSC.cpp
    variable.cpp)

ADD_LIBRARY(${VPSCLibrary} STATIC ${LIB_SRCS})
