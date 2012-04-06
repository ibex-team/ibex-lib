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
#include "ibex_FunctionVisitor.h"
#include "ibex_FwdAlgorithm.h"
#include "ibex_BwdAlgorithm.h"

namespace ibex {

class Function;

/**
 * \ingroup level1
 * \brief A low-level representation of a function for speeding up forward/backward algorithms.
 *
 */
class CompiledFunction : public FunctionVisitor {
public:

	CompiledFunction();

	/**
	 * Create a compiled version of the function \a f, where
	 * each node is decorated with an object of type "T" via the decorator \a d.
	 */
	void compile(const Function& f);

	/**
	 * \brier Delete this.
	 */
	~CompiledFunction();

	/**
	 * Run the forward phase of a forward algorithm and
	 * return a reference to the label
	 * of the root node. V must be a subclass of FwdAlgorithm<T>.
	 * Note that the type V is just passed in order to have static linkage.
	 */
	template<class V,typename T>
	T& forward(const V& algo) const;

	/**
	 * Run the backward phase.  V must be a subclass of BwdAlgorithm<T>.
	 * Note that the type V is just passed in order to have static linkage.
	 */
	template<class V,typename T>
	void backward(const V& algo) const;

protected:
	typedef enum {
		IDX, VEC, SYM, CST, APPLY,
		ADD, MUL, SUB, DIV, MAX, MIN, ATAN2,
		MINUS, SIGN, ABS, POWER,
		SQR, SQRT, EXP, LOG,
		COS,  SIN,  TAN,  ACOS,  ASIN,  ATAN,
		COSH, SINH, TANH, ACOSH, ASINH, ATANH,

		ADD_V, ADD_M, SUB_V, SUB_M,
		MUL_SV, MUL_SM, MUL_VV, MUL_MV, MUL_MM
	} operation;

private:

	void visit(const ExprNAryOp& e, operation op);
	void visit(const ExprBinaryOp& b, operation op);
	void visit(const ExprUnaryOp& u, operation op);

	void visit(const ExprNode& e);
	void visit(const ExprIndex& i);
	void visit(const ExprSymbol& v);
	void visit(const ExprConstant& c);
	void visit(const ExprNAryOp& e);
	void visit(const ExprBinaryOp& b);
	void visit(const ExprUnaryOp& u);
	void visit(const ExprVector& e);
	void visit(const ExprApply& e);
	void visit(const ExprAdd& e);
	void visit(const ExprMul& e);
	void visit(const ExprSub& e);
	void visit(const ExprDiv& e);
	void visit(const ExprMax& e);
	void visit(const ExprMin& e);
	void visit(const ExprAtan2& e);
	void visit(const ExprMinus& e);
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

	template<typename T>
	friend std::ostream& operator<<(std::ostream&,const CompiledFunction&);

	int n; // == the size of the root expression of the expression
	const ExprNode** nodes;
	operation *code;
	int* nb_args;
	mutable ExprLabel*** args;

