//============================================================================
//                                  I B E X                                   
// File        : ibex_RasterPicture.h
// Author      : Benoit Desrochers, Gilles Chabert
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
 * Data is stored in a linear big array.
 *
 * with the data, the PixelMap also stores :
 *	- the size of the grid ( leaf_size_)
 *	- the origin (origin_)
 * 	- the number of cells (grid_size_)
 *
 */
class RasterPicture {
public:
	/**
	 * \brief type of data stored in the grid
	 */
	typedef unsigned int DATA_TYPE;

	/**
	 * \brief Create a n-dimensional image.
	 */
	RasterPicture(int ndim);

	/**
	 * \brief Copy constructor.
	 */
	RasterPicture(const RasterPicture& src);

	/**
	 * \brief Delete this.
	 */
	virtual ~RasterPicture();

	/**
	 * \brief Save the RasterPicture into a file given by filename. 
	 */
    void save(const char* filename);

	/**
	 * \brief Save the RasterPicture from a file given by filename.
	 */
    void load(const char* filename);

	/**
	 * \brief Compute the integral image.
	 *
	 * The computation is done in place.
	 *
	 */
    virtual void compute_integral_image()=0;

    /** \brief Either 2 or 3 */
    const int ndim;

    /** \brief The size of a leaf. */
    double *leaf_size_;

    /** \brief The coordinate of the bottom left front corner */
    double *origin_;

    /** \brief The size of the grid in each dimension. */
    int *grid_size_;

protected:
    friend class TestRasterPicture;

	/**
	 * \brief TODO: add comment
	 */
    DATA_TYPE& operator[](int idx);

    /** \brief Vector storing data. */
    std::vector<DATA_TYPE> data;

    /** \brief The division multiplier.
     *
     * Offsets described by divb_mul_ are used to select element. */
    int *divb_mul_;

    DATA_TYPE zero;

private:

    static const char* FORMAT_VERSION;
    static const char* FF_DATA_IMAGE_ND;

	/**
	 * \brief After setting parameters of the RasterPicture( grid_size, leaf_size and origin)
	 * This function initialiez the array which stores pixel data.
	 */
    void init(const RasterPicture& pic);

	/*
	 * Read the header from a file.
	 *  (used by load)
	 */
    void read_header(std::ifstream& in_file, RasterPicture& output);

    /*
     * Write the header in a file.
	 *  (used by save)
	 */
    void write_header(std::ofstream& out_file, const RasterPicture& input);
};

/**
 * \brief Raster 2D picture image.
 */
class RasterPicture2D : public RasterPicture {
public:

    RasterPicture2D() : RasterPicture(2) {};

	virtual void compute_integral_image();

    void set_origin(double ox, double oy);

    void set_leaf_size(double nx, double ny);

    void set_grid_size(uint ni, uint nj);

    DATA_TYPE& operator()(int i, int j);

};


/**
 * \brief Raster 3D picture image.
 */
class RasterPicture3D : public RasterPicture {
public:

    RasterPicture3D(): RasterPicture(3) {};

	virtual void compute_integral_image();

	void set_origin(double ox, double oy, double oz);

	void set_leaf_size(double nx, double ny, double nz);

	void set_grid_size(uint ni, uint nj, uint nk);

	DATA_TYPE& operator()(int i, int j, int k);
};


/*================================== inline implementations ========================================*/

inline RasterPicture::DATA_TYPE& RasterPicture::operator[](int idx) {
	return data.at(idx);
}

} // namespace ibex

#endif // __IBEX_RASTER_PICTURE_H__
