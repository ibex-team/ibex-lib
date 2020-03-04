#include "ibex_ConvexEnvelope.h"
#include "ibex_TemplateVector.h"
#include "ibex_TemplateMatrix.h"
#include "ibex_Function.h"

#include "ibex_SystemFactory.h"
#include "ibex_System.h"
#include "ibex_DefaultSolver.h"

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
static const Function secante_tanh("x", "xp", "1/(cosh(x)^2)*(x-xp) + tanh(x) - tanh(xp)");
static const Function secante_tan("x", "xp", "1/(cos(x)^2*(x-xp) + tan(x) - tan(xp)");

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
    assert(count >= 3);
    ConvexEnvelope ce(1);
    const double dx = x.diam()/(count-1);
    const Interval xlb(x.lb());
    const Interval xub(x.ub());
    const Interval xlb_value = ibex::sqr(xlb);
    const Interval xub_value = ibex::sqr(xub);
    ce.add_linear_constraint(tangente_under(xlb, xlb_value, 2*xlb));
    ce.add_linear_constraint(tangente_under(xub, xub_value, 2*xub));
    double x_pt = x.lb()+dx;
    for(int i = 0; i < count-2; ++i) {
        const Interval pt(x_pt);
        ce.add_linear_constraint(tangente_under(x_pt, ibex::sqr(x_pt), 2*x_pt));
        x_pt += dx;
    }
    ce.add_linear_constraint(line_over(xlb, xlb_value, xub, xub_value));
    return ce;
}

ConvexEnvelope sqrt(const Interval& x, int count) {
    ConvexEnvelope ce(1);
    const Interval xlb(std::max(0.0, x.lb()));
    const Interval xub(x.ub());
    const Interval xlb_value = ibex::sqrt(xlb);
    const Interval xub_value = ibex::sqrt(xub);
    const double dx = (xub.lb() - xlb.ub())/(count-1);

    if(xlb.lb() > 0.0) {
        ce.add_linear_constraint(tangente_over(xlb, xlb_value, 1./(2*xlb_value)));
    } else {
        const ConvexEnvelope::LinearConstraint lc{0, {-1}, 0.0, CmpOp::LEQ};
        ce.add_linear_constraint(lc);
    }
    ce.add_linear_constraint(tangente_over(xub, xub_value, 1./(2*xub_value)));
    double x_pt = x.lb()+dx;
    for(int i = 0; i < count-2; ++i) {
        const Interval pt_value = ibex::sqrt(x_pt);
        const Interval pt_deriv = 1./(2*pt_value);
        ce.add_linear_constraint(tangente_under(x_pt, pt_value, pt_deriv));
        x_pt += dx;
    }
    ce.add_linear_constraint(line_under(xlb, xlb_value, xub, xub_value));
    return ce;
}

ConvexEnvelope exp(const Interval& x, int count) {
    assert(count >= 3);
    ConvexEnvelope ce(1);
    const double dx = x.diam()/(count-1);
    const Interval xlb(x.lb());
    const Interval xub(x.ub());
    const Interval xlb_value = ibex::exp(xlb);
    const Interval xub_value = ibex::exp(xub);
    ce.add_linear_constraint(tangente_under(xlb, xlb_value, xub_value));
    ce.add_linear_constraint(tangente_under(xub, xub_value, xub_value));
    double x_pt = x.lb()+dx;
    for(int i = 0; i < count-2; ++i) {
        const Interval pt_value = ibex::exp(x_pt);
        ce.add_linear_constraint(tangente_under(x_pt, pt_value, pt_value));
        x_pt += dx;
    }
    ce.add_linear_constraint(line_over(xlb, xlb_value, xub, xub_value));
    return ce;
}

