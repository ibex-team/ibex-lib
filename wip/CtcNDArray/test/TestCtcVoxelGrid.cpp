#include "TestCtcVoxelGrid.h"


namespace ibex {

void TestCtcVoxelGrid::setup(){

    leaf_size = {{0.1,0.1,0.05}};
    origin = {{-2,1,0}};
    grid_size = {{20,20,20}};
    array.setOrigin(origin);
    array.setLeafSize(leaf_size);
    array.setGridSize(grid_size);
    array.init();

    array({{10,5,8}}) = 1;
    computeIntegralImage(array);
    TEST_ASSERT_EQUALS(1,array({{19,19,19}}));
}

void TestCtcVoxelGrid::test_emptyBox(){
    CtcVoxelGrid ctc(array);

    IntervalVector v1(3,Interval::EMPTY_SET);

    ctc.contract(v1);
    TEST_ASSERT(v1.is_empty());
}

void TestCtcVoxelGrid::test_allReal(){

    CtcVoxelGrid ctc(array);
    IntervalVector v1(3,Interval::ALL_REALS);

    ctc.contract(v1);
    TEST_ASSERT_DELTA(-1,v1[0].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(-0.9,v1[0].ub() ,std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(1.5,v1[1].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(1.6,v1[1].ub(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(0.4,v1[2].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(0.45,v1[2].ub(),std::numeric_limits<double>::epsilon());
}

void TestCtcVoxelGrid::test_fullImage(){

    CtcVoxelGrid ctc(array);

    double v1_[3][2] = {{-2,0},{1,3},{0,1}};
    IntervalVector v1(3,v1_);

    ctc.contract(v1);
    TEST_ASSERT_DELTA(-1,v1[0].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(-0.9,v1[0].ub() ,std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(1.5,v1[1].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(1.6,v1[1].ub(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(0.4,v1[2].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(0.45,v1[2].ub(),std::numeric_limits<double>::epsilon());
}

void TestCtcVoxelGrid::test_corner(){

    CtcVoxelGrid ctc(array);

    double v1_[3][2] = {{-2,-0.95},{1,1.55},{0,0.45}};
    IntervalVector v1(3,v1_);

    ctc.contract(v1);
    TEST_ASSERT_DELTA(-1,v1[0].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(-0.95,v1[0].ub() ,std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(1.5,v1[1].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(1.55,v1[1].ub(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(0.4,v1[2].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(0.45,v1[2].ub(),std::numeric_limits<double>::epsilon());

}


//void TestCtcVoxelGrid::add_cross(int3 center, int length, Array3D &I){
//    for(int  i = -length ; i <= length; i++){
//        I({{center[0] + i, center[1], center[2] }}) = 1;
//        I({{center[0], center[1] +i , center[2] }}) = 1;
//        I({{center[0], center[1], center[2] + i }}) = 1;
//    }
//}

//void TestCtcVoxelGrid::add_emptyCubes(int3 center, int width, Array3D &I){

//    for(int i = -width; i <= width; i++ ){
//        for(int j = -width; j <= width; j++ ){
//            for(int k = -width; k <= width; k++ ){
//                I({{center[0] + i, center[1] + j, center[2]+k }}) = 1;
//            }
//        }
//    }

//}

//void TestCtcVoxelGrid::setup(){
//    // Generate test shapes :
//    // define leaf size
//    leaf_size = {{1,1,1}};
//    origin = {{-5,-5,-5}};
//    grid_size = {{100,100,100}};

//    array.setOrigin(origin);
//    array.setLeafSize(leaf_size);
//    array.setGridSize(grid_size);
//    array.init();

//    // Cross centered in (-5, 6, 0)
//    add_cross({{50,50,20}},10,array);
////    add_emptyCubes({{120,180,100}},15,array);

//    array.saveNDArray("test.voxel");

//}


//void TestCtcVoxelGrid::contractCross(){
//    IntervalVector box(3), res(3);
//    box[0] = Interval(50).inflate(20);
//    box[1] = Interval(50).inflate(20);
//    box[2] = Interval(20).inflate(20);

//    res[0] = Interval(50).inflate(10);
//    res[1] = Interval(50).inflate(10);
//    res[2] = Interval(20).inflate(10);

//    computeIntegralImage(array);
//    CtcVoxelGrid ctc(array);
//    ctc.contract(box);
//    for(uint i = 0; i < box.size(); i++){
//        TEST_ASSERT_EQUALS(res[i].lb(),box[i].lb());
//        TEST_ASSERT_EQUALS(res[i].ub(),box[i].ub());
//    }


//}

//void TestCtcVoxelGrid::tear_down(){

//}


//void TestCtcVoxelGrid::testContractExternal(){

//    leaf_size = {{1,1,1}};
//    origin = {{0,0,0}};
//    grid_size = {{20,20,20}};
//    array.setOrigin(origin);
//    array.setLeafSize(leaf_size);
//    array.setGridSize(grid_size);
//    array.init();

//    // Generate shape
//    int x0 = 2, y0 = 2, z0 = 2;
//    for(uint i = x0; i < x0+5; i++){
//        for(uint j = y0; j < y0+5; j++){
//            for(uint k = z0; k < z0+5; k++){
//                array({{i,j,k}}) = 1;
//            }
//        }
//    }
//    computeIntegralImage(array);
//    CtcVoxelGrid ctc(array);


//    double v_[3][2] = {{0,10},{0,10},{0,10}};
//    double res_[3][2] = {{2,7},{2,7},{2,7}};
//    IntervalVector v(3,v_);
//    IntervalVector res(3,res_);
//    ctc.contract(v);
//    for(uint i = 0; i < v.size(); i++){
//        TEST_ASSERT_EQUALS(res[i].lb(),v[i].lb());
//        TEST_ASSERT_EQUALS(res[i].ub(),v[i].ub());
//    }


//}

//void TestCtcVoxelGrid::testContractThin(){
//    leaf_size = {{0.1,0.1,0.05}};
//    origin = {{-2,1,0}};
//    grid_size = {{20,20,20}};
//    array.setOrigin(origin);
//    array.setLeafSize(leaf_size);
//    array.setGridSize(grid_size);
//    array.init();

//    array({{10,5,18}}) = 1;
//    computeIntegralImage(array);
//    CtcVoxelGrid ctc(array);


//    double v_[3][2] = {{-5,10},{-5,10},{-10,10}};
//    double res_[3][2] = {{-1,-0.9},{1.5,1.6},{0.9,0.95}};
//    IntervalVector v(3,v_);
//    IntervalVector res(3,res_);
//    ctc.contract(v);
//    for(uint i = 0; i < v.size(); i++){
//        TEST_ASSERT_DELTA(res[i].lb(),v[i].lb(), std::numeric_limits<double>::epsilon());
//        TEST_ASSERT_DELTA(res[i].ub(),v[i].ub(), std::numeric_limits<double>::epsilon());
//    }



//}

}


