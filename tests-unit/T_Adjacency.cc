
/* CHABERT April 5th 2007 */

#include "T_Adjacency.h"
#include "IbexContractorList.h"
#include "IbexHC4Revise.h"

using namespace ibex;

void T_Adjacency() {

  System sys("sync1.txt");
  ContractorList l(ToHC4Revise(), sys);
  Adjacency adj(l,sys.space);
  
  {
    cout << "-------------- check adjacency --------------\n";
    _class="Adjacency";

    _meth="nb_ctr";
    _nber = 0;
    _test(adj.nb_ctr()==4);

    _meth="nb_var";
    _nber = 0;
    _test(adj.nb_var()==4);

    _meth="nb_upr";
    _nber = 0;
    _test(adj.nb_upr()==4);

    _meth="nb_epr";
    _nber = 0;
    _test(adj.nb_epr()==4);

    _meth="nb_syb";
    _nber = 0;
    _test(adj.nb_syb()==4);

    _meth="ctr_nb_vars";
    _nber = 0;
   _test(adj.ctr_nb_vars(0)==2);
   _test(adj.ctr_nb_vars(1)==1);
   _test(adj.ctr_nb_vars(2)==0);
   _test(adj.ctr_nb_vars(3)==1);

//     _meth="ctr_nb_occ_var";
//     _nber = 0;
//     _test(adj.ctr_nb_occ_var(0,0)==1);
//     _test(adj.ctr_nb_occ_var(0,1)==1);
//     _test(adj.ctr_nb_occ_var(0,2)==0);
//     _test(adj.ctr_nb_occ_var(1,0)==0);
//     _test(adj.ctr_nb_occ_var(1,1)==1);
//     _test(adj.ctr_nb_occ_var(2,0)==0);
//     _test(adj.ctr_nb_occ_var(3,0)==0);
// //     _test(adj.ctr_nb_occ_var(3,3)==2 );

    _meth="ctr_ith_var";
    _nber = 0;
    _test(adj.ctr_ith_var(0,0)==0);
    _test(adj.ctr_ith_var(1,0)==1);
    _test(adj.ctr_ith_var(3,0)==3);

    _meth="var_nb_ctrs";
    _nber = 0;
    _test(adj.var_nb_ctrs(0)==1);
    _test(adj.var_nb_ctrs(1)==2);
    _test(adj.var_nb_ctrs(2)==0);
    _test(adj.var_nb_ctrs(3)==1);

    _meth="var_ith_ctr";
    _nber = 0;
    _test(adj.var_ith_ctr(0,0)==0);
    _test(adj.var_ith_ctr(1,0)==0);
    _test(adj.var_ith_ctr(1,1)==1);
    _test(adj.var_ith_ctr(3,0)==3);

    _meth="ctr_nb_uprs";
    _nber = 0;
   _test(adj.ctr_nb_uprs(0)==1);
   _test(adj.ctr_nb_uprs(1)==0);
   _test(adj.ctr_nb_uprs(2)==2);
   _test(adj.ctr_nb_uprs(3)==3);

    _meth="ctr_nb_occ_upr";
    _nber = 0;
//     _test(adj.ctr_nb_occ_upr(0,0)==1);
//     _test(adj.ctr_nb_occ_upr(0,1)==0);
//     _test(adj.ctr_nb_occ_upr(0,2)==0);
//     _test(adj.ctr_nb_occ_upr(1,0)==0);
// //     _test(adj.ctr_nb_occ_upr(2,0)==2);
// //     _test(adj.ctr_nb_occ_upr(2,0)==2);
//     _test(adj.ctr_nb_occ_upr(3,0)==1);
//     _test(adj.ctr_nb_occ_upr(3,1)==1);
//     _test(adj.ctr_nb_occ_upr(3,2)==0);
// //     _test(adj.ctr_nb_occ_upr(3,3)==3);

    _meth="ctr_ith_upr";
    _nber = 0;
    _test(adj.ctr_ith_upr(0,0)==0);
    _test(adj.ctr_ith_upr(2,0)==0);
    _test(adj.ctr_ith_upr(2,1)==2);
    _test(adj.ctr_ith_upr(3,0)==0);
    _test(adj.ctr_ith_upr(3,1)==1);
    _test(adj.ctr_ith_upr(3,2)==3);

    _meth="upr_nb_ctrs";
    _nber = 0;
    _test(adj.upr_nb_ctrs(0)==3);
    _test(adj.upr_nb_ctrs(1)==1);
    _test(adj.upr_nb_ctrs(2)==1);
    _test(adj.upr_nb_ctrs(3)==1);

    _meth="upr_ith_ctr";
    _nber = 0;
    _test(adj.upr_ith_ctr(0,0)==0);
    _test(adj.upr_ith_ctr(0,1)==2);
    _test(adj.upr_ith_ctr(0,2)==3);
    _test(adj.upr_ith_ctr(1,0)==3);
    _test(adj.upr_ith_ctr(2,0)==2);
    _test(adj.upr_ith_ctr(3,0)==3);

    _meth="ctr_nb_eprs";
    _nber = 0;
   _test(adj.ctr_nb_eprs(0)==1);
   _test(adj.ctr_nb_eprs(1)==1);
   _test(adj.ctr_nb_eprs(2)==1);
   _test(adj.ctr_nb_eprs(3)==0);

//     _meth="ctr_nb_occ_epr";
//     _nber = 0;
//     _test(adj.ctr_nb_occ_epr(0,0)==1);
//     _test(adj.ctr_nb_occ_epr(0,1)==0);
//     _test(adj.ctr_nb_occ_epr(0,2)==0);
//     _test(adj.ctr_nb_occ_epr(1,0)==0);
// //     _test(adj.ctr_nb_occ_epr(1,1)==3);
//     _test(adj.ctr_nb_occ_epr(2,0)==0);
//     _test(adj.ctr_nb_occ_epr(2,2)==1);
//     _test(adj.ctr_nb_occ_epr(3,0)==0);

    _meth="ctr_ith_epr";
    _nber = 0;
    _test(adj.ctr_ith_epr(0,0)==0);
    _test(adj.ctr_ith_epr(1,0)==1);
    _test(adj.ctr_ith_epr(2,0)==2);

    _meth="epr_nb_ctrs";
    _nber = 0;
    _test(adj.epr_nb_ctrs(0)==1);
    _test(adj.epr_nb_ctrs(1)==1);
    _test(adj.epr_nb_ctrs(2)==1);
    _test(adj.epr_nb_ctrs(3)==0);

    _meth="epr_ith_ctr";
    _nber = 0;
    _test(adj.epr_ith_ctr(0,0)==0);
    _test(adj.epr_ith_ctr(1,0)==1);
    _test(adj.epr_ith_ctr(2,0)==2);

  }

}
