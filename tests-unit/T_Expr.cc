/* CHABERT juil 25th 2007 */

#include "T_.h"
#include "IbexExpr.h"
#include "IbexEnv.h"
#include <sstream>

using namespace ibex;

/*********************************************************************************/
void T_depth_size() {

  _class="Expr";
  _meth="depth & size (DAG)";
  _nber = 0;

  Env env;  
  const Symbol& x=env.add_symbol("x");
  const Symbol& y=env.add_symbol("y");
  const Symbol& z=env.add_symbol("z");

  /* checking trees only */
  _test(x.depth==0 && x.size==1);

  const Expr& e1=x+1;
  _test(e1.depth==1 && e1.size==3);

  const Expr& e2=x+sin(y);
  _test(e2.depth==2 && e2.size==4);

  const Expr& e3=x+sin(y)*z;
  _test(e3.depth==3 && e3.size==6);

  const Expr& e4=x+sin(y)*z-0.5/exp(z+pow(x,3));
  _test(e4.depth==5 && e4.size==12);

  /* checking dags */
  int size1=env.nb_expressions();
  const Expr& e5=e2+e2;
  _test(env.nb_expressions()-size1==1);

  _test(e5.depth==3 && e5.size==5);

  const Expr& e6=e2-e1;
  _test(e6.depth==3 && e6.size==7);

  size1=env.nb_expressions();
  const Expr& e7=-e5+x-e2;
  _test(env.nb_expressions()-size1==3);
  _test(e7.depth==6 && e7.size==8);

  const Symbol& c=env.add_symbol("c",2,3,4);
  _test(c.size==1);
  _test(c[2].size==2 && c[2].depth==1);
  _test(c[2][3].size==3 && c[2][3].depth==2);

}

/*********************************************************************************/
void T_dim() {  
  _class="Expr";
  _meth="dim (symbols indexes)";
  _nber = 0;

  Env env;

  const Symbol& x=env.add_symbol("x");
  const Symbol& a=env.add_symbol("a",2);
  const Symbol& b=env.add_symbol("b",1,2);
  const Symbol& c=env.add_symbol("c",2,3,4);  const Symbol& d=env.add_symbol("d",4,2);
  const Symbol& e=env.add_symbol("e",2,1);

  _test(x.dim==Dim(0,0,0));
  _test(a.dim==Dim(0,0,2));
  _test(b.dim==Dim(0,1,2));
  _test(c.dim==Dim(2,3,4));
  
  const Expr& e1=x+1;
  _test(e1.scalar());

  const Expr& e2=exp(-sin(x+1));
  _test(e2.scalar());

  _test(a[1].scalar());
  _test(b[1].dim==Dim(0,0,2));
  _test(c[1].dim==Dim(0,3,4));
  _test(c[1][2].dim==Dim(0,0,4));
  _test(c[1][2][3].dim==Dim(0,0,0));
  

  _class="Expr";
  _meth="dim (opérations vecteurs/matrices)";
  _nber = 0;

  _test((a+a).dim==Dim(0,0,2));
  _test((a+b[1]).dim==Dim(0,0,2));
  _test((-b).dim==Dim(0,1,2));
  _test((b-b).dim==Dim(0,1,2));
  try {
    b*b;
    _test(false);
  } catch(NonRecoverableException) { //BadArgDimException
    _test(true);
  }
  _test((b-b-b).dim==Dim(0,1,2));

  _test((-c[1]*d).dim==Dim(0,3,2));

  _test((c[1]*d).dim==Dim(0,3,2));
  _test((c[1]*d*e).dim==Dim(0,3,1));
}

/*********************************************************************************/
void T_key() {  
  _class="ExtendedSymbol";
  _meth="key";
  _nber = 0;

  Env env;

  const Symbol& x=env.add_symbol("x");
  const Symbol& a=env.add_symbol("a",2);
  const Symbol& b=env.add_symbol("b",1,2);
  const Symbol& c=env.add_symbol("c",2,3,4);

  _test(x.key==0);
  _test(a.key==1);
  _test(a[1].key==1);
  _test(a[2].key==2);
  _test(b.key==3);
  _test(b[1][2].key==4);
  _test(c[2][3][4].key==28);
}

