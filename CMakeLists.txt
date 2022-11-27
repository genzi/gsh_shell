cmake_minimum_required (VERSION 3.11)
project (GSH_SHELL)

add_subdirectory (gsh)
add_executable (gsh_shell main.c)

target_link_libraries (gsh_shell LINK_PUBLIC gsh)

target_compile_options(gsh_shell PRIVATE -Wall -Wextra -Wpedantic -Werror)
