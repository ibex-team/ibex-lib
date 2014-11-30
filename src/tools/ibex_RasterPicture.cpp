//============================================================================
//                                  I B E X                                   
// File        : ibex_RasterPicture.cpp
// Author      : ????, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2014
//============================================================================

#include "ibex_RasterPicture.h"


#include <iostream>
#include <fcntl.h>
#include <string>
#include <cerrno>
#include <stdlib.h>
#include <cstring>
#include <assert.h>
#include <sstream>

using namespace std;

namespace ibex {

RasterPicture::RasterPicture(int ndim) : ndim(ndim), zero(0) {
	leaf_size_ = new double[ndim];
	origin_ = new double[ndim];
	grid_size_ = new int[ndim];
	divb_mul_ = new int[ndim];
}

RasterPicture::~RasterPicture() {
	delete[] leaf_size_;
	delete[] origin_;
	delete[] grid_size_;
	delete[] divb_mul_;
}

void RasterPicture::init() {


	int size = grid_size_[0];
	for(uint i=1; i<ndim; i++){
		size*=grid_size_[i];
	} 
	assert(size > 0);
	data.resize(size);
	std::fill(data.begin(),data.end(),0);
	// Compute offsets
	divb_mul_[0] = 1;
	for(uint i=1; i<ndim; i++) {
		divb_mul_[i] = divb_mul_[i-1]*grid_size_[i-1];
	}
	memset(&zero,0,sizeof(DATA_TYPE));
}

void RasterPicture::init(const RasterPicture& array) {     leaf_size_=
array.leaf_size_;     origin_ = array.origin_;     grid_size_ =
array.grid_size_;     init(); }

int RasterPicture::save(const char *filename)
{
	ofstream out_file;
	out_file.open(filename, ios::out | ios::trunc | ios::binary);
	if(out_file.fail())
	{
		std::cerr << "\n[NDArray::loadFromFile] Cannot open file " << filename << "for dumping data \n";
		return -1;
	}

	if(writeHeader(out_file, *this) == -1) {
		//        std::cerr << "[NDArray::saveNDArray]: fail to write header !\n";
		return -1;
	}

	try {
		// write data
		out_file.write((char*)&data[0],data.size()*sizeof(DATA_TYPE));
	} catch (std::exception& e){
		std::cerr  << "[NDArray::loadFromFile] writing error " << e.what() << std::endl;
	}
	//    std::cerr  << "write II3D data into " << filename << "done \n";
	out_file.close();
	return 0;
}

// read header
int RasterPicture::readHeader(ifstream &in_file, RasterPicture& output){

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
				// [gch]: Benoit, please check this part of the code
				sstream >> leaf_size_[0];
				sstream >> leaf_size_[1];
				if (ndim==3) sstream >> leaf_size_[2];

				leaf_size_is_set = true;
				continue;
			}

			// Get the acquisition viewpoint
			if (line_type.substr (0, 6) == "ORIGIN")
			{
				// [gch]: Benoit, please check this part of the code
				sstream >> origin_[0];
				sstream >> origin_[1];
				if (ndim==3) sstream >> origin_[2];

				origin_is_set = true;
				continue;
			}

			// Get the acquisition viewpoint
			if (line_type.substr (0, 9) == "GRID_SIZE")
			{
				// [gch]: Benoit, please check this part of the code
				sstream >> grid_size_[0];
				sstream >> grid_size_[1];
				if (ndim==3) sstream >> grid_size_[2];

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


int RasterPicture::load(const char *filename)
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
			DATA_TYPE tmp = 0;
			in_file.read((char*)&tmp,sizeof(DATA_TYPE));
			this->data[i] = tmp;

		}
	} catch (std::exception &e){
		std::cerr  << "[NDArray::loadFromFile]: error loading data array" << e.what() << std::endl;
	}
	in_file.close();
	return 0;
	//    std::cerr  << " read " << output.data.size() << " cubes\n";
}

int RasterPicture::writeHeader(ofstream& out_file, const RasterPicture& input) {
	std::ostringstream oss;
	oss.imbue (std::locale::classic ());

	oss << "VERSION " << FORMAT_VERSION;
	oss << "\nTYPE " << FF_DATA_IMAGE_ND << " " << ndim << " " << sizeof(DATA_TYPE);
	oss << "\nLEAF_SIZE"; for(int i =0; i < ndim;  i++) oss << " " << input.leaf_size_[i];
	oss << "\nORIGIN"; for(int i =0; i < ndim;  i++) oss << " " << input.origin_[i];
	oss << "\nGRID_SIZE"; for(int i =0; i < ndim;  i++) oss << " " << input.grid_size_[i];
	oss << "\nEND_HEADER\n";
	try {
		out_file << oss.str();
	} catch (std::exception& e) {
		return -1;
	}
	return 0;
}

// ==========================================================================================================

void RasterPicture2D::setOrigin(double ox, double oy) {
	origin_[0] = ox;
	origin_[1] = oy;
}

void RasterPicture2D::setLeafSize(double lx, double ly) {
	leaf_size_[0] = lx;
	leaf_size_[1] = ly;
}

void RasterPicture2D::setGridSize(uint ni, uint nj) {
	grid_size_[0] = ni;
	grid_size_[1] = nj;
}

// [gch]: Benoit, please check this part of the code
RasterPicture::DATA_TYPE& RasterPicture2D::operator()(uint i, uint j) {
	int idx = 0;
	if (i<0 || j<0) return zero;
	idx = divb_mul_[0]*i  + divb_mul_[1]*j;
	assert(idx < data.size());
	return data.at(idx);
}

void RasterPicture2D::computeIntegralImage() {
	assert(data.size()>0);

	for(int i=0; i<grid_size_[0]; i++) {
		for(int j=0; j<grid_size_[1]; j++) {
			uint& val = (*this)(i,j);
			val = (*this)(i-1,j) + (*this)(i,j-1) + (*this)(i,j) - (*this)(i-1,j-1);
		}
	}
}

// ==========================================================================================================

void RasterPicture3D::setOrigin(double ox, double oy, double oz) {
	origin_[0] = ox;
	origin_[1] = oy;
	origin_[2] = oz;
}

void RasterPicture3D::setLeafSize(double lx, double ly, double lz) {
	leaf_size_[0] = lx;
	leaf_size_[1] = ly;
	leaf_size_[2] = lz;
}

void RasterPicture3D::setGridSize(uint ni, uint nj, uint nk) {
	grid_size_[0] = ni;
	grid_size_[1] = nj;
	grid_size_[2] = nk;
}

// [gch]: Benoit, please check this part of the code
RasterPicture::DATA_TYPE& RasterPicture3D::operator()(uint i, uint j, uint k) {
	int idx = 0;
	if (i<0 || j<0 || k<0) return zero;
	idx = divb_mul_[0]*i  + divb_mul_[1]*j + divb_mul_[2]*k;
	assert(idx < data.size());
	return data.at(idx);
}

void RasterPicture3D::computeIntegralImage() {
	assert(data.size()>0);

	for(int i=0; i<grid_size_[0]; i++){
		for(int j=0; j<grid_size_[1]; j++){
			for(int k=0; k<grid_size_[2]; k++){
				uint& val = (*this)(i,j,k);
				val = (*this)(i-1,j,k) + (*this)(i,j-1,k) + (*this)(i,j,k-1) + (*this)(i-1, j-1, k-1) + (*this)(i,j,k)
		            - (*this)(i-1,j,k-1) - (*this)(i,j-1,k-1) - (*this)(i-1,j-1,k);

			}
		}
	}
}

} // namespace ibex
