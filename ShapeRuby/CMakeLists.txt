cmake_minimum_required(VERSION 3.0)

# msys2 の場合、上の行を下の行に修正する必要がある
# similar https://github.com/Alexpux/MINGW-packages/issues/1870
# /mingw64/lib/oce/OCE-libraries-release.cmake:  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "TKernel;TKService;TKV3d;glu32;opengl32;C:/building/msys64/mingw64/lib/libfreetype.dll.a;vfw32"
# /mingw64/lib/oce/OCE-libraries-release.cmake:  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "TKernel;TKService;TKV3d;glu32;opengl32;C:/msys64/mingw64/lib/libfreetype.dll.a;vfw32"

find_package(SWIG REQUIRED)
find_package(Ruby REQUIRED)

include(${SWIG_USE_FILE})

set(Shape_INTERFACE
  RevocapShape.i
)

set_property(SOURCE ${Shape_INTERFACE}
  PROPERTY CPLUSPLUS ON
)

if(WITH_OPENCASCADE)
  set_property(SOURCE ${Shape_INTERFACE}
    PROPERTY SWIG_FLAGS
    "-DOPENCASCADE -DHAVE_CONFIG_H -DHAVE_FSTREAM -DHAVE_IOSTREAM -DHAVE_LIMITS_H -DREVOCAP_RUBY_SUPPORT"
  )
endif()

swig_add_library(RevocapShape
  TYPE MODULE
  LANGUAGE ruby
  SOURCES
    ${Shape_INTERFACE}
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

include_directories(
  ${FREETYPE_INCLUDE_DIRS}
)

if(WITH_OPENCASCADE)
  find_package(freetype REQUIRED)
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
  list(REMOVE_ITEM OCE_LIBRARIES "TKOpenGl")
  list(REMOVE_ITEM OCE_LIBRARIES "FWOSPlugin")

  include_directories(
    ${OCE_INCLUDE_DIRS}
  )
#MESSAGE(STATUS "oce library = ${OCE_LIBRARIES}")
#MESSAGE(STATUS "freetype library = ${FREETYPE_LIBRARIES} INC = ${FREETYPE_INCLUDE_DIRS}")
endif()

swig_link_libraries(RevocapShape
  ${RUBY_LIBRARY}
  RcapShape
  RcapMeshGL
  RcapMeshDB
  RcapGeometry
  RcapMatrix
  RcapCommon
  ${OCE_LIBRARIES}
  ${FREETYPE_LIBRARIES}
)

install(TARGETS RevocapShape
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
