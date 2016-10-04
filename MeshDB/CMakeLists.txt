cmake_minimum_required(VERSION 3.0)

file(GLOB MeshDB_SRCS *.cpp)

add_library(MeshDB OBJECT ${MeshDB_SRCS})
add_library(RcapMeshDB STATIC $<TARGET_OBJECTS:MeshDB>)

install(TARGETS RcapMeshDB
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
