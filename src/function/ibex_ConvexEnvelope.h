#ifndef __IBEX_CONVEX_ENVELOPE_H__
#define __IBEX_CONVEX_ENVELOPE_H__

#include <vector>
#include "ibex_CmpOp.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_Function.h"

namespace ibex {

class ConvexEnvelope {
public:

    struct LinearConstraint {
        double y;
        std::vector<double> x;
        double rhs;
        CmpOp op;
    };

    ConvexEnvelope(int nb_var);

    int nb_var;

    void add_linear_constraint(const LinearConstraint& lc);
    int nb_envelope() const;
    void clear();
    void merge_envelope(const ConvexEnvelope& ce);
    LinearConstraint& operator[](int index);
    const LinearConstraint& operator[](int index) const;

private:
    std::vector<LinearConstraint> linear_constraints_;
};

namespace convex_envelope {

    ConvexEnvelope add(const Interval& x1, const Interval& x2);
    ConvexEnvelope sub(const Interval& x1, const Interval& x2);
    ConvexEnvelope mul(const Interval& x1, const Interval& x2);
    ConvexEnvelope int_mul(int a, const Interval& x);
    ConvexEnvelope sqr(const Interval& x, int count = 3);
    ConvexEnvelope sqrt(const Interval& x, int count = 3);
    ConvexEnvelope exp(const Interval& x, int count = 3);
    ConvexEnvelope log(const Interval& x, int count = 3);
    ConvexEnvelope cos(const Interval& x);
    ConvexEnvelope sin(const Interval& x);
    ConvexEnvelope tan(const Interval& x);
    ConvexEnvelope cosh(const Interval& x, int count = 3);
    ConvexEnvelope sinh(const Interval& x);
    ConvexEnvelope tanh(const Interval& x);
    ConvexEnvelope acos(const Interval& x);
    ConvexEnvelope floor(const Interval& x);
    ConvexEnvelope ceil(const Interval& x);
    ConvexEnvelope saw(const Interval& x);

    ConvexEnvelope cos_offset(const Interval& x, const Interval& offset, bool negate);
    bool find_secante(double x_start, double search_lb, double search_ub, const Function& f, Interval& solution, int max_iter=10);
    ConvexEnvelope::LinearConstraint tangente_under(const Interval& x, const Interval& x_value, const Interval& x_deriv);
    ConvexEnvelope::LinearConstraint tangente_over(const Interval& x, const Interval& x_value, const Interval& x_deriv);
    ConvexEnvelope::LinearConstraint line_under(const Interval& x1, const Interval& x_value1, const Interval& x2, const Interval& x_value2);
    ConvexEnvelope::LinearConstraint line_over(const Interval& x1, const Interval& x_value1, const Interval& x2, const Interval& x_value2);

    template<typename Func, typename Deriv>
    inline ConvexEnvelope convex_function(const Interval& x, const Func& func, const Deriv& deriv) {
        ConvexEnvelope ce(1);
        const Interval f_xlb = func(x.lb());
        const Interval f_xub = func(x.ub());
        const Interval g_xlb = deriv(x.lb());
        const Interval g_xub = deriv(x.ub());
        ce.add_linear_constraint(line_over(f_xlb, g_xlb, f_xub, g_xub));
        ce.add_linear_constraint(tangente_under(x.lb(), f_xlb, g_xlb));
        ce.add_linear_constraint(tangente_under(x.ub(), f_xub, g_xub));
        ce.add_linear_constraint(tangente_under(x.mid(), func(x.mid()), deriv(x.mid())));
        return ce;
    }

