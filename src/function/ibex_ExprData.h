
#ifndef __EXPR_DATA_H__
#define __EXPR_DATA_H__

#include "ibex_Array.h"
#include "ibex_ExprVisitor.h"

namespace ibex {

class Function;

template<class T>
class ExprData;

/**
 * \brief Expression Data factory.
 *
 * Each node of the function expression is decorated
 * with an object of type "T".
 *
 */
template<class T>
class ExprDataFactory : public ExprVisitor {
public:
	// The object to build
	ExprData<T>* data;

	/**
	 * \brief Build the object "data".
	 */
	void build(ExprData<T>& data) const;

	/**
	 * (Does nothing)
	 */
	virtual ~ExprDataFactory();

	/**
	 * Visit an expression.
	 * Principle: Either this function is overridden or the principle
	 * applies for all direct sub-classes of ExprUnaryOp.
	 */
	virtual T* init(const ExprNode&) {
		ibex_error("ExprData::init: Missing implementation for some node type");
		return NULL;
	}

	/** Visit an indexed expression. */
	virtual T* init(const ExprIndex& e, T&) {
		return init((const ExprNode&) e);
	}

	/** Visit a leaf.*/
	virtual T* init(const ExprLeaf& e) {
		return init((const ExprNode&) e);
	}

	/** Visit a n-ary operator. */
	virtual T* init(const ExprNAryOp& e, Array<T>&) {
		return init((const ExprNode&) e);
	}

	/** Visit a binary operator. */
	virtual T* init(const ExprBinaryOp& e, T&, T&) {
		return init((const ExprNode&) e);
	}

	/** Visit an unary operator. */
	virtual T* init(const ExprUnaryOp& e, T&) {
		return init((const ExprNode&) e);
	}

	/*==================== 0-ary operators =========================*/
	/** Visit a symbol.
	 * By default: call init(const ExprLeaf& e). */
	virtual T* init(const ExprSymbol& e) {
		return init((const ExprLeaf&) e);
	}

	/** Visit a constant.
	 * By default: call init(const ExprLeaf& e). */
	virtual T* init(const ExprConstant& e) {
		return init((const ExprLeaf&) e);
	}

	/*==================== n-ary operators =========================*/
	/** Visit a vector of expressions.
	 * By default: call init(const ExprNAryOp& e). */
	virtual T* init(const ExprVector& e, Array<T>& args_deco) {
		return init((const ExprNAryOp&) e, args_deco);
	}

	/** Visit a function application.
	 * By default: call init(const ExprNAryOp& e). */
	virtual T* init(const ExprApply& e, Array<T>& args_deco) {
		return init((const ExprNAryOp&) e, args_deco);
	}

	/** Visit a function Chi.
	 * By default: call init(const ExprNAryOp& e). */
	virtual T* init(const ExprChi& e, Array<T>& args_deco) {
		return init((const ExprNAryOp&) e, args_deco);
	}

	/*==================== binary operators =========================*/
	/** Visit a generic binary op (Implementation is not mandatory).
	 * By default: call init(const ExprBinaryOp& e). */
	virtual T* init(const ExprGenericBinaryOp& e, T& left_deco, T& right_deco) {
		return init((const ExprBinaryOp&) e, left_deco, right_deco);
	}

	/** Visit an addition (Implementation is not mandatory).
	 * By default: call init(const ExprBinaryOp& e). */
	virtual T* init(const ExprAdd& e, T& left_deco, T& right_deco) {
		return init((const ExprBinaryOp&) e, left_deco, right_deco);
	}

	/** Visit an multiplication (Implementation is not mandatory).
	 * By default: call init(const ExprBinaryOp& e). */
	virtual T* init(const ExprMul& e, T& left_deco, T& right_deco) {
		return init((const ExprBinaryOp&) e, left_deco, right_deco);
	}

	/** Visit a subtraction (Implementation is not mandatory).
	 * By default: call init(const ExprBinaryOp& e). */
	virtual T* init(const ExprSub& e, T& left_deco, T& right_deco) {
		return init((const ExprBinaryOp&) e, left_deco, right_deco);
	}

	/** Visit a division (Implementation is not mandatory).
	 * By default: call init(const ExprBinaryOp& e). */
	virtual T* init(const ExprDiv& e, T& left_deco, T& right_deco) {
		return init((const ExprBinaryOp&) e, left_deco, right_deco);
	}

	/** Visit a max  (Implementation is not mandatory).
	 * By default: call init(const ExprBinaryOp& e). */
	virtual T* init(const ExprMax& e, T& left_deco, T& right_deco) {
		return init((const ExprBinaryOp&) e, left_deco, right_deco);
	}

