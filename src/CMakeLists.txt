###############################################################################
#    Model Element   : CMakeLists
#    Component       : Main reactor loop 
#    Author          : Mirza Avdic 
#    File Name       : CMakeLists.txt
###############################################################################

cmake_minimum_required(VERSION 3.14)

project( MainReactorloop 
         VERSION 1.0.0
	 DESCRIPTION "Start reactor loop and test apps" )

add_subdirectory(IPCServer)
add_subdirectory(IPCClient)

add_executable(reactor-main)
target_sources(reactor-main 
	PRIVATE main.cc)
target_link_libraries(reactor-main 
	PRIVATE reactor 
	PRIVATE pthread
	PRIVATE ipcclient
	PRIVATE ipcserver)

