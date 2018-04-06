//============================================================================

//                                  I B E X                                   
// File        : Q-intersection contractor specialized for plane detection 
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 4, 2015
// Last Update : Feb 4, 2015
//============================================================================

#ifndef __IBEX_CTC_Q_INTERLINEARV8_H__
#define __IBEX_CTC_Q_INTERLINEARV8_H__

#include "ibex_CtcQInterAff.h"
#include "ibex_System.h"
#include "ibex_CtcPolytopeHull.h"
//#include "ibex_LinearRelaxXTaylor.h"
namespace ibex {

class CtcQInterLinearv8 : virtual public CtcQInter {
public:
	/**
	 * \brief q-intersection on a list of contractors.
	 *
	 * The list itself is not kept by reference.
	 */

  CtcQInterLinearv8(int n, const Array<Ctc>& ctc_list,double*** linfun, 
		   double epseq, int q,  qintermethod meth=QINTERPROJ);
  double compute_err_iter(IntervalVector & box, int iter);
  void point_contract(IntervalVector& box,int iter); 
  //  void point_contract_exact(IntervalVector& box,int iter); 
  double *** linfun;
  double epseq;
  Interval eps;
  //  bool constraint_check (IntervalVector& box, double epscont);
  //  int points_count(IntervalVector& box);        
  //  int activepoints_count(IntervalVector& box);        
  //  int activepoints_contract_count(IntervalVector& box);        

  

 protected :
  virtual void fwdbwd(IntervalVector & box, int iter, int k);
  //aff  virtual void fwd(IntervalVector & box, int iter, int k);
  Interval sumlin(IntervalVector & box, int iter, int j, int k);
  //        void fwd(IntervalVector & box, int iter);
  virtual   Interval eval_ctc(IntervalVector & box, int iter, int k);
  virtual   Interval eval_dist(IntervalVector & box, int iter, int k);
  void point_fwdbwd(IntervalVector & box, int iter);
  //  int effective_size(const IntervalVector &box) ;
};


 class CtcQInterAffLinearv8 :
   virtual public CtcQInter, 
   public CtcQInterLinearv8, public CtcQInterAff {

   public :

   CtcQInterAffLinearv8(int n, const Array<Ctc>& ctc_list,double*** linfun, 
		     double epseq, int q,  qintermethod meth=QINTERPROJ);

    void point_contract(IntervalVector& box,int iter); 

 
 protected :

   double valmean_compute(int iter, int i, const IntervalVector& box, Affine2& af);
   double slope_compute(int iter, int j, int i , const IntervalVector& box,Affine2& af);
   double err_compute( int iter, int k, const IntervalVector& box,Affine2& af);
   void compute_affine_evaluation( int i, int iter,  Affine2& af , Interval& af2);
   Interval eval_ctc(IntervalVector & box, int iter, int k);
   Interval eval_dist(IntervalVector & box, int iter, int k);
   void fwdbwd(IntervalVector & box, int iter, int k);
   //   void fwd(IntervalVector & box, int iter, int k);
   Interval sumlin(IntervalVector & box, int iter, int j, int k);
   Interval sumlin0(IntervalVector & box, int iter, int j, int k);
   void bwd1(IntervalVector & box, int iter);
   //   int effective_size(const IntervalVector &box) ;
   int affine_threshold();
};


  


} // end namespace ibex
#endif // __IBEX_CTC_Q_INTERLINEARV8_H__
