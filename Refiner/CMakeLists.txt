cmake_minimum_required(VERSION 3.0)

project(REVOCAP_Refiner C CXX Fortran)

set(REVOCAP_REFINER_VERSION "1.2.0")

set(Refiner_SRCS
  rcapRefiner.cpp
  rcapRefinerInterface.F90
)

add_library(RcapRefiner STATIC ${Refiner_SRCS})
add_definitions(-D_CONSOLE)
set(RCAP_LIBRARIES
  RcapRefiner
  RcapIO
  RcapShape
  RcapMeshGen
  RcapMeshDB
  RcapGeometry
  RcapMatrix
  RcapCommon
)
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(sampleC main.cpp)
target_link_libraries(sampleC ${RCAP_LIBRARIES})
add_executable(sampleMultiStage mainMultiStage.cpp)
target_link_libraries(sampleMultiStage ${RCAP_LIBRARIES})
add_executable(sampleNodeVariable mainNV.cpp)
target_link_libraries(sampleNodeVariable ${RCAP_LIBRARIES})
add_executable(sampleTetra mainTetra.cpp)
target_link_libraries(sampleTetra ${RCAP_LIBRARIES})
add_executable(sampleHexa mainHexa.cpp)
target_link_libraries(sampleHexa ${RCAP_LIBRARIES})
add_executable(sampleHexa2 mainHexa2.cpp)
target_link_libraries(sampleHexa2 ${RCAP_LIBRARIES})
add_executable(sampleMultiType mainMultiType.cpp)
target_link_libraries(sampleMultiType ${RCAP_LIBRARIES})
add_executable(sampleMultiByType mainMultiByType.cpp)
target_link_libraries(sampleMultiByType ${RCAP_LIBRARIES})
add_executable(sampleFittingRefine1 mainFittingRefine1.cpp)
target_link_libraries(sampleFittingRefine1 ${RCAP_LIBRARIES})
add_executable(sampleFittingRefine2 mainFittingRefine2.cpp)
target_link_libraries(sampleFittingRefine2 ${RCAP_LIBRARIES})
add_executable(sampleFittingRefine3 mainFittingRefine3.cpp)
target_link_libraries(sampleFittingRefine3 ${RCAP_LIBRARIES})
add_executable(sampleSmoothing mainSmoothingRefine.cpp)
target_link_libraries(sampleSmoothing ${RCAP_LIBRARIES})
add_executable(rcapPreFitting mainPreFitting.cpp)
target_link_libraries(rcapPreFitting ${RCAP_LIBRARIES})

add_executable(sampleF main.F90)
target_link_libraries(sampleF ${RCAP_LIBRARIES})
add_executable(sampleMultiF mainMulti.F90)
target_link_libraries(sampleMultiF ${RCAP_LIBRARIES})
add_executable(test.rcap test.rcap.f)
target_link_libraries(test.rcap ${RCAP_LIBRARIES})

install(TARGETS RcapRefiner
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
install(FILES rcapRefiner.h DESTINATION include)
