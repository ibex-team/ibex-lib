/*---------------------------------------------------------------------------------
 * 
 * Generator (which basically instanciate iterators)
 * -------------------------------------------------
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

#include <assert.h>

#include "Generator.h"
#include "IbexHC4Revise.h"
#include "IbexPropagation.h"
#include "IbexSequence.h"
#include "IbexSystem.h"
#include "IbexShaving.h"
#include "IbexNewton.h"
#include "IbexBox.h"
#include "IbexBoxNarrow.h"
#include "IbexNVector.h"
#include "IbexOctum.h"
#include "IbexPaver.h" // for Precision contractor
#include "IbexFunction.h"
#include "IbexQInter.h"
#include "IbexIO.h"

namespace ibex {

namespace parser {

extern Env* parser_env;
extern Space* parser_space;

static int generated_line;

IdTable<int> top_level; 

void generate_error (const std::string& msg) {
  throw ibex::SyntaxErrorException(msg, NULL, generated_line);
}

void ExprGenerator::visit(const P_Expr& expr) { 
  generated_line = expr.line;
  expr.acceptVisitor(*this); 
}

void ExprGenerator::visit(const P_Symbol& s) {

  if (scopes.top().is_cst_symbol(s.name)) {
    P_Constant v=scopes.top().cst_value(s.name);    
    visit(scopes.top().cst_value(s.name));
  } else {
    cst_expr = false;
    new_expr = &env.symbol(s.name);
  }
}

void ExprGenerator::visit(const P_IterExpr& expr) {
  expr.acceptVisitor(*this); 
}

void ExprGenerator::visit(const P_IterOp& it_op) {
  visit(it_op.left);
  P_Constant x = value;
  visit(it_op.right);
  P_Constant y = value;
  switch(it_op.op) {
  case ADD : visit(x+y); break;
  case SUB : visit(x-y); break;
  case MUL : visit(x*y); break;
  default : throw NonRecoverableException("Generator: Unexpected operator between iterator expressions");
  }
}

void ExprGenerator::visit(const P_IterCst& it_cst) {
  visit(P_Constant(it_cst.value));
}

void ExprGenerator::visit(const P_IterCstSymbolWithIndex& c) {
  visit(c.subscript);
  visit(c.cst[value._2int()]);
}

void ExprGenerator::visit(const P_IterSymbol& it_sbl) {
  visit(P_Constant(context.data(it_sbl.name)));
//   } catch(UnknownIdException) {
//     throw SyntaxErrorException("Uknown iterator symbol",it_sbl.name,ibex_lineno);
//   }
}

void ExprGenerator::visit(const P_SymboWithIterIndex& e) {
  visit(e.symbol);
    
  if (cst_expr) { // e.symbol is an anonymous constant symbol
    P_Constant x = value;
    visit(e.iter); 
    visit(x[value._2int()]);
  } else {
    const ExtendedSymbol* s = (const ExtendedSymbol*) new_expr;
    visit(e.iter); 
  //delete new_expr;
    cst_expr = false;
    new_expr = &((*s)[value._2int()]);
    //delete s;
  }
}

void ExprGenerator::visit(const P_Constant& c) { 
  cst_expr = true;
  value = c;
  if (value.scalar())     
    new_expr = &Constant::new_scalar(env, value._2itv());
  else if (value.type==_itv_mat_)
    new_expr = &Constant::new_matrix(env, value.M);
  else 
    new_expr = &Constant::new_vector(env, value.v);
}

void ExprGenerator::visit(const P_UnaOpExpr& u)    { 
  visit(u.expr);

  if (cst_expr) {
    switch(u.op) {
    case MINUS    : visit(-value);         break;
    case SIGN     : visit(sign(value));    break;
    case ABS      : visit(abs(value));     break;
    case SQR      : visit(pow(value,P_Constant(2)));   break;
    case SQRT     : visit(sqrt(value));    break;
    case LOG      : visit(log(value));     break;
    case EXP      : visit(exp(value));     break;
    case COS      : visit(cos(value));     break;
    case SIN      : visit(sin(value));     break;
    case TAN      : visit(tan(value));     break;
    case ARCCOS   : visit(arccos(value));  break;
    case ARCSIN   : visit(arcsin(value));  break;
    case ARCTAN   : visit(arctan(value));  break;
    case COSH     : visit(cosh(value));    break;
    case SINH     : visit(sinh(value));    break;
    case TANH     : visit(tanh(value));    break;
    case ARCCOSH  : visit(arccosh(value)); break;
    case ARCSINH  : visit(arcsinh(value)); break;
    case ARCTANH  : visit(arctanh(value)); break;
    case M_TRANS  : visit(transpose(value)); break;
    default       : throw NonRecoverableException("Generator: Unexpected unary operator");
    }
  } else {
    cst_expr = false; 
    new_expr = &UnaOpExpr::new_(u.op, *new_expr); 
  }
}

void ExprGenerator::visit(const P_BinOpExpr& b)    { 
  P_Constant v1(0),v2(0);
  bool cst_expr1, cst_expr2;

  visit(b.left);
  if ((cst_expr1=cst_expr)) v1=value;

  const Expr& left=*new_expr;

  visit(b.right);
  if ((cst_expr2=cst_expr)) v2=value;

  const Expr& right=*new_expr;

  if (cst_expr1 && cst_expr2) {
    switch(b.op) {
    case SUB      : visit(v1-v2); break;
    case ADD      : visit(v1+v2); break;
    case DIV      : visit(v1/v2); break;
    case MUL      : visit(v1*v2); break;
    case MAX      : visit(max(v1,v2)); break;
    case MIN      : visit(min(v1,v2)); break;
    case ARCTAN2  : visit(arctan2(v1,v2)); break;
    default       : throw NonRecoverableException("Generator: Unexpected binary operator");
    }
  } else {
    cst_expr = false;      
    new_expr = &BinOpExpr::new_(left, b.op, right);
  }
}
  
void ExprGenerator::visit(const P_PowerExpr& p)    { 
  visit(p.expr);
  if (cst_expr) {
    visit(pow(value, P_Constant(p.expon)));
  } else {
    cst_expr=false;
    new_expr = &pow(*new_expr, p.expon); 
  }
}

void ExprGenerator::visit(const P_Apply& a) {
  vector<const Expr*> args;
  for (vector<const P_Expr*>::const_iterator it=a.args->begin(); it!=a.args->end(); it++) {
    visit(**it);
    args.push_back(new_expr);
  }
  cst_expr=false;
  new_expr = &Apply::new_(*parser_env, a.f,args);
}

void ExprGenerator::visit(const P_DomainOpExpr& d) { 
  visit(d.expr);

  if (cst_expr) {
    switch(d.op) {
    case INF : visit(inf(value)); break;
    case MID : visit(mid(value)); break;
    case SUP : visit(sup(value)); break;
    default  : throw NonRecoverableException("Generator: Unexpected domain operator");
    }
  } else {
    cst_expr = false; 
    new_expr = &DomainOpExpr::new_(d.op, *new_expr); 
  }
}

/*================================================================================*/

