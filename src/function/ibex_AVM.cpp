#include "ibex_AVM.h"

namespace ibex {

AVM::AVM(Eval& eval)
: eval_(eval), f_(eval.f), d_(eval.d), avm_d_(ExprDataAVM(f_)) {
    //new_variables_count_ = f_.nb_nodes();
    solver_ = new LPSolver(avm_d_.nb_var, LPSolver::Mode::Certified);
    for(int i = 0; i < avm_d_.nb_lin; ++i) {
        solver_->add_constraint(SparseVector(), CmpOp::LEQ, 0.0);
    }
    for(int i = avm_d_.top->begin_index; i < avm_d_.top->end_index; ++i) {
        SparseVector vec;
        // TODO temporary to test for f(x) <= 0
        vec[i] = 1.0;
        solver_->add_constraint(vec, CmpOp::LEQ, 0.0);
    }
}

AVM::~AVM() {

}

LPSolver::Status AVM::minimize(const IntervalVector& box) {
    eval_.eval(box);
    f_.forward<decltype(*this)>(*this);
    solver_->write_to_file("lp.lp");
    return solver_->minimize();
}

Interval AVM::minimum() const {
    return solver_->minimum();
}

void AVM::set_cost(const Vector& cost) {
    int var = 0;
    for(int arg = 0; arg < avm_d_.args.size(); ++arg) {
        const AVMData& data = avm_d_.args[arg];
        for(int i = data.begin_index; i < data.end_index; ++i) {
            solver_->set_cost(i, cost[var]);
            var++;
        }
    }
}

Vector AVM::filter_variables(const Vector& solver_result) const {
    Vector result(f_.nb_var());
    int var = 0;
    for(int arg = 0; arg < avm_d_.args.size(); ++arg) {
        const AVMData& data = avm_d_.args[arg];
        for(int i = data.begin_index; i < data.end_index; ++i) {
            result[var] = solver_result[i];
            ++var;
        }
    }
    return result;
}

void AVM::load_node_bounds_in_solver(int y) {
    const AVMData& data = avm_d_[y];
    for(int i = data.begin_index, j = 0; i < data.end_index; ++i, ++j) {
        solver_->set_bounds(i, d_[y][j].i());
    }
}

int AVM::node_index_to_first_var(int index) const {
    return avm_d_[index].begin_index;
}

int AVM::node_index_to_first_lin(int index) const {
    return avm_d_[index].begin_lin_index + avm_d_.nb_var;
}

} // namespace ibex