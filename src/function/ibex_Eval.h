/* ============================================================================
 * I B E X - Function basic evaluation
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 14, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_EVAL_H__
#define __IBEX_EVAL_H__

#include <iostream>

#include "ibex_ExprDomain.h"

namespace ibex {

class Function;

/**
 * \ingroup symbolic
 *
 * \brief Function evaluator.
 *
 */
class Eval : public FwdAlgorithm {

public:
	/**
	 * \brief Build the evaluator for the function f.
	 */
	Eval(Function &f);

	/**
	 * \brief Delete this.
	 */
	~Eval();

	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	Domain& eval(const Array<const Domain>& d);

	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	Domain& eval(const Array<Domain>& d);

	/**
	 * \brief Run the forward algorithm with an input box.
	 */
	Domain& eval(const IntervalVector& box);

	/**
	 * \brief Evaluate a subset of components.
	 *
	 * (Specific for vector-valued functions).
	 *
	 * \pre components must be non empty and contain indices in
	 *      [0,f.image_dim()-1].
	 */
	IntervalVector eval(const IntervalVector& box, const BitSet& components);

protected:
	/**
	 * Class used internally to interrupt the forward procedure
	 * when an empty domain occurs (<=> the input box is outside
	 * the definition domain of the function).
	 */
	class EmptyBoxException { };

public: // because called from CompiledFunction

	       void vector_fwd (int* x, int y);
	       void apply_fwd  (int* x, int y);
	inline void idx_fwd    (int x, int y);
	inline void idx_cp_fwd (int x, int y);
	inline void symbol_fwd (int y);
	inline void cst_fwd    (int y);
	inline void chi_fwd    (int x1, int x2, int x3, int y);
	inline void gen2_fwd   (int x, int x2, int y);
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

	Function& f;
	ExprDomain d;
	Agenda** fwd_agenda; // one agenda for each component
	Agenda** bwd_agenda; // one agenda for each component
};

/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/

inline void Eval::idx_fwd(int, int) { /* nothing to do */ }

inline void Eval::symbol_fwd(int) { /* nothing to do */ }

inline void Eval::cst_fwd(int y) {
	const ExprConstant& c = (const ExprConstant&) f.node(y);
	switch (c.type()) {
	case Dim::SCALAR:       d[y].i() = c.get_value();         break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d[y].v() = c.get_vector_value();  break;
	case Dim::MATRIX:       d[y].m() = c.get_matrix_value();  break;
	}
}

inline void Eval::chi_fwd(int x1, int x2, int x3, int y) { d[y].i() = chi(d[x1].i(),d[x2].i(),d[x3].i()); }
inline void Eval::add_fwd(int x1, int x2, int y)   { d[y].i()=d[x1].i()+d[x2].i(); }
inline void Eval::mul_fwd(int x1, int x2, int y)   { d[y].i()=d[x1].i()*d[x2].i(); }
inline void Eval::sub_fwd(int x1, int x2, int y)   { d[y].i()=d[x1].i()-d[x2].i(); }
inline void Eval::div_fwd(int x1, int x2, int y)   { d[y].i()=d[x1].i()/d[x2].i(); }
inline void Eval::max_fwd(int x1, int x2, int y)   { d[y].i()=max(d[x1].i(),d[x2].i()); }
inline void Eval::min_fwd(int x1, int x2, int y)   { d[y].i()=min(d[x1].i(),d[x2].i()); }
inline void Eval::atan2_fwd(int x1, int x2, int y) { d[y].i()=atan2(d[x1].i(),d[x2].i()); }

