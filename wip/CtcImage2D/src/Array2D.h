#ifndef __ARRAY_2D_H__
#define __ARRAY_2D_H__

#include <vector>
#include <array>

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <string>
#include <stdlib.h>
#include <assert.h>
#include <sstream>
//# include <sys/mman.h>
#include <cstring>
#include <cerrno>

#define FORMAT_VERSION "1.0.0"
#define FF_DATA_IMAGE_2D "DATA_IMD_2D"

typedef std::array<int, 2> int2;
typedef std::array<double,2> double2;

using std::ofstream;
using std::ifstream;
using std::ios;

template< typename T>
struct Array2D{
    inline void init(){
        assert(grid_size_[0]*grid_size_[1] > 0);
        data.resize(grid_size_[0]*grid_size_[1],0);
        std::fill(data.begin(),data.end(),0);
        divb_mul_[0] = 1;
        divb_mul_[1] = grid_size_[0];
        memset(&zero,0,sizeof(T));
    }

    inline void init(const Array2D<T> &array){
        leaf_size_= array.leaf_size_;
        origin_ = array.origin_;
        grid_size_ = array.grid_size_;
        init();
    }

    inline void setOrigin(const double2& origin){
        origin_ = origin;
    }
    inline void setLeafSize(const double2& leaf_size){
        leaf_size_ = leaf_size;
    }

    inline void setGridSize(const int2 &grid_size){
        grid_size_ = grid_size;
    }

    T& operator()(int x, int y) {
        if(x < 0 || y < 0) return zero;
        int idx = x + y * divb_mul_[1];
        return data.at(idx);
    }

    T& operator[](int idx){
        return data.at(idx);
    }

    /** \brief Vector storing data. */
    std::vector<T> data;
    /** \brief The size of a leaf. */
    double2 leaf_size_;
    /** \brief The coordinate of the bottom left front conner */
    double2 origin_;
    /** \brief The size of the grid in each dimension. */
    int2 grid_size_;
    /** \brief The division multiplier. */
    int2 divb_mul_;

    T zero;
};


// Compute the integral image of the array passed as argument.
// The computation is done in place.
inline void computeIntegralImage( Array2D<unsigned int> &I)
{
    assert(I.data.size()>0);
    for(uint i = 0; i < I.grid_size_[0]; i++){
        for(uint j = 0; j < I.grid_size_[1]; j++){
            uint &val = I(i,j);
            val = I(i-1,j) + I(i,j-1) + I(i,j) - I(i-1,j-1);
        }
    }
}

template<typename T>
int writeHeader(ofstream &out_file, const Array2D<T>& input){
    std::ostringstream oss;
    oss.imbue (std::locale::classic ());

    oss << "VERSION " << FORMAT_VERSION;
    oss << "\nTYPE " <<FF_DATA_IMAGE_2D << " " << sizeof(T);
    oss << "\nLEAF_SIZE " << input.leaf_size_[0] << " "  << input.leaf_size_[1];
    oss << "\nORIGIN " << input.origin_[0] << " "  << input.origin_[1];
    oss << "\nGRID_SIZE " << input.grid_size_[0] << " "  << input.grid_size_[1];
    oss << "\n";
    try{
        out_file << oss.str();
    } catch (std::exception &e){
            return -1;
    }
    return 0;
}

template<typename T>
int saveArray2D(const char *filename, const Array2D<T>& input)
{
    ofstream out_file;
    out_file.open(filename, ios::out | ios::trunc | ios::binary);
    if(out_file.fail())
    {
        std::cerr << "\n[Array2D::loadArray2D] Cannot open file " << filename << "for dumping data \n";
        return -1;
    }

    if(writeHeader(out_file, input) == -1){
//        std::cerr << "[Array2D::saveArray2D]: fail to write header !\n";
        return -1;
    }

    try{
        // write data
        out_file.write((char*)&input.data[0],input.data.size()*sizeof(T));
    } catch (std::exception &e){
        std::cerr  << "[Array2D::loadArray2D] writing error " << e.what() << std::endl;
    }
//    std::cerr  << "write II3D data into " << filename << "done \n";
    out_file.close();
    return 0;
}



// read header
template<typename T>
int readHeader(ifstream &in_file, Array2D<T> & output){

    std::string line;
    bool leaf_size_is_set = false, origin_is_set = false, grid_size_is_set  = false;
    // Read the header and fill it in with wonderful values
    try
    {
        while (!in_file.eof ())
        {
            getline (in_file, line);
            // Ignore empty lines
            if (line == "")
                continue;

            std::stringstream sstream (line);
            sstream.imbue (std::locale::classic ());

            std::string line_type;
            sstream >> line_type;

            // Ignore comments
            if (line_type.substr (0, 1) == "#")
                continue;

            // Version numbers are not needed for now, but we are checking to see if they're there
            if (line_type.substr (0, 7) == "VERSION")
                continue;

            // Get the file format tag
            if ( (line_type.substr (0, 4) == "TYPE" ) )
            {
                std::string tag;
                sstream >> tag;
                if( tag.compare(FF_DATA_IMAGE_2D) != 0)
                    throw "File format does not match the required file format" ;
                continue;
            }

            // Get the acquisition viewpoint
            if (line_type.substr (0, 9) == "LEAF_SIZE")
            {
                float dx, dy, dz;
                sstream >> dx >> dy >> dz ;
                output.setLeafSize({{dx,dy}});
                leaf_size_is_set = true;
                continue;
            }

            // Get the acquisition viewpoint
            if (line_type.substr (0, 6) == "ORIGIN")
            {
                float x, y, z;
                sstream >> x >> y >> z ;
                output.setOrigin({{x,y}});
                origin_is_set = true;
                continue;
            }

            // Get the acquisition viewpoint
            if (line_type.substr (0, 9) == "GRID_SIZE")
            {
                int x, y, z;
                sstream >> x >> y >> z ;
                output.setGridSize({{x,y}});
                grid_size_is_set = true;
                continue;
            }

        }
    } catch (const char *exception)
    {
        std::cerr << "[Array2D::readHeader] "  <<  exception << "\n";
        in_file.close ();
        return (-1);
    }

    if(leaf_size_is_set && grid_size_is_set && origin_is_set){
        output.init();
    } else {
        std::cerr << "[Array2D::readHeader] FIELD ";
        if(!leaf_size_is_set) std::cerr << "LEAF_SIZE ";
        if(!grid_size_is_set) std::cerr << "GRID_SIZE ";
        if(!origin_is_set) std::cerr << "ORIGIN ";
        std::cerr << "is missing\n";
        return -1;
    }
    return 0;

}

template<typename T>
int inline loadArray2D(const char *filename, Array2D<T> & output)
{
    std::ifstream in_file;
    in_file.open(filename, ios::in | ios::binary);
    if(in_file.fail())
    {
        std::cerr  << "\n[Array2D::loadArray2D]: Cannot open file " << filename << "for reading data \n";
        return -1;
    }

    if(readHeader(in_file, output) == -1){
//        std::cerr << "[Array2D::loadArray2D]: fail to read header !\n";
        return -1;
    }
    try{
        in_file.read((char*)&output.data[0],output.data.size()*sizeof(T));
    } catch (std::exception &e){
        std::cerr  << "[Array2D::loadArray2D]: error loading data array" << e.what() << std::endl;
    }
    in_file.close();
//    std::cerr  << " read " << output.data.size() << " cubes\n";
}

#endif //__ARRAY_3D_H__
