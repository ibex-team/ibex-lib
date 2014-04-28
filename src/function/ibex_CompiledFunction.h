//============================================================================
//                                  I B E X                                   
// File        : Compiled function
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 31, 2011
// Last Update : 
//============================================================================

#ifndef __IBEX_COMPILED_FUNCTION_H__
#define __IBEX_COMPILED_FUNCTION_H__

#include <stack>

#include "ibex_Expr.h"
#include "ibex_ExprVisitor.h"
#include "ibex_ExprSubNodes.h"
#include "ibex_FwdAlgorithm.h"
#include "ibex_BwdAlgorithm.h"

namespace ibex {

class Function;

/**
 * \ingroup symbolic
 * \brief A low-level representation of a function for speeding up forward/backward algorithms.
 *
 */
class CompiledFunction : public ExprVisitor {
public:

	CompiledFunction();

	/**
	 * Create a compiled version of the function \a f, where
	 * each node is decorated with an object of type "T" via the decorator \a d.
	 */
	void compile(const ExprNode& y);

	/**
	 * \brier Delete this.
	 */
	~CompiledFunction();

	/**
	 * Run the forward phase of a forward algorithm and
	 * return a reference to the label
	 * of the root node. V must be a subclass of FwdAlgorithm.
	 * Note that the type V is just passed in order to have static linkage.
	 */
	template<class V>
	ExprLabel& forward(const V& algo) const;

	/**
	 * Run the backward phase.  V must be a subclass of BwdAlgorithm.
	 * Note that the type V is just passed in order to have static linkage.
	 */
	template<class V>
	void backward(const V& algo) const;

	/**
	 * Print the structure to the standard output.
	 */
	void print() const;

	friend class Function;

protected:
	typedef enum {
		IDX, VEC, SYM, CST, APPLY, CHI,
		ADD, MUL, SUB, DIV, MAX, MIN, ATAN2,
		MINUS, TRANS_V, TRANS_M, SIGN, ABS, POWER,
		SQR, SQRT, EXP, LOG,
		COS,  SIN,  TAN,  ACOS,  ASIN,  ATAN,
		COSH, SINH, TANH, ACOSH, ASINH, ATANH,

		ADD_V, ADD_M, SUB_V, SUB_M,
		MUL_SV, MUL_SM, MUL_VV, MUL_MV, MUL_MM, MUL_VM
	} operation;

private:

	void visit(const ExprNAryOp& e, operation op);
	void visit(const ExprBinaryOp& b, operation op);
	void visit(const ExprUnaryOp& u, operation op);

	void visit(const ExprNode& e);
	void visit(const ExprIndex& i);
	void visit(const ExprLeaf& e);
	void visit(const ExprNAryOp& e);
	void visit(const ExprBinaryOp& b);
	void visit(const ExprUnaryOp& u);
	void visit(const ExprSymbol& v);
	void visit(const ExprConstant& c);
	void visit(const ExprVector& e);
	void visit(const ExprApply& e);
	void visit(const ExprChi& e);
	void visit(const ExprAdd& e);
	void visit(const ExprMul& e);
	void visit(const ExprSub& e);
	void visit(const ExprDiv& e);
	void visit(const ExprMax& e);
	void visit(const ExprMin& e);
	void visit(const ExprAtan2& e);
	void visit(const ExprMinus& e);
	void visit(const ExprTrans& e);
	void visit(const ExprSign& e);
	void visit(const ExprAbs& e);
	void visit(const ExprPower& e);
	void visit(const ExprSqr& e);
	void visit(const ExprSqrt& e);
	void visit(const ExprExp& e);
	void visit(const ExprLog& e);
	void visit(const ExprCos& e);
	void visit(const ExprSin& e);
	void visit(const ExprTan& e);
	void visit(const ExprCosh& e);
	void visit(const ExprSinh& e);
	void visit(const ExprTanh& e);
	void visit(const ExprAcos& e);
	void visit(const ExprAsin& e);
	void visit(const ExprAtan& e);
	void visit(const ExprAcosh& e);
	void visit(const ExprAsinh& e);
	void visit(const ExprAtanh& e);

protected:

	const char* op(operation o) const;

	friend std::ostream& operator<<(std::ostream&,const CompiledFunction&);

	int n; // == the size of the root expression of the expression
	ExprSubNodes nodes;
	operation *code;
	int* nb_args;
	mutable ExprLabel*** args;

