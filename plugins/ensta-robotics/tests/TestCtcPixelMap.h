#include "ibex_CtcPixelMap.h"
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestCtcPixelMap : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE(TestCtcPixelMap);
    CPPUNIT_TEST(test3d_emptyBox);
    CPPUNIT_TEST(test3d_contractEmpty);
    CPPUNIT_TEST(test3d_allReal);
    CPPUNIT_TEST(test3d_fullImage);
    CPPUNIT_TEST(test3d_corner);

    CPPUNIT_TEST(test2d_emptyBox);
    CPPUNIT_TEST(test2d_contractEmpty);
    CPPUNIT_TEST(test2d_allReal);
    CPPUNIT_TEST(test2d_fullImage);
    CPPUNIT_TEST(test2d_corner);
	CPPUNIT_TEST_SUITE_END();

protected:
    void initRaster3D(PixelMap3D& raster);
    void initRaster2D(PixelMap2D& raster);
private:
    void test3d_emptyBox();
    void test3d_contractEmpty();
    void test3d_allReal();
    void test3d_fullImage();
    void test3d_corner();

    void test2d_emptyBox();
    void test2d_contractEmpty();
    void test2d_allReal();
    void test2d_fullImage();
    void test2d_corner();
    
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCtcPixelMap);

} // end namespace

