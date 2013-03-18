/*
 * ibex_Affine2Eval.h
 *
 *  Created on: 7 mars 2013
 *      Author: nininjo
 */

#ifndef IBEX_AFFINE2_EVAL_H_
#define IBEX_AFFINE2_EVAL_H_


#include "ibex_Function.h"
#include "ibex_Affine2MatrixArray.h"
#include "ibex_EmptyBoxException.h"
#include "ibex_FwdAlgorithm.h"
#include "ibex_BwdAlgorithm.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Calculates the gradient of a function.
 */
class Affine2Eval : public FwdAlgorithm, public BwdAlgorithm {

public:
	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	Affine2Domain& eval_affine2(const Function& f, const Array<const Affine2Domain>& d) const;

	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	Affine2Domain& eval_affine2(const Function& f, const Array<Affine2Domain>& d) const;
	/**
	 * \brief Calculate the gradient of f on the box \a box and store the result in \a g.
	 */
	Affine2Domain& eval_affine2(const Function& f, const IntervalVector& box) const;

	void index_fwd(const ExprIndex&, const ExprLabel& x, ExprLabel& y);
	       void vector_fwd(const ExprVector&, const ExprLabel** compL, ExprLabel& y);
	void cst_fwd(const ExprConstant&, ExprLabel& y);
	void symbol_fwd(const ExprSymbol&, ExprLabel& y);
	void apply_fwd(const ExprApply&, ExprLabel** x, ExprLabel& y);
	void add_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void sub_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void div_fwd(const ExprDiv&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void max_fwd(const ExprMax&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void min_fwd(const ExprMin&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void atan2_fwd(const ExprAtan2&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void minus_fwd(const ExprMinus&, const ExprLabel& x, ExprLabel& y);
	void trans_V_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y);
	void trans_M_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y);
	void sign_fwd(const ExprSign&, const ExprLabel& x, ExprLabel& y);
	void abs_fwd(const ExprAbs&, const ExprLabel& x, ExprLabel& y);
	void power_fwd(const ExprPower& p, const ExprLabel& x, ExprLabel& y);
	void sqr_fwd(const ExprSqr&, const ExprLabel& x, ExprLabel& y);
	void sqrt_fwd(const ExprSqrt&, const ExprLabel& x, ExprLabel& y);
	void exp_fwd(const ExprExp&, const ExprLabel& x, ExprLabel& y);
	void log_fwd(const ExprLog&, const ExprLabel& x, ExprLabel& y);
	void cos_fwd(const ExprCos&, const ExprLabel& x, ExprLabel& y);
	void sin_fwd(const ExprSin&, const ExprLabel& x, ExprLabel& y);
	void tan_fwd(const ExprTan&, const ExprLabel& x, ExprLabel& y);
	void cosh_fwd(const ExprCosh&, const ExprLabel& x, ExprLabel& y);
	void sinh_fwd(const ExprSinh&, const ExprLabel& x, ExprLabel& y);
	void tanh_fwd(const ExprTanh&, const ExprLabel& x, ExprLabel& y);
	void acos_fwd(const ExprAcos&, const ExprLabel& x, ExprLabel& y);
	void asin_fwd(const ExprAsin&, const ExprLabel& x, ExprLabel& y);
	void atan_fwd(const ExprAtan&, const ExprLabel& x, ExprLabel& y);
	void acosh_fwd(const ExprAcosh&, const ExprLabel& x, ExprLabel& y);
	void asinh_fwd(const ExprAsinh&, const ExprLabel& x, ExprLabel& y);
	void atanh_fwd(const ExprAtanh&, const ExprLabel& x, ExprLabel& y);

	void add_V_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void add_M_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_SV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_SM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_VV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_MV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_VM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void mul_MM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void sub_V_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);
	void sub_M_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y);


	/**
	 * \brief Project f(x)=y onto x (backward algorithm)
	 */
	void proj_affine2(const Function& f, const Affine2Domain& y, Array<Affine2Domain>& x);

