//============================================================================
//                                  I B E X                                   
// File        : ibex_RasterPicture.h
// Author      : ???, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2014
//============================================================================

#ifndef __IBEX_RASTER_PICTURE_H__
#define __IBEX_RASTER_PICTURE_H__

#include <fstream>
#include <vector>
#include <sys/types.h>

namespace ibex {

/**
 * \brief Raster 2D/3D picture image.
 *
 * It stores occupancy grid and integral image.
 * data are stored in a linear big array and offsets described by divb_mul_
 * are used to select element.
 *
 */
class RasterPicture {
public:
	typedef unsigned int DATA_TYPE;

	RasterPicture(int ndim);

	virtual ~RasterPicture();

    /** \brief Either 2 or 3 */
    const int ndim;

    /** \brief Vector storing data. */
    std::vector<DATA_TYPE> data;

    /** \brief The size of a leaf. */
    double *leaf_size_;
    /** \brief The coordinate of the bottom left front conner */
    double *origin_;
    /** \brief The size of the grid in each dimension. */
    int *grid_size_;
    /** \brief The division multiplier. */
    int *divb_mul_;

    DATA_TYPE zero;

    void init();

    void init(const RasterPicture& pic);

    DATA_TYPE& operator[](int idx);

    int save(const char* filename);

    // read header
    int readHeader(std::ifstream& in_file, RasterPicture& output);

    int load(const char* filename);

    int writeHeader(std::ofstream& out_file, const RasterPicture& input);

    // Compute the integral image of the array passed as argument.
    // The computation is done in place.
    virtual void computeIntegralImage()=0;
};

inline RasterPicture::DATA_TYPE& RasterPicture::operator[](int idx) {
	return data.at(idx);
}

/**
 * \brief Raster 2D picture image.
 */
class RasterPicture2D : public RasterPicture {
public:

    RasterPicture2D() : RasterPicture(2) {};

	virtual void computeIntegralImage();

    void setOrigin(double ox, double oy);

    void setLeafSize(double nx, double ny);

    void setGridSize(uint ni, uint nj);

    DATA_TYPE& operator()(uint i, uint j);

};


/**
 * \brief Raster 3D picture image.
 */
class RasterPicture3D : public RasterPicture {
public:

    RasterPicture3D(): RasterPicture(3) {};

	virtual void computeIntegralImage();

	void setOrigin(double ox, double oy, double oz);

	void setLeafSize(double nx, double ny, double nz);

	void setGridSize(uint ni, uint nj, uint nk);

	DATA_TYPE& operator()(uint i, uint j, uint k);
};


} // namespace ibex

#endif // __IBEX_RASTER_PICTURE_H__
