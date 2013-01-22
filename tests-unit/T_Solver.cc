/* CHABERT jan 2008 */

#include "T_.h"
#include "IbexHC4.h"
#include "IbexHC4Revise.h"
#include "IbexNewton.h"
#include "IbexSequence.h"
#include "IbexPropagation.h"
#include "IbexShaving.h"
#include "IbexBisector.h"
#include "IbexPaver.h"
#include "IbexEnv.h"
#include "ponts.h"
#include "yamamura.h"

using namespace ibex;

static INTERVAL_VECTOR inflat(const INTERVAL_VECTOR& x) {
  INTERVAL_VECTOR y(x);
  for (int i=1; i<=Dimension(x); i++)
    y(i)=INTERVAL(Inf(x(i))-1e-9,Sup(x(i))+1e-9);
  return y;
}


/*********************************************************************************/
void T_Solve_OneEqu() {

  _class="Paver";
  _meth="explore(one equ)";
  _nber = 0;

  System sys("x0.txt");
    
  Paver solver(Sequence(sys),RoundRobin(sys.space, 1e-07));
  solver.explore();
  _test(solver.nb_boxes(1)==1);
}

/*********************************************************************************/
void T_Solve_Yamamura() {

  _class="Paver";
  _meth="explore(yamamura)";
  _nber = 0;

  System sys=yamamura8();

  RoundRobin rr(sys.space,1e-08);
  vector<const Contractor*> ctcs;
  Sequence comp(HC4(sys,0.1), Newton(sys));
  ctcs.push_back(&comp);
  Precision prec(sys.space,1e-08);
  ctcs.push_back(&prec);
  Paver solver(sys.space,ctcs,rr);
  double time=solver.explore();

  _test(time<=70);

  _meth="nb_cells()";
  _nber = 0;
  _test(solver.nb_cells()==136201); // 109761 -> with "bug" 2B

  _meth="nb_boxes()";
  _nber = 0;
  _test(solver.nb_boxes(1)==7);

  _meth="box(num)";
  _nber = 0;

  INTERVAL_MATRIX sol(7,8);

  sol(2,1)=INTERVAL(-0.2205357134,-0.2205357134);
  sol(2,2)=INTERVAL(-0.1582241716,-0.1582241716);
  sol(2,3)=INTERVAL(-0.08934090953,-0.08934090953);
  sol(2,4)=INTERVAL(-0.01172598293,-0.01172598293);
  sol(2,5)=INTERVAL(0.07824313583,0.07824313583);
  sol(2,6)=INTERVAL(2.245937707,2.245937707);
  sol(2,7)=INTERVAL(0.3334978171,0.3334978171);
  sol(2,8)=INTERVAL(0.9619624827,0.9619624827);

  sol(1,1)=INTERVAL(-0.2130155974,-0.2130155974);
  sol(1,2)=INTERVAL(-0.1499672662,-0.1499672662);
  sol(1,3)=INTERVAL(-0.08012658492,-0.08012658492);
  sol(1,4)=INTERVAL(-0.001201479411,-0.001201479411);
  sol(1,5)=INTERVAL(0.09070604299,0.09070604299);
  sol(1,6)=INTERVAL(2.291437501,2.291437501);
  sol(1,7)=INTERVAL(0.3565579234,0.3565579234);
  sol(1,8)=INTERVAL(0.7198020794,0.7198020794);

  sol(5,1)=INTERVAL(-0.2753079349,-0.2753079349);
  sol(5,2)=INTERVAL(-0.2179605539,-0.2179605539);
  sol(5,3)=INTERVAL(-0.1553983183,-0.1553983183);
  sol(5,4)=INTERVAL(-0.0861898734,-0.0861898734);
  sol(5,5)=INTERVAL(-0.008131096858,-0.008131096858);
  sol(5,6)=INTERVAL(0.08249202899,0.08249202899);
  sol(5,7)=INTERVAL(2.262708777,2.262708777);
  sol(5,8)=INTERVAL(2.494429463,2.494429463);

  sol(3,1)=INTERVAL(-0.2142528024,-0.2142528024);
  sol(3,2)=INTERVAL(-0.1513266745,-0.1513266745);
  sol(3,3)=INTERVAL(-0.08164514879,-0.08164514879);
  sol(3,4)=INTERVAL(-0.002938550381,-0.002938550381);
  sol(3,5)=INTERVAL(0.08864402826,0.08864402826);
  sol(3,6)=INTERVAL(0.2006630798,0.2006630798);
  sol(3,7)=INTERVAL(2.504464728,2.504464728);
  sol(3,8)=INTERVAL(0.691156966,0.691156966);

  sol(6,1)=INTERVAL(-0.2258848891,-0.2258848891);
  sol(6,2)=INTERVAL(-0.1640888226,-0.1640888226);
  sol(6,3)=INTERVAL(-0.09587233513,-0.09587233513);
  sol(6,4)=INTERVAL(-0.01916386621,-0.01916386621);
  sol(6,5)=INTERVAL(0.06947793072,0.06947793072);
  sol(6,6)=INTERVAL(0.1765775019,0.1765775019);
  sol(6,7)=INTERVAL(2.471821511,2.471821511);
  sol(6,8)=INTERVAL(1.01714035,1.01714035);

  sol(4,1)=INTERVAL(-0.2507492217,-0.2507492217);
  sol(4,2)=INTERVAL(-0.1912594962,-0.1912594962);
  sol(4,3)=INTERVAL(-0.1259962881,-0.1259962881);
  sol(4,4)=INTERVAL(-0.05324567102,-0.05324567102);
  sol(4,5)=INTERVAL(0.02972607379,0.02972607379);
  sol(4,6)=INTERVAL(0.1277774434,0.1277774434);
  sol(4,7)=INTERVAL(1.561731346,1.561731346);
  sol(4,8)=INTERVAL(2.560460689,2.560460689);

  sol(7,1)=INTERVAL(-0.2075155167,-0.2075155167);
  sol(7,2)=INTERVAL(-0.1439191391,-0.1439191391);
  sol(7,3)=INTERVAL(-0.07336291728,-0.07336291728);
  sol(7,4)=INTERVAL(0.006548113237,0.006548113237);
  sol(7,5)=INTERVAL(0.09993025768,0.09993025768);
  sol(7,6)=INTERVAL(0.2150438698,0.2150438698);
  sol(7,7)=INTERVAL(0.3743068798,0.3743068798);
  sol(7,8)=INTERVAL(2.652150231,2.652150231);

  for (int i=0; i<7; i++) {
//     cout << solver.box(1,i) << " " << inflat(Row(sol,i+1)) << endl << endl << endl;   
    _test(solver.box(1,i).included(inflat(Row(sol,i+1))));
  }
}

