cmake_minimum_required(VERSION 3.8)

file(GLOB RevocapIO_SRCS *.cpp)
file(GLOB RevocapIO_HEADERS *.h)

add_library(RevocapIO_OBJS OBJECT ${RevocapIO_SRCS})
add_library(RcapIO STATIC $<TARGET_OBJECTS:RevocapIO_OBJS>)

install(TARGETS RcapIO
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT DESTINATION lib
)

install(FILES ${RevocapIO_HEADERS}
  DESTINATION include/RevocapIO
)
