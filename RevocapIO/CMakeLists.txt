cmake_minimum_required(VERSION 3.0)

file(GLOB RevocapIO_SRCS *.cpp)

add_library(RcapIO STATIC ${RevocapIO_SRCS})

install(TARGETS RcapIO
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