	mutable int ptr;
};

template<class V,typename T>
T& CompiledFunction::forward(const V& algo) const {
	assert(dynamic_cast<const FwdAlgorithm<T>* >(&algo)!=NULL);

	for (int i=n-1; i>=0; i--) {
		switch(code[i]) {
		case IDX:    ((V&) algo).index_fwd((ExprIndex&)    *(nodes[i]), (T&) *args[i][1],                   (T&) *args[i][0]); break;
		case VEC:    ((V&) algo).vector_fwd((ExprVector&)  *(nodes[i]), (const T**) &(args[i][1]),          (T&) *args[i][0]); break;
		case SYM:    ((V&) algo).symbol_fwd((ExprSymbol&)  *(nodes[i]),                                     (T&) *args[i][0]); break;
		case CST:    ((V&) algo).cst_fwd  ((ExprConstant&) *(nodes[i]),                                     (T&) *args[i][0]); break;
		case APPLY:  ((V&) algo).apply_fwd((ExprApply&)    *(nodes[i]), (const T**) &(args[i][1]),          (T&) *args[i][0]); break;
		case ADD:    ((V&) algo).add_fwd  ((ExprAdd&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case ADD_V:  ((V&) algo).add_V_fwd  ((ExprAdd&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case ADD_M:  ((V&) algo).add_M_fwd  ((ExprAdd&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL:    ((V&) algo).mul_fwd  ((ExprMul&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL_SV: ((V&) algo).mul_SV_fwd ((ExprMul&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL_SM: ((V&) algo).mul_SM_fwd ((ExprMul&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL_VV: ((V&) algo).mul_VV_fwd ((ExprMul&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL_MV: ((V&) algo).mul_MV_fwd ((ExprMul&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL_MM: ((V&) algo).mul_MM_fwd ((ExprMul&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case SUB:    ((V&) algo).sub_fwd  ((ExprSub&)      *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case SUB_V:  ((V&) algo).sub_V_fwd  ((ExprSub&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case SUB_M:  ((V&) algo).sub_M_fwd  ((ExprSub&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
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
	return (T&) *args[0][0];
}

template<class V,typename T>
void CompiledFunction::backward(const V& algo) const {

	assert(dynamic_cast<const BwdAlgorithm<T>* >(&algo)!=NULL);

	for (int i=0; i<n; i++) {
		switch(code[i]) {
		case IDX:    ((V&) algo).index_bwd((ExprIndex&)    *(nodes[i]), (T&) *args[i][1],                      (T&) *args[i][0]); break;
		case VEC:    ((V&) algo).vector_bwd((ExprVector&)  *(nodes[i]), (T**) &(*args[i][1]),                  (T&) *args[i][0]); break;
		case SYM:    ((V&) algo).symbol_bwd((ExprSymbol&)  *(nodes[i]),                                        (T&) *args[i][0]); break;
		case CST:    ((V&) algo).cst_bwd  ((ExprConstant&) *(nodes[i]),                                        (T&) *args[i][0]); break;
		case APPLY:  ((V&) algo).apply_bwd  ((ExprApply&)  *(nodes[i]), (T**) &(*args[i][1]),                  (T&) *args[i][0]); break;
		case ADD:    ((V&) algo).add_bwd    ((ExprAdd&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case ADD_V:  ((V&) algo).add_V_bwd  ((ExprAdd&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case ADD_M:  ((V&) algo).add_M_bwd  ((ExprAdd&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL:    ((V&) algo).mul_bwd    ((ExprMul&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL_SV: ((V&) algo).mul_SV_bwd ((ExprMul&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL_SM: ((V&) algo).mul_SM_bwd ((ExprMul&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL_VV: ((V&) algo).mul_VV_bwd ((ExprMul&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL_MV: ((V&) algo).mul_MV_bwd ((ExprMul&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case MUL_MM: ((V&) algo).mul_MM_bwd ((ExprMul&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case SUB:    ((V&) algo).sub_bwd    ((ExprSub&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case SUB_V:  ((V&) algo).sub_V_bwd  ((ExprSub&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
		case SUB_M:  ((V&) algo).sub_M_bwd  ((ExprSub&)    *(nodes[i]), (T&) *args[i][1], (T&) *args[i][2], (T&) *args[i][0]); break;
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

// for debug only
template<typename T>
std::ostream& operator<<(std::ostream& os,const CompiledFunction& f) {
	for (int i=0; i<f.n; i++) {
		switch(f.code[i]) {
		case CompiledFunction::IDX:
		{
			ExprIndex& e=(ExprIndex&) *(f.nodes[i]);
			os << e.id << ": [-]" << " " << (T&) *f.args[i][0] << " " << e.expr.id << " " << (T&) *f.args[i][1];
		}
		break;
		case CompiledFunction::VEC:
		{
			ExprVector& e=(ExprVector&) *(f.nodes[i]);
			const T** _args=(const T**) &f.args[i][1];
			os << e.id << ": vec " << " ";
			for (int i=0; i<e.nb_args; i++)
				os << (e.arg(i).id) << " " << *(_args[i]) << " ";
		}
		break;
		case CompiledFunction::SYM:
		{
			ExprSymbol& e=(ExprSymbol&) *(f.nodes[i]);
			os << e.id << ": " << e.name << " " << (T&) *f.args[i][0];
		}
		break;
		case CompiledFunction::CST:
		{
			ExprConstant& e=(ExprConstant&) *(f.nodes[i]);
			os << e.id << ": cst=" << e.get_matrix_value() << " " <<  (T&) *f.args[i][0];
		}
		break;
		case CompiledFunction::APPLY:
		{
			ExprApply& e=(ExprApply&) *(f.nodes[i]);
			const T** args=(const T**) f.args[i][1];
			os << e.id << ": " << "func()" << " ";
			for (int i=0; i<e.nb_args; i++)
				os << e.arg(i).id << " " << *args[i] << " ";
		}
		break;
		case CompiledFunction::ADD:
		case CompiledFunction::ADD_V:
		case CompiledFunction::ADD_M:
		case CompiledFunction::MUL:
		case CompiledFunction::MUL_SV:
		case CompiledFunction::MUL_SM:
		case CompiledFunction::MUL_VV:
		case CompiledFunction::MUL_MV:
		case CompiledFunction::MUL_MM:
		case CompiledFunction::SUB:
		case CompiledFunction::SUB_V:
		case CompiledFunction::SUB_M:
		case CompiledFunction::DIV:
		case CompiledFunction::MAX:
		case CompiledFunction::MIN:
		case CompiledFunction::ATAN2:
		{
			ExprBinaryOp& e=(ExprBinaryOp&) *(f.nodes[i]);
			os << e.id << ": " << f.op(f.code[i]) << " " << (T&) *f.args[i][0] << " ";
			os << e.left.id << " " << (T&) *f.args[i][1] << " ";
			os << e.right.id << " " << (T&) *f.args[i][2];
		}
		break;

		case CompiledFunction::MINUS:
		case CompiledFunction::SIGN:
		case CompiledFunction::ABS:
		case CompiledFunction::POWER:
		case CompiledFunction::SQR:
		case CompiledFunction::SQRT:
		case CompiledFunction::EXP:
		case CompiledFunction::LOG:
		case CompiledFunction::COS:
		case CompiledFunction::SIN:
		case CompiledFunction::TAN:
		case CompiledFunction::COSH:
		case CompiledFunction::SINH:
		case CompiledFunction::TANH:
		case CompiledFunction::ACOS:
		case CompiledFunction::ASIN:
		case CompiledFunction::ATAN:
		case CompiledFunction::ACOSH:
		case CompiledFunction::ASINH:
		case CompiledFunction::ATANH:
		{
			ExprUnaryOp& e=(ExprUnaryOp&) *(f.nodes[i]);
			os << e.id << ": " << f.op(f.code[i]) << " " << (T&) *f.args[i][0] << " ";
			os << e.expr.id << " " << (T&) *f.args[i][1];
		}
		break;
		}
		os << endl;
	}
	return os;
}

} // namespace ibex
#endif // _IBEX_COMPILED_FUNCTION_H_
