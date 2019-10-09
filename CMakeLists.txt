cmake_minimum_required(VERSION 3.8)
project(REVOCAP_MESH CXX C Fortran)
set(REVOCAP_Mesh_VERSION_MAJOR 1 CACHE STRING "Major version number of REVOCAP_Mesh")
set(REVOCAP_Mesh_VERSION_MINOR 7 CACHE STRING "Minor version number of REVOCAP_Mesh")
set(REVOCAP_Mesh_VERSION_PATCH 1 CACHE STRING "Patch version number of REVOCAP_Mesh")

option(WITH_EXTENSION "Build with ruby extension libraries" ON)
option(WITH_OPENCASCADE "Build with OpenCASCADE feature" ON)
option(WITH_OPENMP "Enable OpenMP feature" ON)
option(WITH_TEST "Enable tests" ON)
option(WITH_DOC "Generate API documents." ON)

if(WITH_EXTENSION)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  message(STATUS "WITH_EXTENSION=ON")
  message(STATUS "CMAKE_POSITION_INDEPENDENT_CODE=ON")
endif()

if(WITH_OPENCASCADE)
  set(CMAKE_MODULE_PATH $ENV{OCE_DIR})
  find_package(OCE REQUIRED)
  if(OCE_ALL_FOUND)
    message(STATUS "Found OpenCASCADE Community Edition: ${OCE_DIR}")
    add_definitions(
      -DOPENCASCADE
      -DHAVE_CONFIG_H
      -DHAVE_FSTREAM
      -DHAVE_IOSTREAM
      -DHAVE_LIMITS_H
      -DREVOCAP_SUPPORT_GLEW
    )
  else()
    message(WARNING "OpenCASCADE Community Edition not Found!!\n"
      " Please set directory to enviornment variable named OCE_DIR\n"
      " at placed on OCEConfig.cmake\n"
      "ex.) % find /usr -name OCEConfig.cmake\n"
      "       ./lib/x86_64-linux-gnu/oce-0.17/OCEConfig.cmake\n"
      "     % export OCE_DIR=/usr/lib/x86_64-linux-gnu/oce-0.7\n")
  endif()
endif()

if(WITH_OPENMP)
  find_package(OpenMP)
  if(OPENMP_FOUND)
    message(STATUS "WITH_OPENMP=ON")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  endif()
endif()

include_directories(
  ${CMAKE_SOURCE_DIR}
)

set(CMAKE_BUILD_TYPE "RELEASE")
set(CMAKE_C_FLAGS_RELEASE "-Wall -O2")
set(CMAKE_C_FLAGS_DEBUG "-Wall -g")

if(CMAKE_BUILD_TYPE MATCHES "RELEASE")
  add_definitions(
    -DNDEBUG
  )
elseif(CMAKE_BUILD_TYPE MATCHES "DEBUG")
  add_definitions(
    -D_DEBUG=3
  )
endif()

# Build static libraries
add_subdirectory(Common)    # RcapCommon target, static library
add_subdirectory(Matrix)    # RcapMatrix target, static library
add_subdirectory(Geometry)  # RcapGeometry target, static library
add_subdirectory(MeshDB)    # RcapMeshDB target, static library
add_subdirectory(RevocapIO) # RcapIO target,static library
add_subdirectory(MeshGL)    # RcapMeshGL target, static library
add_subdirectory(MeshGen)   # RcapMeshGen target, static library
add_subdirectory(Shape)     # RcapShape target, static library
add_subdirectory(Refiner)

# Build swig modules
if(WITH_EXTENSION)
  find_package(SWIG REQUIRED)
  find_package(Ruby REQUIRED)
  if(RUBY_FOUND AND SWIG_FOUND)
    message(STATUS "Found ${RUBY_EXECUTABLE} (${RUBY_VERSION})")
    message(STATUS "Found ${SWIG_EXECUTABLE} (${SWIG_VERSION})")
    add_subdirectory(MeshDBRuby)    # RcapMesh target, swig module
    add_dependencies(RevocapMesh RcapMeshDB RcapGeometry RcapMatrix RcapCommon)
    add_subdirectory(RevocapIORuby) # RevocapIO target, swig module
    add_dependencies(RevocapIO RcapIO RcapMeshDB RcapGeometry RcapMatrix RcapCommon)
    add_subdirectory(MeshGLRuby)    # RevocapGL target, swig module
    add_dependencies(RevocapGL RcapMeshGL RcapMeshDB RcapMeshDB RcapGeometry RcapMatrix RcapCommon)
    add_subdirectory(MeshGenRuby)   # RevocapMeshGen target, swig module
    add_dependencies(RevocapMeshGen RcapMeshGen RcapMeshDB RcapGeometry RcapMatrix RcapCommon)
    add_subdirectory(ShapeRuby)     # RevocapShape target, swig module
    add_dependencies(RevocapShape RcapShape RcapMeshDB RcapGeometry RcapMatrix RcapCommon)
  endif()
endif()

# Test
if(WITH_TEST)
  message(STATUS "WITH_TEST=ON")
  enable_testing()
  add_subdirectory(Test)
  if(WITH_EXTENSION)
    add_subdirectory(TestRuby)
  endif()
endif()

# -DWITH_DOC
if(WITH_DOC)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    message(STATUS "Generate API documents. Please type \"make Documents\"")
    set(doxy_main_page "${CMAKE_SOURCE_DIR}/INSTALL.md")
    set(doxyfile_in ${CMAKE_SOURCE_DIR}/Documents/Doxyfile.in)
    set(doxyfile ${CMAKE_BINARY_DIR}/Documents/Doxyfile)
    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(Documents
      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Generate API documents."
      VERBATIM
    )
  else()
    message(WARNING "doxygen not found.")
  endif()
endif()