    template<typename Func, typename Deriv>
    inline ConvexEnvelope concave_function(const Interval& x, const Func& func, const Deriv& deriv) {
        ConvexEnvelope ce(1);
        const Interval f_xlb = func(x.lb());
        const Interval f_xub = func(x.ub());
        const Interval g_xlb = deriv(x.lb());
        const Interval g_xub = deriv(x.ub());
        ce.add_linear_constraint(line_under(f_xlb, g_xlb, f_xub, g_xub));
        ce.add_linear_constraint(tangente_over(x.lb(), f_xlb, g_xlb));
        ce.add_linear_constraint(tangente_over(x.ub(), f_xub, g_xub));
        ce.add_linear_constraint(tangente_over(x.mid(), func(x.mid()), deriv(x.mid())));
        return ce;
    }

    inline ConvexEnvelope::LinearConstraint bound(CmpOp op, double rhs) {
        if(op == CmpOp::LEQ) {
            return ConvexEnvelope::LinearConstraint{0,{1}, rhs, CmpOp::LEQ};
        } else {
            return ConvexEnvelope::LinearConstraint{0, {-1}, -rhs, CmpOp::LEQ};
        }
    }

    inline ConvexEnvelope::LinearConstraint bound_node(CmpOp op, double rhs) {
        if(op == CmpOp::LEQ) {
            return ConvexEnvelope::LinearConstraint{1,{0}, rhs, CmpOp::LEQ};
        } else {
            return ConvexEnvelope::LinearConstraint{-1, {0}, -rhs, CmpOp::LEQ};
        }
    }

    /*template<typename Func, typename Deriv>
    ConvexEnvelope convex_concave_decreasing_function(
        const Interval& x, const Func& func, const Deriv& deriv,
        const Function& secante_func,
        const double x_inflexion, bool vertical_lb, bool vertical_ub) {
        // acos-like functions
        using LC = ConvexEnvelope::LinearConstraint;
        if(x.ub() <= x_inflexion) {
            return convex_function(x, func, deriv, vertical_lb, vertical_ub);
        } else if(x.lb() >= x_inflexion) {
            return concave_function(x, func, deriv, vertical_lb, vertical_ub);
        } else {
            ConvexEnvelope ce(1);
            if(!vertical_lb) {
                const Interval secante_left = find_secante(x.lb(), x_inflexion, x.ub(), secante_func);
                if(secante_left.lb() >= x.ub()) {
                    const LC lc = line_over(x.lb(), func(x.lb()), x.ub(), func(x.ub()));
                    ce.add_linear_constraint(lc);
                } else {
                    const LC lc = tangente_over(secante_left, func(secante_left), deriv(secante_left));
                    ce.add_linear_constraint(lc);
                    if(!vertical_ub) {
                        const LC lc2 = tangente_over(x.ub(), func(x.ub()), deriv(x.ub()));
                        ce.add_linear_constraint(lc2);
                        // + middle ?
                    }
                }
            } else {
                const LC lc = bound(CmpOp::GEQ, x.lb());
                ce.add_linear_constraint(lc);
            }
            if(!vertical_ub) {
                const Interval secante_right = find_secante(x.ub(), x.lb(), x_inflexion, secante_func);
                if(secante_right.lb() <= x.lb()) {
                    const LC lc = line_under(x.lb(), func(x.ub()), x.lb(), func(x.ub()));
                    ce.add_linear_constraint(lc);
                } else {
                    const LC lc = tangente_under(secante_right, func(secante_right), deriv(secante_right));
                    ce.add_linear_constraint(lc);
                    if(!vertical_lb) {
                        const LC lc2 = tangente_under(x.lb(), func(x.lb()), deriv(x.lb()));
                        ce.add_linear_constraint(lc2);
                        // + middle ?
                    }
                }
            } else {
                const LC lc = bound(CmpOp::LEQ, x.ub());
                ce.add_linear_constraint(lc);
            }
            return ce;
        }
    }*/

