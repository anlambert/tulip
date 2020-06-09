INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

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
SET_TARGET_PROPERTIES(${VPSCLibrary} PROPERTIES POSITION_INDEPENDENT_CODE ON)