ConvexEnvelope log(const Interval& x, int count) {
    ConvexEnvelope ce(1);
    const Interval xlb(std::max(0.0, x.lb()));
    const Interval xub(x.ub());
    const Interval xlb_value = ibex::log(xlb);
    const Interval xub_value = ibex::log(xub);
    const double dx = (xub.lb() - xlb.ub())/(count-1);

    if(xlb.lb() > 0.0) {
        ce.add_linear_constraint(tangente_over(xlb, xlb_value, 1./xlb_value));
    } else {
        const ConvexEnvelope::LinearConstraint lc{0, {-1}, 0.0, CmpOp::LEQ};
        ce.add_linear_constraint(lc);
    }
    ce.add_linear_constraint(tangente_over(xub, xub_value, 1./(2*xub_value)));
    double x_pt = x.lb()+dx;
    for(int i = 0; i < count-2; ++i) {
        const Interval pt_value = ibex::log(x_pt);
        const Interval pt_deriv = 1./pt_value;
        ce.add_linear_constraint(tangente_under(x_pt, pt_value, pt_deriv));
        x_pt += dx;
    }
    ce.add_linear_constraint(line_under(xlb, xlb_value, xub, xub_value));
    return ce;
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
        const Interval secante_left = find_secante(x.lb(), search_left.lb(), search_right.ub(), secante_tan);
        ce.add_linear_constraint(tangente_under(secante_left, ibex::tan(secante_left), deriv(secante_left)));
        ce.add_linear_constraint(tangente_under(xub, ibex::tan(xub), deriv(xub)));
    }
    {
        const Interval search_left = xlb;
        const Interval search_right = xub - xub_mod;
        const Interval secante_right = find_secante(x.ub(), search_left.lb(), search_right.ub(), secante_tan);
        ce.add_linear_constraint(tangente_over(secante_right, ibex::tan(secante_right), deriv(secante_right)));
        ce.add_linear_constraint(tangente_over(xlb, ibex::tan(xlb), deriv(xlb)));
    }
    return ce;
}

