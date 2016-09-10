cmake_minimum_required(VERSION 3.0)

file(GLOB MeshGen_SRCS *.cpp)

add_library(RcapMeshGen STATIC ${MeshGen_SRCS})

install(TARGETS RcapMeshGen
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
