aux_source_directory (. SRC_LIST)
include_directories ("${PROJECT_SOURCE_DIR}/CommonHeaders")

add_executable (quexpipemain ${SRC_LIST})
