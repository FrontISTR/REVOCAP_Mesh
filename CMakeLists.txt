cmake_minimum_required(VERSION 2.8)
project(REVOCAP_Refiner)
set(REVOCAP_REFINER_VERSION "1.2.0")
enable_language (Fortran)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/Refiner")
set(CMAKE_C_FLAGS_RELEASE "-Wall -O2")
set(CMAKE_C_FLAGS_DEBUG "-Wall -g")
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
	Refiner/rcapRefinerIO.cpp
	Refiner/rcapRefinerInterface.F90
	${common_sources}
	${matrix_sources}
	${geometry_sources}
	${meshdb_sources}
	${meshgen_sources}
	${revocapio_sources}
	${shape_sources}
)
#add_definitions(-DFORTRAN_CALL_C_DOWNCASE_)
add_definitions(-D_CONSOLE)
# C++ Samples
add_executable(sampleC Refiner/Samples/main.cpp)
target_link_libraries(sampleC RcapRefiner)
add_executable(sampleMultiStage Refiner/Samples/mainMultiStage.cpp)
target_link_libraries(sampleMultiStage RcapRefiner)
add_executable(sampleNodeVariable Refiner/Samples/mainNV.cpp)
target_link_libraries(sampleNodeVariable RcapRefiner)
add_executable(sampleTetra Refiner/Samples/mainTetra.cpp)
target_link_libraries(sampleTetra RcapRefiner)
add_executable(sampleHexa Refiner/Samples/mainHexa.cpp)
target_link_libraries(sampleHexa RcapRefiner)
add_executable(sampleHexa2 Refiner/Samples/mainHexa2.cpp)
target_link_libraries(sampleHexa2 RcapRefiner)
add_executable(sampleMultiType Refiner/Samples/mainMultiType.cpp)
target_link_libraries(sampleMultiType RcapRefiner)
add_executable(sampleMultiByType Refiner/Samples/mainMultiByType.cpp)
target_link_libraries(sampleMultiByType RcapRefiner)
add_executable(sampleFittingRefine1 Refiner/Samples/mainFittingRefine1.cpp)
target_link_libraries(sampleFittingRefine1 RcapRefiner)
add_executable(sampleFittingRefine2 Refiner/Samples/mainFittingRefine2.cpp)
target_link_libraries(sampleFittingRefine2 RcapRefiner)
add_executable(sampleFittingRefine3 Refiner/Samples/mainFittingRefine3.cpp)
target_link_libraries(sampleFittingRefine3 RcapRefiner)
add_executable(sampleSmoothing Refiner/Samples/mainSmoothingRefine.cpp)
target_link_libraries(sampleSmoothing RcapRefiner)
# Fortran Samples
add_executable(sampleF Refiner/Samples/main.F90)
target_link_libraries(sampleF RcapRefiner)
add_executable(sampleMultiF Refiner/Samples/mainMulti.F90)
target_link_libraries(sampleMultiF RcapRefiner)
add_executable(test.rcap Refiner/Samples/test.rcap.f)
target_link_libraries(test.rcap RcapRefiner)
# Refiner Utils
add_executable(rcapPreFitting Refiner/mainPreFitting.cpp)
target_link_libraries(rcapPreFitting RcapRefiner)
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
