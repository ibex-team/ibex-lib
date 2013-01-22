
#include "T_.h"
#include "T_Interval.h"
#include "T_IntervalVector.h"
#include "T_Env.h"
#include "T_Expr.h"
#include "T_Constraint.h"
#include "T_AtMostNVector.h"
#include "T_Func.h"
#include "T_Adjacency.h"
#include "T_Space.h"
#include "T_BlockSet.h"
#include "T_Eval.h"
//#include "T_BlockPaver.h"
#include "T_UnivNewton.h"
#include "T_Parser.h"
#include "T_Propag.h"
#include "T_Shave.h"
#include "T_Newton.h"
#include "T_Solver.h"
#include "T_IO.h"
#include "T_Constraint.h"
#include "T_QInter.h"
#include <iostream>
#include <stdlib.h>

void T_Bisection();

int failed=0;
int N=0;
const char* _class=NULL;
const char* _meth=NULL;

int _nber;

using namespace std;

void _test(bool mustbetrue) {
  _nber++;
  N++;
  cout << N << '\t' << _class << '\t' << _meth << '\t' << _nber << '\t';
  if (mustbetrue) {
    cout << "ok\n";
  }
  else {
    cout << "FAILED\n";
    failed++;
  }
}

bool approx(REAL value, REAL ref, REAL delta) {
  return (value >= ref-delta) && (value <= ref+delta);
}

bool approxI(const INTERVAL& x, REAL lb, REAL ub, REAL delta) {
  return approx(Inf(x),lb,delta) && approx(Sup(x),ub,delta);
}

#include <math.h>


int main() {
  try {
    
    T_delta();
    T_ratiodelta();
    T_maxratiodelta();
    T_div_intersect();
    T_perimeter();
    T_Bisection();
    
    T_expr();
    
    T_env();
    T_Space(); 
    T_Transpose();
    //T_Equality();
    
    //T_AtMostNVector();

    T_Propag();

    //T_Shave(); 
    T_Newton(); 

    T_Func();
    T_Apply(); 

    T_Parser(); 
    //    catch(ibex::SyntaxErrorException e) {
    //cout << e << endl;
    //} 
    T_Paver();
    T_IO();
  
    T_Adjacency();

    T_BlockSet();
    T_block_round_robin();

    T_Univ_Newton(); 
    T_Eval();   

    T_QInter();

    cout << endl << endl;
    cout << N << " tests.\n";
    cout << failed << " failed.\n";

  } catch(ibex::NonRecoverableException e) {
    cout << "Error:" << e << endl;
  }

  return 0;
}

