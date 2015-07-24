cmake_minimum_required (VERSION 2.8)
project (Tostitos)

add_subdirectory (Tostitos)
add_subdirectory (tests)

if(MSVC)
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	set (CMAKE_CXX_FLAGS "-std=c++14" CACHE STRING "" FORCE)
endif()

enable_testing()

#include(cmake/BoostTestHelpers.cmake)
#add_boost_test(tests/interpreter_tests.cpp machine)
#add_boost_test(tests/utils_tests.cpp machine)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE)
	add_custom_target(doc ALL
		COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
		COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)