	/**
	 * \brief Project f(x)=y onto x (backward algorithm)
	 */
	void proj_affine2(const Function& f, const Affine2Domain& y, IntervalVector& x);

	/**
	 * \brief Ratio for the contraction of a
	 * matrix-vector / matrix-matrix multiplication.
	 * Set to 0.1.
	 */
	static const double RATIO;

	void index_bwd (const ExprIndex&,   ExprLabel& , const ExprLabel& )   ;
	       void vector_bwd(const ExprVector&,  ExprLabel** compL, const ExprLabel& result);
	void symbol_bwd(const ExprSymbol& , const ExprLabel& )   ;
	void cst_bwd   (const ExprConstant&, const ExprLabel& ) ;
	void apply_bwd (const ExprApply& a, ExprLabel** x, const ExprLabel& y);
	void add_bwd   (const ExprAdd&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void add_V_bwd  (const ExprAdd&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void add_M_bwd  (const ExprAdd&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void mul_bwd    (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void mul_SV_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void mul_SM_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void mul_VV_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void mul_MV_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void mul_VM_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void mul_MM_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void sub_bwd   (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void sub_V_bwd (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void sub_M_bwd (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void div_bwd   (const ExprDiv&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void max_bwd   (const ExprMax&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void min_bwd   (const ExprMin&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y);
	void atan2_bwd (const ExprAtan2& , ExprLabel& x1, ExprLabel& x2, const ExprLabel& y) ;
	void minus_bwd (const ExprMinus& , ExprLabel& x, const ExprLabel& y);
    void trans_V_bwd(const ExprTrans& ,ExprLabel& x, const ExprLabel& y);
    void trans_M_bwd(const ExprTrans& ,ExprLabel& x, const ExprLabel& y);
	void sign_bwd  (const ExprSign& ,  ExprLabel& x, const ExprLabel& y);
	void abs_bwd   (const ExprAbs& ,   ExprLabel& x, const ExprLabel& y);
	void power_bwd (const ExprPower& e, ExprLabel& x, const ExprLabel& y);
	void sqr_bwd   (const ExprSqr& ,   ExprLabel& x, const ExprLabel& y);
	void sqrt_bwd  (const ExprSqrt& ,  ExprLabel& x, const ExprLabel& y);
	void exp_bwd   (const ExprExp& ,   ExprLabel& x, const ExprLabel& y);
	void log_bwd   (const ExprLog& ,   ExprLabel& x, const ExprLabel& y);
	void cos_bwd   (const ExprCos& ,   ExprLabel& x, const ExprLabel& y);
	void sin_bwd   (const ExprSin& ,   ExprLabel& x, const ExprLabel& y);
	void tan_bwd   (const ExprTan& ,   ExprLabel& x, const ExprLabel& y);
	void cosh_bwd  (const ExprCosh& ,  ExprLabel& x, const ExprLabel& y);
	void sinh_bwd  (const ExprSinh& ,  ExprLabel& x, const ExprLabel& y);
	void tanh_bwd  (const ExprTanh& ,  ExprLabel& x, const ExprLabel& y);
	void acos_bwd  (const ExprAcos& ,  ExprLabel& x, const ExprLabel& y);
	void asin_bwd  (const ExprAsin& ,  ExprLabel& x, const ExprLabel& y);
	void atan_bwd  (const ExprAtan& ,  ExprLabel& x, const ExprLabel& y);
	void acosh_bwd (const ExprAcosh& , ExprLabel& x, const ExprLabel& y);
	void asinh_bwd (const ExprAsinh& , ExprLabel& x, const ExprLabel& y);
	void atanh_bwd (const ExprAtanh& , ExprLabel& x, const ExprLabel& y);


protected:
	/**
	 * \brief Run the forward algorithm with input domains.
	 */
	Affine2Domain& eval_affine2(const Function&, ExprLabel** d) const;


	void proj_affine2(const Function& f, const Affine2Domain& y, ExprLabel** x);
};

/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/

inline void Affine2Eval::index_fwd(const ExprIndex& , const ExprLabel& , ExprLabel& ) { /* nothing to do */ }

inline void Affine2Eval::symbol_fwd(const ExprSymbol& , ExprLabel& ) { /* nothing to do */ }

inline void Affine2Eval::cst_fwd(const ExprConstant& c, ExprLabel& y) {
	switch (c.type()) {
	case Dim::SCALAR:      {
		y.af2->i() = Affine2(0,c.get_value());
		y.af2->i().set_actif(false);
		break;
	}
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: {
		for (int i=0; i<c.get_vector_value().size() ; i++ ) {
			y.af2->v()[i] = Affine2(0,c.get_vector_value()[i]);
			y.af2->v()[i].set_actif(false);
		}
		break;
	}
	case Dim::MATRIX: {
		for (int i=0; i<c.get_matrix_value().nb_rows() ; i++ ) {
			for (int j=0 ; j<c.get_matrix_value().nb_cols() ; j++) {
				(y.af2->m()[i])[j] = Affine2(0,c.get_matrix_value()[i][j]);
				y.af2->m()[i][j].set_actif(false);
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
inline void Affine2Eval::apply_fwd(const ExprApply& a, ExprLabel** x, ExprLabel& y)                          { *y.af2 = eval_affine2(a.func,x); }
inline void Affine2Eval::add_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.af2->i()=x1.af2->i()+x2.af2->i(); }
inline void Affine2Eval::mul_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.af2->i()=x1.af2->i()*x2.af2->i(); }
inline void Affine2Eval::sub_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.af2->i()=x1.af2->i()-x2.af2->i(); }
inline void Affine2Eval::div_fwd(const ExprDiv&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.af2->i()=x1.af2->i()/x2.af2->i(); }
inline void Affine2Eval::max_fwd(const ExprMax&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.af2->i() = max(x1.af2->i(),x2.af2->i()); }
inline void Affine2Eval::min_fwd(const ExprMin&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)     { y.af2->i() = min(x1.af2->i(),x2.af2->i()); }
inline void Affine2Eval::atan2_fwd(const ExprAtan2&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y) {  assert(false); /* not yet */ }

inline void Affine2Eval::minus_fwd(const ExprMinus&, const ExprLabel& x, ExprLabel& y)                       { y.af2->i()=-x.af2->i(); }
inline void Affine2Eval::sign_fwd(const ExprSign&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=sign(x.af2->i()); }
inline void Affine2Eval::abs_fwd(const ExprAbs&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=abs(x.af2->i()); }
inline void Affine2Eval::power_fwd(const ExprPower& p, const ExprLabel& x, ExprLabel& y)                     { y.af2->i()=pow(x.af2->i(),p.expon); }
inline void Affine2Eval::sqr_fwd(const ExprSqr&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=sqr(x.af2->i()); }
inline void Affine2Eval::sqrt_fwd(const ExprSqrt&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=sqrt(x.af2->i()); }
inline void Affine2Eval::exp_fwd(const ExprExp&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=exp(x.af2->i()); }
inline void Affine2Eval::log_fwd(const ExprLog&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=log(x.af2->i()); }
inline void Affine2Eval::cos_fwd(const ExprCos&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=cos(x.af2->i()); }
inline void Affine2Eval::sin_fwd(const ExprSin&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=sin(x.af2->i()); }
inline void Affine2Eval::tan_fwd(const ExprTan&, const ExprLabel& x, ExprLabel& y)                           { y.af2->i()=tan(x.af2->i()); }
inline void Affine2Eval::cosh_fwd(const ExprCosh&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=cosh(x.af2->i()); }
inline void Affine2Eval::sinh_fwd(const ExprSinh&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=sinh(x.af2->i()); }
inline void Affine2Eval::tanh_fwd(const ExprTanh&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=tanh(x.af2->i()); }
inline void Affine2Eval::acos_fwd(const ExprAcos&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=acos(x.af2->i()); }
inline void Affine2Eval::asin_fwd(const ExprAsin&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=asin(x.af2->i()); }
inline void Affine2Eval::atan_fwd(const ExprAtan&, const ExprLabel& x, ExprLabel& y)                         { y.af2->i()=atan(x.af2->i()); }
inline void Affine2Eval::acosh_fwd(const ExprAcosh&, const ExprLabel& x, ExprLabel& y)                       { assert(false); /* not yet */ }
inline void Affine2Eval::asinh_fwd(const ExprAsinh&, const ExprLabel& x, ExprLabel& y)                       { assert(false); /* not yet */ }
inline void Affine2Eval::atanh_fwd(const ExprAtanh&, const ExprLabel& x, ExprLabel& y)                       { assert(false); /* not yet */ }


inline void Affine2Eval::trans_V_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y)                     { y.af2->v()=x.af2->v(); }
inline void Affine2Eval::trans_M_fwd(const ExprTrans&, const ExprLabel& x, ExprLabel& y)                     { y.af2->m()=x.af2->m().transpose(); }
inline void Affine2Eval::add_V_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   { y.af2->v()=x1.af2->v()+x2.af2->v(); }
inline void Affine2Eval::add_M_fwd(const ExprAdd&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   { y.af2->m()=x1.af2->m()+x2.af2->m(); }
inline void Affine2Eval::mul_SV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { y.af2->v()=x1.af2->i()*x2.af2->v(); }
inline void Affine2Eval::mul_SM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { y.af2->m()=x1.af2->i()*x2.af2->m(); }
inline void Affine2Eval::mul_VV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { y.af2->i()=x1.af2->v()*x2.af2->v(); }
inline void Affine2Eval::mul_MV_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { y.af2->v()=x1.af2->m()*x2.af2->v(); }
inline void Affine2Eval::mul_VM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { y.af2->v()=x1.af2->v()*x2.af2->m(); }
inline void Affine2Eval::mul_MM_fwd(const ExprMul&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)  { y.af2->m()=x1.af2->m()*x2.af2->m(); }
inline void Affine2Eval::sub_V_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   { y.af2->v()=x1.af2->v()-x2.af2->v(); }
inline void Affine2Eval::sub_M_fwd(const ExprSub&, const ExprLabel& x1, const ExprLabel& x2, ExprLabel& y)   { y.af2->m()=x1.af2->m()-x2.af2->m(); }






inline void Affine2Eval::index_bwd (const ExprIndex&,   ExprLabel& , const ExprLabel& )            { /* nothing to do */ }
inline void Affine2Eval::symbol_bwd(const ExprSymbol& , const ExprLabel& )                             { /* nothing to do */ }
inline void Affine2Eval::cst_bwd   (const ExprConstant&, const ExprLabel& )                                  { /* nothing to do */ }
inline void Affine2Eval::apply_bwd (const ExprApply& a, ExprLabel** x, const ExprLabel& y)                   { proj_affine2(a.func,*y.af2,x); }
inline void Affine2Eval::add_bwd   (const ExprAdd&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_add(y.af2->i(),x1.af2->i(),x2.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::add_V_bwd  (const ExprAdd&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_add(y.af2->v(),x1.af2->v(),x2.af2->v()))) throw EmptyBoxException();  }
inline void Affine2Eval::add_M_bwd  (const ExprAdd&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_add(y.af2->m(),x1.af2->m(),x2.af2->m()))) throw EmptyBoxException();  }
inline void Affine2Eval::mul_bwd    (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.af2->i(),x1.af2->i(),x2.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::mul_SV_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.af2->v(),x1.af2->i(),x2.af2->v()))) throw EmptyBoxException();  }
inline void Affine2Eval::mul_SM_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.af2->m(),x1.af2->i(),x2.af2->m()))) throw EmptyBoxException();  }
inline void Affine2Eval::mul_VV_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.af2->i(),x1.af2->v(),x2.af2->v()))) throw EmptyBoxException();  }
inline void Affine2Eval::mul_MV_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.af2->v(),x1.af2->m(),x2.af2->v(), RATIO))) throw EmptyBoxException();  }
inline void Affine2Eval::mul_VM_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.af2->v(),x1.af2->v(),x2.af2->m(), RATIO))) throw EmptyBoxException();  }
inline void Affine2Eval::mul_MM_bwd (const ExprMul&,    ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_mul(y.af2->m(),x1.af2->m(),x2.af2->m(), RATIO))) throw EmptyBoxException();  }
inline void Affine2Eval::sub_bwd   (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_sub(y.af2->i(),x1.af2->i(),x2.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::sub_V_bwd (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_sub(y.af2->v(),x1.af2->v(),x2.af2->v()))) throw EmptyBoxException();  }
inline void Affine2Eval::sub_M_bwd (const ExprSub&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_sub(y.af2->m(),x1.af2->m(),x2.af2->m()))) throw EmptyBoxException();  }
inline void Affine2Eval::div_bwd   (const ExprDiv&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_div(y.af2->i(),x1.af2->i(),x2.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::max_bwd   (const ExprMax&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_max(y.af2->i(),x1.af2->i(),x2.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::min_bwd   (const ExprMin&,     ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { if (!(proj_min(y.af2->i(),x1.af2->i(),x2.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::atan2_bwd (const ExprAtan2& , ExprLabel& x1, ExprLabel& x2, const ExprLabel& y)    { not_implemented("Inner projection of \"atan2\"");  }
inline void Affine2Eval::minus_bwd (const ExprMinus& , ExprLabel& x, const ExprLabel& y) { x.af2->i() =(x.af2->i() &(-y.af2->i())); if ((x.af2->i()).is_empty()) { throw EmptyBoxException();}  }
inline void Affine2Eval::trans_V_bwd(const ExprTrans& ,ExprLabel& x, const ExprLabel& y) { x.af2->v() =(x.af2->v() &( y.af2->v())); if ((x.af2->i()).is_empty()) { throw EmptyBoxException();}  }
inline void Affine2Eval::trans_M_bwd(const ExprTrans& ,ExprLabel& x, const ExprLabel& y) { x.af2->m() =(x.af2->m() & (y.af2->m().transpose())); if (( x.af2->m()).is_empty()) { throw EmptyBoxException();}}
inline void Affine2Eval::sign_bwd  (const ExprSign& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_sign(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::abs_bwd   (const ExprAbs& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_abs(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::power_bwd (const ExprPower& e, ExprLabel& x, const ExprLabel& y)                    { if (!(proj_pow(y.af2->i(),e.expon, x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::sqr_bwd   (const ExprSqr& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_sqr(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::sqrt_bwd  (const ExprSqrt& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_sqrt(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::exp_bwd   (const ExprExp& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_exp(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::log_bwd   (const ExprLog& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_log(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::cos_bwd   (const ExprCos& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_cos(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::sin_bwd   (const ExprSin& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_sin(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::tan_bwd   (const ExprTan& ,   ExprLabel& x, const ExprLabel& y)                    { if (!(proj_tan(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::cosh_bwd  (const ExprCosh& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_cosh(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::sinh_bwd  (const ExprSinh& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_sinh(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::tanh_bwd  (const ExprTanh& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_tanh(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::acos_bwd  (const ExprAcos& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_acos(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::asin_bwd  (const ExprAsin& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_asin(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::atan_bwd  (const ExprAtan& ,  ExprLabel& x, const ExprLabel& y)                    { if (!(proj_atan(y.af2->i(),x.af2->i()))) throw EmptyBoxException();  }
inline void Affine2Eval::acosh_bwd (const ExprAcosh& , ExprLabel& x, const ExprLabel& y)                    { not_implemented("Affine2 projection of \"acosh\" ");  }
inline void Affine2Eval::asinh_bwd (const ExprAsinh& , ExprLabel& x, const ExprLabel& y)                    { not_implemented("Affine2 projection of \"asinh\" ");  }
inline void Affine2Eval::atanh_bwd (const ExprAtanh& , ExprLabel& x, const ExprLabel& y)                    { not_implemented("Affine2 projection of \"atanh\" "); }












} // namespace ibex


#endif /* IBEX_AFFINE2_EVAL_H_ */
