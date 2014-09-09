#ifndef IBEX_CTCVOXELGRID_H
#define IBEX_CTCVOXELGRID_H

#include <ibex_Ctc.h>
#include <ibex_IntervalVector.h>
#include <ibex_Interval.h>
//#include <IntegralImg3D.h>
#include <Array3D.h>
#include <iostream>

namespace ibex {

class IntInterval
{
public:
    IntInterval();
    IntInterval(int lb, int ub, bool empty = false);

    int lb() const;
    int ub() const;
    int& lb();
    int& ub();
    bool is_empty() const;
    void setEmpty();
    void inflate(int rad);

    IntInterval operator &=(const IntInterval &a);
    friend std::ostream& operator <<(std::ostream& Stream, const IntInterval& obj);
private:
    int _lb;
    int _ub;
    bool empty;
};

typedef std::array<IntInterval,3> IntBox;
typedef std::array<Interval,3> IntervalBox;


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
