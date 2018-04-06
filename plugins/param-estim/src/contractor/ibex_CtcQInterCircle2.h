//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor specialized for circle detection 
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 4, 2015
// Last Update : Feb 4, 2015
//============================================================================

#ifndef __IBEX_CTC_Q_INTERCIRCLE2_H__
#define __IBEX_CTC_Q_INTERCIRCLE2_H__

#include "ibex_CtcQInterAff.h"


namespace ibex {

class CtcQInterCircle2 : virtual public CtcQInter {
public:
	/**
	 * \brief q-intersection for circle estimation
	 *
	 */

  CtcQInterCircle2(int n, const Array<Ctc>& ctc_list,double*** measure, double epseq, int q, qintermethod QINTERPROJ);
  void point_contract(IntervalVector& box,int iter); 
	double *** measure;
	double epseq;
	double compute_err_iter(Vector & mid, int iter);
        int midactivepoints_count(const Vector& vec);
	int activepoints_count(IntervalVector& box);        
	int activepoints_contract_count(IntervalVector& box);        
	IntervalVector pointintersection( int iter1, int iter2, int iter3);

	IntervalVector pointintersection(int* iter);
	IntervalVector boxintersection (IntervalVector& box, int iter1, int iter2, int iter3);
	IntervalVector boxintersection (IntervalVector& box, int* iter);
	bool contract_with_outliers (IntervalVector& box, int gap);
	void ctc_contract_all(IntervalVector& box);

 protected : 
	  void fwd(IntervalVector & box, int iter);
	  void fwdbwd(IntervalVector & box, int iter);
	
	  Interval eval_ctc(IntervalVector & box, int iter , int k);
	Interval eval_dist(IntervalVector & box, int iter);

};


 class CtcQInterAffCircle2 : virtual public CtcQInter, public CtcQInterCircle2, public CtcQInterAff {

   public :
 CtcQInterAffCircle2(int n, const Array<Ctc>& ctc_list,double*** measure, double epseq, int q,  qintermethod QINTERPROJ);

   protected : 
   double slope_compute(int iter, int j, int k, const IntervalVector& box, Affine2& af);
	 double err_compute( int iter, int k, const IntervalVector& box,Affine2& af);
	 double	valmean_compute(int iter, int k, const IntervalVector& box, Affine2& af);
	 void compute_affine_evaluation (int i , int iter, Affine2& af, Interval & af2);
	 int affine_threshold();
	 double max_diam_threshold(const IntervalVector& box);
	 //	 void bwd1 (IntervalVector & box, int iter);
 };

} // end namespace ibex
#endif // __IBEX_CTC_Q_INTERCIRCLE2_H__
