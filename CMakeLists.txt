###############################################################################
#    Model Element   : CMakeLists
#    Component       : Reactor pattern
#    Author          : Mirza Avdic 
#    File Name       : CMakeLists.txt
###############################################################################

cmake_minimum_required(VERSION 3.14)

project(
  Reactor-pattern 
  LANGUAGES CXX
  VERSION "1.0.0"
)

# Allow ycm to use clang C/C++ semantic parsing
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Importing Threads")
find_package(Threads REQUIRED)

add_subdirectory(lib)
add_subdirectory(src)

