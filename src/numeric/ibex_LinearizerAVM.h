#ifndef __IBEX_LINEARIZER_AVM_H__
#define __IBEX_LINEARIZER_AVM_H__

#include "ibex_System.h"
#include "ibex_AVM.h"
#include "ibex_Eval.h"
#include "ibex_Linearizer.h"
#include "ibex_AVMLPSolver.h"

namespace ibex {

class LinearizerAVM : public Linearizer {
public:
    explicit LinearizerAVM(System& sys);
    virtual ~LinearizerAVM();

    int linearize(const IntervalVector& box, LPSolver& lp_solver) override;

    int nb_intermediate_var() const override;
    int nb_initial_lin() const override;
    bool must_clear_constraints() const override;
private:
    Eval* eval_;
    AVMLPSolver* avm_;
    System& sys_;
    int n_intermediate_;
};

} // namespace ibex

#endif  // __IBEX_LINEARIZER_AVM_H__