#include "cpptest.h"
#include "ibex_PixelMap.h"
#include "utils.h"

namespace ibex {

class TestPixelMap : public TestIbex {

public:
    TestPixelMap() {
        TEST_ADD(TestPixelMap::test_init2DPixelMap);
        TEST_ADD(TestPixelMap::test_init3DPixelMap);
        TEST_ADD(TestPixelMap::test_readMissingFile);
        TEST_ADD(TestPixelMap::test_readWrongFileFormat_1);
        TEST_ADD(TestPixelMap::test_readWrongFileFormat_2);
        TEST_ADD(TestPixelMap::test_ImageIntegral2D);
    }

    void setup();    
    void test_init2DPixelMap();
    void test_init3DPixelMap();
    void test_readMissingFile();
    void test_readWrongFileFormat_1();
    void test_readWrongFileFormat_2();
    void test_ImageIntegral2D();


};

}
