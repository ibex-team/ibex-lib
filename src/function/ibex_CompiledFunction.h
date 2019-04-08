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
#include "ibex_Agenda.h"

namespace ibex {

class Function;

template<class T>
class ExprData;

/**
 * \ingroup symbolic
 * \brief A low-level representation of a function for speeding up forward/backward algorithms.
 *
 */
class CompiledFunction : public ExprVisitor {
public:

	CompiledFunction();

	/**
	 * Create a compiled version of the function \a f. */
	void compile(Function& f);

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
	void forward(const V& algo) const;

	/**
	 * Forward phase on a specific set of operations (in the agenda).
	 */
	template<class V>
	void forward(const V& algo, const Agenda& a) const;

	/**
	 * Run the backward phase.  V must be a subclass of BwdAlgorithm.
	 * Note that the type V is just passed in order to have static linkage.
	 */
	template<class V>
	void backward(const V& algo) const;

	/**
	 * Backward phase on a specific set of operations (in the agenda).
	 */
	template<class V>
	void backward(const V& algo, const Agenda& a) const;

	/**
	 * Return an agenda of all the operations
	 * that need to be considered for a subexpression
	 * of a given rank (including itself).
	 *
	 * \note To be deleted by the caller.
	 */
	Agenda* agenda(int rank) const;

	/**
	 * Print the structure to the standard output.
	 */
	friend class Function;

protected:
	typedef enum {
		IDX,    // index with reference
		IDX_CP, // index with copy
		VEC, SYM, CST, APPLY, CHI,
		ADD, MUL, SUB, DIV, MAX, MIN, ATAN2,
		GEN1, GEN2, GENN,
		MINUS, MINUS_V, MINUS_M,
		TRANS_V, TRANS_M, SIGN, ABS, POWER,
		SQR, SQRT, EXP, LOG,
		COS,  SIN,  TAN,  ACOS,  ASIN,  ATAN,
		COSH, SINH, TANH, ACOSH, ASINH, ATANH,

		ADD_V, ADD_M, SUB_V, SUB_M,
		MUL_SV, MUL_SM, MUL_VV, MUL_MV, MUL_MM, MUL_VM
	} operation;

private:

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
	void visit(const ExprNAryOp& e, operation op);
	void visit(const ExprBinaryOp& b, operation op);
	void visit(const ExprUnaryOp& u, operation op);
#ifdef __clang__
#pragma clang diagnostic pop
#endif

	void visit(const ExprNode& e);
	void visit(const ExprIndex& i);
	void visit(const ExprSymbol& v);
	void visit(const ExprConstant& c);
	void visit(const ExprVector& e);
	void visit(const ExprApply& e);
	void visit(const ExprChi& e);
	void visit(const ExprGenericBinaryOp& e);
	void visit(const ExprAdd& e);
	void visit(const ExprMul& e);
	void visit(const ExprSub& e);
	void visit(const ExprDiv& e);
	void visit(const ExprMax& e);
	void visit(const ExprMin& e);
	void visit(const ExprAtan2& e);
	void visit(const ExprGenericUnaryOp& e);
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

private:
	template<class V>
	void forward(const V& algo, int i) const;

	template<class V>
	void backward(const V& algo, int i) const;

	friend std::ostream& operator<<(std::ostream& os, const CompiledFunction& data);

	const char* op(operation o) const;

	int n; // == the size of the root expression

	int n_total; // == the size of the expression, including all arguments

	ExprSubNodes *nodes;

	operation *code;

	int* nb_args;

	mutable int** args;

