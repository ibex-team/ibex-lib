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

namespace ibex {

/**
 * \ingroup level1
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
	/**
	 * \brief Creates a new (anonymous) function.
	 */
	Function();

	/**
	 * \brief Creates a function named \a name.
	 */
	Function(const char* name);

	/**
	 * \brief Transform f into (f_1,...f_n)
	 *
	 * The vector-valued function f is transformed
	 * into n real-valued functions f_1, ... f_n
	 * that can be used independently.
	 *
	 * Of course the dimension of the argument x is
	 * the same for each component. For instance
	 *
	 * (x,y,z)->(x+y,z-x) is transformed into:  <br>
	 *    { (x,y,z)->x+y ; (x,y,z)->z-y } <br>
	 *
	 * *not* in:   <br>
	 *    { (x,y)->x+y ; (z,y)->z-y }
	 */
	Function* separate() const;

	/**
	 * \brief Create a new symbol (argument of the function).
	 */
	const ExprSymbol& add_symbol(const char* name);

	/**
	 * \brief Create a multi-dimensional symbol (new argument of the function).
	 */
	const ExprSymbol& add_symbol(const char* name, const Dim& dim);

	/**
	 * \brief Return the number of symbols (or arguments).
	 */
	int nb_symbols() const;

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


	/**
	 * \brief Set the expression f(x).
	 *
	 * Also calculates which symbols are actually used.
	 */
	void set_expr(const ExprNode&);

	/**
	 * \brief Apply this function to the arguments
	 */
	const ExprApply& operator()(const ExprNode** arg);

	/**
	 * \brief Return the current number of nodes in the DAG.
	 */
	int nb_nodes() const;

	/**
	 * \brief Return the ith symbol.
	 *
	 * Corresponds to the ith parameter of the function.
	 */
	const char* symbol_name(int i) const;

	/**
	 * \brief Return the expression f(x) of the function.
	 */
	const ExprNode& expr() const;

	/**
	 * \brief Name of the function.
	 *
	 * Null pointer if the function is anonymous.
	 */
	const char* name;

private:
	friend class ExprNode;
	void add_node(const ExprNode&);

	const ExprNode* root;                       // the root node
	std::vector<const ExprSymbol*> order2info;  // to retrieve symbol (node)s by appearing order.
	std::vector<bool> is_used;                  // tells whether the i^th symbol is used.
	std::vector<const ExprNode*> exprnodes;     // all the nodes
	SymbolMap<const ExprSymbol*> id2info;       // to retrieve a symbol node from its name.
	int key_count;                              // count the number of symbols
};

std::ostream& operator<<(std::ostream&, const Function&);

/*================================== inline implementations ========================================*/

inline int Function::nb_symbols() const {
	return key_count;
}

inline bool Function::used(const char* name) const {
	return used(id2info.data(name)->key);
}

inline bool Function::used(int i) const {
	return (root!=NULL && is_used[i]);
}

} // namespace ibex
#endif // _IBEX_FUNCTION_H_
