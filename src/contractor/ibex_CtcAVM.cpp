#include "ibex_CtcAVM.h"

namespace ibex {

CtcAVM::CtcAVM(const Function& f): Ctc(f.nb_var()) {
    avm_ = new AVM(f.basic_evaluator());
}

CtcAVM::~CtcAVM() {
    delete avm_;
}

void CtcAVM::contract(IntervalVector& box) {
    ContractContext ctx(box);
    contract(box, ctx);
}

void CtcAVM::contract(IntervalVector& box, ContractContext& context) {
    if(box.is_unbounded()||box.is_empty()) return;
    Vector cost(nb_var, 0.0);
    for(int i = 0; i < nb_var; ++i) {
        cost[i] = 1;
        avm_->set_cost(cost);
        LPSolver::Status status = avm_->minimize(box);
        std::cout << status << std::endl;
        if(status == LPSolver::Status::OptimalProved) {
            box[i] = Interval(avm_->minimum().lb(), box[i].ub());
        } else if(status == LPSolver::Status::InfeasibleProved) {
            box.set_empty();
            return;
        }
        cost[i] = -1;
        avm_->set_cost(cost);
        status = avm_->minimize(box);
        std::cout << status << std::endl;
        if(status == LPSolver::Status::OptimalProved) {
            box[i] = Interval(box[i].lb(), -avm_->minimum().lb());
        } else if(status == LPSolver::Status::InfeasibleProved) {
            box.set_empty();
            return;
        }
        cost[i] = 0;
    }
}



}