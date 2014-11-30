#include "TestRasterPicture.h"

namespace ibex {


void TestRasterPicture::setup(){

    //        leaf_size = {{0.1,0.3}};
    //        origin = {{0,2}};
    //        grid_size = {{10,20}};

    //        array.setOrigin(origin);
    //        array.setLeafSize(leaf_size);
    //        array.setGridSize(grid_size);

    //        array.init();
}

void TestRasterPicture::test_init2DRasterPicture(){

    RasterPicture2D raster;
    double leaf_size[] = {0.1,0.3};
    double origin[] = {0,2};
    int grid_size[] = {10,20};
      
    raster.setOrigin(origin[0],origin[1]);
    raster.setLeafSize(leaf_size[0],leaf_size[1]);
    raster.setGridSize(grid_size[0],grid_size[1]);
    
    raster.init();

    for(uint i = 0; i < raster.data.size(); i++){
        TEST_ASSERT(raster.data[i] == 0);
    }

    for(uint i = 0; i < raster.data.size(); i++){
        raster[i] = i;
    }

    
    raster.save("test.array2D");
    raster.load("test.array2D");



    for(int i = 0; i < raster.ndim; i++){
        TEST_ASSERT( (raster.leaf_size_[i] == leaf_size[i]) );
        TEST_ASSERT( (raster.origin_[i] == origin[i]) );
        TEST_ASSERT( (raster.grid_size_[i] == grid_size[i]) ); 
    }

    for(int i = 0; i < grid_size[0]; i++){
        for(int j = 0; j < grid_size[1]; j++){
            TEST_ASSERT_EQUALS(i + raster.divb_mul_[1]*j, raster(i,j));
        }
    }
}


void TestRasterPicture::test_init3DRasterPicture(){

    RasterPicture3D raster;
    double leaf_size[] = {0.1,0.3,0.5};
    double origin[] = {0,2,-1.632};
    int grid_size[] = {10,20,100};

    raster.setOrigin(origin[0],origin[1],origin[2]);
    raster.setLeafSize(leaf_size[0],leaf_size[1],leaf_size[2]);
    raster.setGridSize(grid_size[0],grid_size[1],grid_size[2]);

    raster.init();

    for(uint i = 0; i < raster.data.size(); i++){
        TEST_ASSERT(raster[i] == 0);
    }

    for(uint i = 0; i < raster.data.size(); i++){
        raster[i] = i;
    }

    raster.save("test.array3D");
    raster.load("test.array3D");

    for(int i = 0; i < raster.ndim; i++){
        TEST_ASSERT( (raster.leaf_size_[i] == leaf_size[i]) );
        TEST_ASSERT( (raster.origin_[i] == origin[i]) );
        TEST_ASSERT( (raster.grid_size_[i] == grid_size[i]) ); 
    }


    for(int i = 0; i < grid_size[0]; i++){
        for(int j = 0; j < grid_size[1]; j++){
            for(int k = 0; k < grid_size[2];k++){
                TEST_ASSERT_EQUALS(i + raster.divb_mul_[1]*j+raster.divb_mul_[2]*k, raster(i,j,k));
            }
        }
    }
}

void TestRasterPicture::test_readMissingFile(){
    RasterPicture3D array3d;
    TEST_ASSERT(array3d.load("missing.array3D")!= 0);
}

void TestRasterPicture::test_readWrongFileFormat_1(){
    // Generate wrong file Header
    std::ofstream out_file;
    out_file.open("wrong.array2D", std::ios::out | std::ios::trunc | std::ios::binary);

    std::ostringstream oss;
    oss.imbue (std::locale::classic ());

    oss << "VERSION " << FORMAT_VERSION;
    oss << "\nTYPE " << FF_DATA_IMAGE_ND << " " << 2 << " " << sizeof(unsigned char);
    oss << "\nLEAF_SIZE"; for(int i =0; i < 2;  i++) oss << " " << 0;
    oss << "\nORIGIN"; for(int i =0; i < 2;  i++) oss << " " << 0;
    oss << "\nEND_HEADER\n";
    try {
        out_file << oss.str();
    } catch (std::exception& e) {
        std::cerr << "error writting wrong file header";
    }
    out_file.close();
    RasterPicture2D array2d;
    TEST_ASSERT(array2d.load("wrong.array2D")!= 0);
}

void TestRasterPicture::test_readWrongFileFormat_2(){
    // Array2D array2d;
    // TEST_ASSERT(array2d.loadFromFile("test.array3D")!=0);
}


void TestRasterPicture::test_ImageIntegral2D(){
    // Array2D array2d;
    // TEST_ASSERT(array2d.loadFromFile("test.array2D")==0);
    // computeIntegralImage(array2d);
}


};


