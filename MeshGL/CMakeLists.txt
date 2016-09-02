cmake_minimum_required(VERSION 3.0)

file(GLOB MeshGL_SRCS *.cpp)

add_library(RcapMeshGL STATIC ${MeshGL_SRCS})

install(TARGETS RcapMeshGL
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
