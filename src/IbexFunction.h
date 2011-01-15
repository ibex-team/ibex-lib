/*---------------------------------------------------------------------------------
 * 
 * Function
 * -----------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#ifndef _IBEX_FUNCTION_H
#define _IBEX_FUNCTION_H

#include "IbexExpr.h"
#include "IbexEnv.h"
#include "IbexSpace.h"
#include "IbexEvaluatorFactory.h"

namespace ibex {

class FunctionFactory;
  
/** \ingroup exprgroup
 *
 * \brief Assignment expression (y=f(x))
 *
 * Assignment is a particular class of equalities where the left part is necessarily a symbol. 
 * This class is only used inside the definition of a function.
 * The main difference between an equality constraint is in the evaluation mechanism. An
 * assignment has "inputs " and one "output" (the symbol \a y on the left side).
 * This means that the \link ibex::Assign::forward(Space&) const forward \endlink function 
 * does not intersect the domain of the left symbol (just as an intermediate node 
 * inside an expression), since this is the output of the expression.
 * Furthermore, the order of assignments in a function definition plays an important
 * role since evaluation through assignments should amount to the evaluation of a DAG.
 *
 * \author Gilles Chabert
 * \date December 2007
 */
class Assign { 

 public:
  /** Create a new assignment \a symbol=\a right. */
  Assign(const ExtendedSymbol& symbol, const Expr& right);

  /** Forward evaluation. */
  void forward(Space& space) const;

  /** Backward evaluation. May throw #ibex::EmptyBoxException. */
  void backward(Space& space) const;

  /** The left side symbol \a y. */
  const ExtendedSymbol& symbol;

  /** The right side expression \a f(x). */
  const Expr& right;
  
  /** Evaluator */
  Evaluator evl;
};

ostream& operator<<(ostream& os, const Assign& a);      
 
/**\ingroup exprgroup
 *
 * \brief Function declaration expression
 *
 * Example of a function definition in Quimper:
 *
 * \code
 * function y=f(x,A[2][4],B[4][3]) 
 *   W1 = A*B;
 *   w2 = x+1;
 *   w3 = w2*W1[1][1]-w2;
 *   y = w3+w3;
 * end
 * \endcode
 *
 * \note 
 * <ul><li>x,a,B, are <b>input</b> symbols
 * <li>y is the <b>output</b> symbol
 * <li>w1,W2 and w3 are <b>temporary</b> symbols
 * \author Gilles Chabert
 * \date December 2007
 *
 */
class Function {
 public:

  /** Return the dimension of the ith input argument. 
   * \pre i< #nb_inputs(). */
  Dim arg_dim(int i) const;

  /** Number of input arguments (or: dimension of the input variable vector). Warning, this may 
   * not equal input_size()
   * since some input variable may be vectors or matrices. */
  unsigned int nb_inputs() const;

  /** Return the total number of entries (i.e., keys) 
   * of the input. */
  int input_size() const;

  /** Return the output size (or dimension) */
  int output_size() const;

  /** Return the total number of temporary variables (i.e., keys). */
  int temp_size() const;

  /** Return the dimension of the image (the output). */
  Dim img_dim() const;

  /** Return:
   * <ul>
   *  <li>0 - if \a key is the key of an input symbol</li>
   *  <li>1 - if \a key is the key of an output symbol</li>
   *  <li>2 - if \a key is the key of a temporary symbol</li>
   * </ul>
  */
  int status(int key) const;
  
  /* Return the maximum depth of the DAG. */
/*   int depth() const { */
/*     int max_depth=0; */
/*     for (int i=0; i<code_size; i++) */
/*       if (code[i]->right.depth > max_depth) max_depth = code[i]->right.depth; */
/*     return max_depth; */
/*   } */
  
  /** Deletes this instance. */
  ~Function();

  /** Contract y under the constraint y=f(x) with a forward evaluation. */
  void forward(Space& space) const;

  /** Contract x under the constraint y=f(x) with a backward evaluation. */
  void backward(Space& space) const;

  /** Computes the gradient of f on a space.
   * The result is stored in the #ibex::Entity::deriv fields. 
   * \throw #ibex::NotDifferentiableException, 
   *        #ibex::UnboundedResultException, 
   *        #ibex::EmptyBoxException */
  void gradient(Space& space) const;

  /** The function name (the symbol). */
  const char* name;            

  /** The function local environment (the one that contains input, temporary and output arguments). */
  Env env;                     

  /** The code of the function. This array of assignments   
   * represents how to compute output arguments from the input arguments.
   * This structure also corresponds to the DAG of the function. */
  const Assign** code;  

  /** Number of assignments in the code. */
  int code_size;

  int first_tmp_key;

 private:

  unsigned int n;                 

  friend class FunctionFactory;

  /* Builds a function with the given name. 
   * \param name - the function name (the identifier used in expressions),
   * \param n - the number of input arguments.
   * Once this constructor is called,
   * three steps are left to be performed (a factory might be introduced in the future):
   *   1- Add input arguments into #env (the local environment) in their 
   *      declaration order. Set the appropriate dimensions.
   *   2- Add the output argument in #env, with the appropriate dimension. This symbol will
   * be the last of the environment. 
   *   3- Push the sequence of assignments with the #add_assign function. */
  Function(const char* name, int n) : name(strdup(name)), n(n) { }

  Function(const Function& f); // copy constructor forbidden
};

/** \ingroup exprgroup
 *
 * \brief Function factory.
 *
 * \author Gilles Chabert
 * \date December 2007
 */
class FunctionFactory {

 public:  

  /* Create a factory for the function \a name. */
  FunctionFactory(const char* name) : func(new Function(name,0)) { }

  /* Add an input argument. */
  const Symbol& add_input(const char* name, Dim d=Dim(0,0,0)) const;

  /** Return the symbol expression corresponding to an argument.      
   *  In return, status is:
   *  0 - if the name is an input symbol,
   *  1 - if the name is an output symbol,
   *  2 - if the name is a tmp symbol. */
  const Symbol& symbol_expr(const char* base_name, int& status) const;

  /** Sets the output argument of the function.
    * This function must be called after all inputs have been added. */
  const Symbol& set_output(const char* name, Dim d=Dim(0,0,0));

  /** Add a temporary symbol for the function code.
   * This function must be called after output and all inputs have been set. */
  const Symbol& add_tmp_symbol(const char* name, Dim d=Dim(0,0,0));
  
  /** Add an assignemnt x:=expr in the code of the function.
   * Symbol x must be either a temporary symbol or the output symbol, with
   * possibly some subscripts (e.g., x[1][2]:=expr). */
  void add_assign(const Assign& assign);
  
  /** Builds the function and returns a reference to it. */
  const Function& build_func();

  /** Return the environment of the function to be built. */
  Env& func_env() { return func->env; }

 private:

  Function* func;

  vector<const Assign*> code;
};
  
ostream& operator<<(ostream&, const Function& f);


} // end namespace

#endif
