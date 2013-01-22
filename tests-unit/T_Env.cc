/* CHABERT jan 05th 2008 */

#include "T_.h"
#include "IbexExpr.h"
#include "IbexConstraint.h"
#include "IbexEnv.h"
#include <sstream>

using namespace ibex;

void T_env() {

  _class="Env";
  _meth="add_symbol";
  _nber = 0;

  Env env;  
  env.add_symbol("x");                   // create the symbol x   
  env.add_symbol("y");
  env.add_symbol("a",2);
  env.add_symbol("b",1,2);
  env.add_symbol("c",2,3,4);
  env.add_symbol("d",1);
  try {
    env.add_symbol("c",2);
    _test(false);
  } catch(NonRecoverableException) { // redeclared id
    _test(true);
  }

  const Symbol& x=env.symbol("x");
  const Symbol& y=env.symbol("y");

  /*================================================================================*/
  _meth="nb_keys()";
  _nber = 0;
  _test(env.nb_keys()==31);

  /*================================================================================*/
  _meth="symbol_key";
  _nber = 0;

  _test(x.key==0);
  _test(y.key==1);
  _test(env.symbol_key("x")==0);
  _test(env.symbol_key("y")==1);
  _test(env.symbol_key("a[1]")==2);
  _test(env.symbol_key("a[2]")==3);
  _test(env.symbol_key("b[1][1]")==4);
  _test(env.symbol_key("c[1]")==6);
  _test(env.symbol_key("c[2][3][4]")==29);
  _test(env.symbol_key("d[1]")==30);
  _test(env.symbol_key("a")==2);
  _test(env.symbol_key("b[1]")==4);
  _test(env.symbol_key("c[1][1]")==6);

  /*================================================================================*/
  _meth="symbol_dim";
  _nber = 0;

  _test(x.dim==Dim(0,0,0));
  _test(y.dim==Dim(0,0,0));
  _test(env.symbol_dim("x")==Dim(0,0,0));
  _test(env.symbol_dim("y")==Dim(0,0,0));
  _test(env.symbol_dim("a[1]")==Dim(0,0,0));
  _test(env.symbol_dim("a[2]")==Dim(0,0,0));
  _test(env.symbol_dim("b[1][1]")==Dim(0,0,0));
  _test(env.symbol_dim("c[1]")==Dim(0,3,4));
  _test(env.symbol_dim("c[2][3][4]")==Dim(0,0,0));
  _test(env.symbol_dim("d[1]")==Dim(0,0,0));
  _test(env.symbol_dim("a")==Dim(0,0,2));
  _test(env.symbol_dim("b[1]")==Dim(0,0,2));
  _test(env.symbol_dim("c")==Dim(2,3,4));
  _test(env.symbol_dim("c[1][1]")==Dim(0,0,4));


  /*================================================================================*/
  _meth="symbol_name";
  _nber = 0;

  _test(strcmp(x.name,"x")==0);
  _test(strcmp(y.name,"y")==0);
  _test(strcmp(env.symbol_name(0),"x")==0);
  _test(strcmp(env.symbol_name(1),"y")==0);
  _test(strcmp(env.symbol_name(2),"a[1]")==0);
  _test(strcmp(env.symbol_name(3),"a[2]")==0);
  _test(strcmp(env.symbol_name(4),"b[1][1]")==0);
  _test(strcmp(env.symbol_name(6),"c[1][1][1]")==0);
  _test(strcmp(env.symbol_name(28),"c[2][3][3]")==0);
  _test(strcmp(env.symbol_name(30),"d[1]")==0);

  /*================================================================================*/
  const Constraint& c1=x+1=0;
  const Constraint& c2=x-1=0;
  env.add_ctr(c1,"c1");
  env.add_ctr(c2,"c2");

  _meth="nb_constraints()";
  _nber = 0;
  _test(env.nb_constraints()==2);

  _meth="constraint(num)";
  _test(&env.constraint(0)==&c1);
  _test(&env.constraint(1)==&c2);
  _nber = 0;

  _meth="constraint(name)";
  _test(&env.constraint("c1")==&c1);
  _test(&env.constraint("c2")==&c2);

 
}
