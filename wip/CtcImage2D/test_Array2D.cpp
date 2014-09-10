#include "src/Array2D.h"
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

        leaf_size = {{0.1,0.3}};
        origin = {{0,2}};
        grid_size = {{10,20}};

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
                array(i,j) = 1;
            }
        }
        computeIntegralImage(array);
        std::cerr << *array.data.end() << std::endl;
        TEST_ASSERT(array(9,19) == 25);
    }

    void saveToDisk(){
        TEST_ASSERT(saveArray2D("out.test",array) == 0);
    }

    void loadFromDisk(){
        computeIntegralImage(array);
        TEST_ASSERT(saveArray2D("out.ii2D",array) == 0);
        Array2D<unsigned int> tmp;
        loadArray2D("out.test",tmp);
        TEST_ASSERT_EQUALS( leaf_size[0], tmp.leaf_size_[0] );
        TEST_ASSERT_EQUALS( leaf_size[1], tmp.leaf_size_[1] );

        TEST_ASSERT( (tmp.origin_ == origin) );
        TEST_ASSERT( (tmp.grid_size_ == grid_size) );

        TEST_ASSERT_EQUALS(array.data.size(), tmp.data.size());
        for(uint i = 0; i < array.data.size(); i++){
            TEST_ASSERT_EQUALS(array[i],tmp[i]);
        }
    }

    void loadFromDisk_failed(){
        Array2D<char> tmp;
        TEST_ASSERT(loadArray2D("failled.test",tmp) == -1);

    }
private:
    double2 leaf_size;
    double2 origin;
    int2 grid_size;

    Array2D<unsigned int> array;

};


int main(int argc, char** argv){


    TestArray3D sts;
    Test::TextOutput output(Test::TextOutput::Verbose);
    return sts.run(output,false) ? EXIT_SUCCESS : EXIT_FAILURE;

    return 0;
}
