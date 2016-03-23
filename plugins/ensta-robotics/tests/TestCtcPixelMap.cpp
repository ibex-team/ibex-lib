#include "TestCtcPixelMap.h"
#include "utils.h"

namespace ibex {

void TestCtcPixelMap::initRaster3D(PixelMap3D& raster){
    raster.set_leaf_size(0.1,0.1,0.05);
    raster.set_origin(-2,1,0);
    raster.set_grid_size(20,20,20);
    // raster.init();

    raster(10,5,8) = 1;
    raster.compute_integral_image();
}

void TestCtcPixelMap::test3d_emptyBox(){
    PixelMap3D raster;
    initRaster3D(raster);
    CtcPixelMap ctc(raster);

    IntervalVector v1(3,Interval::EMPTY_SET);

    ctc.contract(v1);

    CPPUNIT_ASSERT(v1.is_empty());
}

void TestCtcPixelMap::test3d_contractEmpty(){
    PixelMap3D raster;
    initRaster3D(raster);
    CtcPixelMap ctc(raster);


    double v1_[3][2] = {{1,0},{2,3},{0,1}};
    IntervalVector v1(3,v1_);

    ctc.contract(v1);

    CPPUNIT_ASSERT(v1.is_empty());

}
void TestCtcPixelMap::test3d_allReal(){
    PixelMap3D raster;
    initRaster3D(raster);
    CtcPixelMap ctc(raster);
    IntervalVector v1(3,Interval::ALL_REALS);

    ctc.contract(v1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1,v1[0].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.9,v1[0].ub() ,ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5,v1[1].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.6,v1[1].ub(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4,v1[2].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.45,v1[2].ub(),ERROR);
}

void TestCtcPixelMap::test3d_fullImage(){
    PixelMap3D raster;
    initRaster3D(raster);
    CtcPixelMap ctc(raster);

    double v1_[3][2] = {{-2,0},{1,3},{0,1}};
    IntervalVector v1(3,v1_);

    ctc.contract(v1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1,v1[0].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.9,v1[0].ub() ,ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5,v1[1].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.6,v1[1].ub(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4,v1[2].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.45,v1[2].ub(),ERROR);
}

void TestCtcPixelMap::test3d_corner(){
    PixelMap3D raster;
    initRaster3D(raster);
    CtcPixelMap ctc(raster);

    double v1_[3][2] = {{-2,-0.95},{1,1.55},{0,0.45}};
    IntervalVector v1(3,v1_);

    ctc.contract(v1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1,v1[0].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.95,v1[0].ub() ,ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5,v1[1].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.55,v1[1].ub(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4,v1[2].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.45,v1[2].ub(),ERROR);

}


// -------------------------------------------------------------------------------------
// ---------------------- 2D Test ------------------------------------------------------
// -------------------------------------------------------------------------------------

void TestCtcPixelMap::initRaster2D(PixelMap2D& raster){
    raster.set_leaf_size(0.1,0.1);
    raster.set_origin(-2,1);
    raster.set_grid_size(20,20);
    // raster.init();

    raster(10,5) = 1;
    raster.compute_integral_image();
}

void TestCtcPixelMap::test2d_emptyBox(){
    PixelMap2D raster;
    initRaster2D(raster);
    CtcPixelMap ctc(raster);

    IntervalVector v1(2,Interval::EMPTY_SET);

    ctc.contract(v1);

    CPPUNIT_ASSERT(v1.is_empty());
}

void TestCtcPixelMap::test2d_contractEmpty(){
    PixelMap2D raster;
    initRaster2D(raster);
    CtcPixelMap ctc(raster);


    double v1_[2][2] = {{1,0},{2,3}};
    IntervalVector v1(2,v1_);

    ctc.contract(v1);

    CPPUNIT_ASSERT(v1.is_empty());

}
void TestCtcPixelMap::test2d_allReal(){
    PixelMap2D raster;
    initRaster2D(raster);
    CtcPixelMap ctc(raster);
    IntervalVector v1(2,Interval::ALL_REALS);

    ctc.contract(v1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1,v1[0].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.9,v1[0].ub() ,ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5,v1[1].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.6,v1[1].ub(),ERROR);
}

void TestCtcPixelMap::test2d_fullImage(){
    PixelMap2D raster;
    initRaster2D(raster);
    CtcPixelMap ctc(raster);

    double v1_[2][2] = {{-2,0},{1,3}};
    IntervalVector v1(2,v1_);

    ctc.contract(v1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1,v1[0].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.9,v1[0].ub() ,ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5,v1[1].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.6,v1[1].ub(),ERROR);
}

void TestCtcPixelMap::test2d_corner(){
    PixelMap2D raster;
    initRaster2D(raster);
    CtcPixelMap ctc(raster);

    double v1_[2][2] = {{-2,-0.95},{1,1.55}};
    IntervalVector v1(2,v1_);

    ctc.contract(v1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1,v1[0].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.95,v1[0].ub() ,ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5,v1[1].lb(),ERROR);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.55,v1[1].ub(),ERROR);
}


//void TestCtcPixelMap::add_cross(int3 center, int length, Array2d &I){
//    for(int  i = -length ; i <= length; i++){
//        I({{center[0] + i, center[1], center[2] }}) = 1;
//        I({{center[0], center[1] +i , center[2] }}) = 1;
//        I({{center[0], center[1], center[2] + i }}) = 1;
//    }
//}

//void TestCtcPixelMap::add_emptyCubes(int3 center, int width, Array3D &I){

//    for(int i = -width; i <= width; i++ ){
//        for(int j = -width; j <= width; j++ ){
//            for(int k = -width; k <= width; k++ ){
//                I({{center[0] + i, center[1] + j, center[2]+k }}) = 1;
//            }
//        }
//    }

//}

//void TestCtcPixelMap::setup(){
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


//void TestCtcPixelMap::contractCross(){
//    IntervalVector box(3), res(3);
//    box[0] = Interval(50).inflate(20);
//    box[1] = Interval(50).inflate(20);
//    box[2] = Interval(20).inflate(20);

//    res[0] = Interval(50).inflate(10);
//    res[1] = Interval(50).inflate(10);
//    res[2] = Interval(20).inflate(10);

//    computeIntegralImage(array);
//    CtcPixelMap ctc(array);
//    ctc.contract(box);
//    }catch (EmptyBoxException& ) { box.set_empty();}
//    for(unsigned int i = 0; i < box.size(); i++){
//        CPPUNIT_ASSERT_EQUALS(res[i].lb(),box[i].lb());
//        CPPUNIT_ASSERT_EQUALS(res[i].ub(),box[i].ub());
//    }


//}

//void TestCtcPixelMap::tear_down(){

//}


//void TestCtcPixelMap::testContractExternal(){

//    leaf_size = {{1,1,1}};
//    origin = {{0,0,0}};
//    grid_size = {{20,20,20}};
//    array.setOrigin(origin);
//    array.setLeafSize(leaf_size);
//    array.setGridSize(grid_size);
//    array.init();

//    // Generate shape
//    int x0 = 2, y0 = 2, z0 = 2;
//    for(unsigned int i = x0; i < x0+5; i++){
//        for(unsigned int j = y0; j < y0+5; j++){
//            for(unsigned int k = z0; k < z0+5; k++){
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
//    }catch (EmptyBoxException& ) { v.set_empty();}
//    for(unsigned int i = 0; i < v.size(); i++){
//        CPPUNIT_ASSERT_EQUALS(res[i].lb(),v[i].lb());
//        CPPUNIT_ASSERT_EQUALS(res[i].ub(),v[i].ub());
//    }


//}

//void TestCtcPixelMap::testContractThin(){
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
//    }catch (EmptyBoxException& ) { v.set_empty();}
//    for(unsigned int i = 0; i < v.size(); i++){
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(res[i].lb(),v[i].lb(), std::numeric_limits<double>::epsilon());
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(res[i].ub(),v[i].ub(), std::numeric_limits<double>::epsilon());
//    }



//}

}


