cmake_minimum_required (VERSION 2.8)
project (Tostitos)

enable_testing()

if(MSVC)
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /EHa")
else()
	set (CMAKE_CXX_FLAGS "-std=c++14" CACHE STRING "" FORCE)
    if(($ENV{COVERAGE}) AND (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU"))
        message("Adding flags for code coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
        set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
    elseif(($ENV{SANITIZE}) AND (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"))
        message("Adding flags for code coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS "-fsanitize=address")
	endif()
endif()

add_subdirectory (Tostitos)
add_subdirectory (TosLang)
add_subdirectory (tests)

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
