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
#include "ibex_Fnc.h"
#include "ibex_CompiledFunction.h"
#include "ibex_Decorator.h"
#include "ibex_Array.h"
#include "ibex_SymbolMap.h"
#include "ibex_ExprSubNodes.h"
#include <stdarg.h>

namespace ibex {

class System;

/**
 * \ingroup function
 * \brief Symbolic function (x->f(x) where f(x) is the DAG of an arithmetical expression).
 *
 * Every expression in ibex (like x^2+y^2) is considered as a function,
 * (here: (x,y)->x^2+y^2) where the order of the arguments ("x" and "y")
 * is simply the order of symbol declaration.
 *
 * <br>
 * We distinguish <i>arguments</i> from <i>variables</i>. For instance, if
 * f:(x,y)->x[0]+y where x is a vector of 9 components, the functions has
 * 2 arguments and 10 variables.
 *
 * Note that contrary to the input of a function, there
 * is a unique output and if this output is a vector/matrix, all
 * the components have the same dimension.
 *
 * As a vector-valued function (#ibex::Fnc), if f is real-valued, the
 * output vector has 1 component. If f is a m*n matrix function,
 * the output vector has m*n components.
 *
 *
 */
class Function : public Fnc {
public:
	/**
	 * \brief Creates a function y=f(x).
	 *
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
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprSymbol& x19,  const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprSymbol& x19, const ExprSymbol& x20, const ExprNode& y, const char* name=NULL);

	/**
	 * \brief Creates a new (anonymous) function y=f(x1,...,xn).
	 */
	Function(const Array<const ExprSymbol>& x, const ExprNode& y, const char* name=NULL);

	/**
	 * \brief Creates a function y=f(x).
	 *
	 * The parameter x may not only contain the name
	 * of the function argument but also the dimension of the symbol
	 * (like for the arguments of a Minibex function). E.g.:
	 *
	 * Function f("x[2]","x(1)+x(2)");
	 *
	 * \warning: Note that the Minibex convention (not C++) is applied for indices!
	 *
	 */
	Function(const char* x1, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* y);

	/**
	 * \brief Load a function from a file.
	 */
	Function(const char* filename);

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

	typedef enum { COPY, DIFF } copy_mode;

	/**
	 * \brief Build a function from another function.
	 *
	 * The new function can either be a clone of the function
	 * in argument (COPY mode), or its differential (DIFF mode).
	 *
	 * \param mode: either Function::COPY or Function::DIFF.
	 *
	 * The resulting function is independent from *this
	 * (no reference shared). In particular, in copy mode,
	 * The DAG is entirely duplicated.
	 *
	 * However, decoration (considered as temporary data) is not copied.
	 * The resulting function is not decorated.
	 */
	Function(const Function&, copy_mode mode=COPY);

	/**
	 * \brief Differentiate this function.
	 */
	const Function& diff() const;

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

	/**
	 * \brief Return the number of arguments.
	 *
	 * \note The number of variables returned by nb_var()
	 * is the sum of the number of components of each argument.
	 *
	 */
	int nb_arg() const;

	/**
	 * \brief Return true if the ith variable is used in the function.
	 *
	 * \warning The function is seen as a function from R^n to R^m. So, the
	 * ith variable is <b>not</b> the ith symbol.
	 *
	 */
	bool used(int i) const;

	/**
	 * \brief Return the current number of nodes in the DAG.
	 */
	int nb_nodes() const;

	/**
	 * \brief Return the ith node.
	 */
	const ExprNode& node(int i) const;

	/**
	 * \brief Return the arguments.
	 */
	const Array<const ExprSymbol>& args() const;

	/**
	 * \brief Return the name of the ith argument.
	 *
	 * Corresponds to the ith parameter of the function.
	 */
	const char* arg_name(int i) const;

	/**
	 * Return the ith argument.
	 */
	const ExprSymbol& arg(int i) const;

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
	 * V must be a subclass of FwdAlgorithm.
	 *
	 * Note that the type V is just passed in order to have static linkage.
	 */
	template<class V>
	ExprLabel& forward(const V& algo) const;

	/**
	 * \brief Run a backward algorithm.
	 *
	 * V must be a subclass of FwdAlgorithm.
	 *
	 * Note that the type V is just passed in order to have static linkage.
	 */
	template<class V>
	void backward(const V& algo) const;

