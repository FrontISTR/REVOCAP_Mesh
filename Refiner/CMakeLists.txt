cmake_minimum_required(VERSION 3.8)
project(REVOCAP_Refiner C CXX Fortran)
set(REVOCAP_REFINER_VERSION "1.2.0")

include_directories("${PROJECT_SOURCE_DIR}/..")
include_directories("${PROJECT_SOURCE_DIR}")

set(refiner_sources
  rcapRefiner.cpp
  rcapRefinerIO.cpp
  rcapRefinerInterface.F90
)

set(refiner_headers
  rcapRefiner.h
  rcapRefinerIO.h
)

add_library(Refiner OBJECT ${refiner_sources})
add_library(RcapRefiner STATIC
  $<TARGET_OBJECTS:Refiner>
  $<TARGET_OBJECTS:Common_OBJS>
  $<TARGET_OBJECTS:Matrix_OBJS>
  $<TARGET_OBJECTS:Geometry_OBJS>
  $<TARGET_OBJECTS:MeshDB_OBJS>
  $<TARGET_OBJECTS:MeshGen_OBJS>
  $<TARGET_OBJECTS:RevocapIO_OBJS>
  $<TARGET_OBJECTS:Shape_OBJS>
)

# Refiner Utils
add_definitions(-D_CONSOLE)
add_executable(rcapPreFitting mainPreFitting.cpp)
target_link_libraries(rcapPreFitting RcapRefiner)

add_subdirectory(Samples)

install(TARGETS RcapRefiner
  EXPORT RcapRefinerConfig
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  INCLUDES DESTINATION include
  COMPONENT DESTINATION lib
)

install(FILES ${refiner_headers}
  DESTINATION include
)

install(EXPORT RcapRefinerConfig
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/
  EXPORT_LINK_INTERFACE_LIBRARIES
)


# -DBUILD_DOC
#if(BUILD_DOC)
#  find_package(Doxygen)
#  if(DOXYGEN_FOUND)
#    message(STATUS "Generate API documents. Please type \"make doc\"")
#    set(doxy_main_page "${CMAKE_SOURCE_DIR}/INSTALL.md")
#    set(doxyfile_in ${CMAKE_SOURCE_DIR}/Documents/Doxyfile.in)
#    set(doxyfile ${CMAKE_BINARY_DIR}/Documents/Doxyfile)
#    configure_file(${doxyfile_in} ${doxyfile} @ONLY)
#
#    add_custom_target(doc
#      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
#      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#      COMMENT "Generate API documents."
#      VERBATIM
#    )
#  else()
#    message(WARNING "doxygen not found.")
#  endif()
#endif()

# Package
# % cpack
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "REVOCAP_Refiner")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "FEM Mesh Refiner Tool")
set(CPACK_PACKAGE_VENDOR "The University of Tokyo, CISS Project")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
set(CPACK_PACKAGE_VERSION "${REVOCAP_REFINER_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")
#set(CPACK_SOURCE_IGNORE_FILES "/MeshGL/;/MeshDBRuby/;/MeshGenRuby/;/MeshGLRuby/;/RevocapIORuby/;/ShapeRuby/;/Test/;/ThirdParty/;/VS2012/;/build/;/lib/;/config/;/MakefileRuby.in;/.git/;~$;${CPACK_SOURCE_IGNORE_FILES}")
set(CPACK_SOURCE_GENERATOR TGZ)
include(CPack)

