/* CHABERT jan 2008 */

#include "T_.h"
#include "IbexPropagation.h"
#include "IbexEnv.h"
#include "IbexHC4.h"
#include "ponts.h"
#include "yamamura.h"

using namespace ibex;

#define inflat(x) (INTERVAL(Inf(x)-1e-9,Sup(x)+1e-9))

/*********************************************************************************/
void T_Propag() {


  _class="Propagation";

/*
  _meth="[ponts]";
  _nber = 0;

  System sys= ponts();
  HC4 propag(sys);

  propag.contract();
  

  _test(sys.space.box(1).included(inflat(INTERVAL(0.9999999999999955,3.9174243050441673))));
  _test(sys.space.box(2).included(inflat(INTERVAL(-4.0000000000000036,4.0000000000000036))));
  _test(sys.space.box(3).included(inflat(INTERVAL(0.9999999999999955,3.6674243050441669))));
  _test(sys.space.box(4).included(inflat(INTERVAL(-4.0000000000000036,4.0000000000000036))));
  _test(sys.space.box(5).included(inflat(INTERVAL(1.2499999999999957,3.9174243050441673))));
  _test(sys.space.box(6).included(inflat(INTERVAL(-3.7500000000000032,3.7500000000000032))));
  _test(sys.space.box(7).included(inflat(INTERVAL(1.4999999999999960,3.9674243050441671))));
  _test(sys.space.box(8).included(inflat(INTERVAL(-3.5000000000000027,3.5000000000000027))));
  _test(sys.space.box(9).included(inflat(INTERVAL(1.1999999999999957,3.6674243050441669))));
  _test(sys.space.box(10).included(inflat(INTERVAL(-3.8000000000000030,3.8000000000000030))));
  _test(sys.space.box(11).included(inflat(INTERVAL(0.9499999999999955,3.4174243050441664))));
  _test(sys.space.box(12).included(inflat(INTERVAL(-4.0500000000000034,4.0500000000000034))));
  _test(sys.space.box(13).included(inflat(INTERVAL(-0.0000000000000045,0.4174243050441647))));
  _test(sys.space.box(14).included(inflat(INTERVAL(-2.0000000000000014,2.0000000000000014))));
  _test(sys.space.box(15).included(inflat(INTERVAL(1.9999999999999964,4.4174243050441673))));
  _test(sys.space.box(16).included(inflat(INTERVAL(-2.9428906808861828,2.9428906808861828))));
  _test(sys.space.box(17).included(inflat(INTERVAL(1.7499999999999962,4.2174243050441680))));
  _test(sys.space.box(18).included(inflat(INTERVAL(-3.2500000000000023,3.2500000000000023))));
  _test(sys.space.box(19).included(inflat(INTERVAL(1.4999999999999960,4.2174243050441680))));
  _test(sys.space.box(20).included(inflat(INTERVAL(-3.5000000000000027,3.5000000000000027))));
  _test(sys.space.box(21).included(inflat(INTERVAL(1.4999999999999960,4.4674243050441689))));
  _test(sys.space.box(22).included(inflat(INTERVAL(-3.5000000000000027,3.5000000000000027))));
  _test(sys.space.box(23).included(inflat(INTERVAL(1.7499999999999962,4.4674243050441689))));
  _test(sys.space.box(24).included(inflat(INTERVAL(-3.2500000000000023,3.2500000000000023))));
  _test(sys.space.box(25).included(inflat(INTERVAL(1.9999999999999964,4.4674243050441689))));
  _test(sys.space.box(26).included(inflat(INTERVAL(-2.9523487478857109,2.9523487478857109))));
  _test(sys.space.box(27).included(inflat(INTERVAL(4.9999999999999982,4.99999999999999127))));
  _test(sys.space.box(28).included(inflat(INTERVAL(0,0))));
  _test(sys.space.box(29).included(inflat(INTERVAL(0,0))));
  _test(sys.space.box(30).included(inflat(INTERVAL(0,0)))); 
*/
  _meth="[yamamura8]";
  _nber = 0;

  System sys2=yamamura8();
  HC4 propag2(sys2);
  propag2.contract();

  _test(sys2.space.box(1).included(inflat(INTERVAL(-3.8014127357256729,6.1617520897756811))));
  _test(sys2.space.box(2).included(inflat(INTERVAL(-3.7849709210426128,6.1635198868938890))));
  _test(sys2.space.box(3).included(inflat(INTERVAL(-3.7684033546407716,6.1653003921116368))));
  _test(sys2.space.box(4).included(inflat(INTERVAL(-3.7517073018899288,6.1670936178285390))));
  _test(sys2.space.box(5).included(inflat(INTERVAL(-3.7348799320416455,6.1688995672749770))));
  _test(sys2.space.box(6).included(inflat(INTERVAL(-3.7179183133927909,6.1707182331049158))));
  _test(sys2.space.box(7).included(inflat(INTERVAL(-3.7008194081076061,6.1725495957368724))));
  _test(sys2.space.box(8).included(inflat(INTERVAL(-3.9089126941267827,6.9362672412906346))));

}
