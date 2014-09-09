#include "src/Array3D.h"
#include "cpptest.h"

class TestArray3D : public Test::Suite {

public:
    TestArray3D() {
        TEST_ADD(TestArray3D::testInitArray);
        TEST_ADD(TestArray3D::saveToDisk);
        TEST_ADD(TestArray3D::loadFromDisk);
        TEST_ADD(TestArray3D::loadFromDisk_failed);
    }

    void setup(){

        leaf_size = {{0.1,0.1,0.1}};
        origin = {{0,0,0}};
        grid_size = {{5,5,20}};

        array.setOrigin(origin);
        array.setLeafSize(leaf_size);
        array.setGridSize(grid_size);

        array.init();
    }

    void testInitArray(){
        TEST_ASSERT( (array.leaf_size_ == leaf_size) );
        TEST_ASSERT( (array.origin_ == origin) );
        TEST_ASSERT( (array.grid_size_ == grid_size) );

    }

    void saveToDisk(){
        TEST_ASSERT(saveArray3D("out.test",array) == 0);
    }

    void loadFromDisk(){
        Array3D<char> tmp;
        loadArray3D("out.test",tmp);
        TEST_ASSERT( (tmp.leaf_size_ == leaf_size) );
        TEST_ASSERT( (tmp.origin_ == origin) );
        TEST_ASSERT( (tmp.grid_size_ == grid_size) );
    }

    void loadFromDisk_failed(){
        Array3D<char> tmp;
        TEST_ASSERT(loadArray3D("failled.test",tmp) == -1);

    }
private:
    float3 leaf_size;
    float3 origin;
    int3 grid_size;

    Array3D<char> array;

};


int main(int argc, char** argv){


    TestArray3D sts;
    Test::TextOutput output(Test::TextOutput::Verbose);
    return sts.run(output,false) ? EXIT_SUCCESS : EXIT_FAILURE;

    return 0;
}
