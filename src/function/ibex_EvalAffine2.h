/*
 * ibex_EvalAffine2.h
 *
 *  Created on: 7 mars 2013
 *      Author: nininjo
 */

#ifndef IBEX_EVALAFFINE2_H_
#define IBEX_EVALAFFINE2_H_


#include "ibex_Function.h"
#include "ibex_Affine2.h"
#include "ibex_BwdAlgorithm.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Calculates the gradient of a function.
 */
class EvalAffine2 : public FwdAlgorithm {

public:
	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	DomainAffine2& eval_affine2(const Function& f, const Array<const DomainAffine2>& d) const;

	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	DomainAffine2& eval_affine2(const Function& f, const Array<DomainAffine2>& d) const;
	/**
	 * \brief Calculate the gradient of f on the box \a box and store the result in \a g.
	 */
	DomainAffine2& eval_affine2(const Function& f, const IntervalVector& box) const;

	inline void index_fwd(const ExprIndex&, const ExprLabel& x, ExprLabel& y);
	       void vector_fwd(const ExprVector&, const ExprLabel** compL, ExprLabel& y);
	inline void cst_fwd(const ExprConstant&, ExprLabel& y);
	inline void symbol_fwd(const ExprSymbol&, ExprLabel& y);
	inline void apply_fwd(const ExprApply&, ExprLabel** x, ExprLabel& y);
	inline void add_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void sub_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void div_fwd(const ExprDiv&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void max_fwd(const ExprMax&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void min_fwd(const ExprMin&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void atan2_fwd(const ExprAtan2&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void minus_fwd(const ExprMinus&, const ExprLabel& x, ExprLabel& y);
	inline void trans_V_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y);
	inline void trans_M_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y);
	inline void sign_fwd(const ExprSign&, const ExprLabel& x, ExprLabel& y);
	inline void abs_fwd(const ExprAbs&, const ExprLabel& x, ExprLabel& y);
	inline void power_fwd(const ExprPower& p, const ExprLabel& x, ExprLabel& y);
	inline void sqr_fwd(const ExprSqr&, const ExprLabel& x, ExprLabel& y);
	inline void sqrt_fwd(const ExprSqrt&, const ExprLabel& x, ExprLabel& y);
	inline void exp_fwd(const ExprExp&, const ExprLabel& x, ExprLabel& y);
	inline void log_fwd(const ExprLog&, const ExprLabel& x, ExprLabel& y);
	inline void cos_fwd(const ExprCos&, const ExprLabel& x, ExprLabel& y);
	inline void sin_fwd(const ExprSin&, const ExprLabel& x, ExprLabel& y);
	inline void tan_fwd(const ExprTan&, const ExprLabel& x, ExprLabel& y);
	inline void cosh_fwd(const ExprCosh&, const ExprLabel& x, ExprLabel& y);
	inline void sinh_fwd(const ExprSinh&, const ExprLabel& x, ExprLabel& y);
	inline void tanh_fwd(const ExprTanh&, const ExprLabel& x, ExprLabel& y);
	inline void acos_fwd(const ExprAcos&, const ExprLabel& x, ExprLabel& y);
	inline void asin_fwd(const ExprAsin&, const ExprLabel& x, ExprLabel& y);
	inline void atan_fwd(const ExprAtan&, const ExprLabel& x, ExprLabel& y);
	inline void acosh_fwd(const ExprAcosh&, const ExprLabel& x, ExprLabel& y);
	inline void asinh_fwd(const ExprAsinh&, const ExprLabel& x, ExprLabel& y);
	inline void atanh_fwd(const ExprAtanh&, const ExprLabel& x, ExprLabel& y);

	inline void add_V_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void add_M_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_SV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_SM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_VV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_MV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_VM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void mul_MM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void sub_V_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	inline void sub_M_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);

//protected:
	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	DomainAffine2& eval_affine2(const Function&, ExprLabel** d) const;
};

/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/

inline void EvalAffine2::index_fwd(const ExprIndex& , const ExprLabel& , ExprLabel& ) { /* nothing to do */ }

inline void EvalAffine2::symbol_fwd(const ExprSymbol& , ExprLabel& ) { /* nothing to do */ }

inline void EvalAffine2::cst_fwd(const ExprConstant& c, ExprLabel& y) {
	switch (c.type()) {
	case Dim::SCALAR:      {
		y.af2->i() = Affine2(0,c.get_value());
		y.af2->i().setActif(false);
		break;
	}
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: {
		for (int i=0; i<c.get_vector_value().size() ; i++ ) {
			y.af2->v()[i] = Affine2(0,c.get_vector_value()[i]);
			y.af2->v()[i].setActif(false);
		}
		break;
	}
	case Dim::MATRIX: {
		for (int i=0; i<c.get_matrix_value().nb_rows() ; i++ ) {
			for (int j=0 ; j<c.get_matrix_value().nb_cols() ; j++) {
				(y.af2->m()[i])[j] = Affine2(0,c.get_matrix_value()[i][j]);
				y.af2->m()[i][j].setActif(false);
			}
		}
		break;
	}
	case Dim::MATRIX_ARRAY:  {
		assert(false); /* impossible */
		break;
	}
	}
}
inline void EvalAffine2::apply_fwd(const ExprApply& a, ExprLabel** x, ExprLabel& y)                          { *y.af2 = eval_affine2(a.func,x); }
inline void EvalAffine2::add_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.af2->i()=x1.af2->i()+x2.af2->i(); }
inline void EvalAffine2::mul_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.af2->i()=x1.af2->i()*x2.af2->i(); }
inline void EvalAffine2::sub_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.af2->i()=x1.af2->i()-x2.af2->i(); }
inline void EvalAffine2::div_fwd(const ExprDiv&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.af2->i()=x1.af2->i()/x2.af2->i(); }
inline void EvalAffine2::max_fwd(const ExprMax&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.af2->i()=max(x1.af2->i(),x2.af2->i()); }
inline void EvalAffine2::min_fwd(const ExprMin&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.af2->i()=min(x1.af2->i(),x2.af2->i()); }
inline void EvalAffine2::atan2_fwd(const ExprAtan2&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y) {  assert(false); /* not yet */ }

