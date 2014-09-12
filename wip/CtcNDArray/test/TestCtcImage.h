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
class TestCtcImage : public Test::Suite {

public:
    TestCtcImage() {
        TEST_ADD(TestCtcImage::testContractExternal);
        TEST_ADD(TestCtcImage::testContractThin);
    }

//    void setup();
//    void tear_down();

    void testContractExternal();

    void testContractThin();

private:
    Array2D array;
//    CtcVoxelGrid *ctc;
    double2 leaf_size;
    double2 origin;
    int2 grid_size;
};

}


#endif // __TEST_CTCIMAGE_H__