const Function& FuncGenerator::generate(const P_Function& f) {
  FunctionFactory fac(f.name);

  for (vector<const char*>::const_iterator it=f.inputs->begin(); it!=f.inputs->end(); it++) {
    fac.add_input(*it, scopes.top().symbol_dim(*it));
  }

  fac.set_output(f.output, scopes.top().symbol_dim(f.output));

  ExprGenerator g(fac.func_env());

  for (vector<const P_Assign*>::const_iterator it=f.vec->begin(); it!=f.vec->end(); it++) {
    const Expr& right=g.generate((*it)->expr);

    const char* left_sbl =  (*it)->left.base_name();
    if (scopes.top().is_tmp_symbol(left_sbl)) 
      fac.add_tmp_symbol(left_sbl, right.dim);
    
    /* now that the tmp symbol is added into the environment (of the generator),
       we can transform the left part into an ExtendedSymbol. */
    const ExtendedSymbol& left = (const ExtendedSymbol&) g.generate((*it)->left);

    fac.add_assign(*new Assign(left,right));
  }

  return fac.build_func();
}

/*================================================================================*/


const Equality& _2equality(const Constraint& ctr) {
  if (!ctr.is_equality())
    generate_error("equality expected");  
  return (const Equality&) ctr;
}

System _2system(const vector<const Constraint*>& vec, Space& space) {
  
  vector<const ArithConstraint*> vec2;

  try {
    for (vector<const Constraint*>::const_iterator it=vec.begin(); it!=vec.end(); it++) {      
      vec2.push_back(dynamic_cast<const ArithConstraint*>(*it));
    }
  } catch(std::bad_cast) {
    generate_error("System of (in)equations expected");
  }
  return System(vec2, space);    
}