ConvexEnvelope tanh(const Interval& x) {
    ConvexEnvelope ce(1);
    auto func = [](const Interval& x) {
        return ibex::tanh(x);
    };
    auto deriv = [](const Interval& x) {
        return -1./ibex::sqr(ibex::cosh(x));
    };
    const Interval xlb = x.lb();
    const Interval xub = x.ub();

    if(xub <= 0) {
        return convex_function(x, func, deriv);
    } else if (xlb >= 0) {
        return concave_function(x, func, deriv);
    else {
        const Interval secante_left = find_secante(x.lb(), 0, x.ub(), secante_tanh);
        ce.add_linear_constraint(tangente_over(secante_left, func(secante_left), deriv(secante_left)));
        ce.add_linear_constraint(tangente_over(xub, func(xub), deriv(xub)));

        const Interval secante_right = find_secante(x.ub(), x.lb(), x.ub(), secante_tanh);
        ce.add_linear_constraint(tangente_under(secante_right, func(secante_right), deriv(secante_right)));
        ce.add_linear_constraint(tangente_under(xlb, func(xlb), deriv(xlb)));
    }
    return ce;
}

ConvexEnvelope acos(const Interval& x) {
    auto func = [](const Interval& x) {
        return ibex::acos(x);
    };
    auto deriv = [](const Interval& x) {
        return -1./ibex::sqrt(1-ibex::sqr(x));
    };
    const double xlb = x.lb();
    const double xub = x.ub();

    if(xub <= 0) {
        return convex_function(x, func, deriv);
    } else if (xlb >= 0) {
        return concave_function(x, func, deriv);
    } else {
        ConvexEnvelope ce(1);
        const Interval secante_left = find_secante(x.lb(), 0, 1, secante_acos);
        ce.add_linear_constraint(tangente_over(secante_left, func(secante_left), deriv(secante_left)));
        ce.add_linear_constraint(tangente_over(xub, func(xub), deriv(xub)));

        const Interval secante_right = find_secante(x.ub(), -1, 0, secante_tan);
        ce.add_linear_constraint(tangente_under(secante_right, func(secante_right), deriv(secante_right)));
        ce.add_linear_constraint(tangente_under(xlb, func(xlb), deriv(xlb)));
        return ce;
    }
}

ConvexEnvelope sinh(const Interval& x) {
    ConvexEnvelope ce(1);
    auto deriv = [](const Interval x) {
        return ibex::cosh(x);
    };
    const Interval xlb = x.lb();
    const Interval xub = x.ub();
    {
        const Interval secante_left = find_secante(x.lb(), x.lb(), x.ub(), secante_sinh);
        ce.add_linear_constraint(tangente_over(secante_left, ibex::sinh(secante_left), deriv(secante_left)));
        ce.add_linear_constraint(tangente_over(xub, ibex::sinh(xub), deriv(xub)));
    }
    {
        const Interval secante_right = find_secante(x.ub(), x.lb(), x.ub(), secante_sinh);
        ce.add_linear_constraint(tangente_under(secante_right, ibex::sinh(secante_right), deriv(secante_right)));
        ce.add_linear_constraint(tangente_under(xlb, ibex::sinh(xlb), deriv(xlb)));
    }
    return ce;
}

ConvexEnvelope cosh(const Interval& x, int count) {
    ConvexEnvelope ce(1);
    const double dx = x.diam()/(count-1);
    const Interval xlb(x.lb());
    const Interval xub(x.ub());
    const Interval xlb_value = ibex::cosh(xlb);
    const Interval xub_value = ibex::cosh(xub);
    const Interval xlb_deriv = ibex::sinh(xlb);
    const Interval xub_deriv = ibex::sinh(xub);
    ce.add_linear_constraint(tangente_under(xlb, xlb_value, xlb_deriv));
    ce.add_linear_constraint(tangente_under(xub, xub_value, xub_deriv));
    double x_pt = x.lb()+dx;
    for(int i = 0; i < count-2; ++i) {
        const Interval pt(x_pt);
        ce.add_linear_constraint(tangente_under(x_pt, ibex::cosh(x_pt), ibex::sinh(x_pt)));
        x_pt += dx;
    }
    ce.add_linear_constraint(line_over(xlb, xlb_value, xub, xub_value));
    return ce;
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
        secante_left = find_secante(xlb, search_left.lb(), search_right.ub(), secante_cos);
    }
    if(xub_mod.ub() >= Interval::half_pi().lb()) {
        const Interval search_left = xub - xub_mod;
        const Interval search_right = xub + Interval::half_pi() - xub_mod;
        secante_right = find_secante(xub, search_left.lb(), search_right.ub(), secante_cos);
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
}

Interval find_secante(double x_start, double search_lb, double search_ub, const Function& f) {
    // TODO FIXME Faux, temporaire
    //Function cos_secante_point_ = Function("x", "xp", "sin(x)*(x-xp) + cos(x) - cos(xp)");
    SystemFactory fac;
    Variable x;
    fac.add_var(x);
    const ExprConstant& cst = ExprConstant::new_scalar(x_start);
    fac.add_ctr(f(x, cst) = 0);
    System sys(fac);
    DefaultSolver s(sys);
    s.solve(Interval(search_lb, search_ub));
    if(s.get_data().nb_solution() > 0) {
        return s.get_data().solution(0)[0];
    } else {
        return x_start;
    }
}


ConvexEnvelope::LinearConstraint tangente_under(const Interval& x_pt, const Interval& x_value, const Interval& x_deriv) {
    // y >= x_value + x_deriv(x - x_pt)
    // -y + x_deriv x <= -x_value + x_deriv*x_pt
    const double coef = x_deriv.lb();
    const double rhs = (x_deriv*x_pt- x_value).lb();
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
    const Interval coef = (y2-y1)/(x2-x1);
    const Interval rhs = (x1*y2 - x2*y1)/(x2-x1);
    return ConvexEnvelope::LinearConstraint{-1, {coef.lb()}, -rhs.lb(), CmpOp::LEQ};
}

ConvexEnvelope::LinearConstraint line_over(const Interval& x1, const Interval& y1, const Interval& x2, const Interval& y2) {
    // y <= a x + b
    // y - ax <= b
    const Interval coef = (y2-y1)/(x2-x1);
    const Interval rhs = (x1*y2 - x2*y1)/(x2-x1);
    return ConvexEnvelope::LinearConstraint{1, {-coef.ub()}, -rhs.ub(), CmpOp::LEQ};
}


} // end namespace convex_envelope


} // namespace ibex