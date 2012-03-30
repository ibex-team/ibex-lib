//============================================================================
//                                  I B E X                                   
// File        : Compiled function
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 31, 2011
// Last Update : 
//============================================================================

#ifndef _IBEX_COMPILED_FUNCTION_H_
#define _IBEX_COMPILED_FUNCTION_H_

#include <stack>

#include "ibex_Expr.h"
#include "ibex_Function.h"
#include "ibex_FunctionVisitor.h"
#include "ibex_FwdAlgorithm.h"
#include "ibex_BwdAlgorithm.h"
#include "ibex_Decorator.h"

namespace ibex {

/**
 * \ingroup level1
 * \brief A low-level representation of a function for speeding up forward/backward algorithms.
 *
 */
template<typename T>
class CompiledFunction : public FunctionVisitor {
public:
	/**
	 * Create a compiled version of the function \a f, where
	 * each node is decorated with an object of type "T" via the decorator \a d.
	 */
	CompiledFunction(const Function& f, const Decorator<T>& d) : expr(f.expr()), f(f) {
		d.decorate(f);
		code=new operation[expr.size];
		args=new ExprLabel**[expr.size];
		nodes=new const ExprNode*[expr.size];
		ptr=0;
		visit(expr);
	}

	/**
	 * Run the forward phase of a forward algorithm and
	 * return a reference to the label
	 * of the root node. V must be a subclass of FwdAlgorithm<T>.
	 * Note that the type V is just passed in order to have static linkage.
	 */
	template<class V>
	T& forward(const V& algo) const;

	/**
	 * Run the backward phase.  V must be a subclass of BwdAlgorithm<T>.
	 * Note that the type V is just passed in order to have static linkage.
	 */
	template<class V>
	void backward(const V& algo) const;

	/**
	 * \brief The root node of the function expression
	 */
	const ExprNode& expr;

	/**
	 * The function
	 */
	const Function& f;
private:

	void visit(const ExprNode& e) {
		e.acceptVisitor(*this);
	}

	void visit(const ExprIndex& i) {
		code[ptr]=IDX;
		nodes[ptr]=&i;
		nb_args[ptr]=1;
		args[ptr]=new ExprLabel*[2];
		args[ptr][0]=i.deco;
		args[ptr][1]=i.expr.deco;

		ptr++;
		visit(i.expr);
	}

	void visit(const ExprSymbol& v) {
		code[ptr]=SYM;
		nodes[ptr]=&v;
		nb_args[ptr]=0;
		args[ptr]=new ExprLabel*[1]; // the unique argument of a Variable is the corresponding index in "csts"
		args[ptr][0]=v.deco;

		ptr++;
	}

	void visit(const ExprConstant& c) {
		code[ptr]=CST;

		nodes[ptr]=&c;
		nb_args[ptr]=0;
		args[ptr]=new ExprLabel*[1];
		args[ptr][0]=c.deco;

		ptr++;
	}

	void visit(const ExprNAryOp& e) {
		e.acceptVisitor(*this);
		nodes[ptr]=&e;
		nb_args[ptr]=e.nb_args;
		args[ptr]=new ExprLabel*[e.nb_args+1];
		args[ptr][0]=e.deco;
		for (int i=1; i<e.nb_args; i++)
			args[ptr][i]=e.arg(i).deco;

		ptr++;
		for (int i=0; i<e.nb_args; i++) {
			visit(e.arg(i));
		}
	}

	void visit(const ExprBinaryOp& b) {
		b.acceptVisitor(*this);
		nodes[ptr]=&b;
		nb_args[ptr]=2;
		args[ptr]=new ExprLabel*[3];
		args[ptr][0]=b.deco;
		args[ptr][1]=b.left.deco;
		args[ptr][2]=b.right.deco;

		ptr++;
		visit(b.left);
		visit(b.right);
	}

	void visit(const ExprUnaryOp& u) {
		u.acceptVisitor(*this);
		nodes[ptr]=&u;
		nb_args[ptr]=1;
		args[ptr]=new ExprLabel*[2];
		args[ptr][0]=u.deco;
		args[ptr][1]=u.expr.deco;

		ptr++;
		visit(u.expr);
	}

	void visit(const ExprVector&){ code[ptr]=VEC; }

	void visit(const ExprApply&) { code[ptr]=APPLY; }

	void visit(const ExprAdd&)   { code[ptr]=ADD; }

	void visit(const ExprMul&)   { code[ptr]=MUL; }

