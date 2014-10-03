//============================================================================
//                                  I B E X
// File        : Image contractor using Yan Sliwka's algorithm
// Author      : Benoit DESROCHERS
// Copyright   : ENSTA BRETAGNE (France)
// License     : See the LICENSE file
// Created     : Sep 04, 2014
//============================================================================

#ifndef IBEX_CTCIMAGE_H
#define IBEX_CTCIMAGE_H

#include <iostream>
#include <ibex_Ctc.h>
#include <ibex_IntervalVector.h>
#include <ibex_NDArray.h>

#include <ibex_IntInterval.h>
namespace ibex {



#ifndef USE_OPENCV

/**
 * \ingroup contractor
 * \brief Contractor on image using Integral image algorithm
 *
 *  This class implement the image contractor based on Integral Image or
 *  Summed table area. See Yan Sliwka thesis for more details.
 *
 *
 */
class CtcImage : public Ctc
{
public:
    /**
     * @brief CtcImage
     * @param data : array containing the integral image of the occupancy grid which describes the map.
     *  1-valued pixels are said to be occuped while 0-valued are not.
     *
     */
    CtcImage(Array2D &data);

    /**
     * @brief Contract a box
     * @param box  box to be contracted
     */
    void contract(IntervalVector &box);

    /**
     * @brief worldToGrid_V2 convert coordinates in world frame into coordinates in the image frame.
     * @param x x-axis coordinates in the world frame
     * @param y y-axis coordinates in the world frame
     * @return a pair containing pixels coordinates.
     */
    std::pair<IntInterval, IntInterval> worldToGrid_V2(const Interval &x, const Interval &y);

    /**
     * @brief gridToWorld_V2 convert coordinates in pixel into world frame coordinates.
     * @param ix x-axis coordinates in pixel
     * @param iy y-axis coordinates in pixel
     * @return pair of world frame coordinates
     */
    std::pair<Interval, Interval> gridToWorld_V2(const IntInterval &ix, const IntInterval &iy);

private:


    /**
     * @brief I 2D array used for storing the integral image of the occupancy grid
     */
    Array2D I;

    /**
     * @brief contract the box defined by [cxmin, cxmax]x[cymin, cymax] w.r.t the Integral image
     */
    void contract(int &cxmin, int &cxmax, int &cymin, int &cymax);

    /**
     * @brief enclosed_pixels return the number of 1-valued pixel in the box [xmin,xmax] x [ymin, ymax]
     * @return the number of 1-valued pixels.
     */
    unsigned int enclosed_pixels(int xmin, int xmax, int ymin, int ymax);
};

#else


class CtcImage : public Ctc
{
public:
    CtcImage(Mat &image_psi, float x0, float y0, float dx, float dy);
    CtcImage(const char *BW_image, float x0, float y0, float dx, float dy);
    Mat psi;
    void contract(IntervalVector &box);

    std::pair<IntInterval, IntInterval> worldToGrid_V2(const Interval &x, const Interval &y);
    std::pair<Interval, Interval> gridToWorld_V2(const IntInterval &ix, const IntInterval &iy);

private:
    float custom_x_origin;
    float custom_y_origin;
    float scale_x;
    float scale_y;


    void psi_image_contract(IntInterval &x, IntInterval &y);
    int enclosed_pixels(int xmin, int xmax, int ymin, int ymax);
};


#endif
} // end namespace ibex
#endif // IBEX_CTCIMAGE_H
