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

#ifndef _IBEX_EVAL_H_
#define _IBEX_EVAL_H_

#include "ibex_CompiledFunction.h"
#include "ibex_IntervalMatrixArray.h"
#include "ibex_Dim.h"
#include "ibex_Decorator.h"

namespace ibex {

class Eval;

/**
 * \ingroup level1
 * \brief Label for the basic evaluation of a function.
 */
class EvalLabel : public ExprLabel {
public:

	/**
	 * The dimension of the domain
	 */
	const Dim dim;

	/**
	 * \brief Creates a new domain of dimension \a dim.
	 */
	EvalLabel(const Dim& dim) : dim(dim), is_reference(false) {
		switch(dim.type()) {
		case Dim::SCALAR:       domain = new Interval(); break;
		case Dim::ROW_VECTOR:   domain = new IntervalVector(dim.dim3); break;
		case Dim::COL_VECTOR:   domain = new IntervalVector(dim.dim2); break;
		case Dim::MATRIX:       domain = new IntervalMatrix(dim.dim2,dim.dim3); break;
		case Dim::MATRIX_ARRAY: domain = new IntervalMatrixArray(dim.dim1,dim.dim2,dim.dim3); break;
		}
	}

	/**
	 * \brief Creates a reference to an interval.
	 */
	EvalLabel(Interval& itv) : dim(0,0,0), is_reference(true) {
		domain = &itv;
	}

	/**
	 * \brief Creates a reference to an interval vector.
	 */
	EvalLabel(IntervalVector& v, bool in_row) : dim(0,in_row?0:v.size(),in_row?v.size():0), is_reference(true) {
		domain = &v;
	}

	/**
	 * \brief Creates a reference to an interval matrix.
	 */
	EvalLabel(IntervalMatrix& m) : dim(0,m.nb_rows(),m.nb_cols()), is_reference(true) {
		domain = &m;
	}

	/**
	 * \brief Creates a reference to an array of interval matrices.
	 */
	EvalLabel(IntervalMatrixArray& ma) : dim(ma.size(),ma.nb_rows(),ma.nb_cols()), is_reference(true) {
		domain = &ma;
	}

	virtual ~EvalLabel() {
		if (!is_reference)
			switch(dim.type()) {
			case Dim::SCALAR:       delete &i();  break;
			case Dim::ROW_VECTOR:
			case Dim::COL_VECTOR:   delete &v();  break;
			case Dim::MATRIX:       delete &m();  break;
			case Dim::MATRIX_ARRAY: delete &ma(); break;
			}
	}

	/**
	 * Return the domain as an interval.
	 */
	inline Interval& i()        {
		assert(dim.is_scalar());
		return *(Interval*) domain;
	}

	inline IntervalVector& v()  {
		assert(dim.is_vector());
		return *(IntervalVector*) domain;
	}

	inline IntervalMatrix& m()  {
		assert(dim.type()==Dim::MATRIX);
		return *(IntervalMatrix*) domain;
	}

	inline IntervalMatrixArray& ma() {
		assert(dim.type()==Dim::MATRIX_ARRAY);
		return *(IntervalMatrixArray*) domain;
	}

	inline const Interval& i() const  {
		assert(dim.is_scalar());
		return *(Interval*) domain;
	}

	inline const IntervalVector& v() const  {
		assert(dim.is_vector());
		return *(IntervalVector*) domain;
	}

	inline const IntervalMatrix& m() const  {
		assert(dim.type()==Dim::MATRIX);
		return *(IntervalMatrix*) domain;
	}

	inline const IntervalMatrixArray& ma() const {
		assert(dim.type()==Dim::MATRIX_ARRAY);
		return *(IntervalMatrixArray*) domain;
	}

private:
	void* domain;
	bool is_reference;
};

/**
 * \ingroup level1
 * \brief Decorates/undecorates a function for basic evaluation.
 */
class EvalDecorator : public Decorator<EvalLabel> {
public:

	void decorate(const Function& f);

protected:
	/* Visit an expression. */
	virtual void visit(const ExprNode& n);
	/* Visit an indexed expression. */
	virtual void visit(const ExprIndex& idx);
	/* Visit a symbol. */
	virtual void visit(const ExprSymbol& s);
	/* Visit a constant. */
	virtual void visit(const ExprConstant& c);
	/* Visit an n-ary operator. */
	virtual void visit(const ExprNAryOp&);
	/* Visit an unary operator. */
	virtual void visit(const ExprUnaryOp&);
	/* Visit a binary operator. */
	virtual void visit(const ExprBinaryOp&);
	/* Visit a vector of expressions. */
	virtual void visit(const ExprVector& v);
	/* Visit a function application. */
	virtual void visit(const ExprApply&);

};

/**
 * \brief Function evaluator.
 */
class Eval : public FwdAlgorithm<EvalLabel> {

public:
	/**
	 * \brief Build an evaluator for a function f.
	 *
	 * The function f in argument is not compiled.
	 * The evaluator is in charge of decorating/compiling the function.
	 * A reference to the compiled function is stored in the
	 * field \a f and can therefore be used from outside (like
	 * in HC4Revise).
	 */
	Eval(const Function& f);

