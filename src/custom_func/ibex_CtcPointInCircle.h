#ifndef IBEX_CTCPOINTINCIRCLE_H
#define IBEX_CTCPOINTINCIRCLE_H
#include <vector>
#include "ibex_IntervalVector.h"
#include "ibex_EmptyBoxException.h"
#include "ibex_NumConstraint.h"
#include "ibex_CtcFwdBwd.h"


using namespace std;
namespace ibex {
class CtcPointInCircle : public Ctc
{
public:

    CtcPointInCircle(double cx, double cy, double r);
    void contract(IntervalVector& box);
    void contract(IntervalVector &box, double cx, double cy, double r);
    ~CtcPointInCircle();

protected:
    NumConstraint *ncf, *ncg;
    CtcFwdBwd *ctc_f, *ctc_g;
    double _cx,_cy,_r;
};


}
#endif // IBEX_CTCPOINTINCIRCLE_H
