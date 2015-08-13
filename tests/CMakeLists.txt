cmake_minimum_required (VERSION 2.8)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

include_directories("${CMAKE_SOURCE_DIR}/Tostitos/machine")
include_directories("${CMAKE_SOURCE_DIR}/TosLang")

find_package(Boost COMPONENTS unit_test_framework)
if(Boost_FOUND)
    if(WIN32)
        set( SOURCES 
                main.cpp 
                interpreter_tests.cpp 
                interpreter_tests.h 
				lexer_tests.cpp
                utils_tests.cpp
        )

        set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
		
		# Copy inputs
		file(COPY inputs DESTINATION ${EXECUTABLE_OUTPUT_PATH})

        # Boost
        include_directories("${Boost_INCLUDE_DIR}")
        link_directories("${Boost_LIBRARY_DIRS}")
        
        # Machine 
        link_directories("${CMAKE_BINARY_DIR}/lib")
        
        add_executable(Tests ${SOURCES}) 
        target_link_libraries(Tests ${Boost_LIBRARIES} lang machine)
    else()
        include(../cmake/BoostTestHelpers.cmake)

        add_boost_test(interpreter_tests.cpp machine)
        add_boost_test(utils_tests.cpp machine)
		
		# Copy inputs
		file(COPY inputs DESTINATION ${CMAKE_BINARY_DIR})
    endif()
endif()
