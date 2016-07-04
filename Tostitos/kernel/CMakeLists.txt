cmake_minimum_required (VERSION 2.8)

add_library( kernel STATIC 
		kernel.h
		kernel.cpp
		scheduler.h
		scheduler.cpp
		)
	   
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)