#include "src/Array3D.h"
#include "cpptest.h"

class TestArray3D : public Test::Suite {

public:
    TestArray3D() {
        TEST_ADD(TestArray3D::testInitArray);
        TEST_ADD(TestArray3D::saveToDisk);
        TEST_ADD(TestArray3D::loadFromDisk);
        TEST_ADD(TestArray3D::loadFromDisk_failed);
        TEST_ADD(TestArray3D::testIntegralImage);
    }

    void setup(){

        leaf_size = {{0.1,0.1,0.1}};
        origin = {{0,0,0}};
        grid_size = {{10,10,10}};

        array.setOrigin(origin);
        array.setLeafSize(leaf_size);
        array.setGridSize(grid_size);

        array.init();
    }

    void testInitArray(){
        TEST_ASSERT( (array.leaf_size_ == leaf_size) );
        TEST_ASSERT( (array.origin_ == origin) );
        TEST_ASSERT( (array.grid_size_ == grid_size) );
        for(uint i = 0; i < array.data.size(); i++){
            TEST_ASSERT(array[i] == 0);
        }

    }

    void testIntegralImage(){
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
        std::cerr << *array.data.end() << std::endl;
        TEST_ASSERT(array(9,9,9) == 125);
    }

    void saveToDisk(){
        TEST_ASSERT(saveArray3D("out.test",array) == 0);
    }

    void loadFromDisk(){
        computeIntegralImage(array);
        Array3D<unsigned int> tmp;
        loadArray3D("out.test",tmp);
        TEST_ASSERT( (tmp.leaf_size_ == leaf_size) );
        TEST_ASSERT( (tmp.origin_ == origin) );
        TEST_ASSERT( (tmp.grid_size_ == grid_size) );

        TEST_ASSERT_EQUALS(array.data.size(), tmp.data.size());
        for(uint i = 0; i < array.data.size(); i++){
            TEST_ASSERT_EQUALS(array[i],tmp[i]);
        }
    }

    void loadFromDisk_failed(){
        Array3D<char> tmp;
        TEST_ASSERT(loadArray3D("failled.test",tmp) == -1);

    }
private:
    float3 leaf_size;
    float3 origin;
    int3 grid_size;

    Array3D<unsigned int> array;

};


int main(int argc, char** argv){


    TestArray3D sts;
    Test::TextOutput output(Test::TextOutput::Verbose);
    return sts.run(output,false) ? EXIT_SUCCESS : EXIT_FAILURE;

    return 0;
}
