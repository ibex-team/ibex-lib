#include "ibex_CtcImage.h"

namespace ibex{

#ifndef USE_OPENCV
CtcImage::CtcImage(Array2D &data): Ctc(2), I(data)
{

}


//-------------------------------------------------------------------------------------------------------------
std::pair<IntInterval, IntInterval> CtcImage::worldToGrid_V2(const Interval &x, const Interval &y)
{
    Interval xt = (x - I.origin_[0]) / I.leaf_size_[0];
    Interval yt = (y - I.origin_[1]) / I.leaf_size_[1];

    IntInterval i1( floor(xt.lb()), ceil(xt.ub()-1));
    IntInterval i2( floor(yt.lb()), ceil(yt.ub()-1));

    return {i1,i2};
}

//-------------------------------------------------------------------------------------------------------------
std::pair<Interval,Interval> CtcImage::gridToWorld_V2(const IntInterval &ix,const  IntInterval& iy)
{
    if(ix.is_empty() || iy.is_empty())
        return {Interval::EMPTY_SET,Interval::EMPTY_SET};

    Interval i1, i2;

    i1 = Interval(ix.lb(), ix.ub()+1) * I.leaf_size_[0] + I.origin_[0];
    i2 = Interval(iy.lb(), iy.ub()+1) * I.leaf_size_[1] + I.origin_[1];

    return {i1,i2};
}

//----------------------------------------------------------------------------------------------------------------
void CtcImage::contract(IntervalVector &box){

    assert(box.size() == 2);
    if(box.is_empty()) return;

    // Convert world coordinates into pixel coordinates
    IntInterval x, y;
    std::tie(x,y) = worldToGrid_V2(box[0],box[1]);

    // Contractor the box
    contract(x.lb(),x.ub(),y.lb(),y.ub());
    if(x.lb() == -1) {
        box.set_empty();
        return;
    }

    // Convert pixel coordinates into world coordinates
    Interval x1, x2;
    std::tie(x1,x2) = gridToWorld_V2(x,y);
    box[0] &= x1;
    box[1] &= x2;

    if(box[0].is_empty() || box[1].is_empty()){
        box.set_empty();
    }
}

//------------------------------------------------------------------------------
//psi contraction
void CtcImage::contract(int &cxmin, int &cxmax, int &cymin, int &cymax){

    //compute enclosed pixels on consecutive lines from all dimensions of the box

    cxmax = std::max(0,std::min(I.grid_size_[0]-1,cxmax));
    cxmin = std::min(I.grid_size_[0]-1,std::max(0,cxmin));
    cymax = std::max(0,std::min(I.grid_size_[1]-1,cymax));
    cymin = std::min(I.grid_size_[1]-1,std::max(0,cymin));


    if ( enclosed_pixels(cxmin,cxmax,cymin,cymax) == 0){
        cxmin = -1;
        return;
    }

    //right
    for (int i = cxmin; i <= cxmax; i++)
    {
        if(enclosed_pixels(cxmin,i,cymin,cymax)>0) {
            cxmin = i;
            break;
        }
    }
    //left
    for (int i = cxmax; i >= cxmin ; i--)
    {
        if(enclosed_pixels(i,cxmax,cymin,cymax)>0) {
            cxmax = i;
            break;
        }
    }
    //down
    for (int j = cymin; j <=cymax; j++)
    {
        if(enclosed_pixels(cxmin,cxmax,cymin,j)>0) {
            cymin = j;
            break;
        }
    }
    //up
    for (int j = cymax; j >= cymin ; j--)
    {
        if(enclosed_pixels(cxmin,cxmax,j,cymax)>0) {
            cymax = j;
            break;
        }
    }
}
//////////////////////////////////////////////////////////////////////
int CtcImage::enclosed_pixels(int xmin,int xmax,int ymin,int ymax){

    int b1 = I({{ymax,xmax}});
    int b2 = I({{ymax,xmin-1}});
    int b3 = I({{ymin-1,xmax}});
    int b4 = I({{ymin-1,xmin-1}});
    return b1 - b2 - b3 + b4;
}

#else


#endif


} // end namespace ibex