	mutable int ptr;
};

template<class V>
ExprLabel& CompiledFunction::forward(const V& algo) const {
	assert(dynamic_cast<const FwdAlgorithm* >(&algo)!=NULL);

	for (int i=n-1; i>=0; i--) {
		switch(code[i]) {
		case IDX:    ((V&) algo).index_fwd((ExprIndex&)    nodes[i], *args[i][1],  *args[i][0]); break;
		case VEC:    ((V&) algo).vector_fwd((ExprVector&)  nodes[i], (const ExprLabel**) &(args[i][1]),*args[i][0]); break;
		case SYM:    ((V&) algo).symbol_fwd((ExprSymbol&)  nodes[i],               *args[i][0]); break;
		case CST:    ((V&) algo).cst_fwd  ((ExprConstant&) nodes[i],               *args[i][0]); break;
		case APPLY:  ((V&) algo).apply_fwd((ExprApply&)    nodes[i], &(args[i][1]),*args[i][0]); break;
		case CHI:    ((V&) algo).chi_fwd  ((ExprChi&)      nodes[i], *args[i][1], *args[i][2],  *args[i][3],*args[i][0]); break;
		case ADD:    ((V&) algo).add_fwd  ((ExprAdd&)      nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case ADD_V:  ((V&) algo).add_V_fwd  ((ExprAdd&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case ADD_M:  ((V&) algo).add_M_fwd  ((ExprAdd&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL:    ((V&) algo).mul_fwd  ((ExprMul&)      nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL_SV: ((V&) algo).mul_SV_fwd ((ExprMul&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL_SM: ((V&) algo).mul_SM_fwd ((ExprMul&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL_VV: ((V&) algo).mul_VV_fwd ((ExprMul&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL_MV: ((V&) algo).mul_MV_fwd ((ExprMul&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL_MM: ((V&) algo).mul_MM_fwd ((ExprMul&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL_VM: ((V&) algo).mul_VM_fwd ((ExprMul&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case SUB:    ((V&) algo).sub_fwd  ((ExprSub&)      nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case SUB_V:  ((V&) algo).sub_V_fwd  ((ExprSub&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case SUB_M:  ((V&) algo).sub_M_fwd  ((ExprSub&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case DIV:    ((V&) algo).div_fwd  ((ExprDiv&)      nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MAX:    ((V&) algo).max_fwd  ((ExprMax&)      nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MIN:    ((V&) algo).min_fwd  ((ExprMin&)      nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case ATAN2:  ((V&) algo).atan2_fwd((ExprAtan2&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MINUS:  ((V&) algo).minus_fwd((ExprMinus&)    nodes[i], *args[i][1],                   *args[i][0]); break;
		case TRANS_V:((V&) algo).trans_V_fwd((ExprTrans&)  nodes[i], *args[i][1],                   *args[i][0]); break;
		case TRANS_M:((V&) algo).trans_M_fwd((ExprTrans&)  nodes[i], *args[i][1],                   *args[i][0]); break;
		case SIGN:   ((V&) algo).sign_fwd ((ExprSign&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case ABS:    ((V&) algo).abs_fwd  ((ExprAbs&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case POWER:  ((V&) algo).power_fwd((ExprPower&)    nodes[i], *args[i][1],                   *args[i][0]); break;
		case SQR:    ((V&) algo).sqr_fwd  ((ExprSqr&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case SQRT:   ((V&) algo).sqrt_fwd ((ExprSqrt&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case EXP:    ((V&) algo).exp_fwd  ((ExprExp&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case LOG:    ((V&) algo).log_fwd  ((ExprLog&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case COS:    ((V&) algo).cos_fwd  ((ExprCos&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case SIN:    ((V&) algo).sin_fwd  ((ExprSin&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case TAN:    ((V&) algo).tan_fwd  ((ExprTan&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case COSH:   ((V&) algo).cosh_fwd ((ExprCosh&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case SINH:   ((V&) algo).sinh_fwd ((ExprSinh&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case TANH:   ((V&) algo).tanh_fwd ((ExprTanh&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case ACOS:   ((V&) algo).acos_fwd ((ExprAcos&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case ASIN:   ((V&) algo).asin_fwd ((ExprAsin&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case ATAN:   ((V&) algo).atan_fwd ((ExprAtan&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case ACOSH:  ((V&) algo).acosh_fwd((ExprAcosh&)    nodes[i], *args[i][1],                   *args[i][0]); break;
		case ASINH:  ((V&) algo).asinh_fwd((ExprAsinh&)    nodes[i], *args[i][1],                   *args[i][0]); break;
		case ATANH:  ((V&) algo).atanh_fwd((ExprAtanh&)    nodes[i], *args[i][1],                   *args[i][0]); break;
		default: 	 assert(false);
		}
	}
	return *args[0][0];
}

template<class V>
void CompiledFunction::backward(const V& algo) const {

	assert(dynamic_cast<const BwdAlgorithm* >(&algo)!=NULL);

	for (int i=0; i<n; i++) {
		switch(code[i]) {
		case IDX:    ((V&) algo).index_bwd((ExprIndex&)    nodes[i], *args[i][1],   *args[i][0]); break;
		case VEC:    ((V&) algo).vector_bwd((ExprVector&)  nodes[i], &(args[i][1]), *args[i][0]); break;
		case SYM:    ((V&) algo).symbol_bwd((ExprSymbol&)  nodes[i],                *args[i][0]); break;
		case CST:    ((V&) algo).cst_bwd  ((ExprConstant&) nodes[i],                *args[i][0]); break;
		case APPLY:  ((V&) algo).apply_bwd  ((ExprApply&)  nodes[i], &(args[i][1]), *args[i][0]); break;
		case CHI:    ((V&) algo).chi_bwd    ((ExprChi&)    nodes[i], *args[i][1], *args[i][2], *args[i][3], *args[i][0]); break;
		case ADD:    ((V&) algo).add_bwd    ((ExprAdd&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case ADD_V:  ((V&) algo).add_V_bwd  ((ExprAdd&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case ADD_M:  ((V&) algo).add_M_bwd  ((ExprAdd&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL:    ((V&) algo).mul_bwd    ((ExprMul&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL_SV: ((V&) algo).mul_SV_bwd ((ExprMul&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL_SM: ((V&) algo).mul_SM_bwd ((ExprMul&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL_VV: ((V&) algo).mul_VV_bwd ((ExprMul&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL_MV: ((V&) algo).mul_MV_bwd ((ExprMul&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL_MM: ((V&) algo).mul_MM_bwd ((ExprMul&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MUL_VM: ((V&) algo).mul_VM_bwd ((ExprMul&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case SUB:    ((V&) algo).sub_bwd    ((ExprSub&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case SUB_V:  ((V&) algo).sub_V_bwd  ((ExprSub&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case SUB_M:  ((V&) algo).sub_M_bwd  ((ExprSub&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case DIV:    ((V&) algo).div_bwd  ((ExprDiv&)      nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MAX:    ((V&) algo).max_bwd  ((ExprMax&)      nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MIN:    ((V&) algo).min_bwd  ((ExprMin&)      nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case ATAN2:  ((V&) algo).atan2_bwd((ExprAtan2&)    nodes[i], *args[i][1], *args[i][2], *args[i][0]); break;
		case MINUS:  ((V&) algo).minus_bwd((ExprMinus&)    nodes[i], *args[i][1],                   *args[i][0]); break;
		case TRANS_V:((V&) algo).trans_V_bwd((ExprTrans&)  nodes[i], *args[i][1],                   *args[i][0]); break;
		case TRANS_M:((V&) algo).trans_M_bwd((ExprTrans&)  nodes[i], *args[i][1],                   *args[i][0]); break;
		case SIGN:   ((V&) algo).sign_bwd ((ExprSign&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case ABS:    ((V&) algo).abs_bwd  ((ExprAbs&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case POWER:  ((V&) algo).power_bwd((ExprPower&)    nodes[i], *args[i][1],                   *args[i][0]); break;
		case SQR:    ((V&) algo).sqr_bwd  ((ExprSqr&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case SQRT:   ((V&) algo).sqrt_bwd ((ExprSqrt&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case EXP:    ((V&) algo).exp_bwd  ((ExprExp&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case LOG:    ((V&) algo).log_bwd  ((ExprLog&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case COS:    ((V&) algo).cos_bwd  ((ExprCos&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case SIN:    ((V&) algo).sin_bwd  ((ExprSin&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case TAN:    ((V&) algo).tan_bwd  ((ExprTan&)      nodes[i], *args[i][1],                   *args[i][0]); break;
		case COSH:   ((V&) algo).cosh_bwd ((ExprCosh&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case SINH:   ((V&) algo).sinh_bwd ((ExprSinh&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case TANH:   ((V&) algo).tanh_bwd ((ExprTanh&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case ACOS:   ((V&) algo).acos_bwd ((ExprAcos&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case ASIN:   ((V&) algo).asin_bwd ((ExprAsin&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case ATAN:   ((V&) algo).atan_bwd ((ExprAtan&)     nodes[i], *args[i][1],                   *args[i][0]); break;
		case ACOSH:  ((V&) algo).acosh_bwd((ExprAcosh&)    nodes[i], *args[i][1],                   *args[i][0]); break;
		case ASINH:  ((V&) algo).asinh_bwd((ExprAsinh&)    nodes[i], *args[i][1],                   *args[i][0]); break;
		case ATANH:  ((V&) algo).atanh_bwd((ExprAtanh&)    nodes[i], *args[i][1],                   *args[i][0]); break;
		default: 	 assert(false);
		}
	}
}


} // namespace ibex
#endif // _IBEX_COMPILED_FUNCTION_H_