inline void Eval::minus_fwd(int x, int y)          { d[y].i()=-d[x].i(); }
inline void Eval::minus_V_fwd(int x, int y)        { d[y].v()=-d[x].v(); }
inline void Eval::minus_M_fwd(int x, int y)        { d[y].m()=-d[x].m(); }
inline void Eval::sign_fwd(int x, int y)           { d[y].i()=sign(d[x].i()); }
inline void Eval::abs_fwd(int x, int y)            { d[y].i()=abs(d[x].i()); }
inline void Eval::power_fwd(int x, int y, int p)   { d[y].i()=pow(d[x].i(),p); }
inline void Eval::sqr_fwd(int x, int y)            { d[y].i()=sqr(d[x].i()); }
inline void Eval::sqrt_fwd(int x, int y)           { if ((d[y].i()=sqrt(d[x].i())).is_empty()) throw EmptyBoxException(); }
inline void Eval::exp_fwd(int x, int y)            { d[y].i()=exp(d[x].i()); }
inline void Eval::log_fwd(int x, int y)            { if ((d[y].i()=log(d[x].i())).is_empty()) throw EmptyBoxException(); }
inline void Eval::cos_fwd(int x, int y)            { d[y].i()=cos(d[x].i()); }
inline void Eval::sin_fwd(int x, int y)            { d[y].i()=sin(d[x].i()); }
inline void Eval::tan_fwd(int x, int y)            { if ((d[y].i()=tan(d[x].i())).is_empty()) throw EmptyBoxException(); }
inline void Eval::cosh_fwd(int x, int y)           { d[y].i()=cosh(d[x].i()); }
inline void Eval::sinh_fwd(int x, int y)           { d[y].i()=sinh(d[x].i()); }
inline void Eval::tanh_fwd(int x, int y)           { d[y].i()=tanh(d[x].i()); }
inline void Eval::acos_fwd(int x, int y)           { if ((d[y].i()=acos(d[x].i())).is_empty()) throw EmptyBoxException(); }
inline void Eval::asin_fwd(int x, int y)           { if ((d[y].i()=asin(d[x].i())).is_empty()) throw EmptyBoxException(); }
inline void Eval::atan_fwd(int x, int y)           { d[y].i()=atan(d[x].i()); }
inline void Eval::acosh_fwd(int x, int y)          { if ((d[y].i()=acosh(d[x].i())).is_empty()) throw EmptyBoxException(); }
inline void Eval::asinh_fwd(int x, int y)          { d[y].i()=asinh(d[x].i()); }
inline void Eval::atanh_fwd(int x, int y)          { if ((d[y].i()=atanh(d[x].i())).is_empty()) throw EmptyBoxException(); }

inline void Eval::trans_V_fwd(int x, int y)        { d[y].v()=d[x].v(); }
inline void Eval::trans_M_fwd(int x, int y)        { d[y].m()=d[x].m().transpose(); }
inline void Eval::add_V_fwd(int x1, int x2, int y) { d[y].v()=d[x1].v()+d[x2].v(); }
inline void Eval::add_M_fwd(int x1, int x2, int y) { d[y].m()=d[x1].m()+d[x2].m(); }
inline void Eval::mul_SV_fwd(int x1, int x2, int y){ d[y].v()=d[x1].i()*d[x2].v(); }
inline void Eval::mul_SM_fwd(int x1, int x2, int y){ d[y].m()=d[x1].i()*d[x2].m(); }
inline void Eval::mul_VV_fwd(int x1, int x2, int y){ d[y].i()=d[x1].v()*d[x2].v(); }
inline void Eval::mul_MV_fwd(int x1, int x2, int y){ d[y].v()=d[x1].m()*d[x2].v(); }
inline void Eval::mul_VM_fwd(int x1, int x2, int y){ d[y].v()=d[x1].v()*d[x2].m(); }
inline void Eval::mul_MM_fwd(int x1, int x2, int y){ d[y].m()=d[x1].m()*d[x2].m(); }
inline void Eval::sub_V_fwd(int x1, int x2, int y) { d[y].v()=d[x1].v()-d[x2].v(); }
inline void Eval::sub_M_fwd(int x1, int x2, int y) { d[y].m()=d[x1].m()-d[x2].m(); }

} // namespace ibex

#endif // __IBEX_EVAL_H__
