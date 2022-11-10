cmake_minimum_required(VERSION 2.8)
project(REVOCAP_Mesh_Test)
set(REVOCAP_MESH_VERSION "1.7.1")

include_directories(${PROJECT_SOURCE_DIR})

find_package(Boost
  REQUIRED
)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(HelloWorld HelloWorld.cpp)
add_test(
  NAME HelloWorld_Test
  COMMAND $<TARGET_FILE:HelloWorld>
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

set(RCAP_LIBRARIES
  RcapIO
  RcapShape
  RcapMeshGen
  RcapMeshDB
  RcapGeometry
  RcapMatrix
  RcapCommon
)

add_executable(Common_Test Common_Test.cpp)
target_link_libraries(Common_Test ${RCAP_LIBRARIES})
target_compile_definitions(Common_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
  NAME Common_Test
  COMMAND $<TARGET_FILE:Common_Test>
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(Array_Test Array_Test.cpp)
target_link_libraries(Array_Test ${RCAP_LIBRARIES})
target_compile_definitions(Array_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
  NAME Array_Test
  COMMAND $<TARGET_FILE:Array_Test>
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(BLArray_Test BLArray_Test.cpp)
target_link_libraries(BLArray_Test ${RCAP_LIBRARIES})
target_compile_definitions(BLArray_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
  NAME BLArray_Test
  COMMAND $<TARGET_FILE:BLArray_Test>
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(Bucket_Test Bucket_Test.cpp)
target_link_libraries(Bucket_Test ${RCAP_LIBRARIES})
target_compile_definitions(Bucket_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
  NAME Bucket_Test
  COMMAND $<TARGET_FILE:Bucket_Test>
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(Element_Test Element_Test.cpp)
target_link_libraries(Element_Test ${RCAP_LIBRARIES})
target_compile_definitions(Element_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
  NAME Element_Test
  COMMAND $<TARGET_FILE:Element_Test>
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(Geometry_Test Geometry_Test.cpp)
target_link_libraries(Geometry_Test ${RCAP_LIBRARIES})
target_compile_definitions(Geometry_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
  NAME Geometry_Test
  COMMAND $<TARGET_FILE:Geometry_Test>
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(Matrix_Test Matrix_Test.cpp)
target_link_libraries(Matrix_Test ${RCAP_LIBRARIES})
target_compile_definitions(Matrix_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
  NAME Matrix_Test
  COMMAND $<TARGET_FILE:Matrix_Test>
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(MeshData_Test MeshData_Test.cpp)
target_link_libraries(MeshData_Test ${RCAP_LIBRARIES})
target_compile_definitions(MeshData_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
  NAME MeshData_Test
  COMMAND $<TARGET_FILE:MeshData_Test>
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(MeshBrep_Test MeshBrep_Test.cpp)
target_link_libraries(MeshBrep_Test ${RCAP_LIBRARIES})
target_compile_definitions(MeshBrep_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
  NAME MeshBrep_Test
  COMMAND $<TARGET_FILE:MeshBrep_Test>
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_executable(RevocapIO_Test RevocapIO_Test.cpp)
target_link_libraries(RevocapIO_Test ${RCAP_LIBRARIES})
target_compile_definitions(RevocapIO_Test PRIVATE "BOOST_TEST_NO_LIB=1")
add_test(
  NAME RevocapIO_Test
  COMMAND $<TARGET_FILE:RevocapIO_Test>
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
