cmake_minimum_required(VERSION 3.0)

include(${SWIG_USE_FILE})

set(Shape_INTERFACE
  RevocapShape.i
)

add_definitions(
  -DREVOCAP_SUPPORT_RUBY
)

set_source_files_properties(${Shape_INTERFACE} PROPERTIES CPLUSPLUS ON)

swig_add_module(RevocapShape
  ruby
  ${Shape_INTERFACE}
)

include_directories(
  ${RUBY_INCLUDE_DIRS}
)

if(WITH_OPENCASCADE)
  link_directories($ENV{OCE_DIR})
  list(REMOVE_ITEM OCE_LIBRARIES "TKDraw")
  list(REMOVE_ITEM OCE_LIBRARIES "TKTopTest")
  list(REMOVE_ITEM OCE_LIBRARIES "TKViewerTest")
  list(REMOVE_ITEM OCE_LIBRARIES "TKXSDRAW")
  list(REMOVE_ITEM OCE_LIBRARIES "TKDCAF")
  list(REMOVE_ITEM OCE_LIBRARIES "TKXDEDRAW")
  list(REMOVE_ITEM OCE_LIBRARIES "TKTObjDRAW")
  list(REMOVE_ITEM OCE_LIBRARIES "TKQADraw")
  list(REMOVE_ITEM OCE_LIBRARIES "DRAWEXE")
  include_directories(
    ${OCE_INCLUDE_DIRS}
  )
  swig_link_libraries(RevocapShape
    ${OCE_LIBRARIES}
  )
endif()

swig_link_libraries(RevocapShape
  ${RUBY_LIBRARIES}
  RcapShape
  RcapMeshDB
  RcapGeometry
  RcapMatrix
  RcapCommon
)


install(TARGETS RevocapShape
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
