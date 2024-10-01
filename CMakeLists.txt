cmake_minimum_required(VERSION 3.22)

project(UwU-shell)

set(CMAKE_CXX_FLAGS "-g3")

find_path(READLINE_INCLUDE_DIR readline/readline.h)
find_library(READLINE_LIBRARY NAMES readline)

add_executable(UwU-shell main.cpp src/Shell.cpp src/Error.cpp src/Command.cpp src/builtins/cd.cpp src/builtins/exit.cpp src/builtins/export.cpp src/builtins/unset.cpp)

include_directories(hdr)

target_include_directories(UwU-shell PRIVATE ${READLINE_INCLUDE_DIR})
target_link_libraries(UwU-shell ${READLINE_LIBRARY} )