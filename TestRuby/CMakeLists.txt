cmake_minimum_required(VERSION 3.0)
project(REVOCAP_MESH_TestRuby)
set(REVOCAP_MESH_VERSION "1.7.01")

add_test(
 NAME HelloWorld_TestRuby
 COMMAND ruby ${PROJECT_SOURCE_DIR}/HelloWorld.rb
 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_test(
 NAME LoadLibrary_TestRuby
 COMMAND ruby -I MeshDBRuby:MeshGenRuby:MeshGLRuby:RevocapIORuby:ShapeRuby ${PROJECT_SOURCE_DIR}/LoadLibrary_Test.rb
 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
