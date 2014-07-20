#include "ibex_CtcPointInSegments.h"
namespace ibex {

CtcPointInSegments::CtcPointInSegments(vector<double> &_ax, vector<double> &_ay, vector<double> &_bx, vector<double> &_by) : Ctc(),
    ax(_ax),
    ay(_ay),
    bx(_bx),
    by(_by),
    _ctc(CtcPointInSegment(0,0,0,0))
{

}

CtcPointInSegments::~CtcPointInSegments()
{
}

void CtcPointInSegments::contract(IntervalVector &box)
{
    IntervalVector savebox(box);
    IntervalVector result(IntervalVector::empty(box.size()));
    for(uint i = 0; i < ax.size(); i++){
        if (i>0) box=savebox;
        try {
            _ctc.contract(box,ax[i],ay[i],bx[i],by[i]);
            result |= box;
        }
        catch(EmptyBoxException& ) {
        }
    }
    box = result;
    if (box.is_empty()) throw EmptyBoxException();


}

} // end IBEX namespace