	/**
	 * \brief True if all the arguments are scalar
	 *
	 * Useful for various code optimization.
	 */
	bool all_args_scalar() const;

	/**
	 * \brief Calculate f(box) using interval arithmetic.
	 */
	Domain& eval_domain(const IntervalVector& box) const;

	/**
	 * \brief Calculate f(box) using affine arithmetic.
	 */
	Domain& eval_affine2_domain(const IntervalVector& box) const;

	/**
	 * \brief Calculate f(box) using affine arithmetic.
	 *
	 * The resulting affine form is stored in \a result.
	 */
	Domain& eval_affine2_domain(const IntervalVector& box, Affine2Domain& result) const;

	/**
	 * \brief Calculate f(box) using interval arithmetic.
	 *
	 * \pre f must be real-valued
	 */
	Interval eval(const IntervalVector& box) const;

	/**
	 * \brief Calculate f(box) using affine arithmetic.
	 *
	 */
	Interval eval_affine2(const IntervalVector& box) const;

	/**
	 * \brief Calculate f(box) using affine arithmetic.
	 *
	 * The resulting affine form is stored in \a affine.
	 */
	Interval eval_affine2(const IntervalVector& box, Affine2& result) const;

	/**
	 * \brief Calculate f(box) using interval arithmetic.
	 *
	 * \pre f must be vector-valued
	 */
	virtual IntervalVector eval_vector(const IntervalVector& box) const;

	/**
	 * \brief Calculate f(box) using affine arithmetic.
	 *
	 * \pre f must be vector-valued
	 */
	IntervalVector eval_affine2_vector(const IntervalVector& box) const;

	/**
	 * \brief Calculate f(box) using affine arithmetic.
	 *
	 * The resulting affine form is stored in \a affine.
	 * \pre f must be vector-valued
	 */
	IntervalVector eval_affine2_vector(const IntervalVector& box, Affine2Vector& affine) const;

	/**
	 * \brief Calculate f(x) using interval arithmetic.
	 *
	 * \pre f must be matrix-valued
	 */
	IntervalMatrix eval_matrix(const IntervalVector& x) const;

	/**
	 * \brief Calculate f(box) using affine arithmetic.
	 *
	 * \pre f must be matrix-valued
	 */
	IntervalMatrix eval_affine2_matrix(const IntervalVector& box) const;

	/**
	 * \brief Calculate f(box) using affine arithmetic.
	 *
	 * The resulting affine form is stored in \a affine.
	 * \pre f must be matrix-valued
	 */
	IntervalMatrix eval_affine2_matrix(const IntervalVector& box, Affine2Matrix& affine) const;

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
	 *
	 * \param x - the input box
	 * \param J - where the Jacobian matrix has to be stored (output parameter).
	 *
	 * \pre f must be vector-valued
	 */
	virtual void jacobian(const IntervalVector& x, IntervalMatrix& J) const;

	/**
	 * \brief Calculate the Jacobian matrix of f
	 * \pre f must be vector-valued
	 *
	 * \pre f must be vector-valued
	 */
	IntervalMatrix jacobian(const IntervalVector& x) const;

	/**
	 * \brief Calculate the Hansen matrix of f
	 *
	 * \pre f must be vector-valued
	 */
	void hansen_matrix(const IntervalVector& x, IntervalMatrix& h) const;

	/**
	 * \brief Contract x w.r.t. f(x)=y.
	 * \throw EmptyBoxException if x is empty.
	 */
	void backward(const Domain& y, IntervalVector& x) const;

	/**
	 * \brief Contract x w.r.t. f(x)=y.
	 * \throw EmptyBoxException if x is empty.
	 */
	void backward(const Interval& y, IntervalVector& x) const;

	/**
	 * \brief Contract x w.r.t. f(x)=y.
	 * \throw EmptyBoxException if x is empty.
	 */
	void backward(const IntervalVector& y, IntervalVector& x) const;

	/**
	 * \brief Contract x w.r.t. f(x)=y.
	 * \throw EmptyBoxException if x is empty.
	 */
	void backward(const IntervalMatrix& y, IntervalVector& x) const;

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
	 * \brief The domains of the arguments.
	 *
	 */
	mutable Array<Domain> arg_domains;

