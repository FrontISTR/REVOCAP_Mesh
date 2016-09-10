cmake_minimum_required(VERSION 3.0)

file(GLOB Geometry_SRCS *.cpp)

add_library(RcapGeometry STATIC ${Geometry_SRCS})

install(TARGETS RcapGeometry
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
