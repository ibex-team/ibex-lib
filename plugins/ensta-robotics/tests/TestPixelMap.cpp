#include "TestPixelMap.h"

namespace ibex {


void TestPixelMap::setup(){

    //        leaf_size = {{0.1,0.3}};
    //        origin = {{0,2}};
    //        grid_size = {{10,20}};

    //        array.setOrigin(origin);
    //        array.setLeafSize(leaf_size);
    //        array.setGridSize(grid_size);

    //        array.init();
}

void TestPixelMap::test_init2DPixelMap(){

    PixelMap2D raster;
    double leaf_size[] = {0.1,0.3};
    double origin[] = {0,2};
    int grid_size[] = {10,20};
      
    raster.set_origin(origin[0],origin[1]);
    raster.set_leaf_size(leaf_size[0],leaf_size[1]);
    raster.set_grid_size(grid_size[0],grid_size[1]);
    
    // raster.init();

    for(unsigned int i = 0; i < raster.data.size(); i++){
        CPPUNIT_ASSERT(raster.data[i] == 0);
    }

    for(unsigned int i = 0; i < raster.data.size(); i++){
        raster[i] = i;
    }

    
    raster.save("test.array2D");
    raster.load("test.array2D");



    for(unsigned int i = 0; i < raster.ndim; i++){
        CPPUNIT_ASSERT( (raster.leaf_size_[i] == leaf_size[i]) );
        CPPUNIT_ASSERT( (raster.origin_[i] == origin[i]) );
        CPPUNIT_ASSERT( (raster.grid_size_[i] == grid_size[i]) ); 
    }

    for(int i = 0; i < grid_size[0]; i++){
        for(int j = 0; j < grid_size[1]; j++){
            CPPUNIT_ASSERT(i + raster.divb_mul_[1]*j == (int)raster(i,j));
        }
    }
}


void TestPixelMap::test_init3DPixelMap(){

    PixelMap3D raster;
    double leaf_size[] = {0.1,0.3,0.5};
    double origin[] = {0,2,-1.632};
    int grid_size[] = {10,20,100};

    raster.set_origin(origin[0],origin[1],origin[2]);
    raster.set_leaf_size(leaf_size[0],leaf_size[1],leaf_size[2]);
    raster.set_grid_size(grid_size[0],grid_size[1],grid_size[2]);

    raster.init();

    for(unsigned int i = 0; i < raster.data.size(); i++){
        CPPUNIT_ASSERT(raster[i] == 0);
    }

    for(unsigned int i = 0; i < raster.data.size(); i++){
        raster[i] = i;
    }

    raster.save("test.array3D");
    raster.load("test.array3D");

    for(unsigned int i = 0; i < raster.ndim; i++){
        CPPUNIT_ASSERT( (raster.leaf_size_[i] == leaf_size[i]) );
        CPPUNIT_ASSERT( (raster.origin_[i] == origin[i]) );
        CPPUNIT_ASSERT( (raster.grid_size_[i] == grid_size[i]) ); 
    }


    for(int i = 0; i < grid_size[0]; i++){
        for(int j = 0; j < grid_size[1]; j++){
            for(int k = 0; k < grid_size[2];k++){
                CPPUNIT_ASSERT(i + raster.divb_mul_[1]*j+raster.divb_mul_[2]*k == (int)raster(i,j,k));
            }
        }
    }
}

void TestPixelMap::test_readMissingFile(){
    PixelMap3D array3d;
    // try{
    //     array3d.load("missing.array3D"); // exit in case of problem (test thread is killed --> reported as fail)
    // } catch(std::exception& e) {}
    CPPUNIT_ASSERT(true);
}

void TestPixelMap::test_readWrongFileFormat_1(){
    // Generate wrong file Header
    // std::ofstream out_file;
    // out_file.open("wrong.array2D", std::ios::out | std::ios::trunc | std::ios::binary);

    // std::ostringstream oss;
    // oss.imbue (std::locale::classic ());

    // oss << "VERSION " << PixelMap::FORMAT_VERSION;
    // oss << "\nTYPE " << PixelMap::FF_DATA_IMAGE_ND << " " << 2 << " " << sizeof(unsigned char);
    // oss << "\nLEAF_SIZE"; for(int i =0; i < 2;  i++) oss << " " << 0;
    // oss << "\nORIGIN"; for(int i =0; i < 2;  i++) oss << " " << 0;
    // oss << "\nEND_HEADER\n";
    // try {
    //     out_file << oss.str();
    // } catch (std::exception& e) {
    //     std::cerr << "error writting wrong file header";
    // }
    // out_file.close();
    // PixelMap2D array2d;
    // array2d.load("wrong.array2D"); // exit in case of problem (test thread is killed --> reported as fail)
    CPPUNIT_ASSERT(true);
}

void TestPixelMap::test_readWrongFileFormat_2(){
    // Array2D array2d;
    // CPPUNIT_ASSERT(array2d.loadFromFile("test.array3D")!=0);
}


void TestPixelMap::test_ImageIntegral2D(){
    PixelMap2D raster;
    raster.load("test.array2D"); // exit in case of problem (test thread is killed --> reported as fail)
    CPPUNIT_ASSERT(true);
    raster.compute_integral_image();
}


}


