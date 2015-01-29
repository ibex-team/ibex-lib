//============================================================================
//                                  I B E X                                   
// File        : ibex_PixelMap.cpp
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2014
//============================================================================

#include "ibex_PixelMap.h"
#include "ibex_Exception.h"

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


const char* PixelMap::FORMAT_VERSION="1.0.0";
const char* PixelMap::FF_DATA_IMAGE_ND="DATA_IMD_ND";

PixelMap::PixelMap(unsigned int ndim) : ndim(ndim), zero(0) {
	leaf_size_ = new double[ndim];
	origin_ = new double[ndim];
	grid_size_ = new int[ndim];
	divb_mul_ = new int[ndim];
}

PixelMap::PixelMap(const PixelMap& src): ndim(src.ndim), zero(0) {
	leaf_size_ = new double[ndim];
	origin_ = new double[ndim];
	grid_size_ = new int[ndim];
	divb_mul_ = new int[ndim];
	for(unsigned int i = 0; i < ndim; i++){
		leaf_size_[i] = src.leaf_size_[i];
		origin_[i] = src.origin_[i];
		grid_size_[i] = src.grid_size_[i];	
	}
	init();

	// copy image data
	data = src.data;

}

PixelMap::~PixelMap() {
	delete[] leaf_size_;
	delete[] origin_;
	delete[] grid_size_;
	delete[] divb_mul_;
}

void PixelMap::init() {

	int size = grid_size_[0];
	for(unsigned int i=1; i<ndim; i++){
		size*=grid_size_[i];
	} 
	assert(size > 0);
	data.resize(size);
	std::fill(data.begin(),data.end(),0);
	// Compute offsets
	divb_mul_[0] = 1;
	for(unsigned int i=1; i<ndim; i++) {
		divb_mul_[i] = divb_mul_[i-1]*grid_size_[i-1];
	}
	memset(&zero,0,sizeof(DATA_TYPE));
}

void PixelMap::save(const char *filename) {
	ofstream out_file;
	out_file.open(filename, ios::out | ios::trunc | ios::binary);

	if(out_file.fail()) {
		std::stringstream s;
		s << "PixelMap [save]: cannot open file " << filename << "for dumping data";
		ibex_error(s.str().c_str());
	}

	try {
        write_header(out_file, *this);
		// write data
		out_file.write((char*)&data[0],data.size()*sizeof(DATA_TYPE));
	} catch (std::exception& e) {
		std::stringstream s;
		s << "PixelMap [save]: writing error " << e.what() << std::endl;
		ibex_error(s.str().c_str());
	}

	out_file.close();
}

