#ifndef IBEX_CTCPOINTINSEGMENTS_H
#define IBEX_CTCPOINTINSEGMENTS_H
#include <vector>
#include "ibex_CtcPointInSegment.h"



using namespace std;
namespace ibex {

class CtcPointInSegments : public Ctc
{
public:
    CtcPointInSegments(vector<double> &_ax, vector<double>& _ay, vector<double>& _bx, vector<double>& _by);
    void contract(IntervalVector& box);
    ~CtcPointInSegments();

protected:
    vector<double> &ax, &ay, &bx, &by;
    CtcPointInSegment _ctc;
};

}
#endif // IBEX_CTCPOINTINSEGMENTS_H
