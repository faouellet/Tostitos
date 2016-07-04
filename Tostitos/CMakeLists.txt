cmake_minimum_required (VERSION 2.8)

add_subdirectory(common)
add_subdirectory(communicationWithHost)
add_subdirectory(dataStructures)
add_subdirectory(fileSystem)
add_subdirectory(kernel)
add_subdirectory(machine)
add_subdirectory(memory)
add_subdirectory(threading)

set( SOURCES main.cpp system.h )

add_executable (Tostitos ${SOURCES})
target_link_libraries (Tostitos communicationWithHost fileSystem kernel machine memory threading)

if(NOT WIN32)
  target_link_libraries (Tostitos pthread)
endif()