	/** Visit a min (Implementation is not mandatory).
	 * By default: call init(const ExprBinaryOp& e). */
	virtual T* init(const ExprMin& e, T& left_deco, T& right_deco) {
		return init((const ExprBinaryOp&) e, left_deco, right_deco);
	}

	/** Visit a arctan2 (Implementation is not mandatory).
	 * By default: call init(const ExprBinaryOp& e). */
	virtual T* init(const ExprAtan2& e, T& left_deco, T& right_deco) {
		return init((const ExprBinaryOp&) e, left_deco, right_deco);
	}


	/*==================== unary operators =========================*/
	/** Visit a generic unary op (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprGenericUnaryOp& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a minus sign (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprMinus& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a transpose (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprTrans& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a sign (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprSign& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit an abs (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprAbs& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a power expression.
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprPower& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a sqr (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprSqr& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a sqrt (Implementation is no
	 * By default: does nothing. t mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprSqrt& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit an exp (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprExp& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a log (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprLog& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a cos (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprCos& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a sin (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprSin& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a tan (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprTan& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a cosh (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprCosh& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a sinh (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprSinh& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a tanh (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprTanh& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a acos (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprAcos& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a asin (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprAsin& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a atan (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprAtan& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a acosh (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprAcosh& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a asinh (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprAsinh& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

	/** Visit a atanh (Implementation is not mandatory).
	 * By default call init(const ExprUnaryOp&). */
	virtual T* init(const ExprAtanh& e, T& expr_deco) {
		return init((const ExprUnaryOp&) e, expr_deco);
	}

//private:

	template<class Node>
	void __visit_nary(const Node& e);

	template<class Node>
	void __visit_binary(const Node& e);

	template<class Node>
	void __visit_unary(const Node& e);

	template<class Node>
	void __visit_0ary(const Node& e);

	virtual void visit(const ExprPower& e)           { __visit_unary<ExprPower>(e); }
	virtual void visit(const ExprIndex& e)           { __visit_unary<ExprIndex>(e); }
	virtual void visit(const ExprVector& e)          { __visit_nary<ExprVector>(e); }
	virtual void visit(const ExprApply& e)           { __visit_nary<ExprApply>(e); }
	virtual void visit(const ExprChi& e)             { __visit_nary<ExprChi>(e); }
	virtual void visit(const ExprGenericBinaryOp& e) { __visit_binary<ExprGenericBinaryOp>(e); }
	virtual void visit(const ExprAdd& e)             { __visit_binary<ExprAdd>(e); }
	virtual void visit(const ExprMul& e)             { __visit_binary<ExprMul>(e); }
	virtual void visit(const ExprSub& e)             { __visit_binary<ExprSub>(e); }
	virtual void visit(const ExprDiv& e)             { __visit_binary<ExprDiv>(e); }
	virtual void visit(const ExprMax& e)             { __visit_binary<ExprMax>(e); }
	virtual void visit(const ExprMin& e)             { __visit_binary<ExprMin>(e); }
	virtual void visit(const ExprAtan2& e)           { __visit_binary<ExprAtan2>(e); }
	virtual void visit(const ExprGenericUnaryOp& e)  { __visit_unary<ExprGenericUnaryOp>(e); }
	virtual void visit(const ExprMinus& e)           { __visit_unary<ExprMinus>(e); }
	virtual void visit(const ExprTrans& e)           { __visit_unary<ExprTrans>(e); }
	virtual void visit(const ExprSign& e)            { __visit_unary<ExprSign>(e); }
	virtual void visit(const ExprAbs& e)             { __visit_unary<ExprAbs>(e); }
	virtual void visit(const ExprSqr& e)             { __visit_unary<ExprSqr>(e); }
	virtual void visit(const ExprSqrt& e)            { __visit_unary<ExprSqrt>(e); }
	virtual void visit(const ExprExp& e)             { __visit_unary<ExprExp>(e); }
	virtual void visit(const ExprLog& e)             { __visit_unary<ExprLog>(e); }
	virtual void visit(const ExprCos& e)             { __visit_unary<ExprCos>(e); }
	virtual void visit(const ExprSin& e)             { __visit_unary<ExprSin>(e); }
	virtual void visit(const ExprTan& e)             { __visit_unary<ExprTan>(e); }
	virtual void visit(const ExprCosh& e)            { __visit_unary<ExprCosh>(e); }
	virtual void visit(const ExprSinh& e)            { __visit_unary<ExprSinh>(e); }
	virtual void visit(const ExprTanh& e)            { __visit_unary<ExprTanh>(e); }
	virtual void visit(const ExprAcos& e)            { __visit_unary<ExprAcos>(e); }
	virtual void visit(const ExprAsin& e)            { __visit_unary<ExprAsin>(e); }
	virtual void visit(const ExprAtan& e)            { __visit_unary<ExprAtan>(e); }
	virtual void visit(const ExprAcosh& e)           { __visit_unary<ExprAcosh>(e); }
	virtual void visit(const ExprAsinh& e)           { __visit_unary<ExprAsinh>(e); }
	virtual void visit(const ExprAtanh& e)           { __visit_unary<ExprAtanh>(e); }
	virtual void visit(const ExprConstant& e)        { __visit_0ary<ExprConstant>(e); }
	virtual void visit(const ExprSymbol& e)          { __visit_0ary<ExprSymbol>(e); }
};

/**
 * \brief Data associated to a function.
 *
 * Each node of the function expression is decorated
 * with an object of type "T".
 *
 */
template<class T>
class ExprData {
public:

