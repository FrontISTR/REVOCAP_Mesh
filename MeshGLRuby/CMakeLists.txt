cmake_minimum_required(VERSION 3.8)

find_package(SWIG REQUIRED)
find_package(Ruby REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include(${SWIG_USE_FILE})
include_directories(
  ${RUBY_INCLUDE_DIRS}
)

set(RevocapGL_INTERFACE
  RevocapGL.i
)

set_property(SOURCE ${RevocapGL_INTERFACE}
  PROPERTY CPLUSPLUS ON
)

set_property(SOURCE ${RevocapGL_INTERFACE}
  PROPERTY SWIG_FLAGS
  "-DREVOCAP_SUPPORT_RUBY"
)

swig_add_library(RevocapGL
  TYPE MODULE
  LANGUAGE ruby
  SOURCES
    ${RevocapGL_INTERFACE}
)

if(APPLE)
  set_target_properties(RevocapGL
    PROPERTIES SUFFIX ".bundle")
else()
  set_target_properties(RevocapGL
    PROPERTIES SUFFIX ".so")
endif()

swig_link_libraries(RevocapGL
  ${RUBY_LIBRARY}
  RcapMeshGL
  RcapMeshDB
  RcapGeometry
  RcapMatrix
  RcapCommon
  ${GLEW_LIBRARIES}
  ${GLU_LIBRARY}
  ${OPENGL_LIBRARIES}
)

install(TARGETS RevocapGL
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
