#ifndef __ARRAY_ND_H__
#define __ARRAY_ND_H__

#include <vector>
#include <array>

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <string>
#include <cerrno>
#include <stdlib.h>
#include <cstring>
#include <assert.h>
#include <sstream>


#define FORMAT_VERSION "1.0.0"
#define FF_DATA_IMAGE_ND "DATA_IMD_ND"



using std::ofstream;
using std::ifstream;
using std::ios;

namespace ibex {
/**
 * struct NDArray is a n-dimensinnal array.
 * It stores occupency grid and integral image.
 * data are stored in a big array and ofsets described by divb_mul_
 * is used to
 *
 */
template< typename T, unsigned int ndim>
struct NDArray{
    // type definition for
    typedef std::array<int, ndim> intN;
    typedef std::array<uint, ndim> uintN;
    typedef std::array<double,ndim> doubleN;


    /** \brief Vector storing data. */
    std::vector<T> data;
    /** \brief The size of a leaf. */
    doubleN leaf_size_;
    /** \brief The coordinate of the bottom left front conner */
    doubleN origin_;
    /** \brief The size of the grid in each dimension. */
    intN grid_size_;
    /** \brief The division multiplier. */
    intN divb_mul_;

    T zero;

    inline void init(){

        int size = grid_size_[0];
        for(uint i = 1; i < ndim; i++) size*=grid_size_[i];
        assert(size > 0);
        data.resize(size);
        std::fill(data.begin(),data.end(),0);
        // Compute offsets
        divb_mul_[0] = 1;
        for(uint i = 1; i < ndim; i++){
            divb_mul_[i] = divb_mul_[i-1]*grid_size_[i-1];
        }
        memset(&zero,0,sizeof(T));
    }

    inline void init(const NDArray<T,ndim> &array){
        leaf_size_= array.leaf_size_;
        origin_ = array.origin_;
        grid_size_ = array.grid_size_;
        init();
    }

    inline void setOrigin(const doubleN& origin){
        origin_ = origin;
    }
    inline void setLeafSize(const doubleN& leaf_size){
        leaf_size_ = leaf_size;
    }

    inline void setGridSize(const intN &grid_size){
        grid_size_ = grid_size;
    }

    T& operator()(const intN &p) {
        int idx = 0;
        for(uint i = 0;i < p.size(); i++){
            if(p[i] < 0) return zero;
            idx += divb_mul_[i]*p[i];
        }
        assert(idx < data.size());
        return data.at(idx);
    }

    T& operator[](int idx){
        return data.at(idx);
    }


    int inline saveNDArray(const char *filename)
    {
        ofstream out_file;
        out_file.open(filename, ios::out | ios::trunc | ios::binary);
        if(out_file.fail())
        {
            std::cerr << "\n[NDArray::loadFromFile] Cannot open file " << filename << "for dumping data \n";
            return -1;
        }

        if(writeHeader(out_file, *this) == -1){
    //        std::cerr << "[NDArray::saveNDArray]: fail to write header !\n";
            return -1;
        }

        try{
            // write data
            out_file.write((char*)&data[0],data.size()*sizeof(T));
        } catch (std::exception &e){
            std::cerr  << "[NDArray::loadFromFile] writing error " << e.what() << std::endl;
        }
    //    std::cerr  << "write II3D data into " << filename << "done \n";
        out_file.close();
        return 0;
    }



    // read header
    int inline readHeader(ifstream &in_file, NDArray<T, ndim> & output){

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
                    if( tag.compare(FF_DATA_IMAGE_ND) != 0)
                        throw "File format does not match the required file format" ;
                    int dim; sstream >> dim;
                    if(dim != ndim){
                        char buf[128];
                        sprintf(buf,"Array dimension of the file does not match the dimension of the array %d != %d",dim,ndim );
                        throw buf;
                    }
//                    assert(dim == ndim);
                    continue;
                }

                // Get the acquisition viewpoint
                if (line_type.substr (0, 9) == "LEAF_SIZE")
                {
                    doubleN tmp;
                    int n = 0;
                    while(sstream >> tmp[n]){
                        n++;
                    }
                    output.setLeafSize(tmp);
                    leaf_size_is_set = true;
                    continue;
                }

                // Get the acquisition viewpoint
                if (line_type.substr (0, 6) == "ORIGIN")
                {
                    doubleN tmp;
                    int n = 0;
                    while(sstream >> tmp[n]){
                        n++;
                    }
                    output.setOrigin(tmp);
                    origin_is_set = true;
                    continue;
                }

