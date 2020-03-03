#include "ibex_AVM.h"
#include "ibex_Newton.h"
#include <queue>
#include "ibex_System.h"
#include "ibex_SystemFactory.h"
#include "ibex_DefaultSolver.h"

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

void AVM::setup_node(int x, int y) {
    xvar_ = node_index_to_first_var(x);
    yvar_ = node_index_to_first_var(y);
    lin_ = node_index_to_first_lin(y);
    load_node_bounds_in_solver(y);
}

void AVM::setup_node(int x1, int x2, int y) {
    x1var_ = node_index_to_first_var(x1);
    x2var_ = node_index_to_first_var(x2);
    yvar_ = node_index_to_first_var(y);
    lin_ = node_index_to_first_lin(y);
    load_node_bounds_in_solver(y);
}

void AVM::finish_node(int x, int y) {
    while(lin_ != avm_d_[y].end_lin_index) {
        load_constraint(lin_++, {{yvar_, 0}, {xvar_, 0}}, 0.0);
    }
}

void AVM::finish_node(int x1, int x2, int y) {
    while(lin_ != avm_d_[y].end_lin_index) {
        load_constraint(lin_++, {{yvar_, 0}, {x1var_, 0}, {x2var_, 0}}, 0.0);
    }
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

double AVM::node_d_ub(int y) const {
    return d_[y].i().ub();
}

double AVM::node_d_lb(int y) const {
    return d_[y].i().lb();
}

void AVM::load_constraint(int lin, const std::vector<coef_pair>& list, double rhs) {
    // std::cout << "load " << lin << " = ";
    for(const coef_pair& z : list) {
        // std::cout << z.coef << " x" << z.var << " + ";
        solver_->set_coef(lin, z.var, z.coef);
    }
    // std::cout << " <= " << rhs << std::endl;
    solver_->set_rhs(lin, rhs);
}

void AVM::load_constraint(int lin, const std::vector<coef_pair>& list, double lhs, double rhs) {
    for(const coef_pair& z : list) {
        solver_->set_coef(lin, z.var, z.coef);
    }
    solver_->set_lhs_rhs(lin, lhs, rhs);
}
Interval AVM::find_cos_secante(double x_start, double search_lb, double search_ub) {
    SystemFactory fac;
    Variable x;
    fac.add_var(x);
    const ExprConstant& cst = ExprConstant::new_scalar(x_start);
    fac.add_ctr(cos_secante_point_(x, cst) = 0);
    System sys(fac);
    DefaultSolver s(sys);
    s.solve(Interval(search_lb, search_ub));
    if(s.get_data().nb_solution() > 0) {
        return s.get_data().solution(0)[0];
    } else {
        return x_start;
    }
    /*VarSet varset(cos_secante_point_, cos_secante_point_.arg(0));
    std::cout << search_lb << " " << search_ub << std::endl;
    IntervalVector box = varset.full_box(IntervalVector{{search_lb, search_ub}}, IntervalVector{{x_start}});
    std::queue<IntervalVector> search_stack;
    auto pair = box.bisect(0);
    search_stack.push(pair.first);
    search_stack.push(pair.second);
    bool success = false;
    while(!success) {
        IntervalVector sbox = search_stack.front();
        search_stack.pop();
        success = newton(cos_secante_point_, varset, sbox);
        if(sbox.is_empty() || !success) {
            continue;
        } else if(success) {
            return sbox[0];
        } else {
            pair = sbox.bisect(0);
            search_stack.push(pair.first);
            search_stack.push(pair.second);
        }
    }
    // error*/
}


} // namespace ibex