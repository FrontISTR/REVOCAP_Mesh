cmake_minimum_required(VERSION 3.0)

file(GLOB Common_SRCS *.cpp)

add_library(Common OBJECT ${Common_SRCS})
add_library(RcapCommon STATIC $<TARGET_OBJECTS:Common>)

install(TARGETS RcapCommon
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
