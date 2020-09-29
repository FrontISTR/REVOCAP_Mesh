cmake_minimum_required(VERSION 3.8)

file(GLOB MeshGL_SRCS *.cpp)
file(GLOB MeshGen_HEADERS *.h)

add_library(MeshGL_OBJS OBJECT ${MeshGL_SRCS})
add_library(RcapMeshGL STATIC $<TARGET_OBJECTS:MeshGL_OBJS>)

install(TARGETS RcapMeshGL
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)

install(FILES ${MeshGL_HEADERS}
  DESTINATION include/MeshGL
)
