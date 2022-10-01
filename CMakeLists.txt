cmake_minimum_required(VERSION 3.13)

#include the cmake build functions from pico SDK
include(pico_sdk_import.cmake)

#project name and code here
#project(projectname_here C CXX ASM)
#for instance:
project(AD9833LIB C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


#bring in this function to add cmake Pico SDK functionality to build
pico_sdk_init()

#list out files used--as long as .h are in the same directory don't list those.
add_executable(${PROJECT_NAME}
	#for instance:
	#file1.c   file2.c  file3.c  all_your_dookies.c
	main.c  hex2bin.c  ad9833.c spiRP2040.c UART.c
	)

#create extra uf2 etc files
pico_add_extra_outputs(${PROJECT_NAME})

#linker needs to use these libraries
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
	hardware_spi
	
)

#control USB output--1 means on.
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
