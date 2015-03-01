//============================================================================
//                                  I B E X                                   
// File        : ibex_PixelMap.h
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2014
//============================================================================

#ifndef __IBEX_PIXEL_MAP_H__
#define __IBEX_PIXEL_MAP_H__

#include <fstream>
#include <vector>

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
class PixelMap {
public:

    /**
	 * \brief Create a n-dimensional image.
	 */
	PixelMap(unsigned int ndim);

	/**
	 * \brief Copy constructor.
	 */
	PixelMap(const PixelMap& src);

	/**
	 * \brief Delete this.
	 */
	virtual ~PixelMap();

	/**
	 * \brief type of data stored in the grid
	 */
	typedef unsigned int DATA_TYPE;

	/**
	 * \brief Save the PixelMap into a file given by filename. 
	 */
    void save(const char* filename);

	/**
	 * \brief Save the PixelMap from a file given by filename.
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
    const unsigned int ndim;

    /** \brief The size of a leaf. */
    double *leaf_size_;

    /** \brief The coordinate of the bottom left front corner */
    double *origin_;

    /** \brief The size of the grid in each dimension. */
    int *grid_size_;

protected:

	friend class TestPixelMap;

	/**
	 * \brief return the value of the element idx in the array data
	 */
    DATA_TYPE& operator[](int idx);

    /** \brief Vector storing data. */
    std::vector<DATA_TYPE> data;

    /** \brief The division multiplier.
     *
     * Offsets described by divb_mul_ are used to select element. */
    int *divb_mul_;

    /**
     *	\brief internal zero value returned when a pixel is acceded with negatif indice.
     *	see operator() for more details.
     */
    DATA_TYPE zero;

    /**
	 *	\brief Initialize the PixelMap. 
	 *		field <leaf_size_> needs to be setted before.
	 */
	void init();
	
	/**
	 * \brief After setting parameters of the PixelMap( grid_size, leaf_size and origin)
	 * This function initialiez the array which stores pixel data.
	 */
    void init(const PixelMap& pic);

private:

    static const char* FORMAT_VERSION;
    static const char* FF_DATA_IMAGE_ND;

	/*
	 * Read the header from a file.
	 *  (used by load)
	 */
    void read_header(std::ifstream& in_file, PixelMap& output);

    /*
     * Write the header in a file.
	 *  (used by save)
	 */
    void write_header(std::ofstream& out_file, const PixelMap& input);
};

/**
 * \brief Raster 2D picture image.
 */
class PixelMap2D : public PixelMap {
public:

    PixelMap2D();

	virtual void compute_integral_image();

    void set_origin(double ox, double oy);

    void set_leaf_size(double nx, double ny);

    void set_grid_size(unsigned int ni, unsigned int nj);

    DATA_TYPE& operator()(int i, int j);

};


/**
 * \brief Raster 3D picture image.
 */
class PixelMap3D : public PixelMap {
public:

    PixelMap3D();

	virtual void compute_integral_image();

	void set_origin(double ox, double oy, double oz);

	void set_leaf_size(double nx, double ny, double nz);

	void set_grid_size(unsigned int ni, unsigned int nj, unsigned int nk);

	DATA_TYPE& operator()(int i, int j, int k);
};


/*================================== inline implementations ========================================*/

inline PixelMap::DATA_TYPE& PixelMap::operator[](int idx) {
	return data.at(idx);
}

} // namespace ibex

#endif // __IBEX_PIXEL_MAP_H__
