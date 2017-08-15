cmake_minimum_required(VERSION 3.8)

file(GLOB MeshDB_SRCS *.cpp)
file(GLOB MeshDB_HEADERS *.h)

add_library(MeshDB OBJECT ${MeshDB_SRCS})
add_library(RcapMeshDB STATIC $<TARGET_OBJECTS:MeshDB>)

install(TARGETS RcapMeshDB
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)

install(FILES ${MeshDB_HEADERS}
  DESTINATION include/MeshDB
)
