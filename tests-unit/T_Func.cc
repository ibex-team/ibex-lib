/* CHABERT juil 25th 2007 */

#include "T_.h"
#include "IbexFunction.h"
#include "IbexConstraint.h"
#include "IbexEnv.h"
#include <sstream>

using namespace ibex;

/*********************************************************************************/
void T_Func() {

  FunctionFactory fac("foo");

  _class="FunctionFactory";
  _meth="add_input()";
  _nber = 0;

  const Symbol& x=fac.add_input("x");
  const Symbol& A=fac.add_input("A",Dim(0,2,4));

  try {
    fac.add_input("zzz",Dim(2,2,2));
    _test(false);
  }
  catch(ibex::NonRecoverableException) {
    _test(true);
  }
  
  const Symbol& B=fac.add_input("B",Dim(0,4,3));
  const Symbol& y=fac.set_output("y");
  const Symbol& W1=fac.add_tmp_symbol("W1",Dim(0,2,3));
  const Symbol& w2=fac.add_tmp_symbol("w2");
  const Symbol& w3=fac.add_tmp_symbol("w3");
  
  fac.add_assign(*new Assign(W1,A*B));
  fac.add_assign(*new Assign(w2,x+1));
  fac.add_assign(*new Assign(w3,w2*W1[1][1]-w2));
  fac.add_assign(*new Assign(y,w3+w3));

  _meth="symbol_expr() (inputs)";
  _nber = 0;

  int status;
  _test(strcmp(fac.symbol_expr("x", status).name,"x")==0);
  _test(fac.symbol_expr("x", status).key==0);
  _test(status==0);
  _test(strcmp(fac.symbol_expr("A", status).name,"A")==0);
  _test(fac.symbol_expr("A", status).key==1);
  _test(status==0);
  _test(strcmp(fac.symbol_expr("B", status).name,"B")==0);
  _test(fac.symbol_expr("B", status).key==9);
  _test(status==0);
  
  _meth="symbol_expr() (output)";
  _nber = 0;

  _test(strcmp(fac.symbol_expr("y", status).name,"y")==0);
  _test(fac.symbol_expr("y", status).key==21);
  _test(status==1);

  _meth="symbol_expr() (temporaries)";
  _nber = 0;

  const Symbol& ww1=fac.symbol_expr("W1", status);
  _test(strcmp(ww1.name,"W1")==0);
  _test(ww1.key==22);
  _test(ww1.dim==Dim(0,2,3));
  _test(status==2);
  const Symbol& ww2=fac.symbol_expr("w2", status);
  _test(strcmp(ww2.name,"w2")==0);
  _test(ww2.key==28);
  _test(ww2.scalar());
  _test(status==2);
  const Symbol& ww3=fac.symbol_expr("w3", status);
  _test(strcmp(ww3.name,"w3")==0);
  _test(ww3.key==29);
  _test(ww3.scalar());
  _test(status==2);

  _class="Function";

  const Function& f=fac.build_func();
  
  Env env;
  env.add_function(f);
  
  _meth="name";
  _nber = 0;
  _test(strcmp(f.name,"foo")==0);
  _test(&env.function("foo") == &f);

  _meth="arg_dim()";
  _nber = 0;
  _test(f.arg_dim(0)==Dim(0,0,0));
  _test(f.arg_dim(1)==Dim(0,2,4));
  _test(f.arg_dim(2)==Dim(0,4,3));

  _meth="arg_dim()";
  _nber = 0;
  _test(f.img_dim()==Dim(0,0,0));

  _meth="input_size()";
  _nber = 0;
  _test(f.input_size()==21);

  _meth="output_size()";
  _nber = 0;
  _test(f.output_size()==1);

  _meth="temp_size()";
  _nber = 0;
  _test(f.temp_size()==8);

  _meth="status()";
  _nber = 0;
  _test(f.status(0)==0 && f.status(20)==0);
  _test(f.status(21)==1);
  _test(f.status(22)==2);

  _meth="nb_inputs()";
  _nber = 0;
  _test(f.nb_inputs()==3);

  _meth="forward()";
  _nber = 0;

  SpaceFactory spacef(f.env);
  spacef.set_entity("x",IBEX_VAR);
  spacef.set_entity("A",IBEX_EPR);
  spacef.set_entity("B",IBEX_VAR);
  spacef.set_entity("y",IBEX_VAR);
  spacef.set_entity("W1",IBEX_VAR);
  spacef.set_entity("w2",IBEX_VAR);  
  spacef.set_entity("w3",IBEX_VAR);
  
  Space space(spacef);

  space.domain("x")=INTERVAL(1,2);
  space.domain("A[1][1]")=INTERVAL(1,1);
  space.domain("A[1][2]")=INTERVAL(1,1);
  space.domain("A[1][3]")=INTERVAL(1,1);
  space.domain("A[1][4]")=INTERVAL(1,1);
  space.domain("A[2][1]")=INTERVAL(2,2);
  space.domain("A[2][2]")=INTERVAL(2,2);
  space.domain("A[2][3]")=INTERVAL(2,2);
  space.domain("A[2][4]")=INTERVAL(2,2);

  space.domain("B[1][1]")=INTERVAL(1,1);
  space.domain("B[1][2]")=INTERVAL(2,2);
  space.domain("B[1][3]")=INTERVAL(3,3);
  space.domain("B[2][1]")=INTERVAL(1,1);
  space.domain("B[2][2]")=INTERVAL(2,2);
  space.domain("B[2][3]")=INTERVAL(3,3);
  space.domain("B[3][1]")=INTERVAL(1,1);
  space.domain("B[3][2]")=INTERVAL(2,2);
  space.domain("B[3][3]")=INTERVAL(3,3);
  space.domain("B[4][1]")=INTERVAL(1,1);
  space.domain("B[4][2]")=INTERVAL(2,2);
  space.domain("B[4][3]")=INTERVAL(3,3);

  space.domain("y")=INTERVAL(BiasNegInf,BiasPosInf);

  f.forward(space);
  space.set_output_flags(15);

  _test(Inf(space.domain("y"))==10 && Sup(space.domain("y"))==20);
  _test(Inf(space.domain("W1[1][1]"))==4);
  _test(Inf(space.domain("W1[1][2]"))==8);
  _test(Inf(space.domain("W1[1][3]"))==12);
  _test(Inf(space.domain("W1[2][1]"))==8);
  _test(Inf(space.domain("W1[2][2]"))==16);
  _test(Inf(space.domain("W1[2][3]"))==24);
  _test(Inf(space.domain("w2"))==2 && Sup(space.domain("w2"))==3);
  _test(Inf(space.domain("w3"))==5 && Sup(space.domain("w3"))==10);

  space.domain("y")=INTERVAL(18,18);
  f.backward(space);

  _test(Inf(space.domain("W1[1][1]"))==4);
  _test(Inf(space.domain("w2"))==2.5 && Sup(space.domain("w2"))==3);
  _test(Inf(space.domain("w3"))==8 && Sup(space.domain("w3"))==10);
}

