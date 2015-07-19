cmake_minimum_required (VERSION 2.8)
project (Tostitos)

add_subdirectory (tests)
add_subdirectory (Tostitos)

if(MSVC)
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
	set (CMAKE_CXX_FLAGS "-std=c++14" CACHE STRING "" FORCE)
endif()
