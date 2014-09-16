#include "ibex_NDArray.h"
#include "ibex_CtcVoxelGrid.h"
#include "cpptest.h"


namespace ibex {

class TestCtcVoxelGrid : public Test::Suite {

public:
    TestCtcVoxelGrid(){
    TEST_ADD(TestCtcVoxelGrid::test_emptyBox);
    TEST_ADD(TestCtcVoxelGrid::test_allReal);
    TEST_ADD(TestCtcVoxelGrid::test_fullImage);
    TEST_ADD(TestCtcVoxelGrid::test_corner);

    }

protected:
    void setup();
private:
    void test_emptyBox();
    void test_allReal();
    void test_fullImage();
    void test_corner();

    Array3D array;
    double3 leaf_size;
    double3 origin;
    int3 grid_size;
};
//class TestCtcVoxelGrid : public Test::Suite {

//public:
//    TestCtcVoxelGrid() {
//        TEST_ADD(TestCtcVoxelGrid::testContractExternal);
//        TEST_ADD(TestCtcVoxelGrid::testContractThin);
//        TEST_ADD(TestCtcVoxelGrid::contractCross);
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


