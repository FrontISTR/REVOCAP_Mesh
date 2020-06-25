cmake_minimum_required(VERSION 3.0)

cmake_policy(SET CMP0078 OLD)
cmake_policy(SET CMP0086 OLD)

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

if(WITH_OPENCASCADE)
  set_property(SOURCE ${Shape_INTERFACE}
    PROPERTY SWIG_FLAGS
    "-DOPENCASCADE -DHAVE_CONFIG_H -DHAVE_FSTREAM -DHAVE_IOSTREAM -DHAVE_LIMITS_H -DREVOCAP_RUBY_SUPPORT"
  )
endif()

swig_add_module(RevocapShape
  ruby
  ${Shape_INTERFACE}
)

include_directories(
  ${RUBY_INCLUDE_DIRS}
)

if(WITH_OPENCASCADE)
  link_directories($ENV{OCE_DIR})
  list(REMOVE_ITEM OCE_LIBRARIES "DRAWEXE")
  list(REMOVE_ITEM OCE_LIBRARIES "TKDraw")
  list(REMOVE_ITEM OCE_LIBRARIES "TKTopTest")
  list(REMOVE_ITEM OCE_LIBRARIES "TKViewerTest")
  list(REMOVE_ITEM OCE_LIBRARIES "TKXSDRAW")
  list(REMOVE_ITEM OCE_LIBRARIES "TKDCAF")
  list(REMOVE_ITEM OCE_LIBRARIES "TKXDEDRAW")
  list(REMOVE_ITEM OCE_LIBRARIES "TKTObjDRAW")
  list(REMOVE_ITEM OCE_LIBRARIES "TKQADraw")

  include_directories(
    ${OCE_INCLUDE_DIRS}
  )
  set(EXTRA_LIBS
    ${OCE_LIBRARIES}
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
