#ifndef IBEX_CTCIMAGE_H
#define IBEX_CTCIMAGE_H

#include <iostream>

#include <ibex_Ctc.h>
#include <ibex_IntervalVector.h>
#include <ibex_Interval.h>
#include <ibex_NDArray.h>
#include <ibex_IntInterval.h>
namespace ibex {



#ifndef USE_OPENCV

/**
 * @brief The CtcImage class
 */
class CtcImage : public Ctc
{
public:
    /**
     * @brief CtcImage
     * @param data
     */
    CtcImage(Array2D &data);

    void contract(IntervalVector &box);

    std::pair<IntInterval, IntInterval> worldToGrid_V2(const Interval &x, const Interval &y);
    std::pair<Interval, Interval> gridToWorld_V2(const IntInterval &ix, const IntInterval &iy);

private:

    Array2D I;

    void contract(int &cxmin, int &cxmax, int &cymin, int &cymax);
    int enclosed_pixels(int xmin, int xmax, int ymin, int ymax);
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
