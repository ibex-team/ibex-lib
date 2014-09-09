#ifndef IBEX_CTCVOXELGRID_H
#define IBEX_CTCVOXELGRID_H

#include <ibex_Ctc.h>
#include <ibex_IntervalVector.h>
#include <ibex_Interval.h>
#include <Array3D.h>
#include <iostream>

#include "ibex_IntInterval.h"
namespace ibex {

class CtcVoxelGrid : public Ctc
{
public:
    CtcVoxelGrid(const char *ii3D_filename);

    void contract(IntervalVector &box);

    IntBox worldToGrid_V2(const Interval &x, const Interval &y, const Interval &z);
    IntervalBox gridToWorld_V2(const IntInterval &ix, const IntInterval &iy, const IntInterval &iz);

private:
    Array3D<unsigned int> ii3D;
};

} // end namespace ibex
#endif // IBEX_CTCVOXELGRID_H
