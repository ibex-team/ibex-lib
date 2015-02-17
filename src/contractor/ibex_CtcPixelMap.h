//============================================================================
//                                  I B E X
// File        : Image contractor using Yan Sliwka's algorithm
// Author      : Benoit DESROCHERS
// Copyright   : ENSTA BRETAGNE (France)
// License     : See the LICENSE file
// Created     : Sep 04, 2014
//============================================================================

#ifndef __IBEX_CTC_PIXEL_MAP_H__
#define __IBEX_CTC_PIXEL_MAP_H__

#include <iostream>
#include "ibex_Ctc.h"
#include "ibex_IntervalVector.h"
#include "ibex_PixelMap.h"

namespace ibex {


/**
 * \ingroup contractor
 *
 * \brief Contractor on image using Integral image algorithm
 *
 *  This class implement the image contractor based on Integral Image or
 *  Summed table area. See Yan Sliwka thesis for more details.
 *
 */

class CtcPixelMap : public Ctc {

public:
    /**
     * \brief CtcPixelMap
     *
     * \param data - array containing the integral image of the occupancy grid which describes the map.
     *               1-valued pixels are said to be occupied while 0-valued are not.
     */
    CtcPixelMap(PixelMap& data);

    /**
     * \brief Delete this.
     */
    ~CtcPixelMap();

    /**
     * \brief Contract a box
     *
     * \param box - box to be contracted
     */
    void contract(IntervalVector& box);


private:

    /**
     * \brief 2D array used for storing the integral image of the occupancy grid.
     */
    PixelMap &I;

    /**
     * \brief Array storing pixel coordinates.
     *
     * This array is allocated at the creation of the object.
     * Data are organized as follow : 
     *   [xmin, xmax, ymin, ymax, zmin, zmax ,...]
     */
    int *pixel_coords;

    
    /**
     * \brief Converts coordinates from world frame to the image frame.
     *
     * \param box         - IntervalVector of coordinates in the world frame
     */
    void world_to_grid(IntervalVector box);

    /**
     * \brief Converts coordinates of pixels into world frame coordinates.
     *
     * \param box         - IntervalVector of coordinates in the world frame
     */
    void grid_to_world(IntervalVector& box);

    /**
     * \brief contract the box defined by [cxmin, cxmax]x[cymin, cymax] w.r.t the integral image.
     */
    void contract(int &cxmin, int &cxmax, int &cymin, int &cymax);

    /**
     * \brief contract the box defined by [cxmin, cxmax]x[cymin, cymax]x[zmin, zmax] w.r.t the integral image.
     */
    void contract(int &cxmin, int &cxmax, int &cymin, int &cymax, int &czmin, int &czmax);
    
    /**
     * \brief Return the number of 1-valued pixels in the box [xmin,xmax] x [ymin, ymax].
     *
     * \return the number of 1-valued pixels.
     */
    unsigned int enclosed_pixels(int xmin, int xmax, int ymin, int ymax);

    /**
     * \brief Return the number of 1-valued pixels in the box [xmin,xmax] x [ymin, ymax] x [zmin, zmax].
     *
     * \return the number of 1-valued pixels.
     */
    unsigned int enclosed_pixels(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax);

};

} // end namespace ibex

#endif // __IBEX_CTC_PIXEL_MAP_H__
