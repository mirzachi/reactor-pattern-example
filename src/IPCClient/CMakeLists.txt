###############################################################################
#    Model Element   : CMakeLists
#    Component       : IPCClient
#    Author          : Mirza Avdic 
#    File Name       : CMakeLists.txt
###############################################################################

cmake_minimum_required(VERSION 3.14)

add_library( ipcclient STATIC )
target_sources( ipcclient
	PRIVATE   "src/IPCClient.cc"
	PUBLIC    "include/IPCClient.h" )

target_include_directories( ipcclient 
    PUBLIC "include" )

target_link_libraries( ipcclient
	PRIVATE reactor)


