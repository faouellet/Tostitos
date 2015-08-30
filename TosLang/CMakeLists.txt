cmake_minimum_required (VERSION 2.8)

add_library( lang STATIC
		ast.h
		astprinter.h
		astvisitor.h
		declarations.h
		errorlogger.h
		errorlogger.cpp
		expressions.h
        instructionselector.h
        instructionselector.cpp
		lexer.h
		lexer.cpp
		parser.h
		parser.cpp
        ruletable.h
        ruletable.cpp
		statements.h
		symboltable.h
		symboltable.cpp
		typechecker.h
		typechecker.cpp
		)
		
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
		
add_executable (TosLang driver.cpp)
target_link_libraries(TosLang lang)