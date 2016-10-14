cmake_minimum_required(VERSION 3.0)

file(GLOB MeshGen_SRCS *.cpp)

add_library(MeshGen OBJECT ${MeshGen_SRCS})
add_library(RcapMeshGen STATIC $<TARGET_OBJECTS:MeshGen>)

install(TARGETS RcapMeshGen
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
