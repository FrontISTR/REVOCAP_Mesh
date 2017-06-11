cmake_minimum_required(VERSION 2.8)
project(REVOCAP_Refiner_Test)
set(REVOCAP_REFINER_VERSION "1.2.0")

include_directories(${PROJECT_SOURCE_DIR})

find_package(Boost
  REQUIRED
  COMPONENTS unit_test_framework
)

#include_directories("${PROJECT_SOURCE_DIR}")
include_directories(${Boost_INCLUDE_DIRS})
add_executable(HelloWorld HelloWorld.cpp)
add_test(
 NAME HelloWorld_Test
 COMMAND HelloWorld
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

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

add_executable(Common_Test Common_Test.cpp)
target_link_libraries(Common_Test ${RCAP_LIBRARIES} ${Boost_LIBRARIES})
add_test(
 NAME Common_Test
 COMMAND $<TARGET_FILE:Common_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(Array_Test Array_Test.cpp)
target_link_libraries(Array_Test RcapRefiner)
#target_link_libraries(Array_Test boost_unit_test_framework-mt)
target_link_libraries(Array_Test ${Boost_LIBRARIES})
add_test(
 NAME Array_Test
 COMMAND $<TARGET_FILE:Array_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/Test
)

add_executable(Geometry_Test Geometry_Test.cpp)
target_link_libraries(Geometry_Test ${RCAP_LIBRARIES} ${Boost_LIBRARIES})
add_test(
 NAME Geometry_Test
 COMMAND $<TARGET_FILE:Geometry_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(Matrix_Test Matrix_Test.cpp)
target_link_libraries(Matrix_Test ${RCAP_LIBRARIES} ${Boost_LIBRARIES})
add_test(
 NAME Matrix_Test
 COMMAND $<TARGET_FILE:Matrix_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(RevocapIO_Test RevocapIO_Test.cpp)
target_link_libraries(RevocapIO_Test ${RCAP_LIBRARIES} ${Boost_LIBRARIES})
add_test(
 NAME RevocapIO_Test
 COMMAND $<TARGET_FILE:RevocapIO_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
