cmake_minimum_required (VERSION 2.8)

if(MSVC)
	MACRO(SUBDIRLIST result curdir)
	  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
	  SET(dirlist "")
	  FOREACH(child ${children})
		IF(IS_DIRECTORY ${curdir}/${child})
			LIST(APPEND dirlist ${child})
		ENDIF()
	  ENDFOREACH()
	  SET(${result} ${dirlist})
	ENDMACRO()

	SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

	set(SRCS "")
	set(HDRS "")
	#file(GLOB SRCS *.cpp)
	#file(GLOB HDRS *.h)
	
	source_group("Tostitos" FILES ${SRCS} ${HDRS})

	FOREACH(subdir ${SUBDIRS})
		file(GLOB SUBDIR_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/*" *.cpp)
		file(GLOB SUBDIR_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/*" *.h)
		
		list(REMOVE_ITEM SUBDIR_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/system.cpp")
		list(REMOVE_ITEM SUBDIR_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/system.h")
		
		source_group("${subdir}" FILES ${SUBDIR_SRCS} ${SUBDIR_HDRS})
		set(SRCS ${SRCS} ${SUBDIR_SRCS})
		set(HDRS ${HDRS} ${SUBDIR_HDRS})
	ENDFOREACH()
	
	file(GLOB CUR_SRCS *.cpp)
	file(GLOB CUR_HDRS *.h)
	
	source_group("Tostitos" FILES ${CUR_SRCS} ${CUR_HDRS})
	set(SRCS ${SRCS} ${CUR_SRCS})
	set(HDRS ${HDRS} ${CUR_HDRS})	
else()
	file(GLOB_RECURSE SRCS *.cpp)
	file(GLOB_RECURSE HDRS *.h)
endif()

add_executable (Tostitos ${SRCS} ${HDRS})
set_target_properties (Tostitos PROPERTIES LINKER_LANGUAGE C)
target_link_libraries (Tostitos)
