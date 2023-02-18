add_subdirectory(helpers)

include_directories(${CMAKE_SOURCE_DIR}/scripts)

aux_source_directory(token    sub_token)

add_library(Compiler ${sub_compiler} ${sub_lexer} ${sub_token})
target_link_libraries(Compiler Helpers)