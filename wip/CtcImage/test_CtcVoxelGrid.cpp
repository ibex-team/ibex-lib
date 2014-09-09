#include "src/Array3D.h"
#include "cpptest.h"

class TestCtcVoxelGrid : public Test::Suite {

public:
    TestCtcVoxelGrid() {
//        TEST_ADD(TestCtcVoxelGrid::testInitArray);
//        TEST_ADD(TestCtcVoxelGrid::saveToDisk);
//        TEST_ADD(TestCtcVoxelGrid::loadFromDisk);
//        TEST_ADD(TestCtcVoxelGrid::loadFromDisk_failed);
    }

    void setup(){

//        leaf_size = {{0.1,0.1,0.1}};
//        origin = {{0,0,0}};
//        grid_size = {{5,5,20}};

//        array.setOrigin(origin);
//        array.setLeafSize(leaf_size);
//        array.setGridSize(grid_size);

//        array.init();
    }



};


int main(int argc, char** argv){


    TestCtcVoxelGrid sts;
    Test::TextOutput output(Test::TextOutput::Verbose);
    return sts.run(output,false) ? EXIT_SUCCESS : EXIT_FAILURE;

    return 0;
}


