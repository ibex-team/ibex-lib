/* ============================================================================
 * I B E X - Functions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 5, 2012
 * ---------------------------------------------------------------------------- */

#ifndef _IBEX_FUNCTION_H_
#define _IBEX_FUNCTION_H_

#include "ibex_Expr.h"
#include "ibex_CompiledFunction.h"
#include "ibex_Decorator.h"
#include "ibex_Array.h"

#include <stdarg.h>

namespace ibex {

class System;

/**
 * \ingroup symbolic
 * \brief Function (x->f(x) where f(x) is the DAG of an arithmetical expression).
 *
 * Every expression in ibex (like x^2+y^2) is considered as a function,
 * (here: (x,y)->x^2+y^2) where the order of the arguments ("x" and "y")
 * is simply the order of symbol declaration.
 */
class Function {
public:
	/**
	 * \brief Creates a function y=f(x).
	 */
	Function(const ExprSymbol& x, const ExprNode& y, const char* name=NULL);

	/**
	 * \brief Creates a function y=f(x1,x2).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprNode& y, const char* name=NULL);

	/**
	 * \brief Creates a function y=f(x1,x2,x3).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprNode& y, const char* name=NULL);

	/**
	 * \brief Creates a new (anonymous) function y=f(x1,...,xn).
	 */
	Function(const Array<const ExprSymbol>& x, const ExprNode& y, const char* name=NULL);

	/**
	 * \brief Build an uninitialized function.
	 *
	 * A call to #init(const Array<const ExprSymbol>&, const ExprNode&) must follow
	 * to complete the intialization. This constructo+init can be
	 * used when a function has to be built and initialized in two distinct steps.
	 */
	Function();

	/**
	 * \brief Initialize this function (set the "x" and the "y").
	 */
	void init(const Array<const ExprSymbol>& x, const ExprNode& y);

	/**
	 * \brief Delete the function.
	 *
	 * This will delete all the associated expression nodes (including symbols).
	 */
	~Function();

	/**
	 * \brief Duplicate this function.
	 *
	 * The resulting function is independent from *this
	 * (no reference shared). The DAG is entirely duplicated.
	 *
	 * Decoration is not copied.
	 * The resulting function is not decorated.
	 */
	Function(const Function&);

	/**
	 * \brief Return the total input size.
	 *
	 * This size is the sum of the numbers of components, for
	 * all symbols.
	 *
	 * \see #nb_symbols() const.
	 */
	int input_size() const;

	/**
	 * \brief Number of components of f
	 *
	 * If f is a real-valued function, the result is 1.
	 * If f is a m*n matrix function, the result is m*n.
	 *
	 * Note that contrary to the input of a function, there
	 * is a unique output and if this output is a vector, all
	 * the components have the same dimension.
	 */
	int output_size() const;

	/**
	 * \brief Return the ith component of f.
	 *
	 * The vector-valued function f is also
	 * n real-valued functions f_1, ... f_n
	 * that can be used independently.
	 *
	 * Of course the list of arguments "x" is
	 * the same for each component. For instance
	 *
	 * (x,y,z)->(x+y,z-x) is transformed into:  <br>
	 *    { (x,y,z)->x+y ; (x,y,z)->z-y } <br>
	 *
	 * *not* in:   <br>
	 *    { (x,y)->x+y ; (z,y)->z-y }
	 */
	Function& operator[](int i);

	/**
	 * \brief Return the ith component of f.
	 * \see operator[](int).
	 */
	Function& operator[](int i) const;

	/*
	 * \brief Create a new symbol (new argument of the function).
	 *
	 * The name of the symbol is generated automatically.
	 *
	 * By default, the symbol is zero-dimensional (scalar symbol).
	 *
	 */
	//const ExprSymbol& add_symbol(const Dim& dim=Dim(0,0,0));

	/*
	 * \brief Create a new symbol (new argument of the function) with a specific name.
	 *
	 * By default, the symbol is zero-dimensional (scalar symbol).
	 *
	 * The string \a name is duplicated.
	 */
	//const ExprSymbol& add_symbol(const char* name, const Dim& dim=Dim(0,0,0));

	/**
	 * \brief Return the number of symbols (or arguments).
	 */
	int nb_symbols() const;

	/**
	 * \brief Return true if the ith argument is used in the function.
	 *
	 * \warning The function is seen as a function from R^n to R^m. So, the
	 * ith "argument" is not the ith symbol.
	 *
	 */
	bool used(int i) const;

	/*
	 * \brief Set the expression f(x).
	 *
	 * Also calculates which symbols are actually used.
	 */
	//void set_expr(const ExprNode&);

	/**
	 * \brief Return the current number of nodes in the DAG.
	 */
	int nb_nodes() const;

