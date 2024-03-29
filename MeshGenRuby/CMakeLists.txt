cmake_minimum_required(VERSION 3.0)

find_package(SWIG REQUIRED)
find_package(Ruby REQUIRED)

include(${SWIG_USE_FILE})
include_directories(
  ${RUBY_INCLUDE_DIRS}
)

set(RevocapMeshGen_INTERFACE
  RevocapMeshGen.i
)

set_property(SOURCE ${RevocapMeshGen_INTERFACE}
  PROPERTY CPLUSPLUS ON
)

swig_add_library(RevocapMeshGen
  TYPE SHARED
  LANGUAGE ruby
  SOURCES ${RevocapMeshGen_INTERFACE}
)
swig_link_libraries(RevocapMeshGen
  ${RUBY_LIBRARY}
  RcapMeshGen
  RcapMeshDB
  RcapGeometry
  RcapMatrix
  RcapCommon
)

install(TARGETS RevocapMeshGen
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
