cmake_minimum_required(VERSION 3.0)

file(GLOB MeshGL_SRCS *.cpp)

add_library(MeshGL_OBJS OBJECT ${MeshGL_SRCS})
add_library(RcapMeshGL STATIC $<TARGET_OBJECTS:MeshGL_OBJS>)

add_definitions(-DGL_SILENCE_DEPRECATION)

install(TARGETS RcapMeshGL
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
