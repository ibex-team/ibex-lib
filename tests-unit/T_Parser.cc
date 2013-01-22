/* CHABERT jan 2008 */

#include "T_.h"
#include "IbexSystemFactory.h"
#include "IbexEnv.h"
#include "IbexFunction.h"
#include "IbexSequence.h"
#include <sstream>

using namespace ibex;

/*********************************************************************************/

const char* equs_ponts[30] = {
  "(((N_x-O_x)^2+(N_y-O_y)^2)-0.0625)=0",
  "(((M_x-O_x)^2+(M_y-O_y)^2)-0.0625)=0",
  "(((M_x-N_x)^2+(M_y-N_y)^2)-0.0625)=0",
  "(((J_x-N_x)^2+(J_y-N_y)^2)-0.0625)=0",
  "(((L_x-M_x)^2+(L_y-M_y)^2)-0.0625)=0",
  "(((K_x-M_x)^2+(K_y-M_y)^2)-0.0625)=0",
  "(((G_x-L_x)^2+(G_y-L_y)^2)-0.0625)=0",
  "(((K_x-L_x)^2+(K_y-L_y)^2)-0.09)=0",
  "(((J_x-K_x)^2+(J_y-K_y)^2)-0.0625)=0",
  "(((K_x-N_x)^2+(K_y-N_y)^2)-0.0625)=0",
  "(((I_x-J_x)^2+(I_y-J_y)^2)-9)=0",
  "(((H_x-J_x)^2+(H_y-J_y)^2)-25)=0",
  "(((B_x-I_x)^2+(B_y-I_y)^2)-25)=0",
  "(((A_x-I_x)^2+(A_y-I_y)^2)-4)=0",
  "(((B_x-H_x)^2+(B_y-H_y)^2)-9)=0",
  "(((H_x-I_x)^2+(H_y-I_y)^2)-16)=0",
  "(((F_x-G_x)^2+(F_y-G_y)^2)-0.0625)=0",
  "(((C_x-G_x)^2+(C_y-G_y)^2)-0.0625)=0",
  "(((F_x-L_x)^2+(F_y-L_y)^2)-0.0625)=0",
  "(((D_x-F_x)^2+(D_y-F_y)^2)-0.0625)=0",
  "(((D_x-E_x)^2+(D_y-E_y)^2)-0.0625)=0",
  "(((E_x-F_x)^2+(E_y-F_y)^2)-0.0625)=0",
  "(((D_x-G_x)^2+(D_y-G_y)^2)-0.0625)=0",
  "(((C_x-D_x)^2+(C_y-D_y)^2)-0.0625)=0",
  "(((C_x-H_x)^2+(C_y-H_y)^2)-18.003)=0",
  "(((B_x-C_x)^2+(B_y-C_y)^2)-9)=0",
  "(((A_x-B_x)^2+(A_y-B_y)^2)-25)=0",
  "B_x=0",
  "A_y=0",
  "A_x=0"
};

