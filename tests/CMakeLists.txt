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
				ast_printer_tests.cpp
				frontend_error_fixture.h
                instruction_selector_tests.cpp
				instruction_tests.cpp
                interpreter_fixture.h                 
                interpreter_tests.cpp 
                lexer_tests.cpp
				parser_error_tests.cpp
				parser_tests.cpp
				typechecker_tests.cpp
                utils_tests.cpp
        )

        set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
		
		# Copy inputs
		file(COPY inputs DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY outputs DESTINATION ${CMAKE_BINARY_DIR})

        # Boost
        include_directories("${Boost_INCLUDE_DIR}")
        link_directories("${Boost_LIBRARY_DIRS}")
        
        # Machine 
        link_directories("${CMAKE_BINARY_DIR}/lib")
        
        add_executable(Tests ${SOURCES}) 
        target_link_libraries(Tests ${Boost_LIBRARIES} lang machine)
    else()
        include(../cmake/BoostTestHelpers.cmake)

		# Machine tests
		add_boost_test(instruction_tests.cpp machine)
        add_boost_test(interpreter_tests.cpp machine)
        add_boost_test(utils_tests.cpp machine)
		
		# TosLang tests
		add_boost_test(ast_printer_tests.cpp lang)
		#add_boost_test(instruction_selector_tests.cpp lang)
		add_boost_test(lexer_tests.cpp lang)
		add_boost_test(parser_error_tests.cpp lang)
		add_boost_test(parser_tests.cpp lang)
		add_boost_test(typechecker_tests.cpp lang)
		
		# Copy inputs and outputs
		file(COPY inputs DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY outputs DESTINATION ${CMAKE_BINARY_DIR})
    endif()
endif()
