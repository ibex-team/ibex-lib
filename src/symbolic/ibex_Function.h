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

/**
 * \ingroup symbolic
 * \brief Function (x->f(x) where f(x) is the DAG of an arithmetical expression).
 *
 * Every expression in ibex (like x^2+y^2) is considered as a function,
 * (here: (x,y)->x^2+y^2) where the order of the arguments ("x" and "y")
 * is simply the order of symbol declaration.
 *
 * <p>
 * A function is build in three steps:<br>
 * 1. Create the function object:
 * \code
 * Function f;
 * \endcode
 * 2. Add the symbols (arguments of the function), and
 * get a reference to them for future usage.
 * \code
 *  const Symbol& x=f.add_symbol("x");
 *  const Symbol& y=f.add_symbol("y");
 *  \endcode
 *  3. Build the expression and set it as the "return" value of the function:
 *  \code
 *   f.set_expr(pow(x,2)+pow(y,2));
 *  \endcode
 */
class Function {
public:
	static const char* DEFAULT_NAME;

	/**
	 * \brief Creates a function y=f(x).
	 */
	Function(const ExprSymbol& x, const ExprNode& y, const char* name=DEFAULT_NAME);

	/**
	 * \brief Creates a function y=f(x1,x2).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprNode& y, const char* name=DEFAULT_NAME);

	/**
	 * \brief Creates a function y=f(x1,x2,x3).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprNode& y, const char* name=DEFAULT_NAME);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprNode& y, const char* name=DEFAULT_NAME);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprNode& y, const char* name=DEFAULT_NAME);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprNode& y, const char* name=DEFAULT_NAME);

	/**
	 * \brief Creates a new (anonymous) function y=f(x1,...,xn).
	 */
	Function(const Array<const ExprSymbol>& x, const ExprNode& y, const char* name=DEFAULT_NAME);

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
	 * \brief Transform f into (f_1,...f_n)
	 *
	 * The vector-valued function f is transformed
	 * into n real-valued functions f_1, ... f_n
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
	Function* separate() const;

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
	 * \brief Return the total input size.
	 *
	 * This size is the sum of the numbers of components, for
	 * all symbols.
	 */
	int input_size() const;

	/**
	 * \brief Return true if the symbol (or argument) \a name is
	 * actually used in the function.
	 *
	 * In other words, return true if the output of the function
	 * may depend on the argument named \a name.
	 */
	bool used(const char* name) const;

	/**
	 * \brief Return true if the ith argument is used in the function.
	 *
	 * #see used(const char*) const;
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
	 * \brief Calculate the gradient of f
	 * \pre f must be real-valued
	 */
	void gradient(const IntervalVector& x, IntervalVector& g) const;

	/**
	 * \brief Calculate the Jacobian matrix of f
	 * \pre f must be vector-valued
	 */
	void jacobian(const IntervalVector& x, IntervalMatrix& g) const;

	/**
	 * \brief Calculate the Hansen matrix of f
	 * \pre f must be vector-valued
	 */
	void hansen_matrix(const IntervalVector& x, IntervalMatrix& h) const;

	/**
	 * \brief Project f(x)=y onto x.
	 */
	void proj(const Domain& y, IntervalVector& x) const;

	/**
	 * \brief Project f(x)=y onto x.
	 */
	void proj(const Interval& y, IntervalVector& x) const;

	/**
	 * \brief Project f(x)=y onto x.
	 */
	void proj(const IntervalVector& y, IntervalVector& x) const;

	/**
	 * \brief Project f(x)=y onto x.
	 */
	void proj(const IntervalMatrix& y, IntervalVector& x) const;

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

private:
	Function();
	void init(const Array<const ExprSymbol>& x, const ExprNode& y);
	void add_symbol(const ExprSymbol* x);

	/*
	 * \brief Apply default Decoration (and compile) the function.
	 *
	 * Declared "const" because the decoration is
	 * not considered as part of the definition of the function.
	 */
	void decorate() const;

	const ExprNode* root;                       // the root node
	std::vector<const ExprSymbol*> order2info;  // to retrieve symbol (node)s by appearing order.
	std::vector<bool> is_used;                  // tells whether the i^th symbol is used.
	std::vector<const ExprNode*> exprnodes;     // all the nodes
	SymbolMap<const ExprSymbol*> id2info;       // to retrieve a symbol node from its name.
	int key_count;                              // count the number of symbols


	bool __all_symbols_scalar;                  // true if all symbols are scalar

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

inline int Function::nb_symbols() const {
	return key_count;
}

inline bool Function::used(const char* name) const {
	return used(id2info[name]->key);
}

inline bool Function::used(int i) const {
	return (root!=NULL && is_used[i]);
}

inline const ExprSymbol& Function::symbol(int i) const {
	return *order2info[i];
}

inline const char* Function::symbol_name(int i) const {
	return order2info[i]->name;
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
	return eval_domain(box).v();
}

inline IntervalMatrix Function::eval_matrix(const IntervalVector& box) const {
	return eval_domain(box).m();
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

} // namespace ibex
#endif // _IBEX_FUNCTION_H_
