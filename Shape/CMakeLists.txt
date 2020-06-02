cmake_minimum_required(VERSION 3.0)

include_directories(
  ${OCE_INCLUDE_DIRS}
)

file(GLOB Shape_SRCS *.cpp)

add_library(Shape_OBJS OBJECT ${Shape_SRCS})
add_library(RcapShape STATIC $<TARGET_OBJECTS:Shape_OBJS>)

install(TARGETS RcapShape
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
