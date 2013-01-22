
/* CHABERT March 7th 2007 */

#include "T_Space.h"
#include "IbexSpace.h"
#include "IbexEnv.h"
#include <math.h>

using namespace ibex;

void T_Space1(Space& space1) {

  _meth="nb_xxx()";
  _nber = 0;
  _test(space1.nb_var()==2);
  _test(space1.nb_syb()==3);
  _test(space1.nb_epr()==4);
  _test(space1.nb_upr()==5);

  _meth="nb(type)";
  _nber = 0;
  _test(space1.nb(IBEX_VAR)==2);
  _test(space1.nb(IBEX_SYB)==3);
  _test(space1.nb(IBEX_EPR)==4);
  _test(space1.nb(IBEX_UPR)==5);

  _meth="var_name(key)";
  _nber = 0;
  _test(strcmp(space1.var_name(0),"a")==0);
  _test(strcmp(space1.var_name(1),"b")==0);

  _meth="syb_name(key)";
  _nber = 0;
  _test(strcmp(space1.syb_name(0),"c")==0);
  _test(strcmp(space1.syb_name(1),"d")==0);
  _test(strcmp(space1.syb_name(2),"e")==0);

  _meth="epr_name(key)";
  _nber = 0;
  _test(strcmp(space1.epr_name(0),"f")==0);
  _test(strcmp(space1.epr_name(1),"g")==0);
  _test(strcmp(space1.epr_name(2),"h")==0);
  _test(strcmp(space1.epr_name(3),"i")==0);

  _meth="upr_name(key)";
  _nber = 0;
  _test(strcmp(space1.upr_name(0),"j")==0);
  _test(strcmp(space1.upr_name(1),"k")==0);
  _test(strcmp(space1.upr_name(2),"l")==0);
  _test(strcmp(space1.upr_name(3),"m")==0);
  _test(strcmp(space1.upr_name(4),"n")==0);
 
  _meth="key(type,num)";
  _nber = 0;
  _test(space1.key(IBEX_VAR,0)==0);
  _test(space1.key(IBEX_VAR,1)==1);

  _test(space1.key(IBEX_SYB,0)==2);
  _test(space1.key(IBEX_SYB,1)==3);
  _test(space1.key(IBEX_SYB,2)==4);

  _test(space1.key(IBEX_EPR,0)==5);
  _test(space1.key(IBEX_EPR,1)==6);
  _test(space1.key(IBEX_EPR,2)==7);
  _test(space1.key(IBEX_EPR,3)==8);

  _test(space1.key(IBEX_UPR,0)==9);
  _test(space1.key(IBEX_UPR,1)==10);
  _test(space1.key(IBEX_UPR,2)==11);
  _test(space1.key(IBEX_UPR,3)==12);
  _test(space1.key(IBEX_UPR,4)==13);

  _meth="component(key)";
  _nber = 0;
  _test(space1.component(0)==0);
  _test(space1.component(1)==1);
  _test(space1.component(2)==0);
  _test(space1.component(3)==1);
  _test(space1.component(4)==2);
  _test(space1.component(5)==0);
  _test(space1.component(6)==1);
  _test(space1.component(7)==2);
  _test(space1.component(8)==3);
  _test(space1.component(9)==0);
  _test(space1.component(10)==1);
  _test(space1.component(11)==2);
  _test(space1.component(12)==3);
  _test(space1.component(13)==4);


  _meth="vec_domain(type)";
  _nber = 0;
  _test(&space1.vec_domain(IBEX_VAR)==&space1.box);
  _test(&space1.vec_domain(IBEX_SYB)==&space1.sbx);
  _test(&space1.vec_domain(IBEX_EPR)==&space1.epx);
  _test(&space1.vec_domain(IBEX_UPR)==&space1.upx);

  _meth="domain(key)";
  _nber = 0;
  _test(Sup(space1.Space::domain(0))==1);
  _test(&space1.Space::domain(0)==&space1.box(1));

  _test(Sup(space1.Space::domain(4))==5);
  _test(&space1.Space::domain(4)==&space1.sbx(3));

  _test(Sup(space1.Space::domain(7))==8);
  _test(&space1.Space::domain(7)==&space1.epx(3));

  _test(Sup(space1.Space::domain(13))==14);
  _test(&space1.Space::domain(13)==&space1.upx(5));

  int key=0;
  EntityType type=IBEX_VAR;
  _meth="ent(IBEX_VAR,dom_num)";
  _nber = 0;  
  for (int num=0; num<=1; num++) {
    const Entity& e=space1.ent(type, num);
    _test(space1.ent(type,num).key==key++);
    _test(space1.ent(type,num).type==type);
    _test(space1.ent(type,num).used());
    _test(space1.ent(type,num).domain==&space1.domain(type,num));
  }

  type=IBEX_SYB;
  _meth="ent(IBEX_SYB,dom_num)";
  _nber = 0;  
  for (int num=0; num<=2; num++) {
    const Entity& e=space1.ent(type, num);
    _test(space1.ent(type,num).key==key++);
    _test(space1.ent(type,num).type==type);
    _test(space1.ent(type,num).used());
    _test(space1.ent(type,num).domain==&space1.domain(type,num));
  }

  type=IBEX_EPR;
  _meth="ent(IBEX_SPR,dom_num)";
  _nber = 0;  
  for (int num=0; num<=3; num++) {
    const Entity& e=space1.ent(type, num);
    _test(space1.ent(type,num).key==key++);
    _test(space1.ent(type,num).type==type);
    _test(space1.ent(type,num).used());
    _test(space1.ent(type,num).domain==&space1.domain(type,num));
  }

  type=IBEX_UPR;
  _meth="ent(IBEX_UPR,dom_num)";
  _nber = 0;  
  for (int num=0; num<=4; num++) {
    const Entity& e=space1.ent(type, num);
    _test(space1.ent(type,num).key==key++);
    _test(space1.ent(type,num).type==type);
    _test(space1.ent(type,num).used());
    _test(space1.ent(type,num).domain==&space1.domain(type,num));
  }  
  
  _meth="var(name)";
  _nber = 0;
  _test(space1.var("a")==0);
  _test(space1.var("b")==1);

  _meth="syb(name)";
  _nber = 0;
  _test(space1.syb("c")==0);
  _test(space1.syb("d")==1);
  _test(space1.syb("e")==2);

  _meth="epr(name)";
  _nber = 0;
  _test(space1.epr("f")==0);
  _test(space1.epr("g")==1);
  _test(space1.epr("h")==2);
  _test(space1.epr("i")==3);

  _meth="upr(name)";
  _nber = 0;
  _test(space1.upr("j")==0);
  _test(space1.upr("k")==1);
  _test(space1.upr("l")==2);
  _test(space1.upr("m")==3);
  _test(space1.upr("n")==4);

  _meth="used(key)";
  _nber = 0;
  for (int i=0; i<=13; i++)
    _test(space1.used(i));
  _test(!space1.used(16));
}


