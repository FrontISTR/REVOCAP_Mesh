cmake_minimum_required(VERSION 3.0)

include_directories(
  ${OCE_INCLUDE_DIRS}
)

file(GLOB Shape_SRCS *.cpp)

add_library(RcapShape STATIC ${Shape_SRCS})

install(TARGETS RcapShape
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)
