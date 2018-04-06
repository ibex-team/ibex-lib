//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor specialized for circle detection 
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 4, 2015
// Last Update : Feb 4, 2015
//============================================================================

#ifndef __IBEX_CTC_Q_INTERCIRCLE_H__
#define __IBEX_CTC_Q_INTERCIRCLE_H__

#include "ibex_CtcQInterAff.h"


namespace ibex {

class CtcQInterCircle : virtual public CtcQInter {
public:
	/**
	 * \brief q-intersection on a list of contractors.
	 *
	 * The list itself is not kept by reference.
	 */

  CtcQInterCircle(int n, const Array<Ctc>& ctc_list,Interval*** measure, double epseq, int q, qintermethod QINTERPROJ);
	
	void point_contract(IntervalVector& box,int iter); 
	Interval *** measure;
	double epseq;
	double compute_err_iter(Vector & mid, int iter);
	int activepoints_count(IntervalVector& box);        
	  int activepoints_contract_count(IntervalVector& box);        

 protected :
        void fwdbwd(IntervalVector & box, int iter);
	Interval eval_ctc(IntervalVector & box, int iter, int k);
	Interval eval_dist(IntervalVector & box, int iter);

};

class CtcQInterAffCircle : virtual public CtcQInter, public CtcQInterCircle, public CtcQInterAff {

   public :
 CtcQInterAffCircle(int n, const Array<Ctc>& ctc_list,Interval*** measure, double epseq, int q,  qintermethod QINTERPROJ);

   protected : 
   double slope_compute(int iter, int j, int k, const IntervalVector& box, Affine2& af);
   double err_compute( int iter, int k, const IntervalVector& box,Affine2& af);
   double valmean_compute(int iter, int k, const IntervalVector& box, Affine2& af);
   void compute_affine_evaluation (int i , int iter, Affine2& af, Interval & af2);
   int affine_threshold();
     };



} // end namespace ibex
#endif // __IBEX_CTC_Q_INTERCIRCLE_H__
