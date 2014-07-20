#ifndef IBEX_CTCPOINTINCIRCLES_H
#define IBEX_CTCPOINTINCIRCLES_H
#include <vector>
#include "ibex_CtcPointInCircle.h"



using namespace std;
namespace ibex {

class CtcPointInCircles : public Ctc
{
public:
    CtcPointInCircles(vector<double> &_cx, vector<double> &_cy, vector<double> &_r);
    void contract(IntervalVector& box);
    ~CtcPointInCircles();

protected:
    vector<double> &cx, &cy, &r;
    CtcPointInCircle _ctc;
};

}
#endif // IBEX_CTCPOINTINCIRCLES_H
