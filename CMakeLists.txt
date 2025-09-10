cmake_minimum_required(VERSION 3.0...3.31.6 FATAL_ERROR)

project(PairAnalysisPhenix)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic -pipe -O2")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_BUILD_TYPE Debug)

find_package(ROOT QUIET)

if(${ROOT_FOUND})
   message(STATUS "ROOT package found")
   link_libraries(${ROOT_LIBRARIES})
elseif(EXISTS "${ROOT_PATH}/bin/root")
   message(STATUS "ROOT path found")
   set(ROOT_INCLUDE_DIRS "${ROOT_PATH}/include")
   set(ROOT_root_CMD "${ROOT_PATH}/bin/root")
   execute_process(COMMAND ${ROOT_root_CMD}-config --glibs OUTPUT_VARIABLE ROOT_LIB_FLAGS)
   string(STRIP ${ROOT_LIB_FLAGS} ROOT_LIB_FLAGS)
   set(CMAKE_SHARED_LINKER_FLAGS ${ROOT_LIB_FLAGS})
   set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LIB_FLAGS})
else()
   message(FATAL_ERROR "ROOT not found: install it via package manager or set ROOT_PATH if you installed it manually")
endif()

#if (EXISTS "${PYTHIA8_PATH}/bin/pythia8-config")
#   message(STATUS "PYTHIA8 path found")
#   set(PYTHIA8_INCLUDE_DIRS "${PYTHIA8_PATH}/include")
#   set(PYTHIA8_pythia8_CMD "${PYTHIA8_PATH}/bin/pythia8")
#   execute_process(COMMAND ${PYTHIA8_pythia8_CMD}-config --libs OUTPUT_VARIABLE PYTHIA8_LIB_FLAGS)
#   string(STRIP ${PYTHIA8_LIB_FLAGS} PYTHIA8_LIB_FLAGS)
#   set(CMAKE_SHARED_LINKER_FLAGS ${PYTHIA8_LIB_FLAGS})
#   set(CMAKE_EXE_LINKER_FLAGS ${PYTHIA8_LIB_FLAGS})
#else()
#   message(FATAL_ERROR "${PYTHIA8_PATH}/bin/pythia8-config PYTHIA8 not found: install it via package manager or set PYTHIA8_PATH if you installed it manually")
#endif()


include_directories(${CMAKE_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})

add_executable(exec ${CMAKE_SOURCE_DIR}/src/exec.cpp)
