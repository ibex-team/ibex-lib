
#include "T_UnivNewton.h"
#include "IbexUnivNewton.h"
#include "IbexEnv.h"
#include "IbexSystemFactory.h"
#include "IbexSystem.h"


using namespace ibex;

void T_Univ_Newton() {
  _class="UnivNewton";
  _meth="solve";
  _nber = 0;

  Env env;
  const Symbol& x=env.add_symbol("x");

  int wilk1fac=env.add_ctr(x-1=0); 
  int wilk2fac=env.add_ctr((x-1)*(x-2)=0);
  int wilk10=env.add_ctr(pow(x,10)-55*pow(x,9)+1320*pow(x,8)-18150*pow(x,7)+
		     157773*pow(x,6)-902055*pow(x,5)+3416930*pow(x,4)-8409500*pow(x,3)+
		     12753576*pow(x,2)-10628640*x+3628800=0);
  
  SpaceFactory fac(env);
  fac.set_entity("x",IBEX_VAR,INTERVAL(0.9,10.1));
  System sys(fac);

  {
    UnivNewton newton((const Equality&) sys.ctr(wilk2fac), sys.space, 1e-07);
    INTERVAL_VECTOR roots=newton.solve(0);
    _test(Dimension(roots)==2);
    _test(roots(1).contains(1.0) && roots(2).contains(2.0));
    _test(Diam(roots(1))<=1e-07 && Diam(roots(2))<=1e-07);
  }


  {
    UnivNewton newton((const Equality&) sys.ctr(wilk10), sys.space, 1e-07);
    INTERVAL_VECTOR roots=newton.solve(0);    
    _test(Dimension(roots)==10);
    _test(roots(1).contains(1.0) && roots(2).contains(2.0) &&
	  roots(3).contains(3.0) && roots(4).contains(4.0) &&
	  roots(5).contains(5.0) && roots(6).contains(6.0) &&
	  roots(7).contains(7.0) && roots(8).contains(8.0) &&
	  roots(9).contains(9.0) && roots(10).contains(10.0));
    _test(Diam(roots(1))<=1e-07 && Diam(roots(2))<=1e-07);
    _test(Diam(roots(3))<=1e-07 && Diam(roots(4))<=1e-07);
    _test(Diam(roots(5))<=1e-07 && Diam(roots(6))<=1e-07);
    _test(Diam(roots(7))<=1e-07 && Diam(roots(8))<=1e-07);
    _test(Diam(roots(9))<=1e-07 && Diam(roots(10))<=1e-07);
  }

//   cout << roots << endl;

  



}