                // Get the acquisition viewpoint
                if (line_type.substr (0, 9) == "GRID_SIZE")
                {
                    intN tmp;
                    int n = 0;
                    while(sstream >> tmp[n]){
                        n++;
                    }
                    output.setGridSize(tmp);
                    grid_size_is_set = true;
                    continue;
                }
                if (line_type.substr(0,10) == "END_HEADER")
                    break;

            }
        } catch (const char *exception)
        {
            std::cerr << "[NDArray::readHeader] "  <<  exception << "\n";
            in_file.close ();
            return (-1);
        }

        if(leaf_size_is_set && grid_size_is_set && origin_is_set){
            output.init();
        } else {
            std::cerr << "[NDArray::readHeader] FIELD ";
            if(!leaf_size_is_set) std::cerr << "LEAF_SIZE ";
            if(!grid_size_is_set) std::cerr << "GRID_SIZE ";
            if(!origin_is_set) std::cerr << "ORIGIN ";
            std::cerr << "is missing\n";
            return -1;
        }
        return 0;

    }


    int inline loadFromFile(const char *filename)
    {
        std::ifstream in_file;
        in_file.open(filename, ios::in | ios::binary);
        if(in_file.fail())
        {
            std::cerr  << "\n[NDArray::loadFromFile]: Cannot open file " << filename << " for reading data \n";
            return -1;
        }

        if(readHeader(in_file, *this) == -1){
    //        std::cerr << "[NDArray::loadFromFile]: fail to read header !\n";
            return -1;
        }
        try{
            for(uint i = 0; i < data.size(); i++){
                T tmp = 0;
                in_file.read((char*)&tmp,sizeof(T));
                this->data[i] = tmp;

            }
        } catch (std::exception &e){
            std::cerr  << "[NDArray::loadFromFile]: error loading data array" << e.what() << std::endl;
        }
        in_file.close();
    //    std::cerr  << " read " << output.data.size() << " cubes\n";
    }

    int inline writeHeader(ofstream &out_file, const NDArray<T,ndim>& input){
        std::ostringstream oss;
        oss.imbue (std::locale::classic ());

        oss << "VERSION " << FORMAT_VERSION;
        oss << "\nTYPE " <<FF_DATA_IMAGE_ND << " " << ndim << " " << sizeof(T);
        oss << "\nLEAF_SIZE"; for(const auto& val: input.leaf_size_) oss << " " << val;
        oss << "\nORIGIN"; for(const auto& val: input.origin_) oss << " " << val;
        oss << "\nGRID_SIZE"; for(const auto& val: input.grid_size_) oss << " " << val;
        oss << "\nEND_HEADER\n";
        try{
            out_file << oss.str();
        } catch (std::exception &e){
                return -1;
        }
        return 0;
    }

};

typedef NDArray<unsigned int, 1> Array1D;
typedef NDArray<unsigned int, 2> Array2D;
typedef NDArray<unsigned int, 3> Array3D;
typedef NDArray<unsigned int, 2>::doubleN double2;
typedef NDArray<unsigned int, 2>::intN int2;
typedef NDArray<unsigned int, 3>::doubleN double3;
typedef NDArray<unsigned int, 3>::intN int3;






// Compute the integral image of the array passed as argument.
// The computation is done in place.
inline void computeIntegralImage( Array2D &I)
{
    assert(I.data.size()>0);
    for(int i = 0; i < I.grid_size_[0]; i++){
        for(int j = 0; j < I.grid_size_[1]; j++){
            uint &val = I({{i,j}});
            val = I({{i-1,j}}) + I({{i,j-1}}) + I({{i,j}}) - I({{i-1,j-1}});
        }
    }
}

inline void computeIntegralImage( Array3D &I)
{
    assert(I.data.size()>0);
        for(int i = 0; i < I.grid_size_[0]; i++){
            for(int j = 0; j < I.grid_size_[1]; j++){
                for(int k = 0; k < I.grid_size_[2]; k++){
                    uint &val = I({{i,j,k}});
                    val = I({{i-1,j,k}}) + I({{i,j-1,k}}) + I({{i,j,k-1}}) + I({{i-1, j-1, k-1}}) + I({{i,j,k}})
                    - I({{i-1,j,k-1}}) - I({{i,j-1,k-1}}) - I({{i-1,j-1,k}});

                }
            }
        }
}

};





#endif //__ARRAY_ND_H__
