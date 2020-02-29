#ifndef __IBEX_AVM_H__
#define __IBEX_AVM_H__

#include "ibex_FwdAlgorithm.h"
#include "ibex_Function.h"
#include "ibex_ExprDomain.h"
#include "ibex_ExprDataAVM.h"
#include "ibex_LPSolver.h"
#include "ibex_Eval.h"
#include "ibex_SparseVector.h"

namespace ibex {

class AVM : public FwdAlgorithm {
public:
    AVM(Eval& eval);
    ~AVM();

    struct Result {
        double uplo;
        Vector point;
    };

    void set_cost(const Vector& vec);
    LPSolver::Status minimize(const IntervalVector& box);
    Interval minimum() const;

    void load_node_bounds_in_solver(int y);



    inline void vector_fwd (int* x, int y);
	inline void apply_fwd  (int* x, int y);
	inline void idx_fwd    (int x, int y);
	inline void idx_cp_fwd (int x, int y);
	inline void symbol_fwd (int y);
	inline void cst_fwd    (int y);
	inline void chi_fwd    (int x1, int x2, int x3, int y);
	inline void gen2_fwd   (int x1, int x2, int y);
	inline void add_fwd    (int x1, int x2, int y);
	inline void mul_fwd    (int x1, int x2, int y);
	inline void sub_fwd    (int x1, int x2, int y);
	inline void div_fwd    (int x1, int x2, int y);
	inline void max_fwd    (int x1, int x2, int y);
	inline void min_fwd    (int x1, int x2, int y);
	inline void atan2_fwd  (int x1, int x2, int y);
	inline void gen1_fwd   (int x, int y);
	inline void minus_fwd  (int x, int y);
	inline void minus_V_fwd(int x, int y);
	inline void minus_M_fwd(int x, int y);
	inline void trans_V_fwd(int x, int y);
	inline void trans_M_fwd(int x, int y);
	inline void sign_fwd   (int x, int y);
	inline void abs_fwd    (int x, int y);
	inline void power_fwd  (int x, int y, int p);
	inline void sqr_fwd    (int x, int y);
	inline void sqrt_fwd   (int x, int y);
	inline void exp_fwd    (int x, int y);
	inline void log_fwd    (int x, int y);
	inline void cos_fwd    (int x, int y);
	inline void sin_fwd    (int x, int y);
	inline void tan_fwd    (int x, int y);
	inline void cosh_fwd   (int x, int y);
	inline void sinh_fwd   (int x, int y);
	inline void tanh_fwd   (int x, int y);
	inline void acos_fwd   (int x, int y);
	inline void asin_fwd   (int x, int y);
	inline void atan_fwd   (int x, int y);
	inline void acosh_fwd  (int x, int y);
	inline void asinh_fwd  (int x, int y);
	inline void atanh_fwd  (int x, int y);
	inline void floor_fwd  (int x, int y);
	inline void ceil_fwd   (int x, int y);
	inline void saw_fwd    (int x, int y);
	inline void add_V_fwd  (int x1, int x2, int y);
	inline void add_M_fwd  (int x1, int x2, int y);
	inline void mul_SV_fwd (int x1, int x2, int y);
	inline void mul_SM_fwd (int x1, int x2, int y);
	inline void mul_VV_fwd (int x1, int x2, int y);
	inline void mul_MV_fwd (int x1, int x2, int y);
	inline void mul_VM_fwd (int x1, int x2, int y);
	inline void mul_MM_fwd (int x1, int x2, int y);
	inline void sub_V_fwd  (int x1, int x2, int y);
	inline void sub_M_fwd  (int x1, int x2, int y);

private:
    Eval& eval_;
    Function& f_;
    const ExprDomain& d_; // = eval_.d
    ExprDataAVM avm_d_;
    LPSolver* solver_;

    int lin_;
    int yvar_;
    int xvar_;
    int x1var_;
    int x2var_;
    AVMData* avm_data_;

