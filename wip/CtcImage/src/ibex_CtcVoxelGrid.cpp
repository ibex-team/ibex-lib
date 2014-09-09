#include "ibex_CtcVoxelGrid.h"

namespace ibex{
CtcVoxelGrid::CtcVoxelGrid(const char* ii3D_filename): Ctc(3)
{

}

//-------------------------------------------------------------------------------------------------------------
IntBox CtcVoxelGrid::worldToGrid_V2(const Interval &x, const Interval &y, const Interval &z)
{ 
    Interval xt = (x - ii3D.origin_[0]) / ii3D.leaf_size_[0];
    Interval yt = (y - ii3D.origin_[1]) / ii3D.leaf_size_[1];
    Interval zt = (z - ii3D.origin_[2]) / ii3D.leaf_size_[2];

    IntInterval i1( floor(xt.lb()), ceil(xt.ub()-1));
    IntInterval i2( floor(yt.lb()), ceil(yt.ub()-1));
    IntInterval i3( floor(zt.lb()), ceil(zt.ub()-1));

    return {{i1,i2,i3}};
}

//-------------------------------------------------------------------------------------------------------------
IntervalBox CtcVoxelGrid::gridToWorld_V2(const IntInterval &ix,const  IntInterval& iy, const  IntInterval& iz)
{
    if(ix.is_empty() || iy.is_empty() || iz.is_empty())
        return {{Interval::EMPTY_SET,Interval::EMPTY_SET,Interval::EMPTY_SET}};
    Interval i1, i2, i3;

    i1 = Interval(ix.lb(), ix.ub()+1) * ii3D.leaf_size_[0] + ii3D.origin_[0];
    i2 = Interval(iy.lb(), iy.ub()+1) * ii3D.leaf_size_[1] + ii3D.origin_[1];
    i3 = Interval(iz.lb(), iz.ub()+1) * ii3D.leaf_size_[2] + ii3D.origin_[2];


    return {{i1,i2,i3}};
}

//----------------------------------------------------------------------------------------------------------------
void CtcVoxelGrid::contract(IntervalVector &box){

    assert(box.size() == 3);
    IntBox x;
    x = worldToGrid_V2(box[0],box[1],box[2]);

    ii3D.contract(x[0].lb(),x[0].ub(),x[1].lb(),x[1].ub(),x[2].lb(),x[2].ub());
    if(x[0].lb() == -1) {
        box.set_empty();
        return;
    }

//    Pimage = Inflate(Pimage,1);//debug


    IntervalBox tmp;
    tmp = gridToWorld_V2(x[0],x[1],x[2]);
    box[0] &= tmp[0];
    box[1] &= tmp[1];
    box[2] &= tmp[2];


    if(box[0].is_empty() || box[1].is_empty() || box[2].is_empty()){
        box.set_empty();
    }
}


} // end namespace ibex
