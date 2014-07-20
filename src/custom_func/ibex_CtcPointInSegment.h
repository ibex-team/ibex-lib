#ifndef IBEX_CTCPOINTINSEGMENT_H
#define IBEX_CTCPOINTINSEGMENT_H
#include <vector>
#include "ibex_IntervalVector.h"
#include "ibex_EmptyBoxException.h"
#include "ibex_NumConstraint.h"
#include "ibex_CtcFwdBwd.h"


using namespace std;
namespace ibex {
class CtcPointInSegment : public Ctc
{
public:

    CtcPointInSegment(double ax, double ay, double bx, double by);
    void contract(IntervalVector& box);
    void contract(IntervalVector &box, double ax, double ay, double bx, double by);
    ~CtcPointInSegment();

protected:
    NumConstraint *ncf, *ncg;
    CtcFwdBwd *ctc_f, *ctc_g;
};


}
#endif // IBEX_CTCPOINTINSEGMENT_H