	/**
	 * \brief Build an evaluator for a function f.
	 *
	 * The function is already compiled with a label type T that
	 * must be a subclass of EvalLabel.
	 */
	Eval(const CompiledFunction<EvalLabel>& f);

	/**
	 * \brief Delete the evaluator.
	 */
	~Eval();

	/**
	 * Run the forward algorithm.
	 */
	EvalLabel& forward(const IntervalVector& box) const;

	/**
	 * Run the forward algorithm and return the
	 * domain of the root node, that must be scalar.
	 */
	Interval eval(const IntervalVector& box) const;

	/**
	 * Run the forward algorithm and return the
	 * domain of the root node, that must be a vector.
	 */
	IntervalVector eval_vector(const IntervalVector& box) const;

	/**
	 * Run the forward algorithm and return the
	 * domain of the root node, that must be a matrix.
	 */
	IntervalMatrix eval_matrix(const IntervalVector& box) const;

	/**
	 * The compiled function.
	 */
	const CompiledFunction<EvalLabel>& f;

protected:

	friend class CompiledFunction<EvalLabel>;
	friend class EvalDecorator;

	inline void index_fwd(const ExprIndex& e, const EvalLabel& x, EvalLabel& y);
	       void vector_fwd(const ExprVector& v, const EvalLabel** compL, EvalLabel& y);
	inline void cst_fwd(const ExprConstant& c, EvalLabel& y);
	inline void symbol_fwd(const ExprSymbol& s, EvalLabel& y);
	       void apply_fwd(const ExprApply& a, const EvalLabel** argL, EvalLabel& y);
	inline void add_fwd(const ExprAdd&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y);
	inline void mul_fwd(const ExprMul&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y);
	inline void sub_fwd(const ExprSub&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y);
	inline void div_fwd(const ExprDiv&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y);
	inline void max_fwd(const ExprMax&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y);
	inline void min_fwd(const ExprMin&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y);
	inline void atan2_fwd(const ExprAtan2&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y);
	inline void minus_fwd(const ExprMinus&, const EvalLabel& x, EvalLabel& y);
	inline void sign_fwd(const ExprSign&, const EvalLabel& x, EvalLabel& y);
	inline void abs_fwd(const ExprAbs&, const EvalLabel& x, EvalLabel& y);
	inline void power_fwd(const ExprPower& p, const EvalLabel& x, EvalLabel& y);
	inline void sqr_fwd(const ExprSqr&, const EvalLabel& x, EvalLabel& y);
	inline void sqrt_fwd(const ExprSqrt&, const EvalLabel& x, EvalLabel& y);
	inline void exp_fwd(const ExprExp&, const EvalLabel& x, EvalLabel& y);
	inline void log_fwd(const ExprLog&, const EvalLabel& x, EvalLabel& y);
	inline void cos_fwd(const ExprCos&, const EvalLabel& x, EvalLabel& y);
	inline void sin_fwd(const ExprSin&, const EvalLabel& x, EvalLabel& y);
	inline void tan_fwd(const ExprTan&, const EvalLabel& x, EvalLabel& y);
	inline void cosh_fwd(const ExprCosh&, const EvalLabel& x, EvalLabel& y);
	inline void sinh_fwd(const ExprSinh&, const EvalLabel& x, EvalLabel& y);
	inline void tanh_fwd(const ExprTanh&, const EvalLabel& x, EvalLabel& y);
	inline void acos_fwd(const ExprAcos&, const EvalLabel& x, EvalLabel& y);
	inline void asin_fwd(const ExprAsin&, const EvalLabel& x, EvalLabel& y);
	inline void atan_fwd(const ExprAtan&, const EvalLabel& x, EvalLabel& y);
	inline void acosh_fwd(const ExprAcosh&, const EvalLabel& x, EvalLabel& y);
	inline void asinh_fwd(const ExprAsinh&, const EvalLabel& x, EvalLabel& y);
	inline void atanh_fwd(const ExprAtanh&, const EvalLabel& x, EvalLabel& y);

	mutable EvalLabel** symbolLabels; // domains of the symbols (created by the decorator)

private:
	void read(const IntervalVector&) const; // load a box into symbolLabels

	bool proper_compiled_func;
};

/**
 * \ingroup level1
 * \brief Evaluation label for function application nodes.
 *
 * An function application node has a specific label that, in addition
 * to the EvalLabel, contains a reference to the evaluator of the function called,
 * as well as the domain that is used to load the arguments domains.
 */
class EvalApplyLabel : public EvalLabel {
public:
	EvalApplyLabel(const Dim& dim, const Function& f) : EvalLabel(dim), fevl(f) { }

