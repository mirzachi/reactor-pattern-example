###############################################################################
#    Model Element   : CMakeLists
#    Component       : IPCServer
#    Author          : Mirza Avdic 
#    File Name       : CMakeLists.txt
###############################################################################

cmake_minimum_required(VERSION 3.14)

add_library( ipcserver STATIC )
target_sources( ipcserver
	PRIVATE   "src/IPCServer.cc"
	PUBLIC    "include/IPCServer.h" )

target_include_directories( ipcserver 
    PUBLIC "include" )

target_link_libraries( ipcserver
	PRIVATE reactor)