	/**
	 * Initialize this data (of type T) associated to a function.
	 */
	ExprData(const Function& f, const ExprDataFactory<T>& factory);

	/**
	 * (Does nothing)
	 */
	virtual ~ExprData();

	/**
	 * Get the data associated to the ith node (in prefix order).
	 */
	const T& operator[](int i) const;

	/**
	 * Get the data associated to the ith node (in prefix order).
	 */
	T& operator[](int i);

	/**
	 *  The function
	 */
	const Function& f;

	/**
	 *  Data
	 */
	Array<T> data;

	/**
	 * Data of the arguments of the function
	 */
	Array<T> args;

	/**
	 * Data of the root node of the expression
	 */
	T* const top;


};

template<class T>
std::ostream& operator<<(std::ostream& os, const ExprData<T>&);

/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/
template<class T>
ExprDataFactory<T>::~ExprDataFactory() {

}

template<class T>
void ExprDataFactory<T>::build(ExprData<T>& data) const {
	if (this->data) ibex_error("ExprDataFactory: build function called twice");

	(ExprData<T>*&) this->data = &data;

	int n=data.f.nodes.size();
	for (int ptr=n-1; ptr>=0; ptr--) {
		data.f.nodes[ptr].acceptVisitor((ExprDataFactory<T>&) *this);
	}

	for (int i=0; i<data.f.nb_arg(); i++) {
		data.args.set_ref(i,data.data[data.f.nodes.rank(data.f.arg(i))]);
	}

	((T*&) data.top)=&data.data[0];
}


template<class T> template<class Node>
void ExprDataFactory<T>::__visit_nary(const Node& e) {
	Array<T> args_data(e.nb_args);
	for (int i=0; i<e.nb_args; i++)
		args_data.set_ref(i,data->data[data->f.nodes.rank(e.arg(i))]);

	data->data.set_ref(data->f.nodes.rank(e), *init(e,args_data));
}

template<class T> template<class Node>
void ExprDataFactory<T>::__visit_binary(const Node& e) {
	T& left_data=data->data[data->f.nodes.rank(e.left)];
	T& right_data=data->data[data->f.nodes.rank(e.right)];

	data->data.set_ref(data->f.nodes.rank(e), *init(e,left_data,right_data));
}

template<class T> template<class Node>
void ExprDataFactory<T>::__visit_unary(const Node& e) {
	T& expr_data=data->data[data->f.nodes.rank(e.expr)];
	data->data.set_ref(data->f.nodes.rank(e), *init(e,expr_data));
}

template<class T> template<class Node>
void ExprDataFactory<T>::__visit_0ary(const Node& e) {
	data->data.set_ref(data->f.nodes.rank(e), *init(e));
}

template<class T>
ExprData<T>::ExprData(const Function& f, const ExprDataFactory<T>& factory) : f(f), data(f.nodes.size()), args(f.nb_arg()), top(NULL) {
	factory.build(*this);
}

template<class T>
ExprData<T>::~ExprData() {
}

template<class T>
inline const T& ExprData<T>::operator[](int i) const {
	return data[i];
}

template<class T>
inline T& ExprData<T>::operator[](int i) {
	return data[i];
}

template<class T>
std::ostream& operator<<(std::ostream& os, const ExprData<T>& data) {
	os << "================================================" << std::endl;
	for (int i=0; i<data.f.nb_nodes(); i++)
		os << "  " << i << '\t' << data[i] << '\t' << data.f.node(i) << std::endl;
	os << "================================================" << std::endl;
	return os;
}

} /* namespace ibex */

#endif /* __EXPR_DATA_H__ */
