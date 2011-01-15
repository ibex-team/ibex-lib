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

#include "IbexFunction.h"

namespace ibex {

/*=========================================================================================================*/
/*                                                  ASSIGN                                                 */
/*=========================================================================================================*/

Assign::Assign(const ExtendedSymbol& symbol, const Expr& right) : //Expr(right.depth+1, right.size+2, right.dim), 
  symbol(symbol), right(right) { 

  if (! (symbol.dim == right.dim)) {
    stringstream s;
    s << "Dimension of \"" << symbol << "\" does not match the assigned expression dimension";
    throw NonRecoverableException(s.str());
  }
  
  EvaluatorFactory fac(right);
  fac.build(&evl);  
}

void Assign::forward(Space& space) const {
//   cout << "forward " << *this;
  evl.forward(space);
  if (symbol.scalar()) {
    //cout << "  " << space.domain(symbol.key) << " ^" << evl.output() << endl;
    space.domain(symbol.key) &= evl.output();
  } else {
    int rows=symbol.dim.size2();
    int cols=symbol.dim.size3();
    int num=symbol.key;
    for (int i=0; i<rows; i++)       
      for (int j=0; j<cols; j++)
	if (!(space.domain(num++) &= evl.m_output()(i+1,j+1))) throw EmptyBoxException();
  }
}

void Assign::backward(Space& space) const {
  if (symbol.scalar()) {
    INTERVAL x = space.domain(symbol.key);
    if (evl.output().included(x)) return; // useless to go backward.
    if (! (evl.output() &= x)) throw EmptyBoxException();
    evl.backward(space);
  } else {
    int rows=symbol.dim.dim2;
    int cols=symbol.dim.dim3;
    INTERVAL_MATRIX X(rows,cols);
    int num=symbol.key;
    bool included=true;
    for (int i=0; i<rows; i++)       
      for (int j=0; j<cols; j++) {
	X(i+1,j+1) = space.domain(num++);
	included &= evl.m_output()(i+1,j+1).included(X(i+1,j+1));
      }
    if (included) return;
    if (! (evl.m_output() &= X)) throw EmptyBoxException();
    evl.backward(space);
  }
}

/*void Assign::gradient(Space& space) const {
  NonRecoverableException("Gradient not implemented yet with user-defined functions.");
}*/

ostream& operator<<(ostream& os, const Assign& a) {
  return os << a.symbol << ":=" << a.right;
}

/*=========================================================================================================*/
/*                                                 FUNCTION                                                */
/*=========================================================================================================*/

Function::~Function() {  
  free((char*) name); 
  for (int i=0; i<code_size; i++) {
    delete code[i];
  }
  delete[] code;
}

ostream& operator<<(ostream& os, const Function& f) {
  int n = f.nb_inputs();
  os << f.name << ":";
  if (n>1) os << "(";
  for (int i=0; i<n; i++) {
    os << f.env.order2info[i]->name;
    if (i<n-1) os << ",";
  }
  if (n>1) os << ")";
  os << " -> " << f.env.order2info[n]->name << " {" << endl;

  for (int i=0; i<f.code_size; i++) {
    os << "  " << *f.code[i] << ";\n";
  }
  os << "}";
  return os;
}

Dim Function::arg_dim(int i) const {
  return env.order2info[i]->dim;
}

/** Number of input arguments (or: dimension of the input variable vector). Warning, this may 
 * not equal input_size()
 * since some input variable may be vectors or matrices. */
unsigned int Function::nb_inputs() const {
  return n;
}

/** Returns the total number of entries (i.e., keys) 
 * of the input. */
int Function::input_size() const {
  // the key of the output variable is also 
  // the number of keys allocated for the input.
  return env.order2info[n]->key; 
}

/** Returns the output size (or dimension) */
int Function::output_size() const {
  // the key of the output variable is also 
  // the number of keys allocated for the input.
  return first_tmp_key-input_size();
}

/** Returns the total number of temporary variables (i.e., keys). */
int Function::temp_size() const {
  return env.nb_keys() - first_tmp_key;
}

/** Returns the dimension of the image (the output). */
Dim Function::img_dim() const {
  return env.order2info[n]->dim;
}

/** Returns:
 * <ul>
 *  <li>0 - if \a key is the key of an input symbol</li>
 *  <li>1 - if \a key is the key of an output symbol</li>
 *  <li>2 - if \a key is the key of a temporary symbol</li>
 * </ul>
 */
int Function::status(int key) const {
  if (key < env.order2info[n]->key) return 0;
  if (env.order2info.size()<=(n+1) || key < env.order2info[n+1]->key) return 1;
  return 2;
}
  


/** Contract y under the constraint y=f(x) with a forward evaluation. */
void Function::forward(Space& space) const {
  for (int tmp=first_tmp_key; tmp<env.nb_keys(); tmp++)
    space.domain(tmp) = INTERVAL(BiasNegInf,BiasPosInf);

  for (int i=0; i<code_size; i++) {
    code[i]->forward(space);
//     cout << "                 ---------- code[" << i << "] ---------------\n";
//     cout << code[i]->evl << endl;
  }

//   cout << "Forward output result:" << space.domain(env.order2info[n]->key) << endl;
}

/** Contract x under the constraint y=f(x) with a backward evaluation. */
void Function::backward(Space& space) const {
  for (int i=code_size-1; i>=0; i--)
    code[i]->backward(space);
}

/** Computes the gradient. */
void Function::gradient(Space& space) const {
  for (int tmp=first_tmp_key; tmp<env.nb_keys(); tmp++)
    space.domain(tmp) = INTERVAL(BiasNegInf,BiasPosInf);

  for (int i=0; i<code_size; i++)
    gradient(space);
}

/*=========================================================================================================*/
/*                                             FUNCTION FACTORY                                            */
/*=========================================================================================================*/

/* Add an input argument. */
const Symbol& FunctionFactory::add_input(const char* name, Dim d) const {
  if (d.dim1>0) throw NonRecoverableException(string("Function argument \"") + name +"\" cannot be a three-dimensional vector");
  func->n++;
  return func->env.add_symbol(name, d);
}

/** Returns the symbol expression corresponding to an argument.      
 *  In return, status is:
 *  0 - if the name is an input symbol
 *  1 - if the name is an output symbol
 *  2 - if the name is a tmp symbol
 */
const Symbol& FunctionFactory::symbol_expr(const char* base_name, int& status) const {
  const Symbol& s=func->env.symbol(base_name);
  status = func->status(s.key);
  return s;
}

/** Sets the output argument of the function.
 * This function must be called after all inputs have been added. */
const Symbol& FunctionFactory::set_output(const char* name, Dim d) {
  if (d.dim1>0) throw NonRecoverableException(string("Output argument \"") + name +"\" cannot be a three-dimensional vector");
  const Symbol& output=func->env.add_symbol(name, d);
  func->first_tmp_key=func->env.nb_keys();
  return output;
}

/** Add a temporary symbol for the function code.
 * This function must be called after output and all inputs have been set. */
const Symbol& FunctionFactory::add_tmp_symbol(const char* name, Dim d) {
  if (d.dim1>0) throw NonRecoverableException(string("Function temporary symbol \"") + name +"\" cannot be a three-dimensional vector");
  return func->env.add_symbol(name, d);
}
  
/** Add an assignemnt x:=expr in the code of the function.
 * Symbol x must be either a temporary symbol or the output symbol, with
 * possibly some subscripts (e.g., x[1][2]:=expr). */
void FunctionFactory::add_assign(const Assign& assign) {
  /*    const char* name=assign.symbol.name; */
  /*     if (func->status(assign.symbol.key)==2) { */
  /*       if (tmp_assigns.used[name]) */
  /* 	tmp_assigns[name]=assign.right.id; */
  /*       else tmp_assigns.insert_new(name, assign.right.id); */
  /*     } */
  code.push_back(&assign);
}


const Function& FunctionFactory::build_func() {
  func->code = new const Assign*[code.size()];
  func->code_size = code.size();
  for (unsigned int i=0; i<code.size(); i++) {
    func->code[i] = code[i];
  }
  return *func;
}


} // end namespace
