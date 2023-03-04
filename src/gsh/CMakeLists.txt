add_library (gsh gsh.c command.c exec.c)

target_include_directories (gsh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(gsh PRIVATE -Wall -Wextra -Wpedantic -Werror)