    void setup_node(int x, int y);
    void setup_node(int x1, int x2, int y);
    void finish_node(int x, int y);
    void finish_node(int x1, int x2, int y);

    Vector filter_variables(const Vector& solver_result) const;
    int node_index_to_first_var(int index) const;
    int node_index_to_first_lin(int index) const;
    double node_d_ub(int y) const;
    double node_d_lb(int y) const;

    struct coef_pair {
        int var;
        double coef;
    };

    void load_constraint(int lin, const std::vector<coef_pair>& list, double rhs);
    void load_constraint(int lin, const std::vector<coef_pair>& list, double lhs, double rhs);
    template<typename T, typename D>
    inline void unary_convex_underestimator(int lin_index, double x, T& func, D& deriv);
    template<typename T>
    inline void unary_convex_overestimator(int lin_index, Interval x, T& func);
    template<typename T, typename D>
    inline void unary_concave_overestimator(int lin_index, double x, T& func, D& deriv);
    template<typename T>
    inline void unary_concave_underestimator(int lin_index, Interval x, T& func);

    // Helper Ibex functions
    Function cos_tangent_point = Function("x", "xp", "sin(x)*(x-xp) + cos(x) - cos(xp)");
};

template<typename T, typename D>
inline void AVM::unary_convex_underestimator(int lin_index, double x, T& func, D& deriv) {
    Interval grad = deriv(x);
    Interval value = func(x);
    double y_coef = -1;
    double x_coef = grad.lb();
    double rhs = (grad*x - value).lb();
    load_constraint(lin_index, {{yvar_, y_coef}, {xvar_, x_coef}}, rhs);
}

template<typename T>
inline void AVM::unary_convex_overestimator(int lin_index, Interval x, T& func) {
    double y_coef = 1;
    Interval fub = func(x.ub());
    Interval flb = func(x.lb());
    Interval a = (fub - flb)/(x.ub()-x.lb());
    double x_coef = -a.ub();
    double rhs = (fub - a*x.ub()).ub();
    load_constraint(lin_index, {{yvar_, y_coef}, {xvar_, x_coef}}, rhs);
}

template<typename T, typename D>
inline void AVM::unary_concave_overestimator(int lin_index, double x, T& func, D& deriv) {
    Interval grad = deriv(x);
    Interval value = func(x);
    double y_coef = 1;
    double x_coef = -grad.ub();
    double rhs = (value - grad*x).ub();
    load_constraint(lin_index, {{yvar_, y_coef}, {xvar_, x_coef}}, rhs);
}

template<typename T>
inline void AVM::unary_concave_underestimator(int lin_index, Interval x, T& func) {
    double y_coef = -1;
    Interval fub = func(x.ub());
    Interval flb = func(x.lb());
    Interval a = (fub - flb)/(x.ub()-x.lb());
    double x_coef = a.lb();
    double rhs = (a*x.ub()-fub).lb();
    load_constraint(lin_index, {{yvar_, y_coef}, {xvar_, x_coef}}, rhs);
}


inline void AVM::vector_fwd(int* x, int y) {
    /* nothing to do */
    not_implemented("Vector not implemented for AVM");
}

inline void AVM::apply_fwd(int* x, int y) {
    /* nothing to do */
    not_implemented("Apply not implemented for AVM");
}

inline void AVM::idx_fwd(int x, int y) {
    /* nothing to do */
}

inline void AVM::idx_cp_fwd(int x, int y) {
    /* nothing to do */
}

inline void AVM::symbol_fwd(int y) {
    load_node_bounds_in_solver(y);
}

inline void AVM::cst_fwd(int y) {
    load_node_bounds_in_solver(y);
}

inline void AVM::chi_fwd(int x1, int x2, int x3, int y) {
    // d[y].i() = chi(d[x1].i(),d[x2].i(),d[x3].i());
    not_implemented("Chi not implemented for AVM");
}

inline void AVM::gen2_fwd(int x1, int x2, int y) {
    // d[y].i() = chi(d[x1].i(),d[x2].i(),d[x3].i());
    not_implemented("Gen2 not implemented for AVM");
}

inline void AVM::add_fwd(int x1, int x2, int y)   {
    // - y + x1 + x2 = 0
    setup_node(x1, x2, y);
    if(x1 == x2) {
        load_constraint(lin_, {{yvar_, -1}, {x1var_, 2}}, 0.0, 0.0);
    } else {
        load_constraint(lin_, {{yvar_, -1}, {x1var_, 1}, {x2var_, 1}}, 0.0, 0.0);
    }
    finish_node(x1, x2, y);
}

inline void AVM::mul_fwd(int x1, int x2, int y)   {
    if(x1 == x2) {
        sqr_fwd(x1, y);
        return;
    }
    // d[y].i()=d[x1].i()*d[x2].i();
    // - y + ub(x1)*x2 + ub(x2)*x1 <= ub(x1)*ub(x2)
    // - y + lb(x1)*x2 + lb(x2)*x1 <= lb(x1)*lb(x2)

    setup_node(x1, x2, y);
    const double x1u = node_d_ub(x1);
    const double x1l = node_d_lb(x1);
    const double x2u = node_d_ub(x2);
    const double x2l = node_d_lb(x2);
    /*
     * Convex/Concave enveloppe for bilinear terms
     * - y + x2u x1 + x1u x2 <= x1u*x2u
     * - y + x2l x1 + x1l x2 <= x1l*x2l
     *   y - x2u x1 - x1l x2 <= -x1l*x2u
     *   y - x2l x1 - x1u x2 <= -x1u*x2l
     */
    load_constraint(lin_++, {{yvar_,-1}, {x1var_, x2u}, {x2var_, x1u}}, x1u*x2u);
    load_constraint(lin_++, {{yvar_,-1}, {x1var_, x2l}, {x2var_, x1l}}, x1l*x2l);
    load_constraint(lin_++, {{yvar_, 1}, {x1var_,-x2u}, {x2var_,-x1l}},-x1l*x2u);
    load_constraint(lin_,   {{yvar_, 1}, {x1var_,-x2l}, {x2var_,-x1u}},-x1u*x2l);
    finish_node(x1, x2, y);
}
inline void AVM::sub_fwd(int x1, int x2, int y)   {
    // - y + x1 - x2 <= 0
    setup_node(x1, x2, y);
    if(x1 == x2) {
        load_constraint(lin_, {{yvar_, 1}, {x1var_, 0}}, 0.0, 0.0);
    } else {
        load_constraint(lin_, {{yvar_, -1}, {x1var_, 1}, {x2var_, -1}}, 0.0, 0.0);
    }
    finish_node(x1, x2, y);
}
inline void AVM::div_fwd(int x1, int x2, int y)   {
    // d[y].i()=d[x1].i()/d[x2].i();
    not_implemented("Div not implemented for AVM");
}
inline void AVM::max_fwd(int x1, int x2, int y)   {
    // d[y].i()=max(d[x1].i(),d[x2].i());
    not_implemented("Max not implemented for AVM");
}
inline void AVM::min_fwd(int x1, int x2, int y)   {
    // d[y].i()=min(d[x1].i(),d[x2].i());
    not_implemented("Min not implemented for AVM");
}
inline void AVM::atan2_fwd(int x1, int x2, int y) {
    // d[y].i()=atan2(d[x1].i(),d[x2].i());
    not_implemented("Atan2 not implemented for AVM");
}

inline void AVM::gen1_fwd(int x, int y) {
    not_implemented("Gen1 not implemented for AVM");
}

inline void AVM::minus_fwd(int x, int y)          {
    // d[y].i()=-d[x].i();
    setup_node(x, y);
    load_constraint(lin_, {{yvar_, 1}, {xvar_, 1}}, 0.0, 0.0);
    finish_node(x, y);
}
inline void AVM::minus_V_fwd(int x, int y)        {
    // d[y].v()=-d[x].v();
    not_implemented("Minus_V not implemented for AVM");
}
inline void AVM::minus_M_fwd(int x, int y)        {
    // d[y].m()=-d[x].m();
    not_implemented("Minus_M not implemented for AVM");
}
inline void AVM::sign_fwd(int x, int y)           {
    // d[y].i()=sign(d[x].i());
    not_implemented("Sign not implemented for AVM");
}
inline void AVM::abs_fwd(int x, int y)            {
    // d[y].i()=abs(d[x].i());
    not_implemented("Abs not implemented for AVM");
}
inline void AVM::power_fwd(int x, int y, int p)   {
    // d[y].i()=pow(d[x].i(),p);
    not_implemented("Power not implemented for AVM");
}

inline void AVM::sqr_fwd(int x, int y)            {
    // 0 >= x^2 = y >= lin
    assert(avm_d_[y].lin_count >= 6);
    setup_node(x, y);

    auto func = [](double x_pt) { return sqr(Interval(x_pt)); };
    auto deriv = [](double x_pt) { return 2*Interval(x_pt); };

    auto linearize = [&](double x_pt) {
        unary_convex_underestimator(lin_++, x_pt, func, deriv);
    };

    const Interval& itv = d_[x].i();
    unary_convex_overestimator(lin_++, itv, func);
    if(itv.contains(0)) {
        linearize(0.0);
        linearize(Interval(0.0, itv.ub()).mid());
        linearize(Interval(itv.lb(), 0.0).mid());
    } else {
        linearize(itv.mid());
        auto pair = itv.bisect();
        linearize(pair.first.mid());
        linearize(pair.second.mid());
    }
    linearize(itv.lb());
    linearize(itv.ub());
    finish_node(x, y);
}
inline void AVM::sqrt_fwd(int x, int y)           {
    // if ((d[y].i()=sqrt(d[x].i())).is_empty()) throw EmptyBoxException();
    setup_node(x, y);

    auto func = [](double x_pt) { return sqrt(Interval(x_pt)); };
    auto deriv = [](double x_pt) { return 1/(2*sqrt(Interval(x_pt))); };

    auto linearize = [&](double x_pt) {
        unary_concave_overestimator(lin_++, x_pt, func, deriv);
    };

    Interval itv = d_[x].i();
    itv = Interval(std::max(+0.0, itv.lb()), itv.ub());
    unary_concave_underestimator(lin_++, itv, func);
    if(itv.lb() == 0) {
        load_constraint(lin_++, {{yvar_, 0.0}, {xvar_, -1}}, 0.0);
    } else {
        linearize(itv.lb());
    }
    linearize(itv.mid());
    linearize(itv.ub());
    finish_node(x, y);
}

inline void AVM::exp_fwd(int x, int y)            {
    // d[y].i()=exp(d[x].i());
    setup_node(x, y);

    auto func = [](double x_pt) { return exp(Interval(x_pt)); };
    auto deriv = [](double x_pt) { return exp(Interval(x_pt)); };

    auto linearize = [&](double x_pt) {
        unary_convex_underestimator(lin_++, x_pt, func, deriv);
    };

    const Interval& itv = d_[x].i();
    unary_convex_overestimator(lin_++, itv, func);

    linearize(itv.mid());
    linearize(itv.lb());
    linearize(itv.ub());
    finish_node(x, y);
}
inline void AVM::log_fwd(int x, int y)            {
    // if ((d[y].i()=log(d[x].i())).is_empty()) throw EmptyBoxException();
    setup_node(x, y);

    auto func = [](double x_pt) { return log(Interval(x_pt)); };
    auto deriv = [](double x_pt) { return 1/(Interval(x_pt)); };

    auto linearize = [&](double x_pt) {
        unary_concave_overestimator(lin_++, x_pt, func, deriv);
    };

    Interval itv = d_[x].i();
    itv = Interval(std::max(+0.0, itv.lb()), itv.ub());
    if(itv.lb() == 0.0) {
        load_constraint(lin_++, {{yvar_, 0}, {xvar_, 1}}, itv.ub());
    } else {
        unary_concave_underestimator(lin_++, itv, func);
    }
    if(itv.lb() == 0.0) {
        load_constraint(lin_++, {{yvar_, 0.0}, {xvar_, -1}}, 0.0);
    } else {
        linearize(itv.lb());
    }
    linearize(itv.mid());
    linearize(itv.ub());
    finish_node(x, y);
}

inline void AVM::cos_fwd(int x, int y)            {
    setup_node(x, y);
    Interval itv(d_[x].i());
    auto func = [](double x_pt) { return cos(Interval(x_pt)); };
    auto deriv = [](double x_pt) { return -sin(Interval(x_pt)); };
    Interval left_eval = func(itv.lb());
    Interval right_eval = func(itv.ub());
    if(itv.diam() >= Interval::two_pi().lb()) {
        load_constraint(lin_++, {{yvar_, 1}, {xvar_, 0}}, 1);
        load_constraint(lin_++, {{yvar_, -1},{xvar_, 0}}, 1);
        finish_node(x, y);
        return;
    }
    /*int k = ibex::ceil(-(1+itv.lb()/Interval::pi())/2.);
    double xlb1 = (itv.lb() + k*2*Interval::two_pi()).lb();
    double xub1 = (itv.ub() + k*2*Interval::two_pi()).ub();
    //on veut -pi <xlb1 < pi =>  xlb1 < xub1 < xlb1 + 2pi < 3pi
    // d'abord, gauche et over
    if(xlb1 < -pi/2) { // xub < 3pi/2
        if(xub1 < -pi/2) {
            // convexe...
        } else if(xub > pi/2) {
            // over:si secante(xlb), pt_secante_over_gauche, sinon pt_secante_over_gauche=xub
        }
    } else if(pi/2 < xlb1 < 0)
    } else if()
    /*const int yvar_ = node_index_to_first_var(y);
    const int xvar = node_index_to_first_var(x);
    int lin = node_index_to_first_lin(y);

    auto func = [](double x_pt) { return cos(Interval(x_pt)); };
    auto deriv = [](double x_pt) { return -sin(Interval(x_pt)); };

    auto linearize = [&](double x_pt) {
        double y_coef, x_coef, rhs;
        unary_concave_overestimator(x_pt, func, deriv, y_coef, x_coef, rhs);
        unary_linearization(lin_,++, {yvar_, y_coef}, {xvar, x_coef}, rhs);
    };
    */
    finish_node(x, y);
    not_implemented("Cos not implemented for AVM");
}
inline void AVM::sin_fwd(int x, int y)            {
    // d[y].i()=sin(d[x].i());
    not_implemented("Sin not implemented for AVM");
}
inline void AVM::tan_fwd(int x, int y)            {
    // if ((d[y].i()=tan(d[x].i())).is_empty()) throw EmptyBoxException();
    not_implemented("Tan not implemented for AVM");
}
inline void AVM::cosh_fwd(int x, int y)           {
    // d[y].i()=cosh(d[x].i());
    not_implemented("Cosh not implemented for AVM");
}
inline void AVM::sinh_fwd(int x, int y)           {
    // d[y].i()=sinh(d[x].i());
    not_implemented("Sinh not implemented for AVM");
}
inline void AVM::tanh_fwd(int x, int y)           {
    // d[y].i()=tanh(d[x].i());
    not_implemented("Tanh not implemented for AVM");
}
inline void AVM::acos_fwd(int x, int y)           {
    // if ((d[y].i()=acos(d[x].i())).is_empty()) throw EmptyBoxException();
    not_implemented("Acos not implemented for AVM");
}
inline void AVM::asin_fwd(int x, int y)           {
    // if ((d[y].i()=asin(d[x].i())).is_empty()) throw EmptyBoxException();
    not_implemented("Asin not implemented for AVM");
}
inline void AVM::atan_fwd(int x, int y)           {
    // d[y].i()=atan(d[x].i());
    not_implemented("Atan not implemented for AVM");
}
inline void AVM::acosh_fwd(int x, int y)          {
    // if ((d[y].i()=acosh(d[x].i())).is_empty()) throw EmptyBoxException();
    not_implemented("Acosh not implemented for AVM");
}
inline void AVM::asinh_fwd(int x, int y)          {
    // d[y].i()=asinh(d[x].i());
    not_implemented("Asinh not implemented for AVM");
}
inline void AVM::atanh_fwd(int x, int y)          {
    // if ((d[y].i()=atanh(d[x].i())).is_empty()) throw EmptyBoxException();
    not_implemented("Atanh not implemented for AVM");
}
inline void AVM::floor_fwd(int x, int y)          {
    // if ((d[y].i()=floor(d[x].i())).is_empty()) throw EmptyBoxException();
    not_implemented("Floor not implemented for AVM");
}
inline void AVM::ceil_fwd(int x, int y)           {
    // if ((d[y].i()=ceil(d[x].i())).is_empty()) throw EmptyBoxException();
    not_implemented("Ceil not implemented for AVM");
}
inline void AVM::saw_fwd(int x, int y)            {
    // if ((d[y].i()=saw(d[x].i())).is_empty()) throw EmptyBoxException();
    not_implemented("Saw not implemented for AVM");
}

inline void AVM::trans_V_fwd(int x, int y)        {
    // d[y].v()=d[x].v();
    not_implemented("Trans_V not implemented for AVM");
}
inline void AVM::trans_M_fwd(int x, int y)        {
    // d[y].m()=d[x].m().transpose();
    not_implemented("Trans_M not implemented for AVM");
}
inline void AVM::add_V_fwd(int x1, int x2, int y) {
    // d[y].v()=d[x1].v()+d[x2].v();
    not_implemented("Add_V not implemented for AVM");
}
inline void AVM::add_M_fwd(int x1, int x2, int y) {
    // d[y].m()=d[x1].m()+d[x2].m();
    not_implemented("Add_M not implemented for AVM");
}
inline void AVM::mul_SV_fwd(int x1, int x2, int y){
    // d[y].v()=d[x1].i()*d[x2].v();
    not_implemented("Mul_SV not implemented for AVM");
}
inline void AVM::mul_SM_fwd(int x1, int x2, int y){
    // d[y].m()=d[x1].i()*d[x2].m();
    not_implemented("Mul_SM not implemented for AVM");
}
inline void AVM::mul_VV_fwd(int x1, int x2, int y){
    // d[y].i()=d[x1].v()*d[x2].v();
    not_implemented("Mul_VV not implemented for AVM");
}
inline void AVM::mul_MV_fwd(int x1, int x2, int y){
    // d[y].v()=d[x1].m()*d[x2].v();
    not_implemented("Mul_MV not implemented for AVM");
}
inline void AVM::mul_VM_fwd(int x1, int x2, int y){
    // d[y].v()=d[x1].v()*d[x2].m();
    not_implemented("Mul_VM not implemented for AVM");
}
inline void AVM::mul_MM_fwd(int x1, int x2, int y){
    // d[y].m()=d[x1].m()*d[x2].m();
    not_implemented("Mul_MM not implemented for AVM");
}
inline void AVM::sub_V_fwd(int x1, int x2, int y) {
    // d[y].v()=d[x1].v()-d[x2].v();
    not_implemented("Sub_V not implemented for AVM");
}
inline void AVM::sub_M_fwd(int x1, int x2, int y) {
    // d[y].m()=d[x1].m()-d[x2].m();
    not_implemented("Sub_M not implemented for AVM");
}


} // namespace ibex

#endif  // __IBEX_AVM_H__