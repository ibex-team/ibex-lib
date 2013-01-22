
/* CHABERT March 7th 2007 */

#include "T_SubSystem.h"
#include "IbexSystem.h"
#include "IbexSystemFactory.h"
#include <math.h>

using namespace ibex;

void T_SubSystem() {

  _class="Space";
  _meth="sync";
  _nber = 0;

  Env env;
  const Symbol& a=env.add_symbol("a");
  const Symbol& b=env.add_symbol("b");
  const Symbol& c=env.add_symbol("c");
  const Symbol& d=env.add_symbol("d");
  const Symbol& e=env.add_symbol("e");
  const Symbol& f=env.add_symbol("f");
  const Symbol& g=env.add_symbol("g");
  const Symbol& h=env.add_symbol("h");
  const Symbol& i=env.add_symbol("i");
  const Symbol& j=env.add_symbol("j");
  const Symbol& k=env.add_symbol("k");
  const Symbol& l=env.add_symbol("l");
  const Symbol& m=env.add_symbol("m");
  const Symbol& n=env.add_symbol("n");
  const Symbol& o=env.add_symbol("o");
  const Symbol& p=env.add_symbol("p");

  SpaceFactory sysf(env);

  sysf.set_entity("a",IBEX_VAR, INTERVAL(0,1));
  sysf.set_entity("b",IBEX_VAR, INTERVAL(0,2));
  sysf.set_entity("c",IBEX_VAR, INTERVAL(0,3));
  sysf.set_entity("d",IBEX_VAR, INTERVAL(0,4));

  sysf.set_entity("e",IBEX_SYB, INTERVAL(0,5));
  sysf.set_entity("f",IBEX_SYB, INTERVAL(0,6));
  sysf.set_entity("g",IBEX_SYB, INTERVAL(0,7));
  sysf.set_entity("h",IBEX_SYB, INTERVAL(0,8));

  sysf.set_entity("i",IBEX_EPR, INTERVAL(0,9));
  sysf.set_entity("j",IBEX_EPR, INTERVAL(0,10));
  sysf.set_entity("k",IBEX_EPR, INTERVAL(0,11));
  sysf.set_entity("l",IBEX_EPR, INTERVAL(0,12));

  sysf.set_entity("m",IBEX_UPR, INTERVAL(0,13));
  sysf.set_entity("n",IBEX_UPR, INTERVAL(0,14));
  sysf.set_entity("o",IBEX_UPR, INTERVAL(0,15));
  sysf.set_entity("p",IBEX_UPR, INTERVAL(0,16));

  env.add_ctr(a+b+e=i+m);
  env.add_ctr(b+f+h=j+j+j);
  env.add_ctr(g*h*g+m=k+o-m);
  env.add_ctr(d+h=h-p+d+m+n+p+p);

  System sys(sysf);

  vector<const Constraint*> vec;
  for (int i=0; i<env.nb_constraints(); i++) 
    vec.push_back(&env.constraint(i));
  
  
  {
    
    /*------------- check building -----------*/
    SystemSpaceFactory factory(sys.space, vec);
    factory.set_entity(IBEX_VAR, 0, IBEX_EPR);  // var=3 epr=5 "a"->epr
    factory.set_entity(IBEX_VAR, 0, IBEX_SYB);  // epr=4 (because var 0 is now epr) syb=5 "a"->syb
    factory.set_entity(IBEX_EPR, 1, IBEX_SYB);  // epr=3 syb=6 "j"->syb
    factory.set_entity(IBEX_VAR, 2,  IBEX_UPR);  // var=2 upr=5 "c"->upr
    factory.set_entity(IBEX_SYB, 3, IBEX_UPR);  // syb=5 upr=6 "h" ->upr
    System sys2(factory);                       // add all equ

    /* result:
       var = {b,c,d} upr = {b,h,m,n,o,p} epr = {i,k} ***warning: {l} is automatically removed ! ***
       syb = {a,j,e,f,g} */

    cout << "-------------- check building --------------\n";
    _class="SysFactory";
    _meth="build";
    _nber = 0;
    _test(sys2.space.nb_var()==2);
    _test(sys2.space.nb_epr()==2);
    _test(sys2.space.nb_syb()==5);
    _test(sys2.space.nb_upr()==6);

    cout << "-------------- check sync --------------\n";
    _class="System";
    _meth="sync";
    _nber = 0;
    sys2.space.sbx(sys2.space.syb("a")+1)=INTERVAL(0,17);
    sys2.space.sbx(sys2.space.syb("j")+1)=INTERVAL(0,18);
    sys2.space.upx(sys2.space.upr("c")+1)=INTERVAL(0,19);
    sys2.space.upx(sys2.space.upr("h")+1)=INTERVAL(0,20);
    sys2.space.box(sys2.space.var("b")+1)=INTERVAL(0,21);

    sys.space.sync(sys2.space);
    _test(sys.space.box(1)==INTERVAL(0,17));
    _test(sys.space.box(2)==INTERVAL(0,21));
    _test(sys.space.box(3)==INTERVAL(0,19));
    _test(sys.space.epx(2)==INTERVAL(0,18));
    _test(sys.space.sbx(4)==INTERVAL(0,20));
  }
  
  {
    /*------------- check building -----------*/
    SystemSpaceFactory factory(sys.space, vec, true);
    factory.set_entity(IBEX_VAR, 0, IBEX_EPR);  // epr=1
    factory.set_entity(IBEX_VAR, 0, IBEX_SYB);  // epr=0 (because var 0 is now epr) syb=16 "a"->syb
    factory.set_entity(IBEX_EPR, 1, IBEX_SYB);  // 
    factory.set_entity(IBEX_VAR, 2, IBEX_UPR);  // upr=1
    factory.set_entity(IBEX_SYB, 3, IBEX_UPR);  // syb=2
    
    cout << "---------- check build \"syb-by-default\" ----------\n";
    _class="System";
    _meth="build";
    _nber = 0;

    /* result:
       upr = {b,h} syb = {a,c,d,e,f,g,i,j,k,m,n,o,p} epr = {i,k} ***warning: {l} is automatically removed ! *** */

    System sys2(factory); // add all equ
    _test(sys2.space.nb_var()==0);
    _test(sys2.space.nb_epr()==0);
    _test(sys2.space.nb_syb()==13);
    _test(sys2.space.nb_upr()==2);
  }

}
