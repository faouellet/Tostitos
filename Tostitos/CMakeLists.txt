cmake_minimum_required (VERSION 2.8)

file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HDRS *.h)

add_executable (Tostitos ${SRCS} ${HDRS})
target_link_libraries (Tostitos)

