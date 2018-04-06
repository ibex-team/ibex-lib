//============================================================================

//                                  I B E X                                   
// File        : Q-intersection contractor specialized for linear observations
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 4, 2015
// Last Update : Feb 4, 2015
//============================================================================

#ifndef __IBEX_CTC_Q_INTERLINEAR_H__
#define __IBEX_CTC_Q_INTERLINEAR_H__

#include "ibex_CtcQInterAff.h"
#include "ibex_System.h"
#include "ibex_CtcPolytopeHull.h"
//#include "ibex_LinearRelaxXTaylor.h"
namespace ibex {

class CtcQInterLinear : virtual public CtcQInter {
public:

  CtcQInterLinear(int n, const Array<Ctc>& ctc_list,double** linfun, 
		  double epseq, int q,  qintermethod meth=QINTERPROJ);
  double compute_err_iter(IntervalVector & box, int iter);
  void point_contract(IntervalVector& box,int iter); 
  void point_contract_exact(IntervalVector& box,int iter); 
  double ** linfun;
  double epseq;
  double get_epseq();
  Interval eps;
  bool constraint_check (IntervalVector& box, double epscont);
  //  int points_count(IntervalVector& box);        
  //  int activepoints_count(IntervalVector& box);        
  //  int activepoints_contract_count(IntervalVector& box);        

  

 protected :
  virtual void fwdbwd(IntervalVector & box, int iter);
  virtual void fwd(IntervalVector & box, int iter);
  Interval sumlin(IntervalVector & box, int iter, int j);
  //        void fwd(IntervalVector & box, int iter);
  virtual   Interval eval_ctc(IntervalVector & box, int iter);
  virtual   Interval eval_dist(IntervalVector & box, int iter);
  void point_fwdbwd(IntervalVector & box, int iter);
  int effective_size(const IntervalVector &box) ;
  double max_diam_threshold(const IntervalVector& box);
};

/* class implementing a CtcQInterLinear contractor using the projection in the supplementary direction
multiple inheritance :  CtcQInterLinear and CtcQInterAff
 */

 class CtcQInterAffLinear :
   virtual public CtcQInter, 
   public CtcQInterLinear, public CtcQInterAff {
   public :

   CtcQInterAffLinear(int n, const Array<Ctc>& ctc_list,double** linfun, 
		     double epseq, int q,  qintermethod meth=QINTERPROJ);

   void point_contract(IntervalVector& box,IntervalMatrix & msol, IntervalMatrix & msolt, int iter); 
   int midactivepoints_count(const Vector& vec);
   vector<int> feasible_points(const Vector& vec);
   double lincoeff(int i, int k);
   int activepoints_contract_count(IntervalVector& box);        
   void ctc_contract_all(IntervalVector& box);

 protected :

   int ctc_contract (IntervalVector & box);
   double valmean_compute(int iter, int i, const IntervalVector& box, Affine2& af);
   double slope_compute(int iter, int j, int i , const IntervalVector& box,Affine2& af);
   double err_compute( int iter, int k, const IntervalVector& box,Affine2& af);
   void compute_affine_evaluation( int i, int iter,  Affine2& af , Interval& af2);
   Interval eval_ctc(IntervalVector & box, int iter);
   Interval eval_dist(IntervalVector & box, int iter);
   void fwdbwd(IntervalVector & box, int iter);
   void fwd(IntervalVector & box, int iter);
   Interval sumlin(IntervalVector & box, int iter, int j);
   Interval sumlin0(IntervalVector & box, int iter, int j);
   void bwd1(IntervalVector & box, int iter);
   int effective_size(const IntervalVector &box) ;
   int affine_threshold();

   void point_epipole_check(IntervalVector & box, const IntervalMatrix & msol, const IntervalMatrix & msolt, int iter);
   double epipole_epsilon;
   double norm_epipole_epsilon;
   
};


  


} // end namespace ibex
#endif // __IBEX_CTC_Q_INTERLINEAR_H__
