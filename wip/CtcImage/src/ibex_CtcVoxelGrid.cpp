#include "ibex_CtcVoxelGrid.h"

namespace ibex{
CtcVoxelGrid::CtcVoxelGrid(const char* ii3D_filename): Ctc(3), ii3D(ii3D_filename)
{

}

//-------------------------------------------------------------------------------------------------------------
IntBox CtcVoxelGrid::worldToGrid_V2(const Interval &x, const Interval &y, const Interval &z)
{ 
    const float3 &origin = ii3D.getOrigin();
    const float3 &scale  = ii3D.getLeafSize();
    Interval xt = (x - origin[0]) / scale[0];
    Interval yt = (y - origin[1]) / scale[1];
    Interval zt = (z - origin[2]) / scale[2];

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

    const float3 &origin = ii3D.getOrigin();
    const float3 &scale  = ii3D.getLeafSize();

    i1 = Interval(ix.lb(), ix.ub()+1) * scale[0] + origin[0];
    i2 = Interval(iy.lb(), iy.ub()+1) * scale[1] + origin[1];
    i3 = Interval(iz.lb(), iz.ub()+1) * scale[2] + origin[2];


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

//-------------------------------------------------------------------------------------------------------------
IntInterval::IntInterval():_lb(std::numeric_limits<int>::min()),
    _ub(std::numeric_limits<int>::min()), empty(true)
{
}
//-------------------------------------------------------------------------------------------------------------
IntInterval::IntInterval(int lb, int ub, bool empty):_lb(lb), _ub(ub), empty(empty)
{
    if(_ub < _lb){
        std::swap(_ub, _lb);
    }
}
//-------------------------------------------------------------------------------------------------------------
void IntInterval::inflate(int rad){
    _ub+= rad;
    _lb-= rad;
}
//-------------------------------------------------------------------------------------------------------------
IntInterval IntInterval::operator&=(const IntInterval &a)
{
    if(this->is_empty() || a.is_empty())
        this->setEmpty();
    else {
        this->_lb = std::max(_lb,a.lb());
        this->_ub = std::min(_ub,a.ub());
    }
}
//-------------------------------------------------------------------------------------------------------------
bool IntInterval::is_empty() const{
    return empty;
}

//-------------------------------------------------------------------------------------------------------------
void IntInterval::setEmpty(){
    empty = true;
    _lb = _ub =std::numeric_limits<int>::min();
}

//-------------------------------------------------------------------------------------------------------------
int IntInterval::lb() const {
    return _lb;
}
//-------------------------------------------------------------------------------------------------------------
int& IntInterval::lb() {
    return _lb;
}

//-------------------------------------------------------------------------------------------------------------
int IntInterval::ub() const{
    return _ub;
}

//-------------------------------------------------------------------------------------------------------------
int& IntInterval::ub() {
    return _ub;
}
//-------------------------------------------------------------------------------------------------------------
std::ostream& operator <<(std::ostream& Stream, const IntInterval& obj)
{
    Stream << "[ " << obj._lb << " , " << obj._ub << "]";
    return Stream; // N'oubliez pas de renvoyer le flux, afin de pouvoir chaîner les appels
}

} // end namespace ibex
