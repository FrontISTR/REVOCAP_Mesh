cmake_minimum_required(VERSION 3.0)

include(${SWIG_USE_FILE})

set(Shape_INTERFACE
  RevocapShape.i
)

#add_definitions(
#  -DREVOCAP_SUPPORT_RUBY
#)

set_property(SOURCE ${Shape_INTERFACE}
  PROPERTY CPLUSPLUS ON
)

if(OPENCASCADE_FOUND)
  set_property(SOURCE ${Shape_INTERFACE}
    PROPERTY SWIG_FLAGS
    "-DOPENCASCADE -DHAVE_CONFIG_H -DHAVE_FSTREAM -DHAVE_IOSTREAM -DHAVE_LIMITS_H -DREVOCAP_RUBY_SUPPORT"
  )
endif()

swig_add_library(RevocapShape
  TYPE SHARED
  LANGUAGE ruby
  SOURCES ${Shape_INTERFACE}
)

if(APPLE)
  set_target_properties(RevocapShape
    PROPERTIES SUFFIX ".bundle")
else()
  set_target_properties(RevocapShape
    PROPERTIES SUFFIX ".so")
endif()

include_directories(
  ${RUBY_INCLUDE_DIRS}
)

if(OPENCASCADE_FOUND OR OCE_ALL_FOUND)
  link_directories($ENV{OPENCASCADE_DIR})
  list(REMOVE_ITEM OpenCASCADE_LIBRARIES "DRAWEXE")
  list(REMOVE_ITEM OpenCASCADE_LIBRARIES "TKDraw")
  list(REMOVE_ITEM OpenCASCADE_LIBRARIES "TKTopTest")
  list(REMOVE_ITEM OpenCASCADE_LIBRARIES "TKViewerTest")
  list(REMOVE_ITEM OpenCASCADE_LIBRARIES "TKXSDRAW")
  list(REMOVE_ITEM OpenCASCADE_LIBRARIES "TKDCAF")
  list(REMOVE_ITEM OpenCASCADE_LIBRARIES "TKXDEDRAW")
  list(REMOVE_ITEM OpenCASCADE_LIBRARIES "TKTObjDRAW")
  list(REMOVE_ITEM OpenCASCADE_LIBRARIES "TKQADraw")

  include_directories(
    ${OPENCASCADE_INCLUDE_DIRS}
    ${OpenCASCADE_INCLUDE_DIR}
  )
  set(EXTRA_LIBS
    ${OpenCASCADE_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLU_LIBRARY}
    ${OPENGL_LIBRARIES}
  )
#  swig_link_libraries(RevocapShape
#    ${OCE_LIBRARIES}
#  )
endif()

swig_link_libraries(RevocapShape
  ${RUBY_LIBRARY}
  RcapShape
  RcapMeshGL
  RcapMeshDB
  RcapGeometry
  RcapMatrix
  RcapCommon
  ${EXTRA_LIBS}
)

install(TARGETS RevocapShape
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
