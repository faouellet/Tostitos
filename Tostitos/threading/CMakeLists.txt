cmake_minimum_required (VERSION 2.8)

add_library( threading STATIC 
		callstack.h
		callstack.cpp
		executor.h
		executor.cpp
		interpretedvalue.h
		thread.h
		thread.cpp
		threadutil.h
		threadutil.cpp
		)
	   
target_link_libraries(threading kernel execution)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
