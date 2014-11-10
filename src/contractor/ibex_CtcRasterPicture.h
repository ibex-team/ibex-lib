//============================================================================
//                                  I B E X
// File        : Image contractor using Yan Sliwka's algorithm
// Author      : Benoit DESROCHERS
// Copyright   : ENSTA BRETAGNE (France)
// License     : See the LICENSE file
// Created     : Sep 04, 2014
//============================================================================

#ifndef IBEX_CTCRASTERPICTURE
#define IBEX_CTCRASTERPICTURE

#include <iostream>
#include <ibex_Ctc.h>
#include <ibex_IntervalVector.h>
#include <ibex_RasterPicture.h>

// #include <ibex_IntInterval.h>
namespace ibex {


/**
 * \ingroup contractor
 * \brief Contractor on image using Integral image algorithm
 *
 *  This class implement the image contractor based on Integral Image or
 *  Summed table area. See Yan Sliwka thesis for more details.
 *
 */

class CtcRasterPicture : public Ctc
{
public:
    /**
     * @brief CtcRasterPicture
     * @param data : array containing the integral image of the occupancy grid which describes the map.
     *  1-valued pixels are said to be occuped while 0-valued are not.
     *
     */
    CtcRasterPicture(RasterPicture &data);

    /**
     * @brief Contract a box
     * @param box  box to be contracted
     */
    void contract(IntervalVector &box);


private:

    /**
     * @brief I 2D array used for storing the integral image of the occupancy grid
     */
    RasterPicture I;

    /**
     * Array storing pixel coordinate. this array is allocated ones at the creation of the object.
     * Data are organized as follow : 
     *   [xmin, xmax, ymin, ymax, zmin, zmax ,...]
     */
    int *pixelCoords;

    
    /**
     * @brief worldToGrid converts coordinates in world frame into coordinates in the image frame.
     * @param box IntervalVector of coordinates in the world frame
     * @param pixel_coord coordinates in the image frame
     */
    void worldToGrid(IntervalVector box, int *pixel_coord);

    /**
     * @brief gridToWorld converts coordinates in pixel into world frame coordinates.
     * @param pixel_coord pixel_coord coordinates in the image frame
     * @param box IntervalVector of coordinates in the world frame
     */
    void gridToWorld(int *pixel_coord, IntervalVector &box);

    /**
     * @brief contract the box defined by [cxmin, cxmax]x[cymin, cymax] w.r.t the Integral image
     */
    void contract(int &cxmin, int &cxmax, int &cymin, int &cymax);

    /**
     * @brief contract the box defined by [cxmin, cxmax]x[cymin, cymax]x[zmin, zmax] w.r.t the Integral image
     */
    void contract(int &cxmin, int &cxmax, int &cymin, int &cymax, int &czmin, int &czmax);
    
    /**
     * @brief enclosed_pixels return the number of 1-valued pixel in the box [xmin,xmax] x [ymin, ymax]
     * @return the number of 1-valued pixels.
     */
    unsigned int enclosed_pixels(int xmin, int xmax, int ymin, int ymax);

    /**
     * @brief enclosed_pixels return the number of 1-valued pixel in the box [xmin,xmax] x [ymin, ymax] x [zmin, zmax]
     * @return the number of 1-valued pixels.
     */
    unsigned int enclosed_pixels(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax);

};



} // end namespace ibex
#endif // IBEX_CTCRASTERPICTURE
