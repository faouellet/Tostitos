cmake_minimum_required (VERSION 2.8)

add_library( machine STATIC 
        constants.h
		instruction.cpp
		instruction.h
		interpreter.cpp
		interpreter.h
		cpu.cpp 
		cpu.h
		hardDrive.h
		hardDrive.cpp
		machine.h
		memory.h
		memory.cpp
		utils.h
		threadInfo.h
		internalHelperFunctions.h
		)
	   
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)