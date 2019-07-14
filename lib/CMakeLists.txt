###############################################################################
#    Model Element   : CMakeLists
#    Component       : Reactor
#    Author          : Mirza Avdic 
#    File Name       : CMakeLists.txt
###############################################################################

cmake_minimum_required(VERSION 3.14)

project( MyLibs
         VERSION 1.0.0
	 DESCRIPTION "Reactor Implementation" )

add_library( reactor STATIC )
target_sources( reactor
    PRIVATE   "src/reactor/reactor.cc"
    PUBLIC    "include/reactor/reactor.h" 
    PUBLIC    "include/reactor/event_handler.h" )

target_include_directories( reactor
    PUBLIC "include" )