    template<typename Func, typename Deriv>
    ConvexEnvelope convex_concave_increasing_function(
        const Interval& x, const Func& func, const Deriv& deriv,
        const Function& secante_func,
        const double x_inflexion) {
        // acos-like functions
        using LC = ConvexEnvelope::LinearConstraint;
        if(x.ub() <= x_inflexion) {
            return convex_function(x, func, deriv);
        } else if(x.lb() >= x_inflexion) {
            return concave_function(x, func, deriv);
        } else {
            ConvexEnvelope ce(1);
            Interval secante_left;
            /* We need an upper bound not to large the newton to effective.
            Upper bound -x.lb() is an heuristic derived from tanh,
            visually we see that the secante point in always less than -x.lb().
            Since the other functions have some sort of symmetries around 0,
            this seems to be a good choice: x.lb() and secante.left seem
            to always meet at 0. Maybe TODO set ub for secante search for each operator.
            idem for the secante_right */
            bool b = find_secante(x.lb(), x_inflexion, 2*x_inflexion-x.lb(), secante_func, secante_left);
            std::cout <<"secl" << secante_left << std::endl;
            if(b && secante_left.lb() >= x.ub()) {
                const LC lc = line_over(x.lb(), func(x.lb()), x.ub(), func(x.ub()));
                ce.add_linear_constraint(lc);
            } else if(b) {
                const LC lc = tangente_over(secante_left, func(secante_left), deriv(secante_left));
                ce.add_linear_constraint(lc);
                if(secante_left.ub() <= x.ub()) {
                    const LC lc2 = tangente_over(x.ub(), func(x.ub()), deriv(x.ub()));
                    ce.add_linear_constraint(lc2);
                }
                    // + middle ?
            } else {
                // default to centered form
                const LC lc = tangente_over(x.lb(), func(x.lb()), deriv(x));
                ce.add_linear_constraint(lc);
                ce.add_linear_constraint(bound_node(CmpOp::LEQ, func(x.ub()).ub()));
            }
            Interval secante_right;
            b = find_secante(x.ub(), 2*x_inflexion-x.ub(), x_inflexion, secante_func, secante_right);
            std::cout <<"secl" << secante_left << std::endl;
            if(b && secante_right.ub() <= x.lb()) {
                const LC lc = line_under(x.lb(), func(x.lb()), x.ub(), func(x.ub()));
                ce.add_linear_constraint(lc);
            } else if(b) {
                const LC lc = tangente_under(secante_right, func(secante_right), deriv(secante_right));
                ce.add_linear_constraint(lc);
                if(secante_right.lb() >= x.lb()) {
                    const LC lc2 = tangente_under(x.lb(), func(x.lb()), deriv(x.lb()));
                    ce.add_linear_constraint(lc2);
                }
                // + middle ?
            } else {
                // default to centered form
                const LC lc = tangente_under(x.ub(), func(x.ub()), deriv(x));
                ce.add_linear_constraint(lc);
                ce.add_linear_constraint(bound_node(CmpOp::GEQ, func(x.lb()).lb()));
            }
            return ce;
        }
}

} // end namespace convex_envelope




class ConvexEnvelopeVector {
public:
    ConvexEnvelopeVector(int n);

    int size() const;
    void resize(int n);
    ConvexEnvelopeVector subvector(int first_index, int last_index) const;
    const ConvexEnvelope& operator[](int i) const;
    ConvexEnvelope& operator[](int i);

private:
    std::vector<ConvexEnvelope> envelopes_;
};

class ConvexEnvelopeMatrix {
public:
    ConvexEnvelopeMatrix(int nb_rows, int nb_cols);

    const ConvexEnvelopeVector& row(int index) const;
    ConvexEnvelopeVector col(int index) const;
    int nb_rows() const;
    ConvexEnvelopeMatrix submatrix(int first_row, int last_row, int first_col, int last_col) const;
    const ConvexEnvelopeVector& operator[](int i) const;
    ConvexEnvelopeVector& operator[](int i);

private:
    std::vector<ConvexEnvelopeVector> rows_;
    int nb_rows_;
    int nb_cols_;
};

} // end namespace ibex


#endif  // __IBEX_CONVEX_ENVELOPE_H__