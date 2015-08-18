cmake_minimum_required (VERSION 2.8)

add_library( lang STATIC
		ast.h
		astvisitor.h
		declarations.h
		expressions.h
		lexer.h
		lexer.cpp
		parser.h
		parser.cpp
		statements.h
		)
		
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
		
add_executable (TosLang driver.cpp)
target_link_libraries(TosLang lang)