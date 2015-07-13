cmake_minimum_required (VERSION 2.8)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

set( SOURCES 
	main.cpp 
	interpreter_tests.cpp 
	interpreter_tests.h 
	utils_tests.cpp
	)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

find_package(Boost COMPONENTS unit_test_framework)

if(Boost_FOUND)
    # Boost
    include_directories("${Boost_INCLUDE_DIR}")
    link_directories("${Boost_LIBRARY_DIRS}")
	
    # Machine 
    include_directories("${CMAKE_SOURCE_DIR}/Tostitos/machine")
    link_directories("${CMAKE_BINARY_DIR}/lib")
    
    add_executable(MachineTests ${SOURCES}) 
    target_link_libraries(MachineTests ${Boost_LIBRARIES} machine)
endif()