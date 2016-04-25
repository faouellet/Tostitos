cmake_minimum_required (VERSION 2.8)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

include_directories("${CMAKE_SOURCE_DIR}/Tostitos/machine")
include_directories("${CMAKE_SOURCE_DIR}/TosLang")

# Copy files for lang
file(COPY lang/asts DESTINATION ${CMAKE_BINARY_DIR})
file(COPY lang/programs DESTINATION ${CMAKE_BINARY_DIR})

find_package(Boost COMPONENTS unit_test_framework)
if(Boost_FOUND)
    if(WIN32)
		file(GLOB_RECURSE SOURCES "*.cpp" "*.h")
        
        set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
		
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
        add_boost_test(machine/cpu_tests.cpp machine)
		add_boost_test(machine/instruction_tests.cpp machine)
        add_boost_test(machine/interpreter_tests.cpp machine)
        add_boost_test(machine/utils_tests.cpp machine)
		
		# TosLang tests
		add_boost_test(lang/ast_printer_tests.cpp lang)
		
        add_boost_test(lang/lexer_tests.cpp lang)
        add_boost_test(lang/lexer_error_tests.cpp lang)

        add_boost_test(lang/parser_call_tests.cpp lang)
        add_boost_test(lang/parser_fn_tests.cpp lang)
        add_boost_test(lang/parser_if_tests.cpp lang)
        add_boost_test(lang/parser_io_tests.cpp lang)
        add_boost_test(lang/parser_var_tests.cpp lang)
        add_boost_test(lang/parser_while_tests.cpp lang)
		
        add_boost_test(lang/symbol_collector_tests.cpp lang)

        add_boost_test(lang/scope_check_call_tests.cpp lang)
        add_boost_test(lang/scope_check_var_tests.cpp lang)
        
        add_boost_test(lang/type_checker_call_tests.cpp lang)
        add_boost_test(lang/type_checker_if_tests.cpp lang)
        add_boost_test(lang/type_checker_io_tests.cpp lang)
        add_boost_test(lang/type_checker_return_tests.cpp lang)
        add_boost_test(lang/type_checker_var_tests.cpp lang)
        add_boost_test(lang/type_checker_while_tests.cpp lang)
		
		add_boost_test(lang/instruction_selector_tests.cpp lang)
    endif()
endif()