void CtrGenerator::visit(const P_Ctr& ctr) {
  generated_line = ctr.line;
  ctr.accept(*this);
}  

void CtrGenerator::visit(const P_Equality& equ) {
  new_ctr=&Equality::new_(ExprGenerator(env,context).generate(equ.expr));
  parser_env->add_ctr(*new_ctr);
}

void CtrGenerator::visit(const P_Inequality& inequ) {
  new_ctr=&Inequality::new_(ExprGenerator(env,context).generate(inequ.expr), inequ.op);
  parser_env->add_ctr(*new_ctr);
}

void CtrGenerator::visit(const P_AtMostNVector& a) {

  vector<const Expr*> vec;  
  ExprGenerator g(env,context);
  for (vector<const P_Expr*>::const_iterator it=a.expr.begin(); it!=a.expr.end(); it++) {
    vec.push_back(& (g.generate(**it)));
  }
    
  new_ctr=&AtMostNVector::new_(vec, a.N);
  parser_env->add_ctr(*new_ctr);
}

void CtrGenerator::visit(const P_NotIn& ni) { 

  ExprGenerator g(env,context);
  new_ctr=&NotIn::new_(g.generate(ni.left), g.generate(ni.right));
  parser_env->add_ctr(*new_ctr);
}

void CtrGenerator::visit(const P_CtrRef& ref) {
  
  new_ctr=scopes.top().ctr(ref.name);
}


void CtrGenerator::visit(const P_CtrListElemRef& ref) {

  ExprGenerator g(env,context);

  g.generate(ref.iter);
  
  unsigned int num = g.value._2int()-1;
  
  if (num<0 || num>scopes.top().ctr_list(ref.name).size()) 
    generate_error(string("index exceeds \"")+ref.name+"\" size");
  
  new_ctr=scopes.top().ctr_list(ref.name)[num];
}

/*================================================================================*/


void CtcGenerator::visit(const P_Ctc& ctc) { 
  generated_line = ctc.line;
  ctc.accept(*this); 
}

void CtcGenerator::visit(const P_CtcRef& ref) {
  new_ctc = scopes.top().ctc(ref.name)->copy();
}

void CtcGenerator::visit(const P_CtcListElemRef& ref) {

  ExprGenerator g(env,context);

  g.generate(ref.iter);
  
  unsigned int num = g.value._2int()-1;
  
  if (num<0 || num>scopes.top().ctc_list(ref.name).size()) 
    generate_error(string("index exceeds \"")+ref.name+"\" size");
  
  new_ctc=scopes.top().ctc_list(ref.name)[num];
}

void CtcGenerator::visit(const P_HC4Revise& h) { 
  const Constraint& ctr=CtrGenerator(env,context).generate(h.ctr);

  new_ctc = new HC4Revise(ctr, *parser_space);
}

void CtcGenerator::visit(const P_Shave& s) { 
  visit(s.ctc);

  const Contractor* sub_ctc=new_ctc;

  new_ctc = new Shaving(*sub_ctc, s.ratio, s.var_ratio);
  delete sub_ctc;
}

void CtcGenerator::visit(const P_Newton& n) { 
  vector<const Constraint*> vec;

  ListGenerator<P_Ctr,Constraint> g(env, vec, context);
  
  g.visit(n.sys);

  new_ctc = new Newton(_2system(vec, *parser_space), n.ceil, n.prec, n.ratio_gauss_seidel);
}

void CtcGenerator::visit(const P_Propag& p) { 
  vector<const Contractor*> vec;

  ListGenerator<P_Ctc,Contractor> g(env, vec, context);

  g.visit(p.list);

  new_ctc = new Propagation(vec, *parser_space, p.ratio, p.incremental);

  for (vector<const Contractor*>::const_iterator it=vec.begin(); it!=vec.end(); it++) {
    delete *it;
  }
}

