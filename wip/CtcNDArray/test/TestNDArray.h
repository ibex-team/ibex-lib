#include "cpptest.h"
#include "ibex_NDArray.h"
//#include "utils.h"

namespace ibex {

class TestNDArray : public Test::Suite {

public:
    TestNDArray() {
        TEST_ADD(TestNDArray::test_init2DArray);
        TEST_ADD(TestNDArray::test_init3DArray);
        TEST_ADD(TestNDArray::test_readMissingFile);
        TEST_ADD(TestNDArray::test_readWrongFileFormat_1);
        TEST_ADD(TestNDArray::test_readWrongFileFormat_2);
//        TEST_ADD(TestNDArray::testIntegralImage);
    }

    void setup();    
    void test_init2DArray();
    void test_init3DArray();
    void test_readMissingFile();
    void test_readWrongFileFormat_1();
    void test_readWrongFileFormat_2();
    void test_ImageIntegral2D();


};

}
