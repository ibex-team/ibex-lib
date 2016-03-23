#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ibex_PixelMap.h"
#include "utils.h"

namespace ibex {

class TestPixelMap : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestPixelMap);
	CPPUNIT_TEST(test_init2DPixelMap);
	CPPUNIT_TEST(test_init3DPixelMap);
	CPPUNIT_TEST(test_readMissingFile);
	CPPUNIT_TEST(test_readWrongFileFormat_1);
	CPPUNIT_TEST(test_readWrongFileFormat_2);
	CPPUNIT_TEST(test_ImageIntegral2D);
	CPPUNIT_TEST_SUITE_END();

    void setup();    
    void test_init2DPixelMap();
    void test_init3DPixelMap();
    void test_readMissingFile();
    void test_readWrongFileFormat_1();
    void test_readWrongFileFormat_2();
    void test_ImageIntegral2D();


};

CPPUNIT_TEST_SUITE_REGISTRATION(TestPixelMap);

} // end namespace
