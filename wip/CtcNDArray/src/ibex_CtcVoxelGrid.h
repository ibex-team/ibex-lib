//============================================================================
//                                  I B E X
// File        : Extension of the image contractor (see Yan Sliwka's algorithm) on voxel grid.
// Author      : Benoit DESROCHERS
// Copyright   : ENSTA BRETAGNE (France)
// License     : See the LICENSE file
// Created     : Sep 04, 2014
//============================================================================

#ifndef IBEX_CTCVOXELGRID_H
#define IBEX_CTCVOXELGRID_H

#include <ibex_Ctc.h>
#include <ibex_IntervalVector.h>
#include <ibex_Interval.h>
#include <ibex_NDArray.h>
#include <iostream>

#include "ibex_IntInterval.h"
namespace ibex {


typedef std::array<Interval,3> IntervalBox;
/**
 * \ingroup contractor
 * \brief Contractor on voxel grid using the summed table area algorithm.
 *
 *  This class implement the voxel grid contractor which is the 3D
 *  extension of the image contractor (see Yan Sliwka thesis).
 *
 */
class CtcVoxelGrid : public Ctc
{
public:

    /**
     * @brief CtcVoxelGrid contractor on a voxel grid described by data
     * @param data
     *  The 3D occupacy grid is described by a voxel grid.
     *          data(x,y,z) = 1 if the voxel is occuped
     *          data(x,y,z) = 0 otherwise.
     *
     * data must describ the 3D integral image of if voxel grid
     */
    CtcVoxelGrid(Array3D & data);

    /**
     * @brief Contract a box
     * @param box  box to be contracted
     */
    void contract(IntervalVector &box);

    /**
     * @brief worldToGrid_V2 convert coordinates in world frame into coordinates in the image frame.
     */
    IntBox worldToGrid_V2(const Interval &x, const Interval &y, const Interval &z);

    /**
     * @brief gridToWorld_V2 convert coordinates in pixel into world frame coordinates.
     */
    IntervalBox gridToWorld_V2(const IntInterval &ix, const IntInterval &iy, const IntInterval &iz);


private:

    /**
     * @brief I 3D array used for storing the integral image of the occupancy grid
     */
    Array3D I;

    /**
     * @brief contract the box defined by [cxmin, cxmax]x[cymin, cymax]x[zmin, zmax] w.r.t the Integral image
     */
    void contract(int &cxmin, int &cxmax, int &cymin, int &cymax, int &czmin, int &czmax);


    /**
     * @brief enclosed_pixels return the number of 1-valued pixel in the box [xmin,xmax] x [ymin, ymax] x [zmin, zmax]
     * @return the number of 1-valued pixels.
     */
    unsigned int enclosed_pixels(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax);
};

} // end namespace ibex
#endif // IBEX_CTCVOXELGRID_H