	void visit(const ExprSub&)   { code[ptr]=SUB; }

	void visit(const ExprDiv&)   { code[ptr]=DIV; }

	void visit(const ExprMax&)   { code[ptr]=MAX; }

	void visit(const ExprMin&)   { code[ptr]=MIN; }

	void visit(const ExprAtan2&) { code[ptr]=ATAN2;}

	void visit(const ExprMinus&) { code[ptr]=MINUS; }

	void visit(const ExprSign&)  { code[ptr]=SIGN; }

	void visit(const ExprAbs&)   { code[ptr]=ABS; }

	void visit(const ExprPower&) { code[ptr]=POWER; }

	void visit(const ExprSqr&)   { code[ptr]=SQR; }

	void visit(const ExprSqrt&)  { code[ptr]=SQRT; }

	void visit(const ExprExp&)   { code[ptr]=EXP; }

	void visit(const ExprLog&)   { code[ptr]=LOG; }

	void visit(const ExprCos&)   { code[ptr]=COS; }

	void visit(const ExprSin&)   { code[ptr]=SIN; }

	void visit(const ExprTan&)   { code[ptr]=TAN; }

	void visit(const ExprCosh&)  { code[ptr]=COSH; }

	void visit(const ExprSinh&)  { code[ptr]=SINH; }

	void visit(const ExprTanh&)  { code[ptr]=TANH; }

	void visit(const ExprAcos&)  { code[ptr]=ACOS; }

	void visit(const ExprAsin&)  { code[ptr]=ASIN; }

	void visit(const ExprAtan&)  { code[ptr]=ATAN; }

	void visit(const ExprAcosh&) { code[ptr]=ACOSH; }

	void visit(const ExprAsinh&) { code[ptr]=ASINH; }

	void visit(const ExprAtanh&) { code[ptr]=ATANH; }

protected:

	typedef enum {
		IDX, VEC, SYM, CST, APPLY,
		ADD, MUL, SUB, DIV, MAX, MIN, ATAN2,
		MINUS, SIGN, ABS, POWER,
		SQR, SQRT, EXP, LOG,
		COS,  SIN,  TAN,  ACOS,  ASIN,  ATAN,
		COSH, SINH, TANH, ACOSH, ASINH, ATANH
	} operation;

	const ExprNode** nodes;
	operation *code;
	int* nb_args;
	mutable ExprLabel*** args;