void CtcGenerator::visit(const P_BoxNarrow& n) { 
  const Constraint& ctr=CtrGenerator(env,context).generate(n.ctr);

  new_ctc = new BoxNarrow(_2equality(ctr), *parser_space, n.var_ratio, n.w_newton);
}

void CtcGenerator::visit(const P_ParamSplit& p) { 
  visit(p.ctc);
  const Contractor* sub_ctc=new_ctc;

  const ExtendedSymbol& s = (const ExtendedSymbol&) ExprGenerator(env, context).generate(p.x);  

  new_ctc = new Slicer(*sub_ctc, s.key, p.nb_slices, p.logic_op);

  delete sub_ctc;
}

void CtcGenerator::visit(const P_Precision& p) { 
  new_ctc = new Precision(*parser_space, p.prec);
}

void CtcGenerator::visit(const P_LogicOp& op) { 
  vector<const Contractor*> vec;

  ListGenerator<P_Ctc,Contractor> g(env, vec, context);

  g.visit(op.list);

  new_ctc = new Sequence(vec, *parser_space, op.logic_op);

  for (vector<const Contractor*>::const_iterator it=vec.begin(); it!=vec.end(); it++) {
    delete *it;
  }
}

void CtcGenerator::visit(const P_LogicOpLoop& loop) { 
  
  const char* name     = loop.iter;
  int         begin    = loop.first_value; //scope.it_first_value(name);
  int         end      = loop.last_value; //scope.it_last_value(name);

  if (context.used(name)) generate_error("embedded redeclaration of an iterator symbol");

  vector<const Contractor*> vec;

  ListGenerator<P_Ctc,Contractor> g(env, vec, context);

  context.insert_new(name,begin);

  for (int i=begin; i<=end; i++) {
    context.data(name)=i;
    g.visit(loop.list);
  }

  new_ctc = new Sequence(vec, *parser_space, loop.logic_op);
  
  for (vector<const Contractor*>::const_iterator it=vec.begin(); it!=vec.end(); it++) {
    delete *it;
  }

  context.erase(name);
}

void CtcGenerator::visit(const P_QInter& q) { 
  
  ExprGenerator g(env,context);

  new_ctc = new QInter(*parser_space, 
		       (const ExtendedSymbol&) g.generate(q.x),
		       (const ExtendedSymbol&) g.generate(q.y),
		       q.q);
}

void CtcGenerator::visit(const P_Load& load) { 
  
  new_ctc = new Loader(*parser_space, load.filename, 
		       (const ExtendedSymbol&) ExprGenerator(env,context).generate(load.x));
}


template<>
void BlkGenerator<P_Ctr,Constraint>::visit(const P_Ctr& ctr) { 
  vec.push_back(& (CtrGenerator(env,context).generate(ctr)));
}

template<>
void BlkGenerator<P_Ctc,Contractor>::visit(const P_Ctc& ctc) { 
  vec.push_back(& (CtcGenerator(env,context).generate(ctc)));
}

template<class Tin, class Tout>
void ListGenerator<Tin,Tout>::visit(const P_BlkList<Tin>& list) { 
  BlkGenerator<Tin,Tout> g(env,vec,context);
  for (int i=0; i<list.size(); i++) {
    g.visit(list(i));
  }
}

template<>
void BlkGenerator<P_Ctr,Constraint>::visit(const P_Blk<P_Ctr>& b) {
  generated_line = b.line;
  b.accept(*this);
}

template<>
void BlkGenerator<P_Ctc,Contractor>::visit(const P_Blk<P_Ctc>& b) {
  generated_line = b.line;
  b.accept(*this);
}

template<>
void ListGenerator<P_Ctr,Constraint>::visit(const P_BlkListRef<P_Ctr>& ref) { 
  const vector<const Constraint*>& vec2=scopes.top().ctr_list(ref.name);
  for (vector<const Constraint*>::const_iterator it=vec2.begin(); it!=vec2.end(); it++) {
    vec.push_back(*it);
  }
}       

template<>
void ListGenerator<P_Ctc,Contractor>::visit(const P_BlkListRef<P_Ctc>& ref) { 
  const vector<const Contractor*>& vec2=scopes.top().ctc_list(ref.name);
  for (vector<const Contractor*>::const_iterator it=vec2.begin(); it!=vec2.end(); it++) {
    vec.push_back(*it);
  }
}       


} // end namespace

} // end namespace
