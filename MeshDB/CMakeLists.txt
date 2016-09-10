cmake_minimum_required(VERSION 3.0)

file(GLOB MeshDB_SRCS *.cpp)

add_library(RcapMeshDB STATIC ${MeshDB_SRCS})

install(TARGETS RcapMeshDB
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
