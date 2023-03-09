add_subdirectory(helpers)

include_directories(${CMAKE_SOURCE_DIR}/scripts)

aux_source_directory(compiler sub_compiler)
aux_source_directory(lexer    sub_lexer)
aux_source_directory(parser   sub_parser)
aux_source_directory(token    sub_token)

add_library(Compiler ${sub_compiler} ${sub_lexer} ${sub_token} ${sub_parser})
target_link_libraries(Compiler Helpers)