	/**
	 * \brief Return the ith node.
	 */
	const ExprNode& node(int i) const;

	/**
	 * \brief Return the symbols.
	 */
	const Array<const ExprSymbol>& symbols() const;

	/**
	 * \brief Return the name of the ith symbol.
	 *
	 * Corresponds to the ith parameter of the function.
	 */
	const char* symbol_name(int i) const;

	/**
	 * Return the ith symbol.
	 */
	const ExprSymbol& symbol(int i) const;

	/**
	 * \brief Return the expression f(x) of the function.
	 *
	 * Corresponds to the root node.
	 */
	const ExprNode& expr() const;

	/**
	 * \brief Name of the function.
	 *
	 * Null pointer if the function is anonymous.
	 */
	const char* name;


	/**
	 * \brief Run a forward algorithm.
	 *
	 * Run a forward algorithm and
	 * return a reference to the label of the root node.
	 *
	 * V must be a subclass of FwdAlgorithm<T> and the
	 * nodes of this function must have been decorated with T.
	 *
	 * Note that the type V is just passed in order to have static linkage.
	 */
	template<class V>
	ExprLabel& forward(const V& algo) const;

	/**
	 * \brief Run a backward algorithm.
	 *
	 * V must be a subclass of FwdAlgorithm<T> and the
	 * nodes of this function must have been decorated with T.
	 *
	 * Note that the type V is just passed in order to have static linkage.
	 */
	template<class V>
	void backward(const V& algo) const;

	/**
	 * \brief True if all the symbols are scalar
	 *
	 * Useful for various code optimization.
	 */
	bool all_symbols_scalar() const;

	/**
	 * \brief Calculate f(box)
	 */
	Domain& eval_domain(const IntervalVector& box) const;

	/**
	 * \brief Calculate f(box).
	 *
	 * \pre f must be real-valued
	 */
	Interval eval(const IntervalVector& box) const;

	/**
	 * \brief Calculate f(box).
	 *
	 * \pre f must be vector-valued
	 */
	IntervalVector eval_vector(const IntervalVector& box) const;

	/**
	 * \brief Calculate f(x).
	 *
	 * \pre f must be matrix-valued
	 */
	IntervalMatrix eval_matrix(const IntervalVector& x) const;

	/**
	 * \brief Calculate the gradient of f.
	 * \pre f must be real-valued
	 */
	IntervalVector gradient(const IntervalVector& x) const;

	/**
	 * \brief Calculate the gradient of f.
	 *
	 * \param x - the input box
	 * \param g - where the gradient has to be stored (output parameter).
	 *
	 * \pre f must be real-valued
	 */
	void gradient(const IntervalVector& x, IntervalVector& g) const;

	/**
	 * \brief Calculate the Jacobian matrix of f
	 * \pre f must be vector-valued
	 */
	IntervalMatrix jacobian(const IntervalVector& x) const;

	/**
	 * \brief Calculate the Jacobian matrix of f
	 *
	 * \param x - the input box
	 * \param J - where the Jacobian matrix has to be stored (output parameter).
	 *
	 * \pre f must be vector-valued
	 */
	void jacobian(const IntervalVector& x, IntervalMatrix& J) const;

	/**
	 * \brief Calculate the Hansen matrix of f
	 * \pre f must be vector-valued
	 */
	void hansen_matrix(const IntervalVector& x, IntervalMatrix& h) const;

	/**
	 * \brief Project f(x)=y onto x.
	 * \throw EmptyBoxException if x is empty.
	 */
	void proj(const Domain& y, IntervalVector& x) const;

	/**
	 * \brief Project f(x)=y onto x.
	 * \throw EmptyBoxException if x is empty.
	 */
	void proj(const Interval& y, IntervalVector& x) const;

	/**
	 * \brief Project f(x)=y onto x.
	 * \throw EmptyBoxException if x is empty.
	 */
	void proj(const IntervalVector& y, IntervalVector& x) const;

	/**
	 * \brief Project f(x)=y onto x.
	 * \throw EmptyBoxException if x is empty.
	 */
	void proj(const IntervalMatrix& y, IntervalVector& x) const;

	/**
	 * \brief Inner projection f(x)=y onto x.
	 */
	void iproj(const Domain& y, IntervalVector& x) const;

	/**
	 * \brief Inner projection f(x)=y onto x, inflating xin.
	 */
	void iproj(const Domain& y, IntervalVector& x, const IntervalVector& xin) const;

	/**
	 * \brief Inner projection f(x)=y onto x.
	 */
	void iproj(const Interval& y, IntervalVector& x) const;

	/**
	 * \brief Inner projection f(x)=y onto x, inflating xin.
	 */
	void iproj(const Interval& y, IntervalVector& x, const IntervalVector& xin) const;


