cmake_minimum_required(VERSION 3.8)
project(REVOCAP_MESH_Test)
set(REVOCAP_MESH_VERSION "1.7.01")

#find_package(Boost 1.64 COMPONENTS program_options REQUIRED)

find_package(Boost
  REQUIRED
  COMPONENTS unit_test_framework
)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${Boost_INCLUDE_DIR}")

add_executable(HelloWorld HelloWorld.cpp)
add_test(
 NAME HelloWorld_Test
 COMMAND $<TARGET_FILE:HelloWorld>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(Common_Test Common_Test.cpp)
target_link_libraries(Common_Test RcapCommon)
target_compile_definitions(Common_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
 NAME Common_Test
 COMMAND $<TARGET_FILE:Common_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(Array_Test Array_Test.cpp)
target_link_libraries(Array_Test RcapCommon)
target_compile_definitions(Array_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
 NAME Array_Test
 COMMAND $<TARGET_FILE:Array_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(Geometry_Test Geometry_Test.cpp)
target_link_libraries(Geometry_Test RcapGeometry)
target_link_libraries(Geometry_Test RcapMatrix)
target_link_libraries(Geometry_Test RcapCommon)
target_compile_definitions(Geometry_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
 NAME Geometry_Test
 COMMAND $<TARGET_FILE:Geometry_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(Matrix_Test Matrix_Test.cpp)
target_link_libraries(Matrix_Test RcapMatrix)
target_link_libraries(Matrix_Test RcapCommon)
target_compile_definitions(Matrix_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
 NAME Matrix_Test
 COMMAND $<TARGET_FILE:Matrix_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(RevocapIO_Test RevocapIO_Test.cpp)
target_link_libraries(RevocapIO_Test RcapIO)
target_link_libraries(RevocapIO_Test RcapMeshDB)
target_link_libraries(RevocapIO_Test RcapGeometry)
target_link_libraries(RevocapIO_Test RcapMatrix)
target_link_libraries(RevocapIO_Test RcapCommon)
target_compile_definitions(RevocapIO_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
 NAME RevocapIO_Test
 COMMAND $<TARGET_FILE:RevocapIO_Test>
 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