	mutable int ptr;
};

template<typename T>template<class V>
T& CompiledFunction<T>::forward(const V& algo) const {
	assert(dynamic_cast<const FwdAlgorithm<T>* >(&algo)!=NULL);

	for (int i=expr.size-1; i>=0; i--) {
		switch(code[i]) {
		case IDX:    ((V&) algo).index_fwd((ExprIndex&)    *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case VEC:    ((V&) algo).vector_fwd((ExprVector&)  *(nodes[i]), (const T**) &(args[i][1]),          (T&) *args[i][0]); break;
		case SYM:    ((V&) algo).symbol_fwd((ExprSymbol&)  *(nodes[i]),                                     (T&) *args[i][0]); break;
		case CST:    ((V&) algo).cst_fwd  ((ExprConstant&) *(nodes[i]),                                     (T&) *args[i][0]); break;
		case APPLY:  ((V&) algo).apply_fwd((ExprApply&)    *(nodes[i]), (const T**) &(args[i][1]),          (T&) *args[i][0]); break;
		case ADD:    ((V&) algo).add_fwd  ((ExprAdd&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL:    ((V&) algo).mul_fwd  ((ExprMul&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case SUB:    ((V&) algo).sub_fwd  ((ExprSub&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case DIV:    ((V&) algo).div_fwd  ((ExprDiv&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MAX:    ((V&) algo).max_fwd  ((ExprMax&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MIN:    ((V&) algo).min_fwd  ((ExprMin&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case ATAN2:  ((V&) algo).atan2_fwd((ExprAtan2&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MINUS:  ((V&) algo).minus_fwd((ExprMinus&)    *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case SIGN:   ((V&) algo).sign_fwd ((ExprSign&)     *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case ABS:    ((V&) algo).abs_fwd  ((ExprAbs&)      *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case POWER:  ((V&) algo).power_fwd((ExprPower&)    *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case SQR:    ((V&) algo).sqr_fwd  ((ExprSqr&)      *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case SQRT:   ((V&) algo).sqrt_fwd ((ExprSqrt&)     *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case EXP:    ((V&) algo).exp_fwd  ((ExprExp&)      *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case LOG:    ((V&) algo).log_fwd  ((ExprLog&)      *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case COS:    ((V&) algo).cos_fwd  ((ExprCos&)      *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case SIN:    ((V&) algo).sin_fwd  ((ExprSin&)      *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case TAN:    ((V&) algo).tan_fwd  ((ExprTan&)      *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case COSH:   ((V&) algo).cosh_fwd ((ExprCosh&)     *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case SINH:   ((V&) algo).sinh_fwd ((ExprSinh&)     *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case TANH:   ((V&) algo).tanh_fwd ((ExprTanh&)     *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case ACOS:   ((V&) algo).acos_fwd ((ExprAcos&)     *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case ASIN:   ((V&) algo).asin_fwd ((ExprAsin&)     *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case ATAN:   ((V&) algo).atan_fwd ((ExprAtan&)     *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case ACOSH:  ((V&) algo).acosh_fwd((ExprAcosh&)    *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case ASINH:  ((V&) algo).asinh_fwd((ExprAsinh&)    *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case ATANH:  ((V&) algo).atanh_fwd((ExprAtanh&)    *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		}
	}
	return (T&) *expr.deco;
}

template<typename T>template<class V>
void CompiledFunction<T>::backward(const V& algo) const {

	assert(dynamic_cast<const BwdAlgorithm<T>* >(&algo)!=NULL);

	for (int i=0; i<expr.size; i++) {
		switch(code[i]) {
		case IDX:    ((V&) algo).index_bwd((ExprIndex&)    *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case VEC:    ((V&) algo).vector_bwd((ExprVector&)  *(nodes[i]), (T**) &(*args[i][1]),                  (T&) *args[i][0]); break;
		case SYM:    ((V&) algo).symbol_bwd((ExprSymbol&)  *(nodes[i]),                                        (T&) *args[i][0]); break;
		case CST:    ((V&) algo).cst_bwd  ((ExprConstant&) *(nodes[i]),                                        (T&) *args[i][0]); break;
		case APPLY:  ((V&) algo).apply_bwd((ExprApply&)    *(nodes[i]), (T**) &(*args[i][1]),                  (T&) *args[i][0]); break;
		case ADD:    ((V&) algo).add_bwd  ((ExprAdd&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL:    ((V&) algo).mul_bwd  ((ExprMul&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case SUB:    ((V&) algo).sub_bwd  ((ExprSub&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case DIV:    ((V&) algo).div_bwd  ((ExprDiv&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MAX:    ((V&) algo).max_bwd  ((ExprMax&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MIN:    ((V&) algo).min_bwd  ((ExprMin&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case ATAN2:  ((V&) algo).atan2_bwd((ExprAtan2&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MINUS:  ((V&) algo).minus_bwd((ExprMinus&)    *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case SIGN:   ((V&) algo).sign_bwd ((ExprSign&)     *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case ABS:    ((V&) algo).abs_bwd  ((ExprAbs&)      *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case POWER:  ((V&) algo).power_bwd((ExprPower&)    *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case SQR:    ((V&) algo).sqr_bwd  ((ExprSqr&)      *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case SQRT:   ((V&) algo).sqrt_bwd ((ExprSqrt&)     *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case EXP:    ((V&) algo).exp_bwd  ((ExprExp&)      *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case LOG:    ((V&) algo).log_bwd  ((ExprLog&)      *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case COS:    ((V&) algo).cos_bwd  ((ExprCos&)      *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case SIN:    ((V&) algo).sin_bwd  ((ExprSin&)      *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case TAN:    ((V&) algo).tan_bwd  ((ExprTan&)      *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case COSH:   ((V&) algo).cosh_bwd ((ExprCosh&)     *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case SINH:   ((V&) algo).sinh_bwd ((ExprSinh&)     *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case TANH:   ((V&) algo).tanh_bwd ((ExprTanh&)     *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case ACOS:   ((V&) algo).acos_bwd ((ExprAcos&)     *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case ASIN:   ((V&) algo).asin_bwd ((ExprAsin&)     *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case ATAN:   ((V&) algo).atan_bwd ((ExprAtan&)     *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case ACOSH:  ((V&) algo).acosh_bwd((ExprAcosh&)    *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case ASINH:  ((V&) algo).asinh_bwd((ExprAsinh&)    *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case ATANH:  ((V&) algo).atanh_bwd((ExprAtanh&)    *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		}
	}
}


} // namespace ibex
#endif // _IBEX_COMPILED_FUNCTION_H_