void T_Ponts() {
  _meth="[ponts]";
  _nber = 0;

  System sys("ponts.txt");
//   cout << sys << endl;
  _test(sys.nb_var()==30);
  _test(sys.nb_ctr()==30);

  for (int i=0; i<30; i++) {
    stringstream oss;
    oss << sys.ctr(i);
    _test(oss.str().compare(equs_ponts[i])==0);
  }

  IdTable<INTERVAL> domains_ponts;

  domains_ponts.insert_new("O_y", INTERVAL(-46.516999999999996, 53.483000000000004));
  domains_ponts.insert_new("O_x", INTERVAL(-53.574999999999996, 46.425000000000004));
  domains_ponts.insert_new("N_y", INTERVAL(-46.759999999999998, 53.240000000000002));
  domains_ponts.insert_new("N_x", INTERVAL(-53.517999999999994, 46.482000000000006));
  domains_ponts.insert_new("M_y", INTERVAL(-46.589999999999996, 53.410000000000004));
  domains_ponts.insert_new("M_x", INTERVAL(-53.335999999999999, 46.664000000000001));
  domains_ponts.insert_new("L_y", INTERVAL(-46.602999999999994, 53.397000000000006));
  domains_ponts.insert_new("L_x", INTERVAL(-53.085999999999999, 46.914000000000001));
  domains_ponts.insert_new("K_y", INTERVAL(-46.832999999999998, 53.167000000000002));
  domains_ponts.insert_new("K_x", INTERVAL(-53.278999999999996, 46.721000000000004));
  domains_ponts.insert_new("J_y", INTERVAL(-47.003999999999998, 52.996000000000002));
  domains_ponts.insert_new("J_x", INTERVAL(-53.461999999999996, 46.538000000000004));
  domains_ponts.insert_new("I_y", INTERVAL(-49.599999999999994, 50.400000000000006));
  domains_ponts.insert_new("I_x", INTERVAL(-51.958999999999996, 48.041000000000004));
  domains_ponts.insert_new("H_y", INTERVAL(-47.596999999999994, 52.403000000000006));
  domains_ponts.insert_new("H_x", INTERVAL(-48.497999999999998, 51.502000000000002));
  domains_ponts.insert_new("G_y", INTERVAL(-46.552999999999997, 53.447000000000003));
  domains_ponts.insert_new("G_x", INTERVAL(-52.840999999999994, 47.159000000000006));
  domains_ponts.insert_new("F_y", INTERVAL(-46.789999999999999, 53.210000000000001));
  domains_ponts.insert_new("F_x", INTERVAL(-52.919999999999995, 47.080000000000005));
  domains_ponts.insert_new("E_y", INTERVAL(-46.976999999999997, 53.023000000000003));
  domains_ponts.insert_new("E_x", INTERVAL(-52.753999999999998, 47.246000000000002));
  domains_ponts.insert_new("D_y", INTERVAL(-46.739999999999995, 53.260000000000005));
  domains_ponts.insert_new("D_x", INTERVAL(-52.674999999999997, 47.325000000000003));
  domains_ponts.insert_new("C_y", INTERVAL(-46.503, 53.497));
  domains_ponts.insert_new("C_x", INTERVAL(-52.595999999999997, 47.404000000000003));
  domains_ponts.insert_new("B_y", INTERVAL(0, 55));
  domains_ponts.insert_new("B_x", INTERVAL(-50, 50));
  domains_ponts.insert_new("A_y", INTERVAL(-50, 50));
  domains_ponts.insert_new("A_x", INTERVAL(-50, 50));

  for (int i=0; i<30; i++) {
    _test(domains_ponts.data(sys.space.env.symbol_name(i)) == sys.box(sys.space.component(i)+1));
  }

}

