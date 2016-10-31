project (QuexPipe)
cmake_minimum_required (VERSION 2.8)

set (QUEXPIPE_VERSION_MAJOR 2) # Main version: feature changes
set (QUEXPIPE_VERSION_MINOR 0) # Minor verson: API changes
set (QUEXPIPE_VERSION_BUILD 0) # Build verson: API is compatible, but the headers can change

configure_file (
  "${PROJECT_SOURCE_DIR}/QuexPipeConfig.hpp.in"
  "${PROJECT_BINARY_DIR}/QuexPipeConfig.hpp"
 )
include_directories("${PROJECT_BINARY_DIR}")

add_subdirectory (Library)
add_subdirectory (Main)
add_subdirectory (CommonHeaders)
#add_subdirectory (Python-bind)

#target_link_libraries (quexpipemain QuexPipe)
