project (QuexPipe)
cmake_minimum_required (VERSION 2.8)

set (QUEXPIPE_VERSION_MAJOR 2) # Main version: feature changes
set (QUEXPIPE_VERSION_MINOR 0) # Minor verson: API changes
set (QUEXPIPE_VERSION_BUILD 0) # Build verson: API is compatible, but the headers can change
set (CMAKE_CXX_FLAGS "-std=c++11")
if (WIN32)
	add_definitions (-DQP_PLATFORM_WINDOWS)
endif (WIN32)
if (UNIX)
	add_definitions (-DQP_PLATFORM_UNIX)
	set (CMAKE_CXX_FLAGS "-ldl")
endif (UNIX)

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DQP_DEBUG")

configure_file (
  "${PROJECT_SOURCE_DIR}/QuexPipeConfig.hpp.in"
  "${PROJECT_BINARY_DIR}/QuexPipeConfig.hpp"
 )
include_directories("${PROJECT_BINARY_DIR}")



file (GLOB_RECURSE HEAD_LIST RELATIVE ${PROJECT_SOURCE_DIR} CommonHeaders/*.h CommonHeaders/*.hpp CommonHeaders/*.c CommonHeaders/*.cpp)
aux_source_directory ("${PROJECT_SOURCE_DIR}/Main" MAIN_LIST)
aux_source_directory ("${PROJECT_SOURCE_DIR}/Library" LIB_LIST)
include_directories ("${PROJECT_SOURCE_DIR}/CommonHeaders")



add_library (QuexPipe ${LIB_LIST} ${HEAD_LIST})
add_executable (quexpipemain ${MAIN_LIST} ${HEAD_LIST})
target_link_libraries (quexpipemain QuexPipe)

