//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor specialized by adding a qintersection in the direction of the gradient of the function of the measurement
//  
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 30, 2012
// Last Update : Apr 30, 2012
//============================================================================



#ifndef __IBEX_CTC_Q_INTERAFF_H__
#define __IBEX_CTC_Q_INTERAFF_H__

#include "ibex_CtcQInter.h"
#include "ibex_Affine2.h"
#include "ibex_AffineEval.h"
#include "ibex_Function.h"

#include <vector>


using namespace std;

namespace ibex {
class CtcQInterAff : virtual public CtcQInter {

 public:

  CtcQInterAff(int n, const Array<Ctc>& ctc_list, int q, qintermethod meth=QINTERPROJ);

 CtcQInterAff(int n, const Array<Ctc>& ctc_list, int q, Function*** funlist,qintermethod meth=QINTERPROJ);
 CtcQInterAff(int n, int nb_obs1, const Array<Ctc>& ctc_list, int q,  qintermethod meth=QINTERPROJ);
   ~CtcQInterAff();


	/**
	 * List of functions 
	 */
	Function*** function_list;
	void contract(IntervalVector& box);

 protected: 
	int pmax;
	bool funl;
	Affine2Eval** eval_af;
	double *** alpha;
	double ** valmean;
	double ** err;
        void init_interaff(bool fun);
	Interval* interaf;
	virtual void compute_affine_evaluation( int i, int iter,  Affine2& af, Interval& af2);
	virtual double slope_compute(int iter, int j, int k , const IntervalVector& box, Affine2& af);
	vector<double> aff_dir; 
	virtual double err_compute( int iter, int k,  const IntervalVector& box,Affine2& af);
	virtual double	valmean_compute(int iter, int i, const IntervalVector& box, Affine2& af);
	void affine_projection (int& p);
	virtual int affine_threshold();

 };


}
#endif // __IBEX_CTC_Q_INTERAFF_H__
