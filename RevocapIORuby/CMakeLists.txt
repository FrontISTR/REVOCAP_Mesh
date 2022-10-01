cmake_minimum_required(VERSION 3.0)

find_package(SWIG REQUIRED)
find_package(Ruby REQUIRED)
include(${SWIG_USE_FILE})
include_directories(
  ${RUBY_INCLUDE_DIRS}
)

set(RevocapIORuby_INTERFACE
  RevocapIO.i
)

set_property(SOURCE ${RevocapIORuby_INTERFACE}
  PROPERTY CPLUSPLUS ON
)

swig_add_library(RevocapIO
  TYPE SHARED
  LANGUAGE ruby
  SOURCES ${RevocapIORuby_INTERFACE}
)
swig_link_libraries(RevocapIO
  ${RUBY_LIBRARY}
  RcapIO
  RcapMeshDB
  RcapGeometry
  RcapMatrix
  RcapCommon
)

install(TARGETS RevocapIO
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
