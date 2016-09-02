cmake_minimum_required(VERSION 3.0)

file(GLOB Common_SRCS *.cpp)

add_library(RcapCommon STATIC ${Common_SRCS})

install(TARGETS RcapCommon
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
