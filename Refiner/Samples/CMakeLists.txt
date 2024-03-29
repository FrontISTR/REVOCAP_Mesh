cmake_minimum_required(VERSION 2.8)
project(REVOCAP_Refiner_Sample C Fortran)
set(REVOCAP_REFINER_VERSION "1.2.0")

include_directories("${PROJECT_SOURCE_DIR}/..")
include_directories("${PROJECT_BINARY_DIR}/..")
set(CMAKE_C_FLAGS_RELEASE "-Wall -O2")
set(CMAKE_C_FLAGS_DEBUG "-Wall -g")

remove_definitions(-DNDEBUG)
add_definitions(-D_CONSOLE)
# C Samples
add_executable(sampleC main.c)
target_link_libraries(sampleC RcapRefiner)
add_executable(sampleMultiStage mainMultiStage.c)
target_link_libraries(sampleMultiStage RcapRefiner)
add_executable(sampleNodeVariable mainNV.c)
target_link_libraries(sampleNodeVariable RcapRefiner)
add_executable(sampleTetra mainTetra.c)
target_link_libraries(sampleTetra RcapRefiner)
add_executable(sampleHexa mainHexa.c)
target_link_libraries(sampleHexa RcapRefiner)
add_executable(sampleHexa2 mainHexa2.c)
target_link_libraries(sampleHexa2 RcapRefiner)
add_executable(sampleMultiType mainMultiType.c)
target_link_libraries(sampleMultiType RcapRefiner)
add_executable(sampleMultiByType mainMultiByType.c)
target_link_libraries(sampleMultiByType RcapRefiner)
add_executable(sampleFittingRefine1 mainFittingRefine1.c)
target_link_libraries(sampleFittingRefine1 RcapRefiner)
add_executable(sampleFittingRefine2 mainFittingRefine2.c)
target_link_libraries(sampleFittingRefine2 RcapRefiner)
add_executable(sampleFittingRefine3 mainFittingRefine3.c)
target_link_libraries(sampleFittingRefine3 RcapRefiner)
add_executable(sampleSmoothing mainSmoothingRefine.c)
target_link_libraries(sampleSmoothing RcapRefiner)

# Fortran Samples
add_executable(sampleF main.F90)
target_link_libraries(sampleF RcapRefiner)
set_target_properties(sampleF PROPERTIES LINKER_LANGUAGE Fortran)
add_executable(sampleIncF main_inc.F90)
target_link_libraries(sampleIncF RcapRefiner)
set_target_properties(sampleIncF PROPERTIES LINKER_LANGUAGE Fortran)
add_executable(sampleMultiF mainMulti.F90)
target_link_libraries(sampleMultiF RcapRefiner)
set_target_properties(sampleMultiF PROPERTIES LINKER_LANGUAGE Fortran)
add_executable(test.rcap test.rcap.f)
target_link_libraries(test.rcap RcapRefiner)
set_target_properties(test.rcap PROPERTIES LINKER_LANGUAGE Fortran)

if(WITH_TEST)
  add_test(
    NAME sampleC
    COMMAND $<TARGET_FILE:sampleC>
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
  add_test(
    NAME sampleTetra
    COMMAND $<TARGET_FILE:sampleTetra>
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
  add_test(
    NAME sampleHexa
    COMMAND $<TARGET_FILE:sampleHexa>
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
  add_test(
    NAME sampleMultiStage
    COMMAND $<TARGET_FILE:sampleMultiStage>
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
  add_test(
    NAME sampleNodeVariable
    COMMAND $<TARGET_FILE:sampleNodeVariable>
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
  add_test(
    NAME sampleMultiType
    COMMAND $<TARGET_FILE:sampleMultiType>
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
  add_test(
    NAME sampleMultiByType
    COMMAND $<TARGET_FILE:sampleMultiByType>
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
  add_test(
    NAME sampleF
    COMMAND $<TARGET_FILE:sampleF>
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
  add_test(
    NAME sampleIncF
    COMMAND $<TARGET_FILE:sampleIncF>
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
  add_test(
    NAME sampleMultiF
    COMMAND $<TARGET_FILE:sampleMultiF>
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
endif()