void T_Apply() {

  Env env;

  FunctionFactory fac("bar");

  /*--------------------------------------------*/
  /* creates foo(x,A,B)->y */
  /*--------------------------------------------*/
  const Symbol& x = fac.add_input("x");
  const Symbol& A = fac.add_input("A",Dim(0,2,4));
  const Symbol& B = fac.add_input("B",Dim(0,4,3));

  const Symbol& y = fac.set_output("y");

  const Symbol& W1 = fac.add_tmp_symbol("W1",Dim(0,2,3));
  const Symbol& w2 = fac.add_tmp_symbol("w2");
  const Symbol& w3 = fac.add_tmp_symbol("w3");
  
  fac.add_assign(*new Assign(W1,A*B));
  fac.add_assign(*new Assign(w2,x+1));
  fac.add_assign(*new Assign(w3,w2*W1[1][1]-w2));
  fac.add_assign(*new Assign(y,w3+w3));
  fac.build_func();

  env.add_function(fac.build_func());
  /*--------------------------------------------*/


  const Symbol& x2 = env.add_symbol("x2");
  const Symbol& x3 = env.add_symbol("x3");
  const Symbol& A2 = env.add_symbol("A2",2,4);
  const Symbol& B2 = env.add_symbol("B2",4,3);
  const Symbol& y2 = env.add_symbol("y2");
  const Symbol& C3 = env.add_symbol("C3",2,1);
  const Symbol& D3 = env.add_symbol("D3",1,4);
  
  SpaceFactory spacef(env);
  spacef.set_entity("x2",IBEX_VAR);
  spacef.set_entity("A2",IBEX_EPR);
  spacef.set_entity("B2",IBEX_SYB);
  spacef.set_entity("y2",IBEX_VAR);
  spacef.set_entity("x3",IBEX_VAR);
  spacef.set_entity("C3",IBEX_EPR);
  spacef.set_entity("D3",IBEX_SYB);
  
  Space space(spacef);
  
  space.domain("x2")=INTERVAL(1,2);
  space.domain("x3")=INTERVAL(0,1);
  space.domain("A2[1][1]")=INTERVAL(1,1);
  space.domain("A2[1][2]")=INTERVAL(1,1);
  space.domain("A2[1][3]")=INTERVAL(1,1);
  space.domain("A2[1][4]")=INTERVAL(1,1);
  space.domain("A2[2][1]")=INTERVAL(2,2);
  space.domain("A2[2][2]")=INTERVAL(2,2);
  space.domain("A2[2][3]")=INTERVAL(2,2);
  space.domain("A2[2][4]")=INTERVAL(2,2);

  space.domain("B2[1][1]")=INTERVAL(1,1);
  space.domain("B2[1][2]")=INTERVAL(2,2);
  space.domain("B2[1][3]")=INTERVAL(3,3);
  space.domain("B2[2][1]")=INTERVAL(1,1);
  space.domain("B2[2][2]")=INTERVAL(2,2);
  space.domain("B2[2][3]")=INTERVAL(3,3);
  space.domain("B2[3][1]")=INTERVAL(1,1);
  space.domain("B2[3][2]")=INTERVAL(2,2);
  space.domain("B2[3][3]")=INTERVAL(3,3);
  space.domain("B2[4][1]")=INTERVAL(1,1);
  space.domain("B2[4][2]")=INTERVAL(2,2);
  space.domain("B2[4][3]")=INTERVAL(3,3);

  space.domain("C3[1][1]")=INTERVAL(1,1);
  space.domain("C3[2][1]")=INTERVAL(2,2);
  space.domain("D3[1][1]")=INTERVAL(1,1);
  space.domain("D3[1][2]")=INTERVAL(1,1);
  space.domain("D3[1][3]")=INTERVAL(1,1);
  space.domain("D3[1][4]")=INTERVAL(1,1);

  vector<const Expr*> args1;
  args1.push_back(&x2);
  args1.push_back(&A2);
  args1.push_back(&B2);

  const Apply& a1 = Apply::new_(env, "bar", args1);

  vector<const Expr*> args2;
  args2.push_back(&(x3+1));
  args2.push_back(&(C3*D3));
  args2.push_back(&B2);

  const Apply& a2 = Apply::new_(env, "bar", args2);

  _class="Apply";
  _meth = "depth & size";
  _nber = 0;

  _test(a1.depth == 1 && a1.size == 4);
  _test(a2.depth == 2 && a2.size == 8);

  _meth = "forward()";
  const Equality& e1 = a1=0;
  const Equality& e2 = a2=0;
  _nber = 0;
  
  _test(Inf(e1.eval(space)) == 10);
  _test(Sup(e1.eval(space)) == 20);
  _test(Inf(e2.eval(space)) == 10);
  _test(Sup(e2.eval(space)) == 20);

  _meth = "backward()";
  const Equality& e3 = a1=INTERVAL(18,18);
  const Equality& e4 = a2=INTERVAL(18,18);
  _nber = 0;
  e3.forward(space);
  e3.backward(space);
  _test(Inf(space.domain("x2"))==1.5);
  e4.forward(space);
  e4.backward(space);
  _test(Inf(space.domain("x3"))==0.5);

  delete &e1; // they don't belong to an environment
  delete &e2;
  delete &e3;
  delete &e4;

  //  cout << space << endl;



}
