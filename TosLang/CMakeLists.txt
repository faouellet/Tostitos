cmake_minimum_required (VERSION 2.8)

file(GLOB AST_SOURCES			"AST/*")
file(GLOB CFG_SOURCES			"CFG/*")
#file(GLOB CODEGEN_SOURCES		"CodeGen/*")
file(GLOB COMMON_SOURCES		"Common/*")
file(GLOB EXECUTION_SOURCES		"Execution/*")
file(GLOB PARSE_SOURCES			"Parse/*")
file(GLOB SEMA_SOURCES			"Sema/*")
#file(GLOB SSA_SOURCES 			"SSA/*")
file(GLOB UTILS_SOURCES			"Utils/*")
if(${USE_LLVM_BACKEND})
	file(GLOB LLVM_BACKEND_SOURCES  "LLVMBackend/*")
endif()

SOURCE_GROUP(lang\\AST FILES ${AST_SOURCES})
SOURCE_GROUP(lang\\CFG FILES ${CFG_SOURCES})
#SOURCE_GROUP(lang\\CodeGen FILES ${CODEGEN_SOURCES})
SOURCE_GROUP(lang\\Common FILES ${COMMON_SOURCES})
SOURCE_GROUP(lang\\Parse FILES ${PARSE_SOURCES})
SOURCE_GROUP(lang\\Sema FILES ${SEMA_SOURCES})
#SOURCE_GROUP(lang\\SSA FILES ${SSA_SOURCES})
SOURCE_GROUP(lang\\Utils FILES ${UTILS_SOURCES})
if(${USE_LLVM_BACKEND})
    SOURCE_GROUP(lang\\LLVMBackend FILES ${LLVM_BACKEND_SOURCES})
endif()

include_directories("${CMAKE_SOURCE_DIR}/Tostitos/threading")

add_library( lang STATIC
		${AST_SOURCES}
		${CFG_SOURCES}
		#${CODEGEN_SOURCES}
		${COMMON_SOURCES}
		${PARSE_SOURCES}
		${SEMA_SOURCES}
		#${SSA_SOURCES}
		${UTILS_SOURCES}
		${LLVM_BACKEND_SOURCES}
		)
		
add_library( execution STATIC
		${EXECUTION_SOURCES}
		)
				
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
	
add_executable (TosLang main.cpp)
if(${USE_LLVM_BACKEND})
	llvm_map_components_to_libnames(llvm_libs support core)
	target_link_libraries(TosLang lang execution kernel threading ${llvm_libs})    
else()
	target_link_libraries(TosLang lang execution kernel threading)
endif()