	// Node counter in Polish prefix notation
	// (only useful during construction)
	mutable int ptr;
};

std::ostream& operator<<(std::ostream& os, const CompiledFunction& data);

template<class V>
inline void CompiledFunction::forward(const V& algo) const {
	assert(dynamic_cast<const FwdAlgorithm* >(&algo)!=NULL);

	for (int i=n-1; i>=0; i--) {
		forward(algo, i);
	}
}

template<class V>
inline void CompiledFunction::forward(const V& algo, const Agenda& a) const {
	assert(dynamic_cast<const FwdAlgorithm* >(&algo)!=NULL);

	for (int i=a.first(); i!=a.end(); i=a.next(i)) {
		forward(algo, i);
	}
}

template<class V>
void CompiledFunction::forward(const V& algo, int i) const {
	switch(code[i]) {
	case IDX:    ((V&) algo).idx_fwd    (args[i][0], i); break;
	case IDX_CP: ((V&) algo).idx_cp_fwd (args[i][0], i); break;
	case VEC:    ((V&) algo).vector_fwd (args[i], i); break;
	case SYM:    ((V&) algo).symbol_fwd (i); break;
	case CST:    ((V&) algo).cst_fwd    (i); break;
	case APPLY:  ((V&) algo).apply_fwd  (args[i],i); break;
	case CHI:    ((V&) algo).chi_fwd    (args[i][0], args[i][1], args[i][2], i); break;
	case GEN2:   ((V&) algo).gen2_fwd   (args[i][0], args[i][1], i); break;
	case ADD:    ((V&) algo).add_fwd    (args[i][0], args[i][1], i); break;
	case ADD_V:  ((V&) algo).add_V_fwd  (args[i][0], args[i][1], i); break;
	case ADD_M:  ((V&) algo).add_M_fwd  (args[i][0], args[i][1], i); break;
	case MUL:    ((V&) algo).mul_fwd    (args[i][0], args[i][1], i); break;
	case MUL_SV: ((V&) algo).mul_SV_fwd (args[i][0], args[i][1], i); break;
	case MUL_SM: ((V&) algo).mul_SM_fwd (args[i][0], args[i][1], i); break;
	case MUL_VV: ((V&) algo).mul_VV_fwd (args[i][0], args[i][1], i); break;
	case MUL_MV: ((V&) algo).mul_MV_fwd (args[i][0], args[i][1], i); break;
	case MUL_MM: ((V&) algo).mul_MM_fwd (args[i][0], args[i][1], i); break;
	case MUL_VM: ((V&) algo).mul_VM_fwd (args[i][0], args[i][1], i); break;
	case SUB:    ((V&) algo).sub_fwd    (args[i][0], args[i][1], i); break;
	case SUB_V:  ((V&) algo).sub_V_fwd  (args[i][0], args[i][1], i); break;
	case SUB_M:  ((V&) algo).sub_M_fwd  (args[i][0], args[i][1], i); break;
	case DIV:    ((V&) algo).div_fwd    (args[i][0], args[i][1], i); break;
	case MAX:    ((V&) algo).max_fwd    (args[i][0], args[i][1], i); break;
	case MIN:    ((V&) algo).min_fwd    (args[i][0], args[i][1], i); break;
	case ATAN2:  ((V&) algo).atan2_fwd  (args[i][0], args[i][1], i); break;
	case GEN1:   ((V&) algo).gen1_fwd   (args[i][0], i); break;
	case MINUS:  ((V&) algo).minus_fwd  (args[i][0], i); break;
	case MINUS_V:((V&) algo).minus_V_fwd(args[i][0], i); break;
	case MINUS_M:((V&) algo).minus_M_fwd(args[i][0], i); break;
	case TRANS_V:((V&) algo).trans_V_fwd(args[i][0], i); break;
	case TRANS_M:((V&) algo).trans_M_fwd(args[i][0], i); break;
	case SIGN:   ((V&) algo).sign_fwd   (args[i][0], i); break;
	case ABS:    ((V&) algo).abs_fwd    (args[i][0], i); break;
	case POWER:  ((V&) algo).power_fwd  (args[i][0], i, ((const ExprPower&) (*nodes)[i]).expon); break;
	case SQR:    ((V&) algo).sqr_fwd    (args[i][0], i); break;
	case SQRT:   ((V&) algo).sqrt_fwd   (args[i][0], i); break;
	case EXP:    ((V&) algo).exp_fwd    (args[i][0], i); break;
	case LOG:    ((V&) algo).log_fwd    (args[i][0], i); break;
	case COS:    ((V&) algo).cos_fwd    (args[i][0], i); break;
	case SIN:    ((V&) algo).sin_fwd    (args[i][0], i); break;
	case TAN:    ((V&) algo).tan_fwd    (args[i][0], i); break;
	case COSH:   ((V&) algo).cosh_fwd   (args[i][0], i); break;
	case SINH:   ((V&) algo).sinh_fwd   (args[i][0], i); break;
	case TANH:   ((V&) algo).tanh_fwd   (args[i][0], i); break;
	case ACOS:   ((V&) algo).acos_fwd   (args[i][0], i); break;
	case ASIN:   ((V&) algo).asin_fwd   (args[i][0], i); break;
	case ATAN:   ((V&) algo).atan_fwd   (args[i][0], i); break;
	case ACOSH:  ((V&) algo).acosh_fwd  (args[i][0], i); break;
	case ASINH:  ((V&) algo).asinh_fwd  (args[i][0], i); break;
	case ATANH:  ((V&) algo).atanh_fwd  (args[i][0], i); break;
	default: 	 assert(false);
	}
}

template<class V>
void CompiledFunction::backward(const V& algo) const {

	assert(dynamic_cast<const BwdAlgorithm* >(&algo)!=NULL);

	for (int i=0; i<n; i++) {
		backward(algo, i);
	}
}

template<class V>
void CompiledFunction::backward(const V& algo, const Agenda& a) const {

	assert(dynamic_cast<const BwdAlgorithm* >(&algo)!=NULL);

	for (int i=a.first(); i!=a.end(); i=a.next(i)) {
		backward(algo, i);
	}
}

template<class V>
void CompiledFunction::backward(const V& algo, int i) const {
	switch(code[i]) {
	case IDX:    ((V&) algo).idx_bwd    (args[i][0], i); break;
	case IDX_CP: ((V&) algo).idx_cp_bwd (args[i][0], i); break;
	case VEC:    ((V&) algo).vector_bwd (args[i], i); break;
	case SYM:    ((V&) algo).symbol_bwd (i); break;
	case CST:    ((V&) algo).cst_bwd    (i); break;
	case APPLY:  ((V&) algo).apply_bwd  (args[i], i); break;
	case CHI:    ((V&) algo).chi_bwd    (args[i][0], args[i][1], args[i][2], i); break;
	case GEN2:   ((V&) algo).gen2_bwd   (args[i][0], args[i][1], i); break;
	case ADD:    ((V&) algo).add_bwd    (args[i][0], args[i][1], i); break;
	case ADD_V:  ((V&) algo).add_V_bwd  (args[i][0], args[i][1], i); break;
	case ADD_M:  ((V&) algo).add_M_bwd  (args[i][0], args[i][1], i); break;
	case MUL:    ((V&) algo).mul_bwd    (args[i][0], args[i][1], i); break;
	case MUL_SV: ((V&) algo).mul_SV_bwd (args[i][0], args[i][1], i); break;
	case MUL_SM: ((V&) algo).mul_SM_bwd (args[i][0], args[i][1], i); break;
	case MUL_VV: ((V&) algo).mul_VV_bwd (args[i][0], args[i][1], i); break;
	case MUL_MV: ((V&) algo).mul_MV_bwd (args[i][0], args[i][1], i); break;
	case MUL_MM: ((V&) algo).mul_MM_bwd (args[i][0], args[i][1], i); break;
	case MUL_VM: ((V&) algo).mul_VM_bwd (args[i][0], args[i][1], i); break;
	case SUB:    ((V&) algo).sub_bwd    (args[i][0], args[i][1], i); break;
	case SUB_V:  ((V&) algo).sub_V_bwd  (args[i][0], args[i][1], i); break;
	case SUB_M:  ((V&) algo).sub_M_bwd  (args[i][0], args[i][1], i); break;
	case DIV:    ((V&) algo).div_bwd    (args[i][0], args[i][1], i); break;
	case MAX:    ((V&) algo).max_bwd    (args[i][0], args[i][1], i); break;
	case MIN:    ((V&) algo).min_bwd    (args[i][0], args[i][1], i); break;
	case ATAN2:  ((V&) algo).atan2_bwd  (args[i][0], args[i][1], i); break;
	case GEN1:   ((V&) algo).gen1_bwd   (args[i][0], i); break;
	case MINUS:  ((V&) algo).minus_bwd  (args[i][0], i); break;
	case MINUS_V:((V&) algo).minus_V_bwd(args[i][0], i); break;
	case MINUS_M:((V&) algo).minus_M_bwd(args[i][0], i); break;
	case TRANS_V:((V&) algo).trans_V_bwd(args[i][0], i); break;
	case TRANS_M:((V&) algo).trans_M_bwd(args[i][0], i); break;
	case SIGN:   ((V&) algo).sign_bwd   (args[i][0], i); break;
	case ABS:    ((V&) algo).abs_bwd    (args[i][0], i); break;
	case POWER:  ((V&) algo).power_bwd  (args[i][0], i, ((const ExprPower&) (*nodes)[i]).expon); break;
	case SQR:    ((V&) algo).sqr_bwd    (args[i][0], i); break;
	case SQRT:   ((V&) algo).sqrt_bwd   (args[i][0], i); break;
	case EXP:    ((V&) algo).exp_bwd    (args[i][0], i); break;
	case LOG:    ((V&) algo).log_bwd    (args[i][0], i); break;
	case COS:    ((V&) algo).cos_bwd    (args[i][0], i); break;
	case SIN:    ((V&) algo).sin_bwd    (args[i][0], i); break;
	case TAN:    ((V&) algo).tan_bwd    (args[i][0], i); break;
	case COSH:   ((V&) algo).cosh_bwd   (args[i][0], i); break;
	case SINH:   ((V&) algo).sinh_bwd   (args[i][0], i); break;
	case TANH:   ((V&) algo).tanh_bwd   (args[i][0], i); break;
	case ACOS:   ((V&) algo).acos_bwd   (args[i][0], i); break;
	case ASIN:   ((V&) algo).asin_bwd   (args[i][0], i); break;
	case ATAN:   ((V&) algo).atan_bwd   (args[i][0], i); break;
	case ACOSH:  ((V&) algo).acosh_bwd  (args[i][0], i); break;
	case ASINH:  ((V&) algo).asinh_bwd  (args[i][0], i); break;
	case ATANH:  ((V&) algo).atanh_bwd  (args[i][0], i); break;
	default: 	 assert(false);
	}
}

} // namespace ibex

#endif // __IBEX_COMPILED_FUNCTION_H__