	Eval fevl;    //  for each function, there is an associated evaluator
};

/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/

inline EvalLabel& Eval::forward(const IntervalVector& box) const {
	read(box); // load the domains of all the symbols
	return f.forward(*this);
}

inline Interval Eval::eval(const IntervalVector& box) const {
	return forward(box).i();
}

inline IntervalVector Eval::eval_vector(const IntervalVector& box) const {
	return forward(box).v();
}

inline IntervalMatrix Eval::eval_matrix(const IntervalVector& box) const {
	return forward(box).m();
}

inline void Eval::Eval::index_fwd(const ExprIndex& e, const EvalLabel& x, EvalLabel& y) { }

inline void Eval::symbol_fwd(const ExprSymbol& s, EvalLabel& y) { }

inline void Eval::cst_fwd(const ExprConstant& c, EvalLabel& y) {
	switch (c.type()) {
	case Dim::SCALAR:       y.i() = c.get_value();         break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   y.v() = c.get_vector_value();  break;
	case Dim::MATRIX:       y.m() = c.get_matrix_value();  break;
	case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
	}
}
inline void Eval::add_fwd(const ExprAdd&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y)     { y.i()=x1.i()+x2.i(); }
inline void Eval::mul_fwd(const ExprMul&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y)     { y.i()=x1.i()*x2.i(); }
inline void Eval::sub_fwd(const ExprSub&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y)     { y.i()=x1.i()-x2.i(); }
inline void Eval::div_fwd(const ExprDiv&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y)     { y.i()=x1.i()/x2.i(); }
inline void Eval::max_fwd(const ExprMax&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y)     { y.i()=max(x1.i(),x2.i()); }
inline void Eval::min_fwd(const ExprMin&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y)     { y.i()=min(x1.i(),x2.i()); }
inline void Eval::atan2_fwd(const ExprAtan2&, const EvalLabel& x1, const EvalLabel& x2, EvalLabel& y) { y.i()=atan2(x1.i(),x2.i()); }

inline void Eval::minus_fwd(const ExprMinus&, const EvalLabel& x, EvalLabel& y)                       { y.i()=-x.i(); }
inline void Eval::sign_fwd(const ExprSign&, const EvalLabel& x, EvalLabel& y)                         { y.i()=sign(x.i()); }
inline void Eval::abs_fwd(const ExprAbs&, const EvalLabel& x, EvalLabel& y)                           { y.i()=abs(x.i()); }
inline void Eval::power_fwd(const ExprPower& p, const EvalLabel& x, EvalLabel& y)                     { y.i()=pow(x.i(),p.expon); }
inline void Eval::sqr_fwd(const ExprSqr&, const EvalLabel& x, EvalLabel& y)                           { y.i()=sqr(x.i()); }
inline void Eval::sqrt_fwd(const ExprSqrt&, const EvalLabel& x, EvalLabel& y)                         { y.i()=sqrt(x.i()); }
inline void Eval::exp_fwd(const ExprExp&, const EvalLabel& x, EvalLabel& y)                           { y.i()=exp(x.i()); }
inline void Eval::log_fwd(const ExprLog&, const EvalLabel& x, EvalLabel& y)                           { y.i()=log(x.i()); }
inline void Eval::cos_fwd(const ExprCos&, const EvalLabel& x, EvalLabel& y)                           { y.i()=cos(x.i()); }
inline void Eval::sin_fwd(const ExprSin&, const EvalLabel& x, EvalLabel& y)                           { y.i()=sin(x.i()); }
inline void Eval::tan_fwd(const ExprTan&, const EvalLabel& x, EvalLabel& y)                           { y.i()=tan(x.i()); }
inline void Eval::cosh_fwd(const ExprCosh&, const EvalLabel& x, EvalLabel& y)                         { y.i()=cosh(x.i()); }
inline void Eval::sinh_fwd(const ExprSinh&, const EvalLabel& x, EvalLabel& y)                         { y.i()=sinh(x.i()); }
inline void Eval::tanh_fwd(const ExprTanh&, const EvalLabel& x, EvalLabel& y)                         { y.i()=tanh(x.i()); }
inline void Eval::acos_fwd(const ExprAcos&, const EvalLabel& x, EvalLabel& y)                         { y.i()=acos(x.i()); }
inline void Eval::asin_fwd(const ExprAsin&, const EvalLabel& x, EvalLabel& y)                         { y.i()=asin(x.i()); }
inline void Eval::atan_fwd(const ExprAtan&, const EvalLabel& x, EvalLabel& y)                         { y.i()=atan(x.i()); }
inline void Eval::acosh_fwd(const ExprAcosh&, const EvalLabel& x, EvalLabel& y)                       { y.i()=acosh(x.i()); }
inline void Eval::asinh_fwd(const ExprAsinh&, const EvalLabel& x, EvalLabel& y)                       { y.i()=asinh(x.i()); }
inline void Eval::atanh_fwd(const ExprAtanh&, const EvalLabel& x, EvalLabel& y)                       { y.i()=atanh(x.i()); }


} // namespace ibex
#endif // IBEX_EVAL_H_
