cmake_minimum_required(VERSION 3.8)

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
  TYPE MODULE
  LANGUAGE ruby
  SOURCES
    ${RevocapIORuby_INTERFACE}
)

if(APPLE)
  set_target_properties(RevocapIO
    PROPERTIES SUFFIX ".bundle")
else()
  set_target_properties(RevocapIO
    PROPERTIES SUFFIX ".so")
endif()

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
