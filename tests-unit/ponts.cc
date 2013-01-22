/* CHABERT jan 2008 */

#include "IbexEnv.h"
#include "IbexSpace.h"
#include "IbexSystemFactory.h"
#include "IbexSystem.h"
#include "IbexConstraint.h"
#include "IbexPropagation.h"

using namespace ibex;

static Env ponts_env;
static bool called=false;

/*********************************************************************************/
System ponts() {
  
  if (!called) {
  const Symbol& O_y = ponts_env.add_symbol("O_y");
  const Symbol& O_x = ponts_env.add_symbol("O_x");
  const Symbol& N_y = ponts_env.add_symbol("N_y");
  const Symbol& N_x = ponts_env.add_symbol("N_x");
  const Symbol& M_y = ponts_env.add_symbol("M_y");
  const Symbol& M_x = ponts_env.add_symbol("M_x");
  const Symbol& L_y = ponts_env.add_symbol("L_y");
  const Symbol& L_x = ponts_env.add_symbol("L_x");
  const Symbol& K_y = ponts_env.add_symbol("K_y");
  const Symbol& K_x = ponts_env.add_symbol("K_x");
  const Symbol& J_y = ponts_env.add_symbol("J_y");
  const Symbol& J_x = ponts_env.add_symbol("J_x");
  const Symbol& I_y = ponts_env.add_symbol("I_y");
  const Symbol& I_x = ponts_env.add_symbol("I_x");
  const Symbol& H_y = ponts_env.add_symbol("H_y");
  const Symbol& H_x = ponts_env.add_symbol("H_x");
  const Symbol& G_y = ponts_env.add_symbol("G_y");
  const Symbol& G_x = ponts_env.add_symbol("G_x");
  const Symbol& F_y = ponts_env.add_symbol("F_y");
  const Symbol& F_x = ponts_env.add_symbol("F_x");
  const Symbol& E_y = ponts_env.add_symbol("E_y");
  const Symbol& E_x = ponts_env.add_symbol("E_x");
  const Symbol& D_y = ponts_env.add_symbol("D_y");
  const Symbol& D_x = ponts_env.add_symbol("D_x");
  const Symbol& C_y = ponts_env.add_symbol("C_y");
  const Symbol& C_x = ponts_env.add_symbol("C_x");
  const Symbol& B_y = ponts_env.add_symbol("B_y");
  const Symbol& B_x = ponts_env.add_symbol("B_x");
  const Symbol& A_y = ponts_env.add_symbol("A_y");
  const Symbol& A_x = ponts_env.add_symbol("A_x");

  ponts_env.add_ctr( (pow(N_x - O_x,2) + pow(N_y - O_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(M_x - O_x,2) + pow(M_y - O_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(M_x - N_x,2) + pow(M_y - N_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(J_x - N_x,2) + pow(J_y - N_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(L_x - M_x,2) + pow(L_y - M_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(K_x - M_x,2) + pow(K_y - M_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(G_x - L_x,2) + pow(G_y - L_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(K_x - L_x,2) + pow(K_y - L_y,2)) = 0.089999999999999997 ); 
  ponts_env.add_ctr( (pow(J_x - K_x,2) + pow(J_y - K_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(K_x - N_x,2) + pow(K_y - N_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(I_x - J_x,2) + pow(I_y - J_y,2)) = 9 ); 
  ponts_env.add_ctr( (pow(H_x - J_x,2) + pow(H_y - J_y,2)) = 25 ); 
  ponts_env.add_ctr( (pow(B_x - I_x,2) + pow(B_y - I_y,2)) = 25 ); 
  ponts_env.add_ctr( (pow(A_x - I_x,2) + pow(A_y - I_y,2)) = 4 ); 
  ponts_env.add_ctr( (pow(B_x - H_x,2) + pow(B_y - H_y,2)) = 9 ); 
  ponts_env.add_ctr( (pow(H_x - I_x,2) + pow(H_y - I_y,2)) = 16 ); 
  ponts_env.add_ctr( (pow(F_x - G_x,2) + pow(F_y - G_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(C_x - G_x,2) + pow(C_y - G_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(F_x - L_x,2) + pow(F_y - L_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(D_x - F_x,2) + pow(D_y - F_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(D_x - E_x,2) + pow(D_y - E_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(E_x - F_x,2) + pow(E_y - F_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(D_x - G_x,2) + pow(D_y - G_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(C_x - D_x,2) + pow(C_y - D_y,2)) = 0.0625 ); 
  ponts_env.add_ctr( (pow(C_x - H_x,2) + pow(C_y - H_y,2)) = 18.003049000000004 ); 
  ponts_env.add_ctr( (pow(B_x - C_x,2) + pow(B_y - C_y,2)) = 9 ); 
  ponts_env.add_ctr( (pow(A_x - B_x,2) + pow(A_y - B_y,2)) = 25 ); 
  ponts_env.add_ctr( B_x = 0 ); 
  ponts_env.add_ctr( A_y = 0 ); 
  ponts_env.add_ctr( A_x = 0 ); 
  called=true;
  }

  SpaceFactory sysf(ponts_env);
  
  sysf.set_entity("O_y", IBEX_VAR, INTERVAL( -46.516999999999996, 53.483000000000004 ));
  sysf.set_entity("O_x", IBEX_VAR, INTERVAL( -53.574999999999996, 46.425000000000004 ));
  sysf.set_entity("N_y", IBEX_VAR, INTERVAL( -46.759999999999998, 53.240000000000002 ));
  sysf.set_entity("N_x", IBEX_VAR, INTERVAL( -53.517999999999994, 46.482000000000006 ));
  sysf.set_entity("M_y", IBEX_VAR, INTERVAL( -46.589999999999996, 53.410000000000004 ));
  sysf.set_entity("M_x", IBEX_VAR, INTERVAL( -53.335999999999999, 46.664000000000001 ));
  sysf.set_entity("L_y", IBEX_VAR, INTERVAL( -46.602999999999994, 53.397000000000006 ));
  sysf.set_entity("L_x", IBEX_VAR, INTERVAL( -53.085999999999999, 46.914000000000001 ));
  sysf.set_entity("K_y", IBEX_VAR, INTERVAL( -46.832999999999998, 53.167000000000002 ));
  sysf.set_entity("K_x", IBEX_VAR, INTERVAL( -53.278999999999996, 46.721000000000004 ));
  sysf.set_entity("J_y", IBEX_VAR, INTERVAL( -47.003999999999998, 52.996000000000002 ));
  sysf.set_entity("J_x", IBEX_VAR, INTERVAL( -53.461999999999996, 46.538000000000004 ));
  sysf.set_entity("I_y", IBEX_VAR, INTERVAL( -49.599999999999994, 50.400000000000006 ));
  sysf.set_entity("I_x", IBEX_VAR, INTERVAL( -51.958999999999996, 48.041000000000004 ));
  sysf.set_entity("H_y", IBEX_VAR, INTERVAL( -47.596999999999994, 52.403000000000006 ));
  sysf.set_entity("H_x", IBEX_VAR, INTERVAL( -48.497999999999998, 51.502000000000002 ));
  sysf.set_entity("G_y", IBEX_VAR, INTERVAL( -46.552999999999997, 53.447000000000003 ));
  sysf.set_entity("G_x", IBEX_VAR, INTERVAL( -52.840999999999994, 47.159000000000006 ));
  sysf.set_entity("F_y", IBEX_VAR, INTERVAL( -46.789999999999999, 53.210000000000001 ));
  sysf.set_entity("F_x", IBEX_VAR, INTERVAL( -52.919999999999995, 47.080000000000005 ));
  sysf.set_entity("E_y", IBEX_VAR, INTERVAL( -46.976999999999997, 53.023000000000003 ));
  sysf.set_entity("E_x", IBEX_VAR, INTERVAL( -52.753999999999998, 47.246000000000002 ));
  sysf.set_entity("D_y", IBEX_VAR, INTERVAL( -46.739999999999995, 53.260000000000005 ));
  sysf.set_entity("D_x", IBEX_VAR, INTERVAL( -52.674999999999997, 47.325000000000003 ));
  sysf.set_entity("C_y", IBEX_VAR, INTERVAL( -46.503, 53.497 ));
  sysf.set_entity("C_x", IBEX_VAR, INTERVAL( -52.595999999999997, 47.404000000000003 ));
  sysf.set_entity("B_y", IBEX_VAR, INTERVAL( 0, 55 ));
  sysf.set_entity("B_x", IBEX_VAR, INTERVAL( -50, 50 ));
  sysf.set_entity("A_y", IBEX_VAR, INTERVAL( -50, 50 ));
  sysf.set_entity("A_x", IBEX_VAR, INTERVAL( -50, 50 ));


  return System(sysf);
}
