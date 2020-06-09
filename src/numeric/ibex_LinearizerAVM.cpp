#include "ibex_LinearizerAVM.h"

namespace ibex {

LinearizerAVM::LinearizerAVM(System& sys) : Linearizer(sys.nb_var), sys_(sys) {
    eval_ = new Eval(sys.f_ctrs);
    avm_ = new AVMLPSolver(*eval_);
    n_intermediate_ = avm_->nb_intermediate_var() - sys.nb_var;
}

LinearizerAVM::~LinearizerAVM() {
    delete avm_;
    delete eval_;
}

int LinearizerAVM::nb_intermediate_var() const {
    return n_intermediate_;
}

int LinearizerAVM::nb_initial_lin() const {
    return avm_->nb_lin();
}

bool LinearizerAVM::must_clear_constraints() const {
    return false;
}

int LinearizerAVM::linearize(const IntervalVector& box, LPSolver& lp_solver) {
    avm_->set_solver(lp_solver);
    avm_->linearize(box);
    avm_->set_top_level_constraints(sys_.ops);
    //lp_solver.write_to_file("intermbefore.lp");
    return 1;
}



} // namespace ibex