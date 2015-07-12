cmake_minimum_required (VERSION 2.8)

add_library( fileSystem STATIC 
        fileHeader.h
		fileheader.cpp
		fileSystem.h
		fileSystem.cpp
		)
	   
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)