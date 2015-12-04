cmake_minimum_required (VERSION 2.8)

file(GLOB AST_SOURCES		"AST/*")
file(GLOB CODEGEN_SOURCES	"CodeGen/*")
file(GLOB COMMON_SOURCES	"Common/*")
file(GLOB PARSE_SOURCES		"Parse/*")
file(GLOB SEMA_SOURCES		"Sema/*")
file(GLOB UTILS_SOURCES		"Utils/*")

SOURCE_GROUP(lang\\AST FILES ${AST_SOURCES})
SOURCE_GROUP(lang\\CodeGen FILES ${CODEGEN_SOURCES})
SOURCE_GROUP(lang\\Common FILES ${COMMON_SOURCES})
SOURCE_GROUP(lang\\Parse FILES ${PARSE_SOURCES})
SOURCE_GROUP(lang\\Sema FILES ${SEMA_SOURCES})
SOURCE_GROUP(lang\\Utils FILES ${UTILS_SOURCES})

add_library( lang STATIC
		${AST_SOURCES}
		${CODEGEN_SOURCES}
		${COMMON_SOURCES}
		${PARSE_SOURCES}
		${SEMA_SOURCES}
		${UTILS_SOURCES}
		)
		
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
		
add_executable (TosLang driver.cpp)
target_link_libraries(TosLang lang)