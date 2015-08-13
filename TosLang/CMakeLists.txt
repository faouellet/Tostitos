cmake_minimum_required (VERSION 2.8)

add_library( lang STATIC
		lexer.h
		lexer.cpp
		)
		
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
		
add_executable (TosLang driver.cpp)
target_link_libraries(TosLang lang)