	/*
	 * \brief The derivative label of the arguments.
	 *
	 * \note The structure is initialized by #ibex::GradDecorator.
	 */
	mutable Array<Domain> arg_deriv;

	/*
	 * \brief The domains of the arguments.
	 *
	 */
	mutable Array<Affine2Domain> arg_af2;

	/**
	 * Number of used variables
	 */
	int nb_used_vars;

	/**
	 * Array of used variables (indices in declaration order)
	 */
	int* used_var;


private:

	void add_symbol(const ExprSymbol* x);

	void build_from_string(const Array<const char*>& x, const char* y);

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
	ExprSubNodes exprnodes;                         // all the nodes (of x and f(x))
	//SymbolMap<const ExprSymbol*> id2info;       // to retrieve a symbol node from its name.
	int key_count;                              // count the number of arguments

	Function* comp;                             // the components. ==this if output_size()==1.

	bool __all_symbols_scalar;                  // true if all symbols are scalar

	Function& operator=(const Function&);       // forbidden

	// if at some point, symbolic differentiation is needed for this function,
	// we store the resulting function for future usage.
	Function* df;
public:

	/**
	 * \brief Apply this function to the argument
	 *
	 * Works only if it is a unary function.
	 */
	const ExprApply& operator()(const ExprNode& arg1) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if it is a binary function.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if it is a ternary function.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has four arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has five arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has six arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 7 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 8 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 9 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 10 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 11 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 12 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 13 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 14 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10,const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 15 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 16 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 17 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 18 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17, const ExprNode& arg18) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 19 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17, const ExprNode& arg18, const ExprNode& arg19) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 20 arguments.
	 */
	const ExprApply& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17, const ExprNode& arg18, const ExprNode& arg19, const ExprNode& arg20) const;

	/**
	 * \brief Apply this function to the arguments
	 */
	const ExprApply& operator()(const ExprNode** arg) const;

	/**
	 * \brief Apply this function to the arguments
	 */
	const ExprApply& operator()(const std::vector<const ExprNode*>& arg) const;

};

std::ostream& operator<<(std::ostream&, const Function&);

/*================================== inline implementations ========================================*/

inline const Function& Function::diff() const {
	return *(df ? df : (((Function*&) df) = new Function(*this,DIFF)));
}

inline Function& Function::operator[](int i) {
	return comp[i];
}

inline Function& Function::operator[](int i) const {
	return comp[i];
}

inline int Function::nb_arg() const {
	return key_count;
}

inline bool Function::used(int i) const {
	return (root!=NULL && is_used[i]);
}

inline const Array<const ExprSymbol>& Function::args() const {
	return symbs;
}

inline const ExprSymbol& Function::arg(int i) const {
	return symbs[i];
}

inline const char* Function::arg_name(int i) const {
	return symbs[i].name;
}

inline int Function::nb_nodes() const {
	return exprnodes.size();
}

inline const ExprNode& Function::node(int i) const {
	return exprnodes[i];
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

inline bool Function::all_args_scalar() const {
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
		IntervalMatrix M(image_dim(),1);
		M.set_row(0,eval_domain(box).v());
		return M;
	}
	case Dim::COL_VECTOR : {
		IntervalMatrix M(1,image_dim());
		M.set_col(0,eval_domain(box).v());
		return M;
	}
	case Dim::MATRIX: return eval_domain(box).m();
	default : {
		assert(false);
	}
	}
}


inline void Function::backward(const Interval& y, IntervalVector& x) const {
	backward(Domain((Interval&) y),x); // y will not be modified
}

inline void Function::backward(const IntervalVector& y, IntervalVector& x) const {
	assert(expr().dim.is_vector());
	backward(Domain((IntervalVector&) y, expr().dim.type()==Dim::ROW_VECTOR),x); // y will not be modified
}

inline void Function::backward(const IntervalMatrix& y, IntervalVector& x) const {
	backward(Domain((IntervalMatrix&) y),x); // y will not be modified
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

// never understood why we have to do this explictly in c++
inline IntervalMatrix Function::jacobian(const IntervalVector& x) const {
	return Fnc::jacobian(x);
}

// never understood why we have to do this explictly in c++
inline void Function::hansen_matrix(const IntervalVector& x, IntervalMatrix& h) const {
	Fnc::hansen_matrix(x,h);
}

} // namespace ibex
#endif // _IBEX_FUNCTION_H_
