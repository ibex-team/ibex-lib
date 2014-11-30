#include "ibex_CtcPixelMap.h"
#include "cpptest.h"


namespace ibex {

class TestCtcPixelMap : public Test::Suite {

public:
    TestCtcPixelMap(){
    TEST_ADD(TestCtcPixelMap::test3d_emptyBox);
    TEST_ADD(TestCtcPixelMap::test3d_contractEmpty);
    TEST_ADD(TestCtcPixelMap::test3d_allReal);
    TEST_ADD(TestCtcPixelMap::test3d_fullImage);
    TEST_ADD(TestCtcPixelMap::test3d_corner);

    TEST_ADD(TestCtcPixelMap::test2d_emptyBox);
    TEST_ADD(TestCtcPixelMap::test2d_contractEmpty);
    TEST_ADD(TestCtcPixelMap::test2d_allReal);
    TEST_ADD(TestCtcPixelMap::test2d_fullImage);
    TEST_ADD(TestCtcPixelMap::test2d_corner);

    }

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
//class TestCtcPixelMap : public Test::Suite {

//public:
//    TestCtcPixelMap() {
//        TEST_ADD(TestCtcPixelMap::testContractExternal);
//        TEST_ADD(TestCtcPixelMap::testContractThin);
//        TEST_ADD(TestCtcPixelMap::contractCross);
//    }

//    void add_cross(int3 center, int length, Array3D &I);
//    void add_emptyCubes(int3 center, int width, Array3D &I);
//    void setup();
//    void tear_down();
//    void contractCross();
//    void testContractExternal();
//    void testContractThin();

//private:
//    Array3D array;
//    double3 leaf_size;
//    double3 origin;
//    int3 grid_size;
//};

}


