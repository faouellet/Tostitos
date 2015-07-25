cmake_minimum_required (VERSION 2.8)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME ON)

include(../cmake/BoostTestHelpers.cmake)

include_directories("${CMAKE_SOURCE_DIR}/Tostitos/machine")

find_package(Boost COMPONENTS unit_test_framework)

add_boost_test(interpreter_tests.cpp machine)
add_boost_test(utils_tests.cpp machine)
