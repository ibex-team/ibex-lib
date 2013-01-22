/* CHABERT jan 2008 */

#include "T_.h"
#include "IbexPropagation.h"
#include "IbexHC4.h"
#include "IbexShaving.h"
#include "IbexEnv.h"
#include "ponts.h"
#include "yamamura.h"

using namespace ibex;

#define inflat(x) (INTERVAL(Inf(x)-1e-9,Sup(x)+1e-9))

/*********************************************************************************/
void T_Shave() {

  _class="Shaving";
  _meth="[yamamura]";
  _nber = 0;

  System csp2=yamamura8();
  HC4 propag(csp2);
  Shaving shave(propag);
  shave.contract();

  _test(csp2.space.box(1).included(inflat(INTERVAL(-1.3106215293503343,4.2936586849941794)))) ;
  _test(csp2.space.box(2).included(inflat(INTERVAL(-1.2978482190584873,3.8318423537837721)))) ;
  _test(csp2.space.box(3).included(inflat(INTERVAL(-1.2849774179526694,4.3027309395955618)))) ;
  _test(csp2.space.box(4).included(inflat(INTERVAL(-1.2720070719603119,4.3073184453813278)))) ;
  _test(csp2.space.box(5).included(inflat(INTERVAL(-1.2589350572124898,3.8477012471226458)))) ;
  _test(csp2.space.box(6).included(inflat(INTERVAL(-1.2457591767683643,3.8530690425195178)))) ;
  _test(csp2.space.box(7).included(inflat(INTERVAL(-1.2324771571464864,3.8584787354608228)))) ;
  _test(csp2.space.box(8).included(inflat(INTERVAL(-1.1976177102724284,3.8860603844544866)))) ;


}