	CompiledFunction cf; // "public" just for debug

	/*
	 * \brief The domains of the symbols.
	 *
	 */
	mutable Array<Domain> symbol_domains;

	/*
	 * \brief The derivative label of the symbols.
	 *
	 * \note The structure is initialized by #ibex::GradDecorator.
	 */
	mutable Array<Domain> symbol_deriv;

	/**
	 * Number of used inputs
	 */
	int nb_used_inputs;

	/**
	 * Array of used inputs (indices in declaration order)
	 */
	int* used_input;


private:

	void add_symbol(const ExprSymbol* x);

	/*
	 * \brief Apply default Decoration (and compile) the function.
	 *
	 * Declared "const" because the decoration is
	 * not considered as part of the definition of the function.
	 */
	void decorate() const;

	void separate();

	const ExprNode* root;                       // the root node
	Array<const ExprSymbol> symbs;              // to retrieve symbol (node)s by appearing order.
	std::vector<bool> is_used;                  // tells whether the i^th component is used.
	std::vector<const ExprNode*> exprnodes;     // all the nodes
	SymbolMap<const ExprSymbol*> id2info;       // to retrieve a symbol node from its name.
	int key_count;                              // count the number of symbols

	Function* comp;                             // the components. ==this if output_size()==1.

	bool __all_symbols_scalar;                  // true if all symbols are scalar

	Function& operator=(const Function&);       // forbidden

public:

	/**
	 * \brief Apply this function to the argument
	 *
	 * Works only if it is a unary function.
	 */
	const ExprApply& operator()(const ExprNode& arg1);

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if it is a binary function.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2);

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if it is a ternary function.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3);

	/**
	 * \brief Apply this function to the arguments
	 */
	const ExprApply& operator()(const ExprNode** arg);

	/**
	 * \brief Apply this function to the arguments
	 */
	const ExprApply& operator()(const std::vector<const ExprNode*>& arg);

	const ExprApply& operator()(const ExprNode& arg1, const Interval& arg2);
	const ExprApply& operator()(const ExprNode& arg1, const IntervalVector& arg2);
	const ExprApply& operator()(const ExprNode& arg1, const IntervalMatrix& arg2);
	const ExprApply& operator()(const Interval& arg1, const ExprNode& arg2);
	const ExprApply& operator()(const IntervalVector& arg1, const ExprNode& arg2);
	const ExprApply& operator()(const IntervalMatrix& arg1, const ExprNode& arg2);

	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const Interval& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const IntervalVector& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const IntervalMatrix& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const Interval& arg2, const ExprNode& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const Interval& arg2, const Interval& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const Interval& arg2, const IntervalVector& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const Interval& arg2, const IntervalMatrix& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const IntervalVector& arg2, const ExprNode& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const IntervalVector& arg2, const Interval& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const IntervalVector& arg2, const IntervalVector& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const IntervalVector& arg2, const IntervalMatrix& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const IntervalMatrix& arg2, const ExprNode& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const IntervalMatrix& arg2, const Interval& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const IntervalMatrix& arg2, const IntervalVector& arg3);
	const ExprApply& operator()(const ExprNode& arg1, const IntervalMatrix& arg2, const IntervalMatrix& arg3);
	const ExprApply& operator()(const Interval& arg1, const ExprNode& arg2, const ExprNode& arg3);
	const ExprApply& operator()(const Interval& arg1, const ExprNode& arg2, const Interval& arg3);
	const ExprApply& operator()(const Interval& arg1, const ExprNode& arg2, const IntervalVector& arg3);
	const ExprApply& operator()(const Interval& arg1, const ExprNode& arg2, const IntervalMatrix& arg3);
	const ExprApply& operator()(const IntervalVector& arg1, const ExprNode& arg2, const ExprNode& arg3);
	const ExprApply& operator()(const IntervalVector& arg1, const ExprNode& arg2, const Interval& arg3);
	const ExprApply& operator()(const IntervalVector& arg1, const ExprNode& arg2, const IntervalVector& arg3);
	const ExprApply& operator()(const IntervalVector& arg1, const ExprNode& arg2, const IntervalMatrix& arg3);
	const ExprApply& operator()(const IntervalMatrix& arg1, const ExprNode& arg2, const ExprNode& arg3);
	const ExprApply& operator()(const IntervalMatrix& arg1, const ExprNode& arg2, const Interval& arg3);
	const ExprApply& operator()(const IntervalMatrix& arg1, const ExprNode& arg2, const IntervalVector& arg3);
	const ExprApply& operator()(const IntervalMatrix& arg1, const ExprNode& arg2, const IntervalMatrix& arg3);
	const ExprApply& operator()(const Interval& arg1, const Interval& arg3, const ExprNode& arg2);
	const ExprApply& operator()(const Interval& arg1, const IntervalVector& arg3, const ExprNode& arg2);
	const ExprApply& operator()(const Interval& arg1, const IntervalMatrix& arg3, const ExprNode& arg2);
	const ExprApply& operator()(const IntervalVector& arg1, const Interval& arg3, const ExprNode& arg2);
	const ExprApply& operator()(const IntervalVector& arg1, const IntervalVector& arg3, const ExprNode& arg2);
	const ExprApply& operator()(const IntervalVector& arg1, const IntervalMatrix& arg3, const ExprNode& arg2);
	const ExprApply& operator()(const IntervalMatrix& arg1, const Interval& arg3, const ExprNode& arg2);
	const ExprApply& operator()(const IntervalMatrix& arg1, const IntervalVector& arg3, const ExprNode& arg2);
	const ExprApply& operator()(const IntervalMatrix& arg1, const IntervalMatrix& arg3, const ExprNode& arg2);
};