void T_Solve_Ponts() {
  _class="Paver";
  _meth="explore(ponts) + parser";
  _nber = 0;
    
  System sys("ponts.txt");
  INTERVAL_VECTOR box=sys.box;

  { /* warning: variable declaration in ponts.txt has been changed (swap between X_x and X_y) */
    RoundRobin rr(sys.space,1e-07);
    HC4 hc4(sys,0.1);
    Paver solver(hc4,rr);
//   solver.trace=true;
    double time=solver.explore();
//     solver.report();

    /* With new variable order without "bug" 2B (which returns false whereas some tiny reduction was made) */
    _test(solver.nb_cells()==114677);
    _test(solver.nb_boxes(1)==1524);
    _test(time<70);

    /* With initial variable order + "bug" 2B */
//     _test(solver.nb_cells()==114003);
//     _test(solver.nb_boxes(1)==1564);
//     _test(time<70);
    /* With old variable order + "bug" 2B : */
//   _test(solver.nb_cells()==127161);
//   _test(solver.nb_boxes(0)==1710);
//   _test(time<70);
  }

  {
//     RoundRobin rr(sys.space,1e-08);
//     Newton newt(sys);
//     Propagation propag(sys,0.1);
//     Sequence ctc(propag,newt);
//     Paver solver(ctc,rr);
    
//     double time=solver.explore();
//     solver.report();
  }


}