const char* equs_yam[8] = {
  "((((((((((((2.5*(x[1])^3)-(10.5*x[1]^2))+(11.8*x[1]))-0)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "((((((((((((2.5*(x[2])^3)-(10.5*x[2]^2))+(11.8*x[2]))-1)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "((((((((((((2.5*(x[3])^3)-(10.5*x[3]^2))+(11.8*x[3]))-2)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "((((((((((((2.5*(x[4])^3)-(10.5*x[4]^2))+(11.8*x[4]))-3)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "((((((((((((2.5*(x[5])^3)-(10.5*x[5]^2))+(11.8*x[5]))-4)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "((((((((((((2.5*(x[6])^3)-(10.5*x[6]^2))+(11.8*x[6]))-5)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "((((((((((((2.5*(x[7])^3)-(10.5*x[7]^2))+(11.8*x[7]))-6)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "((((((((((((2.5*(x[8])^3)-(10.5*x[8]^2))+(11.8*x[8]))-7)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0"
};

const char* equs_yam_func[8] = {
  "(((((((((foo(x[1])-0)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "(((((((((foo(x[2])-1)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "(((((((((foo(x[3])-2)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "(((((((((foo(x[4])-3)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "(((((((((foo(x[5])-4)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "(((((((((foo(x[6])-5)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "(((((((((foo(x[7])-6)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0",
  "(((((((((foo(x[8])-7)+x[1])+x[2])+x[3])+x[4])+x[5])+x[6])+x[7])+x[8])=0"
};

void T_Yamamura() {
  _meth="[yamamura8]";
  _nber = 0;

  System sys("yamamura8.txt");

  _test(sys.nb_var()==8);
  _test(sys.nb_ctr()==8);

  for (int i=0; i<8; i++) {
    stringstream oss;
    oss << sys.ctr(i);
    _test(oss.str().compare(equs_yam[i])==0);
  }

  for (int i=0; i<8; i++) {
    _test(sys.box(i+1)==INTERVAL(-1e08,1e08));
  }

}

void T_Yamamura_loop() {
  _meth="[yamamura-loop]";
  _nber = 0;

  System sys("yamamura-loop.txt");

  _test(sys.nb_var()==8);
  _test(sys.nb_ctr()==8);

  for (int i=0; i<8; i++) {
    stringstream oss;
    oss << sys.ctr(i);
    _test(oss.str().compare(equs_yam[i])==0);
  }

  for (int i=0; i<8; i++) {
    _test(sys.box(i+1)==INTERVAL(-1e08,1e08));
  }
  
}

static INTERVAL_VECTOR inflat(const INTERVAL_VECTOR& x) {
  INTERVAL_VECTOR y(x);
  for (int i=1; i<=Dimension(x); i++)
    y(i)=INTERVAL(Inf(x(i))-1e-9,Sup(x(i))+1e-9);
  return y;
}

void T_Yamamura_func0() {
  _meth="[yamamura-func0]";
  _nber = 0;

  System sys("yamamura-func0.txt");

  _test(sys.nb_var()==8);
  _test(sys.nb_ctr()==8);

  for (int i=0; i<8; i++) {
    _test(sys.box(i+1)==INTERVAL(-1e08,1e08));
  }

  INTERVAL_VECTOR feval(8);
  for (int i=0; i<8; i++) feval(i+1)=INTERVAL(-2.500001e24,2.500001e24);

  INTERVAL_VECTOR contr(8);
  contr(1)=INTERVAL(-9.2521300183655398542E+2,3.4760266667357942788E+5);
  contr(2)=INTERVAL(-9.0942323096641962366E+2,3.4760266656323074131E+5);
  contr(3)=INTERVAL(-8.9306525513448673337E+2,3.4760266645288147266E+5);
  contr(4)=INTERVAL(-8.7608502003495129884E+2,3.4760266634253284429E+5);
  contr(5)=INTERVAL(-8.5841974211194644795E+2,3.4760266623218357564E+5);
  contr(6)=INTERVAL(-8.3999584433550512586E+2,3.4760266612183488906E+5);
  contr(7)=INTERVAL(-8.2072622754139433710E+2,3.4760266601148503833E+5);
  contr(8)=INTERVAL(-8.0050659822683462608E+2,3.4760266590113635175E+5);

  INTERVAL_VECTOR y(8);
  sys.eval(y);
  _test(y.included(inflat(feval)));
  Sequence(sys).contract();
  _test(sys.box.included(inflat(contr)));
}

void T_Yamamura_func() {
  _meth="[yamamura-func]";
  _nber = 0;
  
  System sys("yamamura-func.txt");

  _test(sys.nb_var()==8);
  _test(sys.nb_ctr()==8);

  for (int i=0; i<8; i++) {
    _test(sys.box(i+1)==INTERVAL(-1e08,1e08));
  }

  INTERVAL_VECTOR feval(8);
  for (int i=0; i<8; i++) feval(i+1)=INTERVAL(-2.500001e24,2.500001e24);

  INTERVAL_VECTOR contr(8);
  contr(1)=INTERVAL(-9.2521300183655398542E+2,3.4760266667357942788E+5);
  contr(2)=INTERVAL(-9.0942323096641962366E+2,3.4760266656323074131E+5);
  contr(3)=INTERVAL(-8.9306525513448673337E+2,3.4760266645288147266E+5);
  contr(4)=INTERVAL(-8.7608502003495129884E+2,3.4760266634253284429E+5);
  contr(5)=INTERVAL(-8.5841974211194644795E+2,3.4760266623218357564E+5);
  contr(6)=INTERVAL(-8.3999584433550512586E+2,3.4760266612183488906E+5);
  contr(7)=INTERVAL(-8.2072622754139433710E+2,3.4760266601148503833E+5);
  contr(8)=INTERVAL(-8.0050659822683462608E+2,3.4760266590113635175E+5);

  INTERVAL_VECTOR y(8);

  sys.eval(y);
  _test(y.included(inflat(feval)));
  Sequence(sys).contract();
  _test(sys.box.included(inflat(contr)));
}

void T_Parser() {
  _class="Parser";
  try {
    T_Ponts();
    T_Yamamura();
    T_Yamamura_loop();
    T_Yamamura_func0();  
     T_Yamamura_func();
  } catch(SyntaxErrorException e) {
    _test(false);
    cout << e << endl;
    throw e;
  }
}
