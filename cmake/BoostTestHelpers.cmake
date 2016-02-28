# Utility function for automatically adding Boost based unit tests
# Inspiration: http://ericscottbarr.com/blog/2015/06/driving-boost-dot-test-with-cmake/

function(add_boost_test SOURCE_FILE_NAME DEPENDENCY_LIB)
    get_filename_component(TEST_EXECUTABLE_NAME ${SOURCE_FILE_NAME} NAME_WE)

    add_executable(${TEST_EXECUTABLE_NAME} ${SOURCE_FILE_NAME})
    target_link_libraries(${TEST_EXECUTABLE_NAME} 
                          ${DEPENDENCY_LIB} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

    file(READ "${SOURCE_FILE_NAME}" SOURCE_FILE_CONTENTS)

    string(REGEX MATCH "BOOST_FIXTURE_TEST_SUITE\\( *([A-Za-z_0-9]+), [A-Za-z_0-9]+ *\\)"
           SUITE_MATCHES ${SOURCE_FILE_CONTENTS})

   list(LENGTH SUITE_MATCHES N)
   if(${N} EQUAL 1)
       list(GET SUITE_MATCHES 0 SUITE_MATCH)
       string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+), [A-Za-z_0-9]+ *\\)" "\\1" SUITE_NAME ${SUITE_MATCH})
   else()
       string(REGEX MATCH "BOOST_AUTO_TEST_SUITE\\( *([A-Za-z_0-9]+) *\\)"
       SUITE_MATCHES ${SOURCE_FILE_CONTENTS})
       if(${N} EQUAL 1)
           list(GET SUITE_MATCHES 0 SUITE_MATCH)
           string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+), [A-Za-z_0-9]+ *\\)" "\\1" SUITE_NAME ${SUITE_MATCH})
       endif()
   endif()

    string(REGEX MATCHALL "BOOST_AUTO_TEST_CASE\\( *([A-Za-z_0-9]+) *\\)" 
        FOUND_AUTO_TESTS ${SOURCE_FILE_CONTENTS})

    foreach(HIT ${FOUND_AUTO_TESTS})
        string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+) *\\).*" "\\1" TEST_NAME ${HIT})

        add_test(NAME "${TEST_EXECUTABLE_NAME}.${TEST_NAME}" 
                 COMMAND ${TEST_EXECUTABLE_NAME}
                 --run_test=${SUITE_NAME}/${TEST_NAME} --catch_system_error=yes)
    endforeach()
endfunction()
