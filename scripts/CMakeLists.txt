add_subdirectory(helpers)

include_directories(${CMAKE_SOURCE_DIR}/scripts)

aux_source_directory(compiler sub_compiler)
aux_source_directory(lexer    sub_lexer)
aux_source_directory(parser   sub_parser)
aux_source_directory(token    sub_token)
aux_source_directory(node     sub_node)
aux_source_directory(history  sub_history)

add_library(Compiler 
    ${sub_compiler} 
    ${sub_lexer} 
    ${sub_token} 
    ${sub_parser}
    ${sub_node}
    ${sub_history}
)
target_link_libraries(Compiler Helpers)