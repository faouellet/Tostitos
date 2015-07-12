cmake_minimum_required (VERSION 2.8)

add_library( threads STATIC 
        kernel.h
		kernel.cpp
		scheduler.h
		scheduler.cpp
		thread.h
		thread.cpp
		)
	   
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)