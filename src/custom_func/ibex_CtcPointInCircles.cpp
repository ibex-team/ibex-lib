#include "ibex_CtcPointInCircles.h"
namespace ibex {

CtcPointInCircles::CtcPointInCircles(vector<double> &_cx, vector<double> &_cy, vector<double> &_r) : Ctc(),
    cx(_cx),
    cy(_cy),
    r(_r),
    _ctc(CtcPointInCircle(0,0,0))
{

}

CtcPointInCircles::~CtcPointInCircles()
{
}

void CtcPointInCircles::contract(IntervalVector &box)
{
    IntervalVector savebox(box);
    IntervalVector result(IntervalVector::empty(box.size()));
    for(uint i = 0; i < cx.size(); i++){
        if (i>0) box=savebox;
        try {
            _ctc.contract(box,cx[i],cy[i],r[i]);
            result |= box;
        }
        catch(EmptyBoxException& ) {
        }
    }
    box = result;
    if (box.is_empty()) throw EmptyBoxException();


}

} // end IBEX namespace

