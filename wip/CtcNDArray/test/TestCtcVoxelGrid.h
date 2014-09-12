#include "ibex_NDArray.h"
#include "ibex_CtcVoxelGrid.h"
#include "cpptest.h"


namespace ibex {
class TestCtcVoxelGrid : public Test::Suite {

public:
    TestCtcVoxelGrid() {
        TEST_ADD(TestCtcVoxelGrid::testContractExternal);
        TEST_ADD(TestCtcVoxelGrid::testContractThin);
    }

    void add_cross(int3 center, int length, Array3D &I);
    void add_emptyCubes(int3 center, int width, Array3D &I);
    void setup();
    void tear_down();
    void testContractExternal();
    void testContractThin();

private:
    Array3D array;
    CtcVoxelGrid *ctc;
    double3 leaf_size;
    double3 origin;
    int3 grid_size;
};

}