/*********************************************************************************/
void T_symbol_name() {
  _class="Symbol";
  _meth="name";
  _nber = 0;

  Env env;
  const Symbol& x=env.add_symbol("x");
  const Symbol& c=env.add_symbol("c",2,3,4);

  _test(strcmp(x.name,"x")==0);
  _test(strcmp(c.name,"c")==0);
}

/*********************************************************************************/
void T_constant() {
  _class="Constant";
  _meth="value";
  _nber = 0;

  INTERVAL x(-1,1);
  INTERVAL_MATRIX A(2,3);
  Initialize(A,2.0);
  A(1,1)=x;
  
  Env env;

  const Constant& c1=Constant::new_scalar(env, x);
  _test(Inf(c1.get_value())==-1);
  _test(Sup(c1.get_value())==+1);

  INTERVAL_MATRIX M1=c1.get_matrix_value();
  _test(RowDimension(M1)==1 && ColDimension(M1)==1 && Inf(M1(1,1))==-1 && Sup(M1(1,1))==1);

  const Constant& c2=Constant::new_matrix(env, A);
  _test(Inf(c2.get_value())==-1);
  _test(Sup(c2.get_value())==+1);

  INTERVAL_MATRIX M2=c2.get_matrix_value();
  _test(RowDimension(M2)==2 && ColDimension(M2)==3 && Inf(M2(1,1))==-1 && Sup(M2(1,1))==1);
  _test(Inf(M2(2,2))==2.0 && Inf(M2(2,2))==2.0);
}

void T_apply_scalar() {
  _class="Apply";
  _meth="(scalar case)";
  _nber = 0;

  Env env;
  const Symbol& x=env.add_symbol("x");
  const Symbol& y=env.add_symbol("y");

  const Expr& e1=x+1;
  const Expr& e2=x+sin(y);
  const Expr& e3=e2-e1;

  /* checking apply (scalar case) */
  vector<const Expr*> args(3);
  args.push_back(&e1);
  args.push_back(&e2);
  args.push_back(&e3); 
  const Apply& a = Apply::new_(env, "f",args);
  
  _test(strcmp(a.f,"f")==0);
  _test(a.n==3);
  _test(a.size==8 && a.depth==4);
  _test(&a.arg(0)==&e1);
  _test(&a.arg(1)==&e2);
  _test(&a.arg(2)==&e3);

}

void T_apply_matrix() {
  _class="Apply";
  _meth="(matrix case)";
  _nber = 0;

  Env env;
  const Symbol& c=env.add_symbol("c",2,3,4);
  const Symbol& d=env.add_symbol("d",4,2);
  const Symbol& e=env.add_symbol("e",2,1);

  const Expr& e1=c[1];
  const Expr& e2=-d;
  const Expr& e3=e;

  /* checking apply (matrix case) */
  vector<const Expr*> args(3);
  args.push_back(&e1);
  args.push_back(&e2);
  args.push_back(&e3); 
  const Apply& a = Apply::new_(env, "f",args);
  
  _test(strcmp(a.f,"f")==0);
  _test(a.n==3);
  _test(a.size==5 && a.depth==2);
  _test(&a.arg(0)==&e1);
  _test(&a.arg(1)==&e2);
  _test(&a.arg(2)==&e3);

}

/*********************************************************************************/
void T_assign() {
  _class="Assign";
  _meth="";
  _nber = 0;

  Env env;
  const Symbol& x=env.add_symbol("x");
  const Symbol& y=env.add_symbol("y");  
  const Symbol& a=env.add_symbol("a",2,4);
  const Symbol& b=env.add_symbol("b",2,3);
  const Symbol& c=env.add_symbol("c",3,4);

//   const Expr& e1=Assign::new_(x,y+1);
//   const Expr& e2=Assign::new_(y,x*1);  
//   const Expr& e3=Assign::new_(a,b*c);
//   const Expr& e4=e1+e2;

//   _test(e4.size==9 && e4.depth==3 && e4.scalar());
//   _test(e3.size==5 && e3.depth==2 && e3.dim==Dim(0,2,4));
}

void T_expr() {
  T_depth_size();
  T_dim();
  T_key();
  T_symbol_name();
  T_constant();
//   T_apply_scalar();
//   T_apply_matrix();
  T_assign();
}


