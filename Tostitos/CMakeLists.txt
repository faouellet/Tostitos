cmake_minimum_required (VERSION 2.8)

add_subdirectory(common)
add_subdirectory(communicationWithHost)
add_subdirectory(dataStructures)
add_subdirectory(fileSystem)
add_subdirectory(machine)
add_subdirectory(memory)
add_subdirectory(threads)

set( SOURCES main.cpp system.h system.cpp )

add_executable (Tostitos ${SOURCES})
set_target_properties (Tostitos PROPERTIES LINKER_LANGUAGE C)
target_link_libraries (Tostitos communicationWithHost fileSystem machine memory threads)

if(NOT WIN32)
  target_link_libraries (Tostitos pthread)
endif()
