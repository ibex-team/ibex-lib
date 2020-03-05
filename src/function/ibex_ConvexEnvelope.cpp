#include "ibex_ConvexEnvelope.h"
#include "ibex_TemplateVector.h"
#include "ibex_TemplateMatrix.h"
#include "ibex_Function.h"
#include "ibex_CtcNewton.h"

#include <queue>

namespace ibex {
/* ConvexEnvelope */

ConvexEnvelope::ConvexEnvelope(int nb_var): nb_var(nb_var) {

}

void ConvexEnvelope::clear() {
    linear_constraints_.clear();
}

int ConvexEnvelope::nb_envelope() const {
    return linear_constraints_.size();
}

const ConvexEnvelope::LinearConstraint& ConvexEnvelope::operator[](int i) const {
    return linear_constraints_[i];
}

ConvexEnvelope::LinearConstraint& ConvexEnvelope::operator[](int i) {
    return linear_constraints_[i];
}

void ConvexEnvelope::add_linear_constraint(const LinearConstraint& lc) {
    linear_constraints_.emplace_back(lc);
}

void ConvexEnvelope::merge_envelope(const ConvexEnvelope& ce) {
    for(int i = 0; i < ce.nb_envelope(); ++i) {
        add_linear_constraint(ce[i]);
    }
}

/* ConvexEnvelopeVector */
ConvexEnvelopeVector::ConvexEnvelopeVector(int n) {
    assert(n > 0);
    envelopes_ = std::vector<ConvexEnvelope>(n, ConvexEnvelope(0));
}

int ConvexEnvelopeVector::size() const {
    return envelopes_.size();
}

void ConvexEnvelopeVector::resize(int new_n) {
    assert(new_n > 0);
    envelopes_.resize(new_n, ConvexEnvelope(0));
}

ConvexEnvelopeVector ConvexEnvelopeVector::subvector(int first_index, int last_index) const {
    return _subvector(*this, first_index, last_index);
}

const ConvexEnvelope& ConvexEnvelopeVector::operator[](int i) const {
    return envelopes_[i];
}

ConvexEnvelope& ConvexEnvelopeVector::operator[](int i) {
    return envelopes_[i];
}

/* ConvexEnvelopeMatrix */

ConvexEnvelopeMatrix::ConvexEnvelopeMatrix(int nb_rows, int nb_cols)
: nb_rows_(nb_rows), nb_cols_(nb_cols) {
    rows_ = std::vector<ConvexEnvelopeVector>(nb_rows, ConvexEnvelopeVector(nb_cols));
}

const ConvexEnvelopeVector& ConvexEnvelopeMatrix::row(int index) const {
    return rows_[index];
}

ConvexEnvelopeVector ConvexEnvelopeMatrix::col(int index) const {
    return _col<ConvexEnvelopeMatrix, ConvexEnvelopeVector>(*this, index);
}

int ConvexEnvelopeMatrix::nb_rows() const {
    return rows_.size();
}

ConvexEnvelopeMatrix ConvexEnvelopeMatrix::submatrix(int first_row, int last_row, int first_col, int last_col) const {
    return _submatrix(*this, first_row, last_row, first_col, last_col);

}

const ConvexEnvelopeVector& ConvexEnvelopeMatrix::operator[](int i) const {
    return rows_[i];
}

ConvexEnvelopeVector& ConvexEnvelopeMatrix::operator[](int i) {
    return rows_[i];
}

namespace convex_envelope {

static const Function secante_cos("x", "xp", "sin(x)*(x-xp) + cos(x) - cos(xp)");
static const Function secante_sinh("x", "xp", "-cosh(x)*(x-xp) + sinh(x) - sinh(xp)");
static const Function secante_tanh("x", "xp", "-1/cosh(x)^2*(x-xp) + tanh(x) - tanh(xp)");
static const Function secante_atanh("x", "xp", "1/(x^2-1)*(x-xp) + atanh(x) - atanh(xp)");
static const Function secante_tan("x", "xp", "1/cos(x)^2*(x-xp) + tan(x) - tan(xp)");
static const Function secante_acos("x", "xp", "1/sqrt(1-x^2)*(x-xp) + acos(x) - acos(xp)");
static const Function secante_atan("x", "xp", "-1/(1+x^2)*(x-xp) + atan(x) - atan(xp)");
static const Function secante_asinh("x", "xp", "-1/sqrt(1+x^2)*(x-xp) + asinh(x) - asinh(xp)");
static const Function secante_asin("x", "xp", "-1/(sqrt(1-x^2)*(x-xp) + asin(x) - asin(xp))");
ConvexEnvelope add(const Interval& x1, const Interval& x2) {
    // y = x1 + x2
    ConvexEnvelope ce(2);
    ConvexEnvelope::LinearConstraint lc{1, {-1, -1}, 0, CmpOp::EQ};
    ce.add_linear_constraint(lc);
    return ce;
}

ConvexEnvelope sub(const Interval& x1, const Interval& x2) {
    // y = x1 - x2
    ConvexEnvelope ce(2);
    ConvexEnvelope::LinearConstraint lc{1, {-1, 1}, 0, CmpOp::EQ};
    ce.add_linear_constraint(lc);
    return ce;
}

ConvexEnvelope int_mul(int a, const Interval& x) {
    // y = a x
    ConvexEnvelope ce(2);
    ConvexEnvelope::LinearConstraint lc{1, {static_cast<double>(-a)}, 0, CmpOp::EQ};
    ce.add_linear_constraint(lc);
    return ce;
}

ConvexEnvelope mul(const Interval& x1, const Interval& x2) {
    using LC = ConvexEnvelope::LinearConstraint;
    const Interval uu = Interval(x1.ub())*x2.ub();
    const Interval ll = Interval(x1.lb())*x1.lb();
    const Interval lu = Interval(x1.lb())*x2.ub();
    const Interval ul = Interval(x1.ub())*x2.lb();
    LC lc1{-1, {x1.ub(), x2.ub()}, uu.ub(), CmpOp::LEQ};
    LC lc2{-1, {x1.lb(), x2.lb()}, ll.ub(), CmpOp::LEQ};
    LC lc3{1, {-x1.ub(), -x2.lb()}, -ul.lb(), CmpOp::LEQ};
    LC lc4{1, {-x1.lb(), -x2.ub()}, -lu.lb(), CmpOp::LEQ};
    ConvexEnvelope ce(2);
    ce.add_linear_constraint(lc1);
    ce.add_linear_constraint(lc2);
    ce.add_linear_constraint(lc3);
    ce.add_linear_constraint(lc4);
    return ce;
}

ConvexEnvelope sqr(const Interval& x, int count) {
    auto func = [](const Interval& xpt) { return ibex::sqr(xpt); };
    auto deriv = [](const Interval& xpt) { return 2*xpt; };
    return convex_function(x, func, deriv);
}

ConvexEnvelope sqrt(const Interval& x, int count) {
    auto func = [](const Interval& xpt) { return ibex::sqrt(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./(2*xpt); };
    if(x.lb() <= 0) {
        return concave_function(Interval(0, x.ub()), func, deriv, true, false);
    } else {
        return concave_function(x, func, deriv);
    }
}

ConvexEnvelope exp(const Interval& x, int count) {
    auto func = [](const Interval& xpt) { return ibex::exp(xpt); };
    auto deriv = [](const Interval& xpt) { return ibex::exp(xpt); };
    return convex_function(x, func, deriv);
}

ConvexEnvelope log(const Interval& x, int count) {
    auto func = [](const Interval& xpt) { return ibex::log(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./xpt; };
    if(x.lb() <= 0.0) {
        return concave_function(Interval(0, x.ub()), func, deriv, true, false);
    } else {
        return concave_function(x, func, deriv);
    }
}

ConvexEnvelope cos(const Interval& x) {
    ConvexEnvelope ce1 = cos_offset(x, Interval(0), false);
    ConvexEnvelope ce2 = cos_offset(x, Interval::pi(), true);
    ce1.merge_envelope(ce2);
    return ce1;
}

ConvexEnvelope sin(const Interval& x) {
    ConvexEnvelope ce1 = cos_offset(x, -Interval::half_pi(), false);
    ConvexEnvelope ce2 = cos_offset(x, Interval::half_pi(), true);
    ce1.merge_envelope(ce2);
    return ce1;
}

ConvexEnvelope tan(const Interval& x) {
    ConvexEnvelope ce(1);
    if(ibex::tan(x).is_unbounded()) {
        return ce;
    }
    auto deriv = [](const Interval x) {
        return -1./ibex::sqr(ibex::cos(x));
    };
    const double xlb = x.lb();
    const double xub = x.ub();
    const Interval xlb_mod = Interval(std::fmod(xlb, Interval::two_pi().lb()));
    const Interval xub_mod = Interval(std::fmod(xub, Interval::two_pi().lb()));
    {
        const Interval search_left = xlb - xlb_mod;
        const Interval search_right = xub;
        Interval secante_left;
        bool b = find_secante(x.lb(), search_left.lb(), search_right.ub(), secante_tan, secante_left);
        ce.add_linear_constraint(tangente_under(secante_left, ibex::tan(secante_left), deriv(secante_left)));
        ce.add_linear_constraint(tangente_under(xub, ibex::tan(xub), deriv(xub)));
    }
    {
        const Interval search_left = xlb;
        const Interval search_right = xub - xub_mod;
        Interval secante_right;
        bool b = find_secante(x.ub(), search_left.lb(), search_right.ub(), secante_tan, secante_right);
        ce.add_linear_constraint(tangente_over(secante_right, ibex::tan(secante_right), deriv(secante_right)));
        ce.add_linear_constraint(tangente_over(xlb, ibex::tan(xlb), deriv(xlb)));
    }
    return ce;
}

ConvexEnvelope tanh(const Interval& x) {
    auto func = [](const Interval& xpt) { return ibex::tanh(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./ibex::sqr(ibex::cosh(xpt)); };
    return convex_concave_function(x, func, deriv, secante_tanh, 0, ConvexConcaveProperties());
}

ConvexEnvelope atanh(const Interval& x) {
    auto func = [](const Interval& xpt) { return ibex::atanh(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./(1-ibex::sqr(xpt)); };
    ConvexConcaveProperties prop;
    if(x.lb() <= -1.0) {
        prop.vertical_lb_tangente = true;
    }
    if(x.ub() >= 1.0) {
        prop.vertical_ub_tangente = true;
    }
    Interval new_x(std::max(x.lb(), -1.0), std::min(x.ub(), 1.0));
    ConvexEnvelope ce = concave_convex_function(new_x, func, deriv, secante_atanh, 0, prop);
    return ce;
}

ConvexEnvelope acos(const Interval& x) {
    auto func = [](const Interval& xpt) { return ibex::acos(xpt); };
    auto deriv = [](const Interval& xpt) { return -1./ibex::sqrt(1-ibex::sqr(xpt)); };
    ConvexConcaveProperties prop;
    if(x.lb() <= -1.0) {
        prop.vertical_lb_tangente = true;
    }
    if(x.ub() >= 1.0) {
        prop.vertical_ub_tangente = true;
    }
    Interval new_x(std::max(x.lb(), -1.0), std::min(x.ub(), 1.0));
    ConvexEnvelope ce = convex_concave_function(new_x, func, deriv, secante_acos, 0, prop);
    return ce;
}

ConvexEnvelope asin(const Interval& x) {
    auto func = [](const Interval& xpt) { return ibex::asin(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./ibex::sqrt(1-ibex::sqr(xpt)); };
    ConvexConcaveProperties prop;
    if(x.lb() <= -1.0) {
        prop.vertical_lb_tangente = true;
    }
    if(x.ub() >= 1.0) {
        prop.vertical_ub_tangente = true;
    }
    Interval new_x(std::max(x.lb(), -1.0), std::min(x.ub(), 1.0));
    ConvexEnvelope ce = concave_convex_function(new_x, func, deriv, secante_asin, 0, prop);
    return ce;
}

ConvexEnvelope atan(const Interval& x) {
    auto func = [](const Interval& xpt) { return ibex::atan(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./(1+ibex::sqr(xpt)); };
    return convex_concave_function(x, func, deriv, secante_atan, 0, ConvexConcaveProperties());
}

ConvexEnvelope sinh(const Interval& x) {
    ConvexEnvelope ce(1);
    auto deriv = [](const Interval x) {
        return ibex::cosh(x);
    };
    const Interval xlb = x.lb();
    const Interval xub = x.ub();
    {
        Interval secante_left;
        bool b = find_secante(x.lb(), x.lb(), x.ub(), secante_sinh, secante_left);
        ce.add_linear_constraint(tangente_over(secante_left, ibex::sinh(secante_left), deriv(secante_left)));
        ce.add_linear_constraint(tangente_over(xub, ibex::sinh(xub), deriv(xub)));
    }
    {
        Interval secante_right;
        bool b = find_secante(x.ub(), x.lb(), x.ub(), secante_sinh, secante_right);
        ce.add_linear_constraint(tangente_under(secante_right, ibex::sinh(secante_right), deriv(secante_right)));
        ce.add_linear_constraint(tangente_under(xlb, ibex::sinh(xlb), deriv(xlb)));
    }
    return ce;
}

ConvexEnvelope asinh(const Interval& x) {
    auto func = [](const Interval& xpt) { return ibex::asinh(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./ibex::sqrt(1+ibex::sqr(xpt)); };
    return convex_concave_function(x, func, deriv, secante_asinh, 0, ConvexConcaveProperties());
}

ConvexEnvelope cosh(const Interval& x, int count) {
    auto func = [](const Interval& xpt) { return ibex::cosh(xpt); };
    auto deriv = [](const Interval& xpt) { return ibex::sinh(xpt); };
    return convex_function(x, func, deriv);
}

ConvexEnvelope acosh(const Interval& x, int count) {
    auto func = [](const Interval& xpt) { return ibex::acosh(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./(ibex::sqrt(xpt-1)*ibex::sqrt(xpt+1)); };
    if(x.lb() <= 1) {
        return concave_function(Interval(1.0, x.ub()), func, deriv, true, false);
    } else {
        return concave_function(x, func, deriv);
    }
}

ConvexEnvelope floor(const Interval& x) {
    ConvexEnvelope ce(1);
    ce.add_linear_constraint(line_over(0, 0, 1, 1));
    ce.add_linear_constraint(line_under(0, -1, 1, 0));
    return ce;
}

ConvexEnvelope ceil(const Interval& x) {
    ConvexEnvelope ce(1);
    ce.add_linear_constraint(line_over(-1, 0, 0, 1));
    ce.add_linear_constraint(line_under(-1, -1, 0, 0));
    return ce;
}

ConvexEnvelope saw(const Interval& x) {
    ConvexEnvelope ce(1);
    const Interval xlb(x.lb());
    const Interval xlb_value = ibex::saw(xlb);
    const Interval xub(x.ub());
    const Interval xub_value = ibex::saw(xub);
    ce.add_linear_constraint(line_over(xlb, xlb_value, ibex::ceil(xlb), 1));
    ce.add_linear_constraint(line_under(ibex::floor(xub), 0, xub, xub_value));
    return ce;
}

ConvexEnvelope abs(const Interval& x) {
    ConvexEnvelope ce(1);
    if(x.lb() >= 0) {
        const ConvexEnvelope::LinearConstraint lc{1, {-1}, 0, CmpOp::EQ};
        ce.add_linear_constraint(lc);
    } else if(x.ub() <= 0) {
        const ConvexEnvelope::LinearConstraint lc{1, {1}, 0, CmpOp::EQ};
        ce.add_linear_constraint(lc);
    } else {
        ce.add_linear_constraint(line_over(x.lb(), -x.lb(), x.ub(), x.ub()));
        ce.add_linear_constraint(line_under(0, 0, -1, 1));
        ce.add_linear_constraint(line_under(0, 0, 1, 1));
    }
    return ce;
}

ConvexEnvelope sign(const Interval& x) {
    ConvexEnvelope ce(1);
    if(x.ub() <= 0) {
        ce.add_linear_constraint(bound_node(CmpOp::LEQ, -1));
        ce.add_linear_constraint(bound_node(CmpOp::GEQ, -1));
        ce.add_linear_constraint(bound(CmpOp::GEQ, x.lb()));
        ce.add_linear_constraint(bound(CmpOp::LEQ, x.ub()));
    } else if(x.lb() >= 0) {
        ce.add_linear_constraint(bound_node(CmpOp::LEQ, -1));
        ce.add_linear_constraint(bound_node(CmpOp::GEQ, -1));
        ce.add_linear_constraint(bound(CmpOp::GEQ, x.lb()));
        ce.add_linear_constraint(bound(CmpOp::LEQ, x.ub()));
    } else {
        ce.add_linear_constraint(line_over(x.lb(), -1, 0, 1));
        ce.add_linear_constraint(line_under(0, -1, x.ub(), 1));
        ce.add_linear_constraint(bound_node(CmpOp::LEQ, 1));
        ce.add_linear_constraint(bound_node(CmpOp::GEQ, -1));
    }
    return ce;
}

ConvexEnvelope max(const Interval& x1, const Interval& x2) {
    // y >= max(x1.lb, x2.lb)
    // y <= max(x1.ub, x2.ub)
    ConvexEnvelope ce(2);
    const ConvexEnvelope::LinearConstraint lc{-1, {0, 0}, -std::max(x1.lb(), x2.lb()), CmpOp::LEQ};
    const ConvexEnvelope::LinearConstraint lc{1, {0, 0}, std::max(x1.ub(), x2.ub()), CmpOp::LEQ};
    return ce;
}

ConvexEnvelope min(const Interval& x1, const Interval& x2) {
    // y >= max(x1.lb, x2.lb)
    // y <= max(x1.ub, x2.ub)
    ConvexEnvelope ce(2);
    const ConvexEnvelope::LinearConstraint lc{-1, {0, 0}, -std::min(x1.lb(), x2.lb()), CmpOp::LEQ};
    const ConvexEnvelope::LinearConstraint lc{1, {0, 0}, std::min(x1.ub(), x2.ub()), CmpOp::LEQ};
    return ce;
}

ConvexEnvelope div(const Interval& x1, const Interval& x2) {
    // Tawarmalani, M., & Sahinidis, N. V. (2001). Journal of Global Optimization, 20(2), 133–154. doi:10.1023/a:1011233805045
    // x/y positive orthant
    // concave(y) = 1/(yL*yU)min (yu x−xl y+xl yl ,yl x−xu y+ xu yu )
}

ConvexEnvelope cos_offset(const Interval& x, const Interval& offset, bool negate) {
    double xlb = (x.lb() + offset).lb();
    double xub = (x.ub() + offset).ub();
    ConvexEnvelope ce(1);
    auto secante_over = [&](Interval x_pt) {
        double x_coef = ibex::sin(x_pt).ub();
        double rhs = (ibex::cos(x_pt)+ibex::sin(x_pt)*x_pt - x_coef*offset).ub();
        double y_coef = 1;
        if(negate) {
            y_coef = -1;
        }
        const ConvexEnvelope::LinearConstraint lc{y_coef, {x_coef}, rhs, CmpOp::LEQ};
        ce.add_linear_constraint(lc);
    };

    // TODO exact modulo
    Interval xlb_mod = Interval(std::fmod(xlb, Interval::two_pi().lb()));
    Interval xub_mod = Interval(std::fmod(xub, Interval::two_pi().lb()));
    const Interval _3pi_over_2 = 3*Interval::half_pi();
    Interval secante_left = xlb;
    Interval secante_right = xub;
    if(xlb_mod.lb() <= _3pi_over_2.ub()) {
        const Interval search_left = xlb + _3pi_over_2 - xlb_mod;
        const Interval search_right = xlb + Interval::two_pi() - xlb_mod;
        bool b = find_secante(xlb, search_left.lb(), search_right.ub(), secante_cos, secante_left);
        if(!b) secante_left = xlb;
    }
    if(xub_mod.ub() >= Interval::half_pi().lb()) {
        const Interval search_left = xub - xub_mod;
        const Interval search_right = xub + Interval::half_pi() - xub_mod;
        bool b = find_secante(xub, search_left.lb(), search_right.ub(), secante_cos, secante_right);
        if(!b) secante_right = xub;
    }
    // std::cout << "secleft=" << secante_left << std::endl;
    // std::cout << "secright=" << secante_right << std::endl;
    const bool sec_left_gt_xub = secante_left.lb() >= xub;
    const bool sec_right_lt_xlb = secante_right.ub() <= xlb;
    if(sec_left_gt_xub && sec_right_lt_xlb) {
        if(negate) {
            ce.add_linear_constraint(line_under(xlb, ibex::cos(xlb), xub, ibex::cos(xub)));
        } else {
            ce.add_linear_constraint(line_over(xlb, ibex::cos(xlb), xub, ibex::cos(xub)));
        }
        return ce;
    }
    if(sec_right_lt_xlb) {
        secante_over(xub);
    } else {
        secante_over(secante_right);
    }
    if(sec_left_gt_xub) {
        secante_over(xlb);
    } else {
        secante_over(secante_left);
    }
    const Interval sec_itv = secante_left|secante_right;
    if(!sec_itv.is_empty() && sec_itv.diam() <= Interval::pi().lb()) {
        secante_over(sec_itv.mid());
    }
    return ce;
}

bool find_secante(double x_start, double search_lb, double search_ub, const Function& f, Interval& solution, int max_iter) {
    // TODO FIXME Faux, temporaire
    //Function cos_secante_point_ = Function("x", "xp", "sin(x)*(x-xp) + cos(x) - cos(xp)");
    // std::cout << "Contract "  << " in " << Interval(search_lb, search_ub) << std::endl;
    Variable x;
    const ExprConstant& cst = ExprConstant::new_scalar(x_start);
    Function newf(x, f(x, cst));
    IntervalVector search{{search_lb, search_ub}};
    if(!search.is_bisectable()) {
        solution.set_empty();
        return false;
    }
    auto pair = search.bisect(0);
    std::queue<IntervalVector> search_queue;
    search_queue.push(pair.first);
    search_queue.push(pair.second);
    int iter = 0;
    while(search_queue.size() > 0 && iter < max_iter) {
        iter++;
        search = search_queue.front();
        search_queue.pop();
        IntervalVector unicity;
        IntervalVector existence;
        bool b = inflating_newton(newf, search, existence, unicity);
        if(search.is_empty()) {
            continue;
        }
        if(b) {
            solution = existence[0];
            return true;
        } else if(search.is_bisectable()) {
            pair = search.bisect(0);
            search_queue.push(pair.first);
            search_queue.push(pair.second);
        }
    }
    if(iter == max_iter) {
        solution.set_empty();
        return false;
    }
    ibex_error("Unreachable");
}


ConvexEnvelope::LinearConstraint tangente_under(const Interval& x_pt, const Interval& x_value, const Interval& x_deriv) {
    // y >= x_value + x_deriv(x - x_pt)
    // -y + x_deriv x <= -x_value + x_deriv*x_pt
    const double coef = x_deriv.lb();
    const double rhs = (x_deriv*x_pt - x_value).ub();
    return ConvexEnvelope::LinearConstraint{-1, {coef}, rhs, CmpOp::LEQ};
}

ConvexEnvelope::LinearConstraint tangente_over(const Interval& x_pt, const Interval& x_value, const Interval& x_deriv) {
    // y <= x_value + x_deriv(x - x_pt)
    // y - x_deriv x <= +x_value - x_deriv*x_pt
    const double coef = -x_deriv.ub();
    const double rhs = (x_value -x_deriv*x_pt).ub();
    return ConvexEnvelope::LinearConstraint{1, {coef}, rhs, CmpOp::LEQ};
}

ConvexEnvelope::LinearConstraint line_under(const Interval& x1, const Interval& y1, const Interval& x2, const Interval& y2) {
    // y >= a x + b
    // -y + ax <= -b
    if(x1 == x2) {
        return bound_node(CmpOp::GEQ, y1.lb());
    }
    const Interval coef = (y2-y1)/(x2-x1);
    const Interval rhs = -(x1*y2 - x2*y1)/(x2-x1);
    return ConvexEnvelope::LinearConstraint{-1, {coef.lb()}, -rhs.lb(), CmpOp::LEQ};
}

ConvexEnvelope::LinearConstraint line_over(const Interval& x1, const Interval& y1, const Interval& x2, const Interval& y2) {
    // y <= a x + b
    // y - ax <= b
    if(x1 == x2) {
        return bound_node(CmpOp::LEQ, y1.ub());
    }
    const Interval coef = (y2-y1)/(x2-x1);
    const Interval rhs = -(x1*y2 - x2*y1)/(x2-x1);
    return ConvexEnvelope::LinearConstraint{1, {-coef.ub()}, rhs.ub(), CmpOp::LEQ};
}


} // end namespace convex_envelope


} // namespace ibex