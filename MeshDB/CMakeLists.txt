cmake_minimum_required(VERSION 3.0)

file(GLOB MeshDB_SRCS *.cpp)

add_library(MeshDB_OBJS OBJECT ${MeshDB_SRCS})
add_library(RcapMeshDB STATIC $<TARGET_OBJECTS:MeshDB_OBJS>)

install(TARGETS RcapMeshDB
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
