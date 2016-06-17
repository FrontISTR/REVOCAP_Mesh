cmake_minimum_required(VERSION 2.8)
project(REVOCAP_Refiner)
set(REVOCAP_REFINER_VERSION "1.2.0")
enable_language (Fortran)

include_directories("${PROJECT_SOURCE_DIR}")
set(CMAKE_C_FLAGS_RELEASE "-Wall -O2")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_BUILD_TYPE Release)
file(GLOB common_sources   "Common/*.cpp")
file(GLOB matrix_sources   "Matrix/*.cpp")
file(GLOB geometry_sources   "Geometry/*.cpp")
file(GLOB meshdb_sources   "MeshDB/*.cpp")
file(GLOB meshgen_sources   "MeshGen/*.cpp")
file(GLOB revocapio_sources   "RevocapIO/*.cpp")
file(GLOB shape_sources   "Shape/*.cpp")
add_library(RcapRefiner STATIC
	Refiner/rcapRefiner.cpp
	Refiner/rcapRefinerInterface.F90
	${common_sources}
	${matrix_sources}
	${geometry_sources}
	${meshdb_sources}
	${meshgen_sources}
	${revocapio_sources}
	${shape_sources}
)
add_definitions(-DFORTRAN_CALL_C_DOWNCASE_)
add_executable(sampleF Refiner/main.F90)
target_link_libraries(sampleF RcapRefiner)
add_executable(sampleMultiF Refiner/mainMulti.F90)
target_link_libraries(sampleMultiF RcapRefiner)
add_executable(test.rcap Refiner/test.rcap.f)
target_link_libraries(test.rcap RcapRefiner)
#install(TARGETS RcapRefiner LIBRARY DESTINATION lib)
#install(FILES Refiner/rcapRefiner.h DESTINATION include)
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "REVOCAP_Refiner")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "FEM Mesh Refine Tool")
set(CPACK_PACKAGE_VENDOR "The University of Tokyo, CISS Project")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
set(CPACK_PACKAGE_VERSION "${REVOCAP_REFINER_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_SOURCE_IGNORE_FILES "/MeshGL/;/MeshDBRuby/;/MeshGenRuby/;/MeshGLRuby/;/RevocapIORuby/;/ShapeRuby/;/Test/;/ThirdParty/;/VS2012/;/build/;/lib/;/config/;/MakefileRuby.in;/.git/;~$;${CPACK_SOURCE_IGNORE_FILES}")
set(CPACK_SOURCE_GENERATOR TGZ)
include(CPack)

enable_testing()
add_subdirectory(Test)
