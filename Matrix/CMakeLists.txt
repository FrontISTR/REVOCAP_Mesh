cmake_minimum_required(VERSION 3.0)

file(GLOB Matrix_SRCS *.cpp)

add_library(Matrix OBJECT ${Matrix_SRCS})
add_library(RcapMatrix STATIC $<TARGET_OBJECTS:Matrix>)

install(TARGETS RcapMatrix
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
