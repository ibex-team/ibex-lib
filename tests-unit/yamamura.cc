/* CHABERT jan 2008 */

#include "IbexEnv.h"
#include "IbexSpace.h"
#include "IbexSystemFactory.h"
#include "IbexSystem.h"
#include "IbexPropagation.h"

using namespace ibex;

static Env yam_env;
static bool called=false;

/*********************************************************************************/
System yamamura8() {
  
  if (!called) {
  const Symbol& x = yam_env.add_symbol("x",8);

  yam_env.add_ctr(2.5*pow(x[1],3) -10.5*pow(x[1],2) + 11.8*x[1] -0 +x[1]+x[2]+x[3]+x[4]+x[5]+x[6]+x[7]+x[8]=0);
  yam_env.add_ctr(2.5*pow(x[2],3) -10.5*pow(x[2],2) + 11.8*x[2] -1 +x[1]+x[2]+x[3]+x[4]+x[5]+x[6]+x[7]+x[8]=0);
  yam_env.add_ctr(2.5*pow(x[3],3) -10.5*pow(x[3],2) + 11.8*x[3] -2 +x[1]+x[2]+x[3]+x[4]+x[5]+x[6]+x[7]+x[8]=0);
  yam_env.add_ctr(2.5*pow(x[4],3) -10.5*pow(x[4],2) + 11.8*x[4] -3 +x[1]+x[2]+x[3]+x[4]+x[5]+x[6]+x[7]+x[8]=0);
  yam_env.add_ctr(2.5*pow(x[5],3) -10.5*pow(x[5],2) + 11.8*x[5] -4 +x[1]+x[2]+x[3]+x[4]+x[5]+x[6]+x[7]+x[8]=0);
  yam_env.add_ctr(2.5*pow(x[6],3) -10.5*pow(x[6],2) + 11.8*x[6] -5 +x[1]+x[2]+x[3]+x[4]+x[5]+x[6]+x[7]+x[8]=0);
  yam_env.add_ctr(2.5*pow(x[7],3) -10.5*pow(x[7],2) + 11.8*x[7] -6 +x[1]+x[2]+x[3]+x[4]+x[5]+x[6]+x[7]+x[8]=0);
  yam_env.add_ctr(2.5*pow(x[8],3) -10.5*pow(x[8],2) + 11.8*x[8] -7 +x[1]+x[2]+x[3]+x[4]+x[5]+x[6]+x[7]+x[8]=0);
  called=true;
  }

  SpaceFactory sysf(yam_env);
  
  sysf.set_entity("x", IBEX_VAR, INTERVAL(-1e8, 1e8));

  return System(sysf);
}