void T_Solve_Ponts_dist() {
 _class="Paver";
  _meth="explore(ponts-dist) + parser + function";
  _nber = 0;
    
  System sys("ponts-dist.txt");

  sys.box(sys.space.var("O[2]")+1)=INTERVAL( -46.516999999999996, 53.483000000000004 );
  sys.box(sys.space.var("O[1]")+1)=INTERVAL( -53.574999999999996, 46.425000000000004 );
  sys.box(sys.space.var("N[2]")+1)=INTERVAL( -46.759999999999998, 53.240000000000002 );
  sys.box(sys.space.var("N[1]")+1)=INTERVAL( -53.517999999999994, 46.482000000000006 );
  sys.box(sys.space.var("M[2]")+1)=INTERVAL( -46.589999999999996, 53.410000000000004 );
  sys.box(sys.space.var("M[1]")+1)=INTERVAL( -53.335999999999999, 46.664000000000001 );
  sys.box(sys.space.var("L[2]")+1)=INTERVAL( -46.602999999999994, 53.397000000000006 );
  sys.box(sys.space.var("L[1]")+1)=INTERVAL( -53.085999999999999, 46.914000000000001 );
  sys.box(sys.space.var("K[2]")+1)=INTERVAL( -46.832999999999998, 53.167000000000002 );
  sys.box(sys.space.var("K[1]")+1)=INTERVAL( -53.278999999999996, 46.721000000000004 );
  sys.box(sys.space.var("J[2]")+1)=INTERVAL( -47.003999999999998, 52.996000000000002 );
  sys.box(sys.space.var("J[1]")+1)=INTERVAL( -53.461999999999996, 46.538000000000004 );
  sys.box(sys.space.var("I[2]")+1)=INTERVAL( -49.599999999999994, 50.400000000000006 );
  sys.box(sys.space.var("I[1]")+1)=INTERVAL( -51.958999999999996, 48.041000000000004 );
  sys.box(sys.space.var("H[2]")+1)=INTERVAL( -47.596999999999994, 52.403000000000006 );
  sys.box(sys.space.var("H[1]")+1)=INTERVAL( -48.497999999999998, 51.502000000000002 );
  sys.box(sys.space.var("G[2]")+1)=INTERVAL( -46.552999999999997, 53.447000000000003 );
  sys.box(sys.space.var("G[1]")+1)=INTERVAL( -52.840999999999994, 47.159000000000006 );
  sys.box(sys.space.var("F[2]")+1)=INTERVAL( -46.789999999999999, 53.210000000000001 );
  sys.box(sys.space.var("F[1]")+1)=INTERVAL( -52.919999999999995, 47.080000000000005 );
  sys.box(sys.space.var("E[2]")+1)=INTERVAL( -46.976999999999997, 53.023000000000003 );
  sys.box(sys.space.var("E[1]")+1)=INTERVAL( -52.753999999999998, 47.246000000000002 );
  sys.box(sys.space.var("D[2]")+1)=INTERVAL( -46.739999999999995, 53.260000000000005 );
  sys.box(sys.space.var("D[1]")+1)=INTERVAL( -52.674999999999997, 47.325000000000003 );
  sys.box(sys.space.var("C[2]")+1)=INTERVAL( -46.503, 53.497 );
  sys.box(sys.space.var("C[1]")+1)=INTERVAL( -52.595999999999997, 47.404000000000003 );
  sys.box(sys.space.var("B[2]")+1)=INTERVAL( 0, 55 );
  sys.box(sys.space.var("B[1]")+1)=INTERVAL( -50, 50 );
  sys.box(sys.space.var("A[2]")+1)=INTERVAL( -50, 50 );
  sys.box(sys.space.var("A[1]")+1)=INTERVAL( -50, 50 );


  RoundRobin rr(sys.space,1e-07);
  HC4 hc4(sys,0.1);
  Paver solver(hc4,rr);
  double time=solver.explore();
//   solver.report();
  _test(solver.nb_cells()==114677);
  _test(solver.nb_boxes(1)==1524);
  _test(time<70);
}


void T_Paving() {

  Env env;
  const Symbol& x=env.add_symbol("x");
  const Symbol& y=env.add_symbol("y");
  const Symbol& r=env.add_symbol("r");

  REAL rmin=16;
  REAL rmax=25;

  SpaceFactory spacef(env);

  spacef.set_entity("x",IBEX_VAR, INTERVAL(-10,10));
  spacef.set_entity("y",IBEX_VAR, INTERVAL(-10,10));
  spacef.set_entity("r",IBEX_EPR, INTERVAL(rmin,rmax));

  Space space(spacef);

  env.add_ctr(sqr(x)+sqr(y)=r);
  env.add_ctr(ibex::max( (sqr(x)+sqr(y))-rmax, rmin-(sqr(x)+sqr(y)) )<0);

  
  vector<const Contractor*> ctc;
  HC4Revise inner(env.constraint(1),space);
  HC4Revise outer(env.constraint(0),space);
  Precision prec(space,1);
  ctc.push_back(&inner);
  ctc.push_back(&outer);
  ctc.push_back(&prec);
  Paver solver(space,ctc,RoundRobin(space,1e-01));
//   solver.trace=true;
  solver.explore();
//    solver.report();
//   cout << solver.result() << endl;
}
 
void T_Paver() {
  try {
    T_Solve_OneEqu();
    T_Solve_Yamamura();
    T_Solve_Ponts();    
    T_Solve_Ponts_dist();
//  T_Paving();
  } catch(SyntaxErrorException e) {
    cout << e << endl;
  }
}