inline void EvalAffine2::minus_fwd(const ExprMinus&, const ExprLabel& x, ExprLabel& y)                       { y.af2->i()=-x.af2->i(); }
inline void EvalAffine2::sign_fwd(const ExprSign&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=sign(x.af2->i()); }
inline void EvalAffine2::abs_fwd(const ExprAbs&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=abs(x.af2->i()); }
inline void EvalAffine2::power_fwd(const ExprPower& p, const ExprLabel& x, ExprLabel& y)                     { y.af2->i()=pow(x.af2->i(),p.expon); }
inline void EvalAffine2::sqr_fwd(const ExprSqr&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=sqr(x.af2->i()); }
inline void EvalAffine2::sqrt_fwd(const ExprSqrt&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=sqrt(x.af2->i()); }
inline void EvalAffine2::exp_fwd(const ExprExp&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=exp(x.af2->i()); }
inline void EvalAffine2::log_fwd(const ExprLog&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=log(x.af2->i()); }
inline void EvalAffine2::cos_fwd(const ExprCos&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=cos(x.af2->i()); }
inline void EvalAffine2::sin_fwd(const ExprSin&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=sin(x.af2->i()); }
inline void EvalAffine2::tan_fwd(const ExprTan&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=tan(x.af2->i()); }
inline void EvalAffine2::cosh_fwd(const ExprCosh&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=cosh(x.af2->i()); }
inline void EvalAffine2::sinh_fwd(const ExprSinh&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=sinh(x.af2->i()); }
inline void EvalAffine2::tanh_fwd(const ExprTanh&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=tanh(x.af2->i()); }
inline void EvalAffine2::acos_fwd(const ExprAcos&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=acos(x.af2->i()); }
inline void EvalAffine2::asin_fwd(const ExprAsin&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=asin(x.af2->i()); }
inline void EvalAffine2::atan_fwd(const ExprAtan&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=atan(x.af2->i()); }
inline void EvalAffine2::acosh_fwd(const ExprAcosh&, const ExprLabel& x, ExprLabel& y)                       { assert(false); /* not yet */ }
inline void EvalAffine2::asinh_fwd(const ExprAsinh&, const ExprLabel& x, ExprLabel& y)                       { assert(false); /* not yet */ }
inline void EvalAffine2::atanh_fwd(const ExprAtanh&, const ExprLabel& x, ExprLabel& y)                       { assert(false); /* not yet */ }

inline void EvalAffine2::trans_V_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y)                     { assert(false); /* impossible */ }
inline void EvalAffine2::trans_M_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y)                     { assert(false); /* impossible */ }
inline void EvalAffine2::add_V_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   { assert(false); /* impossible */ }
inline void EvalAffine2::add_M_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   { assert(false); /* impossible */ }
inline void EvalAffine2::mul_SV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { assert(false); /* impossible */ }
inline void EvalAffine2::mul_SM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { assert(false); /* impossible */ }
inline void EvalAffine2::mul_VV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { assert(false); /* impossible */ }
inline void EvalAffine2::mul_MV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { assert(false); /* impossible */ }
inline void EvalAffine2::mul_VM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { assert(false); /* impossible */ }
inline void EvalAffine2::mul_MM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { assert(false); /* impossible */ }
inline void EvalAffine2::sub_V_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   { assert(false); /* impossible */ }
inline void EvalAffine2::sub_M_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   { assert(false); /* impossible */ }

} // namespace ibex


#endif /* IBEX_EVALAFFINE2_H_ */
