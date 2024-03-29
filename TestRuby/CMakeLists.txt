﻿cmake_minimum_required(VERSION 3.0)
project(REVOCAP_MESH_TestRuby)

add_test(
 NAME HelloWorld_TestRuby
 COMMAND ruby ${PROJECT_SOURCE_DIR}/HelloWorld.rb
 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_test(
 NAME LoadLibrary_TestRuby
 COMMAND ruby -I MeshDBRuby -I MeshGenRuby -I MeshGLRuby -I RevocapIORuby -I ShapeRuby ${PROJECT_SOURCE_DIR}/LoadLibrary_Test.rb
 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_test(
 NAME Element_TestRuby
 COMMAND ruby -I MeshDBRuby -I MeshGenRuby -I MeshGLRuby -I RevocapIORuby -I ShapeRuby -I ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/Element_Test.rb
 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_test(
 NAME MeshData_TestRuby
 COMMAND ruby -I MeshDBRuby -I MeshGenRuby -I MeshGLRuby -I RevocapIORuby -I ShapeRuby -I ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/MeshData_Test.rb
 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_test(
 NAME BoundaryExtract_TestRuby
 COMMAND ruby -I MeshDBRuby -I MeshGenRuby -I MeshGLRuby -I RevocapIORuby -I ShapeRuby -I ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/BoundaryExtract_Test.rb
 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

if(WITH_OPENCASCADE)
 add_test(
  NAME LoadRevocapShape_TestRuby
  COMMAND ruby -I MeshDBRuby -I MeshGenRuby -I MeshGLRuby -I RevocapIORuby -I ShapeRuby ${PROJECT_SOURCE_DIR}/LoadRevocapShape_Test.rb
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
 )
endif()
