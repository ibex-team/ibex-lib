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

class CtcVoxelGrid : public Ctc
{
public:
    CtcVoxelGrid();
    CtcVoxelGrid(Array3D & data);
    CtcVoxelGrid(const char *ii3D_filename);

    void contract(IntervalVector &box);
    IntBox worldToGrid_V2(const Interval &x, const Interval &y, const Interval &z);
    IntervalBox gridToWorld_V2(const IntInterval &ix, const IntInterval &iy, const IntInterval &iz);


private:
    Array3D I;
    void contract(int &cxmin, int &cxmax, int &cymin, int &cymax, int &czmin, int &czmax);
    unsigned int enclosed_pixels(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax);
};

} // end namespace ibex
#endif // IBEX_CTCVOXELGRID_H
