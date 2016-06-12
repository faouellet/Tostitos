cmake_minimum_required (VERSION 2.8)
project (Tostitos)

enable_testing()

if(${USE_LLVM_BACKEND})
	find_package(LLVM REQUIRED CONFIG)
	
	include_directories(${LLVM_INCLUDE_DIRS})
	add_definitions(${LLVM_DEFINITIONS})
	add_definitions("-DUSE_LLVM_BACKEND")
endif()

if(MSVC)
	# Set flags for exception handling
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
	
	# Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
	
	# Disabling exception handling warning
	add_definitions(
		-wd4530 # Suppress 'warning C4530: C++ exception handler used, but unwind semantics are not enabled.'
	)
else()
	# Set flag for C++14
	set (CMAKE_CXX_FLAGS "-std=c++14" CACHE STRING "" FORCE)
	
	# Set flags for warnings
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
	
	# Set flags for code coverage or sanitizers
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
    if(NOT MSVC)
        add_custom_target(doc 
            COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
            COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
    endif()
endif(DOXYGEN_FOUND)
