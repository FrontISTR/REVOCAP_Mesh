cmake_minimum_required(VERSION 3.0)

cmake_policy(SET CMP0078 OLD)
cmake_policy(SET CMP0086 OLD)

find_package(SWIG REQUIRED)
find_package(Ruby REQUIRED)
include(${SWIG_USE_FILE})
include_directories(
  ${RUBY_INCLUDE_DIRS}
)

set(MeshDBRuby_INTERFACE
  Revocap.i
)

set_property(SOURCE ${MeshDBRuby_INTERFACE}
  PROPERTY CPLUSPLUS ON
)

swig_add_module(RevocapMesh
  ruby
  ${MeshDBRuby_INTERFACE}
  kmbMeshDB_SWIG.h
  kmbPhysicalValue_SWIG.h
)
swig_link_libraries(RevocapMesh
  ${RUBY_LIBRARY}
  RcapMeshDB
  RcapGeometry
  RcapMatrix
  RcapCommon
)

install(TARGETS RevocapMesh
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
