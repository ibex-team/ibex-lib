#include "TestCtcImage.h"


namespace ibex {

void TestCtcImage_1pixel::setup(){

    leaf_size = {{0.1,0.1}};
    origin = {{-2,1}};
    grid_size = {{20,20}};
    array.setOrigin(origin);
    array.setLeafSize(leaf_size);
    array.setGridSize(grid_size);
    array.init();

    array({{10,5}}) = 1;
    computeIntegralImage(array);
    TEST_ASSERT_EQUALS(1,array({{19,19}}));
}

void TestCtcImage_1pixel::test_emptyBox(){
    CtcImage ctc(array);

    double v1_[2][2] = {{-2,0},{1,3}};
    IntervalVector v1(2,Interval::EMPTY_SET);

    ctc.contract(v1);
    TEST_ASSERT(v1.is_empty());
}

void TestCtcImage_1pixel::test_allReal(){

    CtcImage ctc(array);
    IntervalVector v1(2,Interval::ALL_REALS);

    ctc.contract(v1);
    TEST_ASSERT_DELTA(-1,v1[0].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(-0.9,v1[0].ub() ,std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(1.5,v1[1].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(1.6,v1[1].ub(),std::numeric_limits<double>::epsilon());
}

void TestCtcImage_1pixel::test_fullImage(){

    CtcImage ctc(array);

    double v1_[2][2] = {{-2,0},{1,3}};
    IntervalVector v1(2,v1_);

    ctc.contract(v1);
    TEST_ASSERT_DELTA(-1,v1[0].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(-0.9,v1[0].ub() ,std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(1.5,v1[1].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(1.6,v1[1].ub(),std::numeric_limits<double>::epsilon());
}

void TestCtcImage_1pixel::test_corner(){

    CtcImage ctc(array);

    double v1_[2][2] = {{-2,-0.95},{1,1.55}};
    IntervalVector v1(2,v1_);

    ctc.contract(v1);
    TEST_ASSERT_DELTA(-1,v1[0].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(-0.95,v1[0].ub() ,std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(1.5,v1[1].lb(),std::numeric_limits<double>::epsilon());
    TEST_ASSERT_DELTA(1.55,v1[1].ub(),std::numeric_limits<double>::epsilon());

}

//void TestCtcImage::testContractExternal(){

//    //        leaf_size = {{1,1,1}};
//    //        origin = {{0,0,0}};
//    //        grid_size = {{20,20,20}};
//    //        array.setOrigin(origin);
//    //        array.setLeafSize(leaf_size);
//    //        array.setGridSize(grid_size);
//    //        array.init();

//    //        // Generate shape
//    //        int x0 = 2, y0 = 2, z0 = 2;
//    //        for(uint i = x0; i < x0+5; i++){
//    //            for(uint j = y0; j < y0+5; j++){
//    //                for(uint k = z0; k < z0+5; k++){
//    //                    array(i,j,k) = 1;
//    //                }
//    //            }
//    //        }
//    //        computeIntegralImage(array);
//    //        ctc = new CtcVoxelGrid(array);


//    //        double v_[3][2] = {{0,10},{0,10},{0,10}};
//    //        double res_[3][2] = {{2,7},{2,7},{2,7}};
//    //        IntervalVector v(3,v_);
//    //        IntervalVector res(3,res_);
//    //        ctc->contract(v);
//    //        for(uint i = 0; i < v.size(); i++){
//    //            TEST_ASSERT_EQUALS(res[i].lb(),v[i].lb());
//    //            TEST_ASSERT_EQUALS(res[i].ub(),v[i].ub());
//    //        }

//    //        delete ctc;
//}

//void TestCtcImage::testContractThin(){
//    //        leaf_size = {{0.1,0.1,0.05}};
//    //        origin = {{-2,1,0}};
//    //        grid_size = {{20,20,20}};
//    //        array.setOrigin(origin);
//    //        array.setLeafSize(leaf_size);
//    //        array.setGridSize(grid_size);
//    //        array.init();

//    //        array(10,5,18) = 1;
//    //        computeIntegralImage(array);
//    //        ctc = new CtcVoxelGrid(array);


//    //        double v_[3][2] = {{-5,10},{-5,10},{-10,10}};
//    //        double res_[3][2] = {{-1,-0.9},{1.5,1.6},{0.9,0.95}};
//    //        IntervalVector v(3,v_);
//    //        IntervalVector res(3,res_);
//    //        ctc->contract(v);
//    //        for(uint i = 0; i < v.size(); i++){
//    //            TEST_ASSERT_DELTA(res[i].lb(),v[i].lb(), std::numeric_limits<double>::epsilon());
//    //            TEST_ASSERT_DELTA(res[i].ub(),v[i].ub(), std::numeric_limits<double>::epsilon());
//    //        }

//    //        delete ctc;

//}

//void TestCtcImage::test1pixel(){
//    leaf_size = {{0.1,0.1}};
//    origin = {{-2,1}};
//    grid_size = {{20,20}};
//    array.setOrigin(origin);
//    array.setLeafSize(leaf_size);
//    array.setGridSize(grid_size);
//    array.init();

//    array({{10,5}}) = 1;
//    computeIntegralImage(array);

//    CtcImage ctc(array);
//    TEST_ASSERT_EQUALS(1,array({{19,19}}));

//    double v1_[2][2] = {{-2,0},{1,3}};
//    IntervalVector v1(2,v1_);

//    ctc.contract(v1);
//    TEST_ASSERT_DELTA(-1,v1[0].lb(),std::numeric_limits<double>::epsilon());
//    TEST_ASSERT_DELTA(-0.9,v1[0].ub() ,std::numeric_limits<double>::epsilon());
//    TEST_ASSERT_DELTA(1.5,v1[1].lb(),std::numeric_limits<double>::epsilon());
//    TEST_ASSERT_DELTA(1.6,v1[1].ub(),std::numeric_limits<double>::epsilon());


//}

}

