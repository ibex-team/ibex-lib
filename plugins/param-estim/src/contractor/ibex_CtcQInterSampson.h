//============================================================================

//                                  I B E X                                   
// File        : Q-intersection contractor specialized for plane detection 
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 4, 2015
// Last Update : Feb 4, 2015
//============================================================================

#ifndef __IBEX_CTC_Q_INTERSAMPSON_H__
#define __IBEX_CTC_Q_INTERSAMPSON_H__

#include "ibex_CtcQInter.h"
#include "ibex_CtcQInterAff.h"
#include "ibex_System.h"
#include "ibex_CtcPolytopeHull.h"
//#include "ibex_LinearRelaxXTaylor.h"
namespace ibex {

class CtcQInterSampson : virtual public CtcQInter {
public:
	/**
	 * \brief q-intersection on a list of contractors.
	 *
	 * The list itself is not kept by reference.
	 */

  // CtcQInterSampson(int n, const Array<Ctc>& ctc_list,double*** linfun, double** linfun1,
  //		   double epseq, int q,  qintermethod meth=QINTERPROJ);

  CtcQInterSampson(int n, const Array<Ctc>& ctc_list, double*** linfun, double** linfun1, double epseq, int q, qintermethod meth=QINTERPROJ);
  //  CtcQInterSampson(int n, const Array<Ctc>& ctc_list, double*** linfun, double epseq, int q, qintermethod meth=QINTERPROJ);

  double compute_err_iter(IntervalVector & box, int iter);
  void point_contract(IntervalVector& box,int iter); 
  void point_contract_exact(IntervalVector& box,int iter); 
  double *** linfun;
  double ** mfun1;
  double epseq;
  Interval eps;
  Interval eps0;
  int effective_size(const IntervalVector& box);
  int midactivepoints_count(const Vector& vec);
 protected :
  //virtual void fwdbwd(IntervalVector & box, int iter, int k);
  //  virtual void fwd(IntervalVector & box, int iter, int k);
  //  Interval sumlin(IntervalVector & box, int iter, int j, int k);
  //        void fwd(IntervalVector & box, int iter);

  Interval eval_ctc(const IntervalVector & box, int iter, int k);
  Interval eval_dist(const IntervalVector & box, int iter, int k);
  Interval eval_sampson(const IntervalVector & box, int iter, int k);
  bool midpoint_eval(const Vector & vec, int iter);
  bool mideval_ctc(const Vector & vec, int iter, int k);
  double mideval_dist(const Vector & vec, int iter, int k);
  double mideval_sampson(const Vector & vec, int iter, int k);
  //virtual   Interval eval_dist(IntervalVector & box, int iter, int k);
  void point_fwdbwd(IntervalVector & box, int iter);
  double  max_diam_threshold(const IntervalVector& box);
  //  int effective_size(const IntervalVector &box) ;
};


 class CtcQInterAffSampson :
   virtual public CtcQInter, 
   public CtcQInterSampson, public CtcQInterAff {

 public :
   CtcQInterAffSampson(int n, const Array<Ctc>& ctc_list, double*** linfun, double** linfun1, Function *** mfun, double epseq, int q, qintermethod meth=QINTERPROJ);


 protected :
   int effective_size(const IntervalVector &box) ;
   int affine_threshold();
   //   void compute_affine_evaluation ( int k, int iter,  Affine2& af, Interval& af2);
 };

} // end namespace ibex
#endif // __IBEX_CTC_Q_SAMPSON_H__
