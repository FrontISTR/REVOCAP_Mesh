cmake_minimum_required(VERSION 3.0)

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

add_definitions(
  -DREVOCAP_SUPPORT_RUBY
)

set_source_files_properties(${RevocapGL_INTERFACE} PROPERTIES CPLUSPLUS ON)
swig_add_module(RevocapGL
  ruby
  ${RevocapGL_INTERFACE}
)
swig_link_libraries(RevocapGL
  ${RUBY_LIBRARIES}
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
