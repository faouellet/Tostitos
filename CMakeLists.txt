cmake_minimum_required (VERSION 2.8)
project (Tostitos)

enable_testing()

add_subdirectory (Tostitos)
add_subdirectory (tests)

if(MSVC)
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	set (CMAKE_CXX_FLAGS "-std=c++14" CACHE STRING "" FORCE)
endif()

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE)
	add_custom_target(doc 
		COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
		COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)
