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

set_source_files_properties(${RevocapMeshGen_INTERFACE} PROPERTIES CPLUSPLUS ON)
swig_add_module(RevocapMeshGen
  ruby
  ${RevocapMeshGen_INTERFACE}
)
swig_link_libraries(RevocapMeshGen
  ${RUBY_LIBRARIES}
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
