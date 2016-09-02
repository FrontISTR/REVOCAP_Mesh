cmake_minimum_required(VERSION 3.0)

file(GLOB Matrix_SRCS *.cpp)

add_library(RcapMatrix STATIC ${Matrix_SRCS})

install(TARGETS RcapMatrix
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
