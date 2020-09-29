cmake_minimum_required(VERSION 3.8)

file(GLOB MeshGen_SRCS *.cpp)
file(GLOB MeshGen_HEADERS *.h)

add_library(MeshGen_OBJS OBJECT ${MeshGen_SRCS})
add_library(RcapMeshGen STATIC $<TARGET_OBJECTS:MeshGen_OBJS>)

install(TARGETS RcapMeshGen
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)

install(FILES ${MeshGen_HEADERS}
  DESTINATION include/MeshGen
)