std::ostream& operator<<(std::ostream&, const Function&);

/*================================== inline implementations ========================================*/

inline int Function::output_size() const {
	switch (expr().dim.type()) {
	case Dim::SCALAR: return 1;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: return expr().dim.vec_size();
	case Dim::MATRIX: return expr().dim.dim2*expr().dim.dim3;
	default: assert(false); return 0;
	}
}

inline Function& Function::operator[](int i) {
	return comp[i];
}

inline Function& Function::operator[](int i) const {
	return comp[i];
}

inline int Function::nb_symbols() const {
	return key_count;
}

inline bool Function::used(int i) const {
	return (root!=NULL && is_used[i]);
}

inline const Array<const ExprSymbol>& Function::symbols() const {
	return symbs;
}

inline const ExprSymbol& Function::symbol(int i) const {
	return symbs[i];
}

inline const char* Function::symbol_name(int i) const {
	return symbs[i].name;
}

inline int Function::nb_nodes() const {
	return exprnodes.size();
}

inline const ExprNode& Function::node(int i) const {
	return *exprnodes[i];
}

inline const ExprNode& Function::expr() const {
	return *root;
}

template<class V>
inline ExprLabel& Function::forward(const V& algo) const {
	return cf.forward<V>(algo);
}

template<class V>
inline void Function::backward(const V& algo) const {
	cf.backward<V>(algo);
}

inline bool Function::all_symbols_scalar() const {
	return __all_symbols_scalar;
}

inline Interval Function::eval(const IntervalVector& box) const {
	return eval_domain(box).i();
}

inline IntervalVector Function::eval_vector(const IntervalVector& box) const {
	return expr().dim.is_scalar() ? IntervalVector(1,eval_domain(box).i()) : eval_domain(box).v();
}

inline IntervalMatrix Function::eval_matrix(const IntervalVector& box) const {
	switch (expr().dim.type()) {
	case Dim::SCALAR     :
		return IntervalMatrix(1,1,eval_domain(box).i());
	case Dim::ROW_VECTOR : {
		IntervalMatrix M(output_size(),1);
		M.set_row(0,eval_domain(box).v());
		return M;
	}
	case Dim::COL_VECTOR : {
		IntervalMatrix M(1,output_size());
		M.set_col(0,eval_domain(box).v());
		return M;
	}
	case Dim::MATRIX: return eval_domain(box).m();
	default : assert(false);
	}
}

inline void Function::proj(const Interval& y, IntervalVector& x) const {
	proj(Domain((Interval&) y),x); // y will not be modified
}

inline void Function::proj(const IntervalVector& y, IntervalVector& x) const {
	assert(expr().dim.is_vector());
	proj(Domain((IntervalVector&) y, expr().dim.type()==Dim::ROW_VECTOR),x); // y will not be modified
}

inline void Function::proj(const IntervalMatrix& y, IntervalVector& x) const {
	proj(Domain((IntervalMatrix&) y),x); // y will not be modified
}

inline void Function::iproj(const Interval& y, IntervalVector& x) const {
	iproj(Domain((Interval&) y),x);
}

inline void Function::iproj(const Interval& y, IntervalVector& x, const IntervalVector& xin) const {
	iproj(Domain((Interval&) y),x,xin);
}

inline IntervalVector Function::gradient(const IntervalVector& x) const {
	IntervalVector g(x.size());
	gradient(x,g);
	return g;
}

inline IntervalMatrix Function::jacobian(const IntervalVector& x) const {
	IntervalMatrix J(output_size(),x.size());
	jacobian(x,J);
	return J;
}

} // namespace ibex
#endif // _IBEX_FUNCTION_H_
