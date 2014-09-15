#include "TestNDArray.h"

namespace ibex {


void TestNDArray::setup(){

    //        leaf_size = {{0.1,0.3}};
    //        origin = {{0,2}};
    //        grid_size = {{10,20}};

    //        array.setOrigin(origin);
    //        array.setLeafSize(leaf_size);
    //        array.setGridSize(grid_size);

    //        array.init();
}

void TestNDArray::test_init2DArray(){

    Array2D array;
    double2 leaf_size = {{0.1,0.3}};
    double2 origin = {{0,2}};
    int2 grid_size = {{10,20}};

    array.setOrigin(origin);
    array.setLeafSize(leaf_size);
    array.setGridSize(grid_size);

    array.init();

    for(uint i = 0; i < array.data.size(); i++){
        TEST_ASSERT(array[i] == 0);
    }

    for(uint i = 0; i < array.data.size(); i++){
        array[i] = i;
    }

    array.saveNDArray("test.array2D");
    array.loadFromFile("test.array2D");

    TEST_ASSERT( (array.leaf_size_ == leaf_size) );
    TEST_ASSERT( (array.origin_ == origin) );
    TEST_ASSERT( (array.grid_size_ == grid_size) );

    for(int i = 0; i < grid_size[0]; i++){
        for(int j = 0; j < grid_size[1]; j++){
            TEST_ASSERT_EQUALS(i + array.divb_mul_[1]*j, array({{i,j}}));
        }
    }
}


void TestNDArray::test_init3DArray(){

    Array3D array;
    double3 leaf_size = {{0.1,0.3,0.5}};
    double3 origin = {{0,2,-1.632}};
    int3 grid_size = {{10,20,100}};

    array.setOrigin(origin);
    array.setLeafSize(leaf_size);
    array.setGridSize(grid_size);

    array.init();

    for(uint i = 0; i < array.data.size(); i++){
        TEST_ASSERT(array[i] == 0);
    }

    for(uint i = 0; i < array.data.size(); i++){
        array[i] = i;
    }

    array.saveNDArray("test.array3D");
    array.loadFromFile("test.array3D");

    TEST_ASSERT( (array.leaf_size_ == leaf_size) );
    TEST_ASSERT( (array.origin_ == origin) );
    TEST_ASSERT( (array.grid_size_ == grid_size) );

    for(int i = 0; i < grid_size[0]; i++){
        for(int j = 0; j < grid_size[1]; j++){
            for(int k = 0; k < grid_size[2];k++){
                TEST_ASSERT_EQUALS(i + array.divb_mul_[1]*j+array.divb_mul_[2]*k, array({{i,j,k}}));
            }
        }
    }
}

void TestNDArray::test_readMissingFile(){
    Array3D array3d;
    TEST_ASSERT(array3d.loadFromFile("missing.array3D")!= 0);
}

void TestNDArray::test_readWrongFileFormat_1(){
    Array3D array3d;
    TEST_ASSERT(array3d.loadFromFile("test.array2D")!= 0);
}

void TestNDArray::test_readWrongFileFormat_2(){
    Array2D array2d;
    TEST_ASSERT(array2d.loadFromFile("test.array3D")!=0);
}


void TestNDArray::test_ImageIntegral2D(){
    Array2D array2d;
    TEST_ASSERT(array2d.loadFromFile("test.array2D")==0);
    computeIntegralImage(array2d);
}


};


