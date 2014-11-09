#include "cpptest.h"
#include "ibex_RasterPicture.h"
//#include "utils.h"

namespace ibex {

class TestRasterPicture : public Test::Suite {

public:
    TestRasterPicture() {
        TEST_ADD(TestRasterPicture::test_init2DRasterPicture);
        TEST_ADD(TestRasterPicture::test_init3DRasterPicture);
        TEST_ADD(TestRasterPicture::test_readMissingFile);
        TEST_ADD(TestRasterPicture::test_readWrongFileFormat_1);
        TEST_ADD(TestRasterPicture::test_readWrongFileFormat_2);
//        TEST_ADD(TestRasterPicture::testIntegralImage);
    }

    void setup();    
    void test_init2DRasterPicture();
    void test_init3DRasterPicture();
    void test_readMissingFile();
    void test_readWrongFileFormat_1();
    void test_readWrongFileFormat_2();
    void test_ImageIntegral2D();


};

}
