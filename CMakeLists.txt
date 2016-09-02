cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE "RELEASE")

option(WITH_EXTENSION "Build with ruby extension libraries" ON)
option(WITH_OPENCASCADE "Build with OpenCASCADE feature" ON)
option(WITH_OPENMP "Enable OpenMP feature" ON)
option(WITH_TEST "Enable tests" ON)

if(WITH_EXTENSION)
  find_package(SWIG REQUIRED)
  find_package(Ruby REQUIRED)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
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
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  endif()
endif()

include_directories(
  ${CMAKE_SOURCE_DIR}
)

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
add_subdirectory(Refiner)   # RcapRefiner target, static library
add_dependencies(RcapRefiner RcapCommon RcapMatrix RcapGeometry RcapMeshDB RcapIO RcapShape)

# Build swig modules
if(WITH_EXTENSION)
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

# Test
if(WITH_TEST)
  enable_testing()
  add_subdirectory(Test)
endif()

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
set(CPACK_SOURCE_IGNORE_FILES "/MeshGL/;/MeshDBRuby/;/MeshGenRuby/;/MeshGLRuby/;/RevocapIORuby/;/ShapeRuby/;/Test/;/ThirdParty/;/VS2012/;/build/;/lib/;/config/;/MakefileRuby.in;/.git/;~$;${CPACK_SOURCE_IGNORE_FILES}")
set(CPACK_SOURCE_GENERATOR TGZ)
include(CPack)
