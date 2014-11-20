#include "ibex_CtcRasterPicture.h"
#include "cpptest.h"


namespace ibex {

class TestCtcRasterPicture : public Test::Suite {

public:
    TestCtcRasterPicture(){
    TEST_ADD(TestCtcRasterPicture::test3d_emptyBox);
    TEST_ADD(TestCtcRasterPicture::test3d_contractEmpty);
    TEST_ADD(TestCtcRasterPicture::test3d_allReal);
    TEST_ADD(TestCtcRasterPicture::test3d_fullImage);
    TEST_ADD(TestCtcRasterPicture::test3d_corner);

    TEST_ADD(TestCtcRasterPicture::test2d_emptyBox);
    TEST_ADD(TestCtcRasterPicture::test2d_contractEmpty);
    TEST_ADD(TestCtcRasterPicture::test2d_allReal);
    TEST_ADD(TestCtcRasterPicture::test2d_fullImage);
    TEST_ADD(TestCtcRasterPicture::test2d_corner);

    }

protected:
    void initRaster3D(RasterPicture3D& raster);
    void initRaster2D(RasterPicture2D& raster);
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
//class TestCtcRasterPicture : public Test::Suite {

//public:
//    TestCtcRasterPicture() {
//        TEST_ADD(TestCtcRasterPicture::testContractExternal);
//        TEST_ADD(TestCtcRasterPicture::testContractThin);
//        TEST_ADD(TestCtcRasterPicture::contractCross);
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


