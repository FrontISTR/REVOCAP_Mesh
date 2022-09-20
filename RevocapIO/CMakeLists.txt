cmake_minimum_required(VERSION 3.0)

file(GLOB RevocapIO_SRCS *.cpp)

add_library(RevocapIO_OBJS OBJECT ${RevocapIO_SRCS})
add_library(RcapIO STATIC $<TARGET_OBJECTS:RevocapIO_OBJS>)

install(TARGETS RcapIO
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