// read header
void PixelMap::read_header(ifstream &in_file, PixelMap& output) {

	std::string line;
	bool leaf_size_is_set = false, origin_is_set = false, grid_size_is_set  = false;

	// Read the header and fill it in with wonderful values
	while (!in_file.eof()) {

		getline (in_file, line);
		// Ignore empty lines
		if (line == "")
			continue;

		std::stringstream sstream(line);
		sstream.imbue (std::locale::classic());

		std::string line_type;
		sstream >> line_type;

		// Ignore comments
		if (line_type.substr(0, 1) == "#")
			continue;

		// Version numbers are not needed for now, but we are checking to see if they're there
		if (line_type.substr(0, 7) == "VERSION")
			continue;

		// Get the file format tag
		if (line_type.substr(0, 4) == "TYPE") {

			std::string tag;
			sstream >> tag;

			if (tag.compare(FF_DATA_IMAGE_ND) != 0) {
				in_file.close ();
				ibex_error("PixelMap [read_header]: file format does not match the required file format");
			}

			unsigned int dim; sstream >> dim;

			if (dim != ndim) {
				in_file.close ();
				std::stringstream s;
				s << "PixelMap [read_header]: array dimension of the file (" << dim << ") ";
				s << "does not match the dimension of the array (" << ndim << ")";
				ibex_error(s.str().c_str());
			}
			continue;
		}

		// Get the acquisition viewpoint
		if (line_type.substr(0, 9) == "LEAF_SIZE") {
			// [gch]: Benoit, please check this part of the code
			sstream >> leaf_size_[0];
			sstream >> leaf_size_[1];
			if (ndim==3) sstream >> leaf_size_[2];

			leaf_size_is_set = true;
			continue;
		}

		// Get the acquisition viewpoint
		if (line_type.substr (0, 6) == "ORIGIN") {
			// [gch]: Benoit, please check this part of the code
			sstream >> origin_[0];
			sstream >> origin_[1];
			if (ndim==3) sstream >> origin_[2];

			origin_is_set = true;
			continue;
		}

		// Get the acquisition viewpoint
		if (line_type.substr (0, 9) == "GRID_SIZE") {
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

	if(leaf_size_is_set && grid_size_is_set && origin_is_set) {
		output.init();
	} else {
		std::stringstream s;
		s << "PixelMap [read_header]: field ";
		if(!leaf_size_is_set) s << "LEAF_SIZE ";
		if(!grid_size_is_set) s << "GRID_SIZE ";
		if(!origin_is_set) s << "ORIGIN ";
		s << "is missing\n";

		ibex_error(s.str().c_str());
	}
}


void PixelMap::load(const char *filename) {
	std::ifstream in_file;
	in_file.open(filename, ios::in | ios::binary);
	if(in_file.fail()) {
		std::stringstream s;
		s << "PixelMap [load]: cannot open file " << filename << "for reading data";
		ibex_error(s.str().c_str());

    }

	try {
        read_header(in_file, *this);
		for(unsigned int i = 0; i < data.size(); i++) {
			DATA_TYPE tmp = 0;
			in_file.read((char*)&tmp,sizeof(DATA_TYPE));
			this->data[i] = tmp;

		}
	} catch (std::exception& e) {
		std::stringstream s;
		s << "PixelMap [load]: reading error " << e.what() << std::endl;
		ibex_error(s.str().c_str());
	}
	in_file.close();
	//    std::cerr  << " read " << output.data.size() << " cubes\n";
}

void PixelMap::write_header(ofstream& out_file, const PixelMap& input) {
	std::ostringstream oss;
	oss.imbue (std::locale::classic ());

	oss << "VERSION " << FORMAT_VERSION;
	oss << "\nTYPE " << FF_DATA_IMAGE_ND << " " << ndim << " " << sizeof(DATA_TYPE);
	oss << "\nLEAF_SIZE"; for(unsigned int i =0; i < ndim;  i++) oss << " " << input.leaf_size_[i];
	oss << "\nORIGIN";    for(unsigned int i =0; i < ndim;  i++) oss << " " << input.origin_[i];
	oss << "\nGRID_SIZE"; for(unsigned int i =0; i < ndim;  i++) oss << " " << input.grid_size_[i];
	oss << "\nEND_HEADER\n";
	try {
		out_file << oss.str();
	} catch (std::exception& e) {
		ibex_error("PixelMap [write_header]: fail to write");
	}
}

// ==========================================================================================================
PixelMap2D::PixelMap2D() : PixelMap(2) {

}

void PixelMap2D::set_origin(double ox, double oy) {
	origin_[0] = ox;
	origin_[1] = oy;
}

void PixelMap2D::set_leaf_size(double lx, double ly) {
	leaf_size_[0] = lx;
	leaf_size_[1] = ly;
}

void PixelMap2D::set_grid_size(unsigned int ni, unsigned int nj) {
	grid_size_[0] = ni;
	grid_size_[1] = nj;
	init();
}

// [gch]: Benoit, please check this part of the code
PixelMap::DATA_TYPE& PixelMap2D::operator()(int i, int j) {
	unsigned int idx = 0;
	if (i<0 || j<0) return zero;

	idx = divb_mul_[0]*i  + divb_mul_[1]*j;

	if(idx >= data.size()) {
		cout << idx << " " << data.size() << " " << i << " " << j << "\n";
		cout.flush();
	}

	assert(idx < data.size());
	return data.at(idx);
}

void PixelMap2D::compute_integral_image() {
	assert(data.size()>0);

	for(int i=0; i<grid_size_[0]; i++) {
		for(int j=0; j<grid_size_[1]; j++) {
			unsigned int& val = (*this)(i,j);
			val = (*this)(i-1,j) + (*this)(i,j-1) + (*this)(i,j) - (*this)(i-1,j-1);
		}
	}
}

// ==========================================================================================================

PixelMap3D::PixelMap3D() : PixelMap(3) {

}

void PixelMap3D::set_origin(double ox, double oy, double oz) {
	origin_[0] = ox;
	origin_[1] = oy;
	origin_[2] = oz;
}

void PixelMap3D::set_leaf_size(double lx, double ly, double lz) {
	leaf_size_[0] = lx;
	leaf_size_[1] = ly;
	leaf_size_[2] = lz;
}

void PixelMap3D::set_grid_size(unsigned int ni, unsigned int nj, unsigned int nk) {
	grid_size_[0] = ni;
	grid_size_[1] = nj;
	grid_size_[2] = nk;
	init();
}

// [gch]: Benoit, please check this part of the code
PixelMap::DATA_TYPE& PixelMap3D::operator()(int i, int j, int k) {
	unsigned int idx = 0;
	if (i<0 || j<0 || k<0) return zero;

	idx = divb_mul_[0]*i  + divb_mul_[1]*j + divb_mul_[2]*k;

	assert(idx < data.size());
	return data.at(idx);
}

void PixelMap3D::compute_integral_image() {
	assert(data.size()>0);

	for(int i=0; i<grid_size_[0]; i++) {
		for(int j=0; j<grid_size_[1]; j++) {
			for(int k=0; k<grid_size_[2]; k++) {
				unsigned int& val = (*this)(i,j,k);
				val = (*this)(i-1,j,k) + (*this)(i,j-1,k) + (*this)(i,j,k-1) + (*this)(i-1, j-1, k-1) + (*this)(i,j,k)
		            - (*this)(i-1,j,k-1) - (*this)(i,j-1,k-1) - (*this)(i-1,j-1,k);

			}
		}
	}

//  // generic code in PixelMap?
//	int index[ndim]; // pixel counter (with ndim digits). Starts from (0,0,0) until (n0-1,n1-1,n2-1) with ni=grid_size[i].
//	for (int d=0; d<ndim-1; d++) {
//		index[d]=0;
//	}

//	do {
//      // compute
	    // ...
//		// increment the counter
//		for (int d=0; d<ndim-1; d++) {
//			// find the next digit to increment
//			if (index[d]<grid_size_[d]) { index[d]++; break; }
//			else index[d]=0; // all the smaller digits are reset to 0.
//		}
//
//
//	} while (index[ndim-1]!=grid_size_[ndim-1]); // quit the loop when (0,0,0) is reached again.


}

} // namespace ibex
