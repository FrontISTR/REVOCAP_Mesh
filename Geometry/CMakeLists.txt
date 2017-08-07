cmake_minimum_required(VERSION 3.0)

file(GLOB Geometry_SRCS *.cpp)
file(GLOB Geometry_HEADERS *.h)

add_library(Geometry OBJECT ${Geometry_SRCS})
add_library(RcapGeometry STATIC $<TARGET_OBJECTS:Geometry>)

install(TARGETS RcapGeometry
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)

install(FILES ${Geometry_HEADERS}
  DESTINATION include/Geometry
)
