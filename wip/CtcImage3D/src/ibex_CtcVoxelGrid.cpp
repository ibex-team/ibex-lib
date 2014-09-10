#include "ibex_CtcVoxelGrid.h"

namespace ibex{
CtcVoxelGrid::CtcVoxelGrid(): Ctc(3)
{

}

CtcVoxelGrid::CtcVoxelGrid(Array3D<unsigned int> &data): I(data),Ctc(3)
{

}

CtcVoxelGrid::CtcVoxelGrid(const char* ii3D_filename): Ctc(3)
{
    if(loadArray3D(ii3D_filename, I) == -1){
        std::cerr << "Error loading array3D\n";
    }
}


//-------------------------------------------------------------------------------------------------------------
IntBox CtcVoxelGrid::worldToGrid_V2(const Interval &x, const Interval &y, const Interval &z)
{ 
    Interval xt = (x - I.origin_[0]) / I.leaf_size_[0];
    Interval yt = (y - I.origin_[1]) / I.leaf_size_[1];
    Interval zt = (z - I.origin_[2]) / I.leaf_size_[2];

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

    i1 = Interval(ix.lb(), ix.ub()+1) * I.leaf_size_[0] + I.origin_[0];
    i2 = Interval(iy.lb(), iy.ub()+1) * I.leaf_size_[1] + I.origin_[1];
    i3 = Interval(iz.lb(), iz.ub()+1) * I.leaf_size_[2] + I.origin_[2];


    return {{i1,i2,i3}};
}

//----------------------------------------------------------------------------------------------------------------
void CtcVoxelGrid::contract(IntervalVector &box){

    assert(box.size() == 3);
    IntBox x;
    x = worldToGrid_V2(box[0],box[1],box[2]);

    contract(x[0].lb(),x[0].ub(),x[1].lb(),x[1].ub(),x[2].lb(),x[2].ub());
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

void CtcVoxelGrid::contract(int &cxmin, int& cxmax, int& cymin,int& cymax, int& czmin, int& czmax){

    cxmax = std::max(0,std::min(I.grid_size_[0]-1,cxmax));
    cxmin = std::min(I.grid_size_[0]-1,std::max(0,cxmin));
    cymax = std::max(0,std::min(I.grid_size_[1]-1,cymax));
    cymin = std::min(I.grid_size_[1]-1,std::max(0,cymin));
    czmax = std::max(0, std::min(I.grid_size_[2]-1,czmax));
    czmin = std::min(I.grid_size_[2]-1, std::max(0,czmin));
    if ( enclosed_pixels(cxmin,cxmax,cymin,cymax,czmin,czmax) == 0){
        cxmin = -1;
        return;
    }

    //right
    for (int i = cxmin; i <= cxmax; i++)
    {
        if(enclosed_pixels(cxmin,i,cymin,cymax,czmin,czmax)>0) {
            cxmin = i;
            break;
        }
    }
    //left
    for (int i = cxmax; i >= cxmin ; i--)
    {
        if(enclosed_pixels(i,cxmax,cymin,cymax,czmin,czmax)>0) {
            cxmax = i;
            break;
        }
    }
    //down
    for (int j = cymin; j <=cymax; j++)
    {
        if(enclosed_pixels(cxmin,cxmax,cymin,j,czmin,czmax)>0) {
            cymin = j;
            break;
        }
    }
    //up
    for (int j = cymax; j >= cymin ; j--)
    {
        int res = enclosed_pixels(cxmin,cxmax,j,cymax,czmin,czmax);
        if(res>0) {
            cymax = j;
            break;
        }
    }
    //bottom
    for (int k = czmin; k <=czmax; k++)
    {
        if(enclosed_pixels(cxmin,cxmax,cymin,cymax,czmin,k)>0) {
            czmin = k;
            break;
        }
    }
    //top
    for (int k = czmax; k >= czmin ; k--)
    {
        int res = enclosed_pixels(cxmin,cxmax,cymin,cymax,k,czmax);
        if(res>0) {
            czmax = k;
            break;
        }
    }


}


unsigned int CtcVoxelGrid::enclosed_pixels(int xmin, int xmax, int ymin,int ymax, int zmin, int zmax)
{
    unsigned int L8 = I(xmax, ymax,zmax);
    unsigned int L5 = I(xmin-1, ymin-1, zmax);
    unsigned int L6 = I(xmin-1, ymax,zmax) ;
    unsigned int L7 = I(xmax, ymin-1,zmax);
    unsigned int L4 = I(xmax, ymax,zmin-1);
    unsigned int L1 = I(xmin-1, ymin-1, zmin-1);
    unsigned int L2 = I(xmin-1, ymax,zmin-1);
    unsigned int L3 = I(xmax, ymin-1,zmin-1);
    unsigned int L = (L8 + L5 - L6  - L7) -(L4 + L1 - L2 - L3);
    return L;
}


} // end namespace ibex
