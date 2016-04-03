#ifndef TOSLANG_TEST_UTILS_FIXTURE_H__TOSTITOS
#define TOSLANG_TEST_UTILS_FIXTURE_H__TOSTITOS

#include <boost/test/unit_test.hpp>

#include <fstream>

/*
* \fn                   CompareFiles
* \brief                Compare two files in a line basis
* \param testFile       File containing a test output
* \param expectedFile   File containing a test expected result
*/
inline void CompareFiles(const std::string& testFile, const std::string& expectedFile)
{
    std::ifstream testStream(testFile);
    std::ifstream expectedStream(expectedFile);

    std::string expectedStr;
    std::string testStr;
    while (!expectedStream.eof() && !testStream.eof())
    {
        expectedStream >> expectedStr;
        testStream >> testStr;

        BOOST_REQUIRE_EQUAL(expectedStr, testStr);
    }
}

#endif // TOSLANG_TEST_UTILS_FIXTURE_H__TOSTITOS
