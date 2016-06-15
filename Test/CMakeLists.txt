cmake_minimum_required(VERSION 2.8)
project(REVOCAP_Refiner_Test)
set(REVOCAP_REFINER_VERSION "1.2.0")

include_directories("${PROJECT_SOURCE_DIR}")

#include_directories("${PROJECT_SOURCE_DIR}")
add_executable(HelloWorld HelloWorld.cpp)
add_test(
 NAME HelloWorld_Test
 COMMAND $<TARGET_FILE:HelloWorld>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/Test
)

add_executable(Common_Test Common_Test.cpp)
target_link_libraries(Common_Test RcapRefiner)
target_link_libraries(Common_Test boost_unit_test_framework-mt)
add_test(
 NAME Common_Test
 COMMAND $<TARGET_FILE:Common_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/Test
)

add_executable(Geometry_Test Geometry_Test.cpp)
target_link_libraries(Geometry_Test RcapRefiner)
target_link_libraries(Geometry_Test boost_unit_test_framework-mt)
add_test(
 NAME Geometry_Test
 COMMAND $<TARGET_FILE:Geometry_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/Test
)

add_executable(Matrix_Test Matrix_Test.cpp)
target_link_libraries(Matrix_Test RcapRefiner)
target_link_libraries(Matrix_Test boost_unit_test_framework-mt)
add_test(
 NAME Matrix_Test
 COMMAND $<TARGET_FILE:Matrix_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/Test
)

add_executable(RevocapIO_Test RevocapIO_Test.cpp)
target_link_libraries(RevocapIO_Test RcapRefiner)
target_link_libraries(RevocapIO_Test boost_unit_test_framework-mt)
add_test(
 NAME RevocapIO_Test
 COMMAND $<TARGET_FILE:RevocapIO_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/Test
)
