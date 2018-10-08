//============================================================================
//                                  I B E X                                   
// File        : ibex_InHC4Revise.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 12, 2012
//============================================================================

#ifndef __IBEX_IN_HC4_REVISE_H__
#define __IBEX_IN_HC4_REVISE_H__

#include "ibex_Eval.h"
#include "ibex_Exception.h"
#include "ibex_InnerArith.h"

namespace ibex {

class InHC4Revise : public BwdAlgorithm {
public:
	InHC4Revise(Eval& e);

	void iproj(const Domain& y, IntervalVector& x);

	void iproj(const Domain& y, IntervalVector& x, const IntervalVector& xin);

	Function& f;

	Eval& eval;
	ExprDomain& d;

	Eval p_eval;
	ExprDomain& p;

protected:
	/**
	 * Class used internally to interrupt the
	 * backward procedure when an empty domain occurs.
	 */
	class EmptyBoxException { };

	void iproj(const Domain& y, Array<Domain>& x, const Array<Domain>& xin);

public: // because called from CompiledFunction

	inline void symbol_bwd (int)                    { /* nothing to do */ }
	inline void cst_bwd    (int y)                  { /* TODO: improve this. */ if (d[y]!=((const ExprConstant&) f.nodes[y]).get()) throw EmptyBoxException(); }
	inline void idx_bwd    (int , int)              { /* nothing to do */ }
	       void idx_cp_bwd (int , int);
	       void vector_bwd (int* , int)             { not_implemented("Inner projection of \"vector\""); }
	inline void apply_bwd  (int* x, int y);
	inline void chi_bwd    (int, int, int, int)     { not_implemented("Inner projection of \"chi\""); }
	inline void add_bwd    (int x1, int x2, int y)  { if (!ibwd_add(d[y].i(),d[x1].i(),d[x2].i(),p[x1].i(),p[x2].i())) throw EmptyBoxException(); }
	inline void gen2_bwd   (int , int , int)        { not_implemented("Inner projection of binary generic operator"); }
	inline void add_V_bwd  (int , int , int)        { not_implemented("Inner projection of \"add_V\""); }
	inline void add_M_bwd  (int , int , int)        { not_implemented("Inner projection of \"add_M\""); }
	inline void mul_bwd    (int x1, int x2, int y)  { if (!ibwd_mul(d[y].i(),d[x1].i(),d[x2].i(),p[x1].i(),p[x2].i())) throw EmptyBoxException(); }
	inline void mul_SV_bwd (int , int , int)        { not_implemented("Inner projection of \"mul_SV\""); }
	inline void mul_SM_bwd (int , int , int)        { not_implemented("Inner projection of \"mul_SM\""); }
	inline void mul_VV_bwd (int , int , int)        { not_implemented("Inner projection of \"mul_VV\""); }
	inline void mul_MV_bwd (int , int , int)        { not_implemented("Inner projection of \"mul_MV\""); }
	inline void mul_VM_bwd (int , int , int)        { not_implemented("Inner projection of \"mul_VM\""); }
	inline void mul_MM_bwd (int , int , int)        { not_implemented("Inner projection of \"mul_MM\""); }
	inline void sub_bwd    (int x1, int x2, int y)  { if (!ibwd_sub(d[y].i(),d[x1].i(),d[x2].i(),p[x1].i(),p[x2].i())) throw EmptyBoxException(); }
	inline void sub_V_bwd  (int , int, int)         { not_implemented("Inner projection of \"sub_V\""); }
	inline void sub_M_bwd  (int , int, int)         { not_implemented("Inner projection of \"sub_M\""); }
	inline void div_bwd    (int x1, int x2, int y)  { if (!ibwd_div(d[y].i(),d[x1].i(),d[x2].i(),p[x1].i(),p[x2].i())) throw EmptyBoxException(); }
	inline void max_bwd    (int x1, int x2, int y)  { if (!ibwd_max(d[y].i(),d[x1].i(),d[x2].i(),p[x1].i(),p[x2].i())) throw EmptyBoxException(); }
	inline void min_bwd    (int x1, int x2, int y)  { if (!ibwd_min(d[y].i(),d[x1].i(),d[x2].i(),p[x1].i(),p[x2].i())) throw EmptyBoxException(); }
	inline void atan2_bwd  (int , int , int)        { not_implemented("Inner projection of \"atan2\""); }
	inline void gen1_bwd   (int , int)              { not_implemented("Inner projection of generic unary operator"); }
	inline void minus_bwd  (int x, int y)           { if (!ibwd_minus(d[y].i(),d[x].i())) throw EmptyBoxException(); }
	inline void minus_V_bwd(int x, int y)           { not_implemented("Inner projection of \"minus_V\""); }
	inline void minus_M_bwd(int x, int y)           { not_implemented("Inner projection of \"minus_M\""); }
    inline void trans_V_bwd(int , int)              { not_implemented("Inner projection of \"transpose\""); }
    inline void trans_M_bwd(int , int)              { not_implemented("Inner projection of \"transpose\""); }
	inline void sign_bwd   (int , int)              { not_implemented("Inner projection of \"sign\""); }
	inline void abs_bwd    (int x, int y)           { if (!ibwd_abs(d[y].i(),d[x].i())) throw EmptyBoxException(); }
	inline void power_bwd  (int x, int y, int expo) { if (!ibwd_pow(d[y].i(),d[x].i(),expo,p[x].i())) throw EmptyBoxException(); }
	inline void sqr_bwd    (int x, int y)           { if (!ibwd_sqr(d[y].i(),d[x].i(),p[x].i())) throw EmptyBoxException(); }
	inline void sqrt_bwd   (int x, int y)           { if (!ibwd_sqrt(d[y].i(),d[x].i())) throw EmptyBoxException(); }
	inline void exp_bwd    (int x, int y)           { if (!ibwd_exp(d[y].i(),d[x].i())) throw EmptyBoxException(); }
	inline void log_bwd    (int x, int y)           { if (!ibwd_log(d[y].i(),d[x].i())) throw EmptyBoxException(); }
	inline void cos_bwd    (int x, int y)           { if (!ibwd_cos(d[y].i(),d[x].i(),p[x].i())) throw EmptyBoxException(); }
	inline void sin_bwd    (int x, int y)           { if (!ibwd_sin(d[y].i(),d[x].i(),p[x].i())) throw EmptyBoxException();}
	inline void tan_bwd    (int x, int y)           { if (!ibwd_tan(d[y].i(),d[x].i(),p[x].i())) throw EmptyBoxException(); }
	inline void cosh_bwd   (int , int)              { not_implemented("Inner projection of \"cosh\""); }
	inline void sinh_bwd   (int , int)              { not_implemented("Inner projection of \"sinh\""); }
	inline void tanh_bwd   (int , int)              { not_implemented("Inner projection of \"tanh\""); }
	inline void acos_bwd   (int , int)              { not_implemented("Inner projection of \"acos\""); }
	inline void asin_bwd   (int , int)              { not_implemented("Inner projection of \"asin\""); }
	inline void atan_bwd   (int , int)              { not_implemented("Inner projection of \"atan\""); }
	inline void acosh_bwd  (int , int)              { not_implemented("Inner projection of \"acosh\""); }
	inline void asinh_bwd  (int , int)              { not_implemented("Inner projection of \"asinh\""); }
	inline void atanh_bwd  (int , int)              { not_implemented("Inner projection of \"atanh\""); }
};

} // end namespace ibex

#endif // __IBEX_IN_HC4_REVISE_H__
