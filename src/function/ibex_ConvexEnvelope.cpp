/* ============================================================================
 * I B E X - ConvexEnvelope.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * ---------------------------------------------------------------------------- */


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
    return nb_rows_;
}

int ConvexEnvelopeMatrix::nb_cols() const {
    return nb_cols_;
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
static const Function secante_tan("x", "xp", "-1/cos(x)^2*(x-xp) + tan(x) - tan(xp)");
static const Function secante_acos("x", "xp", "1/sqrt(1-x^2)*(x-xp) + acos(x) - acos(xp)");
static const Function secante_atan("x", "xp", "-1/(1+x^2)*(x-xp) + atan(x) - atan(xp)");
static const Function secante_asinh("x", "xp", "-1/sqrt(1+x^2)*(x-xp) + asinh(x) - asinh(xp)");
static const Function secante_asin("x", "xp", "-1/(sqrt(1-x^2)*(x-xp) + asin(x) - asin(xp))");
static const Function secante_power("x", "xp", "p", "-p*x^(p-1)*(x-xp) + x^p - xp^p");

#define DEFINE_MAX_LIN_CONST(name, value) const int MAX_LIN_##name = value;

DEFINE_MAX_LIN_CONST(CONCAVE, 4)
DEFINE_MAX_LIN_CONST(CONVEX, 4)
// Must be >= CONVEX
DEFINE_MAX_LIN_CONST(CONCAVE_CONVEX, 4)
DEFINE_MAX_LIN_CONST(CONVEX_CONCAVE, 4)

ConvexEnvelope equal() {
    ConvexEnvelope ce(0);
    ConvexEnvelope::LinearConstraint lc{1, {-1}, 0, CmpOp::EQ };
    ce.add_linear_constraint(lc);
    return ce;
}

DEFINE_MAX_LIN_CONST(add, 1)
ConvexEnvelope add(const Interval& x1, const Interval& x2) {
    // y = x1 + x2
    ConvexEnvelope ce(2);
    ConvexEnvelope::LinearConstraint lc{1, {-1, -1}, 0, CmpOp::EQ};
    ce.add_linear_constraint(lc);
    return ce;
}

DEFINE_MAX_LIN_CONST(sub, 1)
ConvexEnvelope sub(const Interval& x1, const Interval& x2) {
    // y = x1 - x2
    ConvexEnvelope ce(2);
    ConvexEnvelope::LinearConstraint lc{1, {-1, 1}, 0, CmpOp::EQ};
    ce.add_linear_constraint(lc);
    return ce;
}

DEFINE_MAX_LIN_CONST(int_mul, 1)
ConvexEnvelope int_mul(int a, const Interval& x) {
    // y = a x
    ConvexEnvelope ce(2);
    ConvexEnvelope::LinearConstraint lc{1, {static_cast<double>(-a)}, 0, CmpOp::EQ};
    ce.add_linear_constraint(lc);
    return ce;
}

DEFINE_MAX_LIN_CONST(mul, 4)
ConvexEnvelope mul(const Interval& x1, const Interval& x2) {
    using LC = ConvexEnvelope::LinearConstraint;
    const Interval uu = Interval(x1.ub())*x2.ub();
    const Interval ll = Interval(x1.lb())*x2.lb();
    const Interval lu = Interval(x1.lb())*x2.ub();
    const Interval ul = Interval(x1.ub())*x2.lb();
    // y >= x2u x1 + x1u x2 - x2u x1u
    // y >= x2l x1 + x1l x2 - x2l x1l
    // y <= x2u x1 + x1l x2 - x2u x1l
    // y <= x2l x1 + x1u x2 - x2l x1u
    LC lc1{-1, {x2.ub(), x1.ub()}, uu.ub(), CmpOp::LEQ};
    LC lc2{-1, {x2.lb(), x1.lb()}, ll.ub(), CmpOp::LEQ};
    LC lc3{1, {-x2.ub(), -x1.lb()}, -lu.lb(), CmpOp::LEQ};
    LC lc4{1, {-x2.lb(), -x1.ub()}, -ul.lb(), CmpOp::LEQ};
    ConvexEnvelope ce(2);
    ce.add_linear_constraint(lc1);
    ce.add_linear_constraint(lc2);
    ce.add_linear_constraint(lc3);
    ce.add_linear_constraint(lc4);
    return ce;
}

DEFINE_MAX_LIN_CONST(sqr, MAX_LIN_CONVEX)
ConvexEnvelope sqr(const Interval& x, int count) {
    auto func = [](const Interval& xpt) { return ibex::sqr(xpt); };
    auto deriv = [](const Interval& xpt) { return 2*xpt; };
    return convex_function(x, func, deriv);
}

DEFINE_MAX_LIN_CONST(sqrt, MAX_LIN_CONCAVE)
ConvexEnvelope sqrt(const Interval& x, int count) {
    auto func = [](const Interval& xpt) { return ibex::sqrt(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./(2*xpt); };
    if(x.lb() <= 0) {
        return concave_function(Interval(0, x.ub()), func, deriv, true, false);
    } else {
        return concave_function(x, func, deriv);
    }
}

DEFINE_MAX_LIN_CONST(exp, MAX_LIN_CONVEX)
ConvexEnvelope exp(const Interval& x, int count) {
    auto func = [](const Interval& xpt) { return ibex::exp(xpt); };
    auto deriv = [](const Interval& xpt) { return ibex::exp(xpt); };
    return convex_function(x, func, deriv);
}

DEFINE_MAX_LIN_CONST(log, MAX_LIN_CONCAVE)
ConvexEnvelope log(const Interval& x, int count) {
    auto func = [](const Interval& xpt) { return ibex::log(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./xpt; };
    if(x.lb() <= 0.0) {
        return concave_function(Interval(0, x.ub()), func, deriv, true, false);
    } else {
        return concave_function(x, func, deriv);
    }
}

DEFINE_MAX_LIN_CONST(cos, 8)
ConvexEnvelope cos(const Interval& x) {
    ConvexEnvelope ce1 = cos_offset(x, Interval(0), false);
    ConvexEnvelope ce2 = cos_offset(x, Interval::pi(), true);
    ce1.merge_envelope(ce2);
    ce1.add_linear_constraint(bound_node(CmpOp::LEQ, 1));
    ce1.add_linear_constraint(bound_node(CmpOp::GEQ, -1));
    return ce1;
}

DEFINE_MAX_LIN_CONST(sin, 8)
ConvexEnvelope sin(const Interval& x) {
    ConvexEnvelope ce1 = cos_offset(x, -Interval::half_pi(), false);
    ConvexEnvelope ce2 = cos_offset(x, Interval::half_pi(), true);
    ce1.merge_envelope(ce2);
    ce1.add_linear_constraint(bound_node(CmpOp::LEQ, 1));
    ce1.add_linear_constraint(bound_node(CmpOp::GEQ, -1));
    return ce1;
}

DEFINE_MAX_LIN_CONST(tan, 4)
ConvexEnvelope tan(const Interval& x) {
    ConvexEnvelope ce(1);
    if(x.diam() > Interval::pi().lb() || ibex::tan(x) == Interval::all_reals()) {
        return ce;
    }
    auto func = [](const Interval& xpt) {
        return ibex::tan(xpt);
    };
    auto deriv = [](const Interval& xpt) {
        return 1./ibex::sqr(ibex::cos(xpt));
    };
    const Interval ql = ibex::floor(x.lb()/Interval::pi());
    const Interval qu = ibex::floor(x.ub()/Interval::pi());
    const Interval xlb_mod = x.lb() - ql*Interval::pi();
    const Interval xub_mod = x.ub() - qu*Interval::pi();
    const Interval inflection_point = (ql+1)*Interval::pi();
    ConvexConcaveProperties prop(inflection_point, secante_tan);
    prop.vertical_lb_tangente = xlb_mod.overlaps(Interval::half_pi());
    prop.vertical_ub_tangente = xub_mod.overlaps(Interval::half_pi());
    return concave_convex_function(x, func, deriv, prop);

    /*const double xlb = x.lb();
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
    return ce;*/
}

DEFINE_MAX_LIN_CONST(tanh, MAX_LIN_CONVEX_CONCAVE)
ConvexEnvelope tanh(const Interval& x) {
    auto func = [](const Interval& xpt) { return ibex::tanh(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./ibex::sqr(ibex::cosh(xpt)); };
    return convex_concave_function(x, func, deriv, ConvexConcaveProperties(0, secante_tanh));
}

DEFINE_MAX_LIN_CONST(atanh, MAX_LIN_CONCAVE_CONVEX)
ConvexEnvelope atanh(const Interval& x) {
    auto func = [](const Interval& xpt) { return ibex::atanh(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./(1-ibex::sqr(xpt)); };
    ConvexConcaveProperties prop(0, secante_atanh);
    if(x.lb() <= -1.0) {
        prop.vertical_lb_tangente = true;
    }
    if(x.ub() >= 1.0) {
        prop.vertical_ub_tangente = true;
    }
    Interval new_x(std::max(x.lb(), -1.0), std::min(x.ub(), 1.0));
    ConvexEnvelope ce = concave_convex_function(new_x, func, deriv, prop);
    return ce;
}

DEFINE_MAX_LIN_CONST(acos, MAX_LIN_CONVEX_CONCAVE)
ConvexEnvelope acos(const Interval& x) {
    auto func = [](const Interval& xpt) { return ibex::acos(xpt); };
    auto deriv = [](const Interval& xpt) { return -1./ibex::sqrt(1-ibex::sqr(xpt)); };
    ConvexConcaveProperties prop(0, secante_acos);
    if(x.lb() <= -1.0) {
        prop.vertical_lb_tangente = true;
    }
    if(x.ub() >= 1.0) {
        prop.vertical_ub_tangente = true;
    }
    Interval new_x(std::max(x.lb(), -1.0), std::min(x.ub(), 1.0));
    ConvexEnvelope ce = convex_concave_function(new_x, func, deriv, prop);
    return ce;
}

DEFINE_MAX_LIN_CONST(asin, MAX_LIN_CONCAVE_CONVEX)
ConvexEnvelope asin(const Interval& x) {
    auto func = [](const Interval& xpt) { return ibex::asin(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./ibex::sqrt(1-ibex::sqr(xpt)); };
    ConvexConcaveProperties prop(0, secante_asin);
    if(x.lb() <= -1.0) {
        prop.vertical_lb_tangente = true;
    }
    if(x.ub() >= 1.0) {
        prop.vertical_ub_tangente = true;
    }
    Interval new_x(std::max(x.lb(), -1.0), std::min(x.ub(), 1.0));
    ConvexEnvelope ce = concave_convex_function(new_x, func, deriv, prop);
    return ce;
}

DEFINE_MAX_LIN_CONST(atan, MAX_LIN_CONVEX_CONCAVE)
ConvexEnvelope atan(const Interval& x) {
    auto func = [](const Interval& xpt) { return ibex::atan(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./(1+ibex::sqr(xpt)); };
    return convex_concave_function(x, func, deriv, ConvexConcaveProperties(0, secante_atan));
}

DEFINE_MAX_LIN_CONST(sinh, MAX_LIN_CONCAVE_CONVEX)
ConvexEnvelope sinh(const Interval& x) {
    auto func = [](const Interval& xpt) {
        return ibex::sinh(xpt);
    };
    auto deriv = [](const Interval xpt) {
        return ibex::cosh(xpt);
    };
    return concave_convex_function(x, func, deriv, ConvexConcaveProperties(0, secante_sinh));
}

DEFINE_MAX_LIN_CONST(asinh, MAX_LIN_CONVEX_CONCAVE)
ConvexEnvelope asinh(const Interval& x) {
    auto func = [](const Interval& xpt) { return ibex::asinh(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./ibex::sqrt(1+ibex::sqr(xpt)); };
    return convex_concave_function(x, func, deriv, ConvexConcaveProperties(0, secante_asinh));
}

DEFINE_MAX_LIN_CONST(cosh, MAX_LIN_CONVEX)
ConvexEnvelope cosh(const Interval& x, int count) {
    auto func = [](const Interval& xpt) { return ibex::cosh(xpt); };
    auto deriv = [](const Interval& xpt) { return ibex::sinh(xpt); };
    return convex_function(x, func, deriv);
}

DEFINE_MAX_LIN_CONST(acosh, MAX_LIN_CONCAVE)
ConvexEnvelope acosh(const Interval& x) {
    auto func = [](const Interval& xpt) { return ibex::acosh(xpt); };
    auto deriv = [](const Interval& xpt) { return 1./(ibex::sqrt(xpt-1)*ibex::sqrt(xpt+1)); };
    if(x.lb() <= 1) {
        return concave_function(Interval(1.0, x.ub()), func, deriv, true, false);
    } else {
        return concave_function(x, func, deriv);
    }
}

DEFINE_MAX_LIN_CONST(floor, 4)
ConvexEnvelope floor(const Interval& x) {
    ConvexEnvelope ce(1);
    ce.add_linear_constraint(line_over(0, 0, 1, 1));
    ce.add_linear_constraint(line_under(0, -1, 1, 0));
    ce.add_linear_constraint(bound_node(CmpOp::LEQ, ibex::floor(x.ub()).ub()));
    ce.add_linear_constraint(bound_node(CmpOp::GEQ, ibex::floor(x.lb()).lb()));
    return ce;
}

DEFINE_MAX_LIN_CONST(ceil, 4)
ConvexEnvelope ceil(const Interval& x) {
    ConvexEnvelope ce(1);
    ce.add_linear_constraint(line_over(-1, 0, 0, 1));
    ce.add_linear_constraint(line_under(-1, -1, 0, 0));
    ce.add_linear_constraint(bound_node(CmpOp::LEQ, ibex::ceil(x.ub()).ub()));
    ce.add_linear_constraint(bound_node(CmpOp::GEQ, ibex::ceil(x.lb()).lb()));

    return ce;
}

DEFINE_MAX_LIN_CONST(saw, 4)
ConvexEnvelope saw(const Interval& x) {
    ConvexEnvelope ce(1);
    const Interval xlb(x.lb());
    const Interval xlb_value = ibex::saw(xlb);
    const Interval xub(x.ub());
    const Interval xub_value = ibex::saw(xub);
    ce.add_linear_constraint(line_over(xlb, xlb_value, ibex::ceil(xlb), 1));
    ce.add_linear_constraint(line_under(ibex::floor(xub), 0, xub, xub_value));
    ce.add_linear_constraint(bound_node(CmpOp::LEQ, 1));
    ce.add_linear_constraint(bound_node(CmpOp::GEQ, 0));
    return ce;
}

DEFINE_MAX_LIN_CONST(abs, 3)
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

DEFINE_MAX_LIN_CONST(sign, 4)
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

DEFINE_MAX_LIN_CONST(max, 2)
ConvexEnvelope max(const Interval& x1, const Interval& x2) {
    // y >= max(x1.lb, x2.lb)
    // y <= max(x1.ub, x2.ub)
    ConvexEnvelope ce(2);
    const ConvexEnvelope::LinearConstraint lc{-1, {0, 0}, -std::max(x1.lb(), x2.lb()), CmpOp::LEQ};
    const ConvexEnvelope::LinearConstraint lc2{1, {0, 0}, std::max(x1.ub(), x2.ub()), CmpOp::LEQ};
    ce.add_linear_constraint(lc);
    ce.add_linear_constraint(lc2);
    return ce;
}

DEFINE_MAX_LIN_CONST(min, 2)
ConvexEnvelope min(const Interval& x1, const Interval& x2) {
    // y >= max(x1.lb, x2.lb)
    // y <= max(x1.ub, x2.ub)
    ConvexEnvelope ce(2);
    const ConvexEnvelope::LinearConstraint lc{-1, {0, 0}, -std::min(x1.lb(), x2.lb()), CmpOp::LEQ};
    const ConvexEnvelope::LinearConstraint lc2{1, {0, 0}, std::min(x1.ub(), x2.ub()), CmpOp::LEQ};
    ce.add_linear_constraint(lc);
    ce.add_linear_constraint(lc2);
    return ce;
}

ConvexEnvelope div(const Interval& x1, const Interval& x2) {
    // Tawarmalani, M., & Sahinidis, N. V. (2001). Journal of Global Optimization, 20(2), 133–154. doi:10.1023/a:1011233805045
    // x/y positive orthant
    // concave(y) = 1/(yL*yU)min (yu x−xl y+xl yl ,yl x−xu y+ xu yu )
}

DEFINE_MAX_LIN_CONST(div_from_mul, MAX_LIN_mul)
ConvexEnvelope div_from_mul(const Interval& x1, const Interval& x2, const Interval& y) {
    // y = x1/x2
    // y x2 = x1 (x1' x2' = y')
    using LC = ConvexEnvelope::LinearConstraint;
    const ConvexEnvelope mul_ce = mul(y, x2);
    ConvexEnvelope ce(2);
    for(int i = 0; i < mul_ce.nb_envelope(); ++i) {
        // y = x1', x2 = x2', x1 = y'
        const LC& lc = mul_ce[i];
        LC newlc{lc.x[0], {lc.y, lc.x[1]}, lc.rhs, lc.op};
        ce.add_linear_constraint(newlc);
    }

    return ce;
}

class FncPower : public Fnc {
private:
    int p_;
public:
    FncPower(int p): Fnc(2, 1), p_(p) {}

    Interval eval(const IntervalVector& box) const override {
        const Interval& x = box[0];
        const Interval& xp = box[1];
        return -p_*ibex::pow(x, p_-1)*(x-xp) + ibex::pow(x, p_) - ibex::pow(xp, p_);
    }

    IntervalVector eval_vector(const IntervalVector& box, const BitSet& components) const override {
        return eval(box);
    }

    void jacobian(const IntervalVector& box, IntervalMatrix& J, const BitSet& components, int v=-1) const override {
        const Interval& x = box[0];
        const Interval& xp = box[1];
        //{-(-1 + p) p x^(-2 + p) (x - xp), p x^(-1 + p) - p xp^(-1 + p)}
        if(components[0])
            J[0] = -(p_-1)*p_*ibex::pow(x, p_-2)*(x-xp);
        if(components[1])
            J[1] = p_*(ibex::pow(x, p_-1) - ibex::pow(xp, p_-1));
    }
};

DEFINE_MAX_LIN_CONST(power, MAX_LIN_CONCAVE_CONVEX)
ConvexEnvelope power(const Interval& x, int p) {
    auto func = [p](const Interval& xpt) {
        return pow(xpt, p);
    };
    auto deriv = [p](const Interval& xpt) {
        return p*pow(xpt, p-1);
    };
    if(p % 2 == 0) {
        return convex_function(x, func, deriv);
    } else {
        const FncPower secante_pow(p);
        auto ce = concave_convex_function(x, func, deriv, ConvexConcaveProperties(0, secante_pow));
        return ce;
    }
}

ConvexEnvelope cos_offset(const Interval& x, const Interval& offset, bool negate) {
    double xlb = (x.lb() + offset).lb();
    double xub = (x.ub() + offset).ub();
    ConvexEnvelope ce(1);
    auto func = [](const Interval& xpt) { return ibex::cos(xpt); };
    auto deriv = [](const Interval& xpt) { return -ibex::sin(xpt); };
    auto local_tangente_over = [&](const Interval& x_pt, const Interval& x_value, const Interval& x_deriv) {
        auto lc = tangente_over(x_pt, x_value, x_deriv);
        lc.rhs = (lc.rhs - lc.x[0]*offset).ub();
        if(negate) {
            lc.y = -lc.y;
        }
        ce.add_linear_constraint(lc);
    };
    auto local_line_over = [&](const Interval& x_l, const Interval& x_u) {
        auto lc = line_over(x_l, func(x_l), x_u, func(x_u));
        lc.rhs = (lc.rhs - lc.x[0]*offset).ub();
        if(negate) {
            lc.y = -lc.y;
        }
        ce.add_linear_constraint(lc);
    };

    const Interval ql = ibex::floor(xlb/Interval::two_pi());
    const Interval qu = ibex::floor(xub/Interval::two_pi());
    Interval secante_left, secante_right;
    bool secante_left_found, secante_right_found;
    Interval search_left = Interval::half_pi()*(3+4*ql);
    Interval search_right = search_left+Interval::half_pi();
    Interval search(search_left.lb(), search_right.ub());
    if(search.is_superset(xlb)) {
        secante_left_found = true;
        secante_left = xlb;
    } else {
        secante_left_found = find_secante(xlb, search_left.lb(), search_right.ub(), secante_cos, secante_left, 10);
    }
    search_left = Interval::two_pi()*qu;
    search_right = search_left + Interval::half_pi();
    search = Interval(search_left.lb(), search_right.ub());
    if(search.is_superset(xub)) {
        secante_right_found = true;
        secante_right = xub;
    } else {
        secante_right_found = find_secante(xub, search_left.lb(), search_right.ub(), secante_cos, secante_right, 10);
    }

    if(secante_left_found && secante_right_found && secante_left.lb() >= xub && secante_right.ub() <= xlb) {
        local_line_over(xlb, xub);
    } else {
        if(secante_left_found) {
            local_tangente_over(secante_left, func(secante_left), deriv(secante_left));
        } else {
            local_tangente_over(xlb, func(xlb), 1);
        }
        if(secante_right_found) {
            local_tangente_over(secante_right, func(secante_right), deriv(secante_right));
        } else {
            local_tangente_over(xub, func(xub), -1);
        }
        if(secante_left_found && secante_right_found) {
            const Interval convex(
                secante_left.lb() >= xub ? xlb : secante_left.lb(),
                secante_right.ub() <= xlb ? xub : secante_right.ub()
            );
            if(!convex.is_empty()) {
                const double mid = convex.mid();
                local_tangente_over(mid, func(mid), deriv(mid));
            }
        }
    }
    return ce;
}

bool find_secante(double x_start, double search_lb, double search_ub, const Fnc& f, Interval& solution, int max_iter) {
    // TODO FIXME Faux, temporaire
    //Function cos_secante_point_ = Function("x", "xp", "sin(x)*(x-xp) + cos(x) - cos(xp)");
    // std::cout << "Contract "  << " in " << Interval(search_lb, search_ub) << std::endl;
    // std::cout << "x_start: " << x_start << std::endl;
    // Variable x;
    // const ExprConstant& cst = ExprConstant::new_scalar(x_start);
    // const Function& asfunc = dynamic_cast<const Function&>(f);
    // Function newf(x, asfunc(x, cst));
    IntervalVector search{{search_lb, search_ub}, x_start};
    BitSet bitset(2);
    bitset.add(0);
    VarSet varset(2, bitset, true);
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
        // std::cout << "search: " << search << std::endl;
        IntervalVector unicity;
        IntervalVector existence;
        bool b = inflating_newton(f, varset, search, existence, unicity);
        // std::cout << "after: " << search << std::endl;
        if(search.is_empty()) {
            continue;
        }
        if(b) {
            solution = existence[0];
            // std::cout << "existence: " << existence << std::endl;
            // std::cout << "unicity: " << unicity << std::endl;
            return !solution.is_empty();
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

ConvexEnvelope::LinearConstraint tangente(const Interval& x_pt, const Interval& x_value, const Interval& x_deriv, EnvelopeType env_type) {
    switch(env_type) {
    case EnvelopeType::Over:
        return tangente_over(x_pt, x_value, x_deriv);
    case EnvelopeType::Under:
        return tangente_under(x_pt, x_value, x_deriv);
    }
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

ConvexEnvelope::LinearConstraint line(const Interval& x1, const Interval& x_value1, const Interval& x2, const Interval& x_value2, EnvelopeType env_type) {
    switch(env_type) {
    case EnvelopeType::Over:
        return line_over(x1, x_value1, x2, x_value2);
    case EnvelopeType::Under:
        return line_under(x1, x_value1, x2, x_value2);
    }
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