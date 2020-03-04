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
    Interval find_secante(double x_start, double search_lb, double search_ub, const Function& f);
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