void T_Space() {

  _class="Space";

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

  const Symbol& A=env.add_symbol("A",3,2);
  const Symbol& B=env.add_symbol("B",2,2);

  SpaceFactory spacef1(env);

  spacef1.set_entity("a",IBEX_VAR, INTERVAL(0,1));
  spacef1.set_entity("b",IBEX_VAR, INTERVAL(0,2));

  spacef1.set_entity("c",IBEX_SYB, INTERVAL(0,3));
  spacef1.set_entity("d",IBEX_SYB, INTERVAL(0,4));
  spacef1.set_entity("e",IBEX_SYB, INTERVAL(0,5));

  spacef1.set_entity("f",IBEX_EPR, INTERVAL(0,6));
  spacef1.set_entity("g",IBEX_EPR, INTERVAL(0,7));
  spacef1.set_entity("h",IBEX_EPR, INTERVAL(0,8));
  spacef1.set_entity("i",IBEX_EPR, INTERVAL(0,9));

  spacef1.set_entity("j",IBEX_UPR, INTERVAL(0,10));
  spacef1.set_entity("k",IBEX_UPR, INTERVAL(0,11));
  spacef1.set_entity("l",IBEX_UPR, INTERVAL(0,12));
  spacef1.set_entity("m",IBEX_UPR, INTERVAL(0,13));
  spacef1.set_entity("n",IBEX_UPR, INTERVAL(0,14));



  Space space1(spacef1);

  T_Space1(space1);

  _meth="copy constr";
  _nber=0;
  Space space2(space1);
  T_Space1(space2);

  _meth="operator=";
  _nber=0;
  SpaceFactory spacef3(env);
  Space space3(env);
  space3=space1;
  T_Space1(space3);

  Env env4;
  SpaceFactory spacef4(env4);
  Space space4(env4);
  try {
    space4 = space1;
    _test(false);
  } catch(NonRecoverableException) { // NotSameEnvException
    _test(true);
  }
 
  _meth="set_entity (with matrix)";
  SpaceFactory spacef0(env);
  spacef0.set_entity("A",IBEX_VAR,INTERVAL(1,1));
  spacef0.set_entity("B[1][1]",IBEX_SYB,INTERVAL(0,2));
  spacef0.set_entity("B[1][2]",IBEX_EPR,INTERVAL(0,3));
  spacef0.set_entity("B[2][1]",IBEX_UPR,INTERVAL(0,4));
  spacef0.set_entity("B[2][2]",IBEX_VAR,INTERVAL(0,5));

  Space space0(spacef0);
  int key=14;
  for (int i=0; i<6; i++) {
    _test(Inf(space0.Space::domain(key))==1 && Sup(space0.Space::domain(key))==1);
    key++;
  }
  _test(Sup(space0.Space::domain(key++))==2);
  _test(Sup(space0.Space::domain(key++))==3);
  _test(Sup(space0.Space::domain(key++))==4);
  _test(Sup(space0.Space::domain(key++))==5);

  _meth="var(name) (with matrix)";
  _test(space0.var("A[1][1]")==0);
  _test(space0.var("A[1][2]")==1);
  _test(space0.var("A[2][1]")==2);
  _test(space0.var("A[2][2]")==3);
  _test(space0.var("A[3][1]")==4);
  _test(space0.var("A[3][2]")==5);
  _test(space0.var("B[2][2]")==6);

  _test(space0.syb("B[1][1]")==0);
  _test(space0.epr("B[1][2]")==0);
  _test(space0.upr("B[2][1]")==0);
  
}
