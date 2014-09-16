/* ============================================================================
 * I B E X - CtcImage Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Benoit DESROCHERS
 * Created     : Sep 13, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_CTCIMAGE_H__
#define __TEST_CTCIMAGE_H__

#include "ibex_CtcImage.h"
#include "cpptest.h"

namespace ibex {

class TestCtcImage_1pixel : public Test::Suite {

public:
    TestCtcImage_1pixel(){
    TEST_ADD(TestCtcImage_1pixel::test_emptyBox);
    TEST_ADD(TestCtcImage_1pixel::test_allReal);
    TEST_ADD(TestCtcImage_1pixel::test_fullImage);
    TEST_ADD(TestCtcImage_1pixel::test_corner);

    }

protected:
    void setup();
private:
    void test_emptyBox();
    void test_allReal();
    void test_fullImage();
    void test_corner();

    Array2D array;
    double2 leaf_size;
    double2 origin;
    int2 grid_size;
};

//class TestCtcImage : public Test::Suite {

//public:
//    TestCtcImage() {
//        TEST_ADD(TestCtcImage::testContractExternal);
//        TEST_ADD(TestCtcImage::testContractThin);
//        TEST_ADD(TestCtcImage::test1pixel);
//    }

////    void setup();
////    void tear_down();

//    void testContractExternal();

//    void testContractThin();

//    void test1pixel();
//private:
//    Array2D array;
////    CtcVoxelGrid *ctc;
//    double2 leaf_size;
//    double2 origin;
//    int2 grid_size;
//};

}


#endif // __TEST_CTCIMAGE_H__

