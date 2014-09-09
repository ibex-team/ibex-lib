#include "src/Array3D.h"
#include "ibex_CtcVoxelGrid.h"
#include "cpptest.h"


using namespace ibex;
class TestCtcVoxelGrid : public Test::Suite {

public:
    TestCtcVoxelGrid() {
        TEST_ADD(TestCtcVoxelGrid::testInitArray);
        TEST_ADD(TestCtcVoxelGrid::testContractExternal);
//        TEST_ADD(TestCtcVoxelGrid::loadFromDisk);
//        TEST_ADD(TestCtcVoxelGrid::loadFromDisk_failed);
    }

    void setup(){

        leaf_size = {{1,1,1}};
        origin = {{0,0,0}};
        grid_size = {{10,10,10}};
        array.setOrigin(origin);
        array.setLeafSize(leaf_size);
        array.setGridSize(grid_size);
        array.init();

        // Generate shape
        int x0 = 2, y0 = 2, z0 = 2;
        for(uint i = x0; i < x0+5; i++){
            for(uint j = y0; j < y0+5; j++){
                for(uint k = z0; k < z0+5; k++){
                    array(i,j,k) = 1;
                }
            }
        }
        computeIntegralImage(array);
        std::cerr << "cII " << array(9,9,9) << std::endl;
//        saveArray3D("out.ii3D",array);
        ctc = new CtcVoxelGrid(array);
    }

    void tear_down(){
        delete ctc;
    }

    void testInitArray(){
        TEST_ASSERT( (array.leaf_size_ == leaf_size) );
        TEST_ASSERT( (array.origin_ == origin) );
        TEST_ASSERT( (array.grid_size_ == grid_size) );
        TEST_ASSERT_EQUALS(array(9,9,9),125);
        TEST_ASSERT_EQUALS(array(2,2,2),1);
        TEST_ASSERT_EQUALS(array(5,5,5),64);
    }


    void testContractExternal(){
        double v_[3][2] = {{0,10},{0,10},{0,10}};
        double res_[3][2] = {{2,7},{2,7},{2,7}};
        IntervalVector v(3,v_);
        IntervalVector res(3,res_);
        ctc->contract(v);
        for(uint i = 0; i < v.size(); i++){
            TEST_ASSERT_EQUALS(res[i].lb(),v[i].lb());
            TEST_ASSERT_EQUALS(res[i].ub(),v[i].ub());
        }

    }

private:
    Array3D<unsigned int> array;
    CtcVoxelGrid *ctc;
    float3 leaf_size;
    float3 origin;
    int3 grid_size;
};


int main(int argc, char** argv){


    TestCtcVoxelGrid sts;
    Test::TextOutput output(Test::TextOutput::Verbose);
    return sts.run(output,false) ? EXIT_SUCCESS : EXIT_FAILURE;

    return 0;
}


