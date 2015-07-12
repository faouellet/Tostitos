cmake_minimum_required (VERSION 2.8)

add_library( communicationWithHost STATIC 
        hardDriveHelper.h
		hardDriveHelper.cpp
		)
	   
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)