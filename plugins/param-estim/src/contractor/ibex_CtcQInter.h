//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 30, 2012
// Last Update : Apr 30, 2012
//============================================================================

#ifndef __IBEX_CTC_Q_INTER_H__
#define __IBEX_CTC_Q_INTER_H__

#include "ibex_Ctc.h"


#include "ibex_IntervalMatrix.h"
#include "ibex_QInter.h"
#include <vector>
#include <list>


using namespace std;

namespace ibex {

/**
 * \ingroup contractor
 * \brief Q-intersection contractor.
 *
 */
class CtcQInter : public Ctc {
public:
  /**
   * \brief q-intersection on a list of contractors.
   *
   * The list itself is not kept by reference.
   */
  
  CtcQInter(int n, const Array<Ctc>& ctc_list,  int q, qintermethod meth=QINTERPROJ, int kfun=1);
  CtcQInter(int n, int nb_obs, const Array<Ctc>& ctc_list, int q, qintermethod meth=QINTERPROJ, int kfun=1);
  ~CtcQInter();
	
  /**
   * \brief Contract the box.
   */


  virtual void contract(IntervalVector& box);
  virtual void point_fwdbwd(IntervalVector& box, int iter);
  
  
  virtual int midactivepoints_count(const Vector& vec);
  virtual int ctc_contract(IntervalVector& box);
  virtual void ctc_contract_all(IntervalVector& box);
  void ctc_contract_observ(IntervalVector& box, list<int> & observ);
  //int fwdbwdcount(IntervalVector& box);

  //  int midbox_activepoints_number(IntervalVector& box);
  //  int midbox_activepoints_contract_count(IntervalVector& box);
  //	int activepoints_number(IntervalVector& box);
  virtual int activepoints_count(const IntervalVector& box);
  int activepoints_count2(IntervalVector& box);
  virtual int activepoints_contract_count(IntervalVector& box);
  double compute_err_sol(Vector & mod);
  virtual double compute_err_iter(Vector & mid, int iter);
  
  virtual bool contract_with_outliers (IntervalVector& box, int gap);
  virtual bool constraint_check (IntervalVector& box, double epscont) {return true;};
  virtual  bool stopping_condition (int q1, int q2);
	/**
	 * List of contractors
	 */
	Array<Ctc> ctc_list;
	int nb_obs;
	
	/**
	 * The number of contractors we have to intersect the
	 * result.
	 */
	virtual int effective_size(const IntervalVector &box);

	
	int q;
	int qmax;
	int varbiss;
        virtual double get_epseq() ;
	list<int>* points;  // the list of current compatible measurements : initialized to all measures and managed by solver when the QInter constraint is used in a parameter estimation solver.

        bool points_to_delete;  //to manage shared pointer  points

	virtual IntervalVector randomvalidpoint(const IntervalVector& box);
	
	virtual IntervalVector validpoint (IntervalVector & box);
	virtual vector<int> initial_observations (int i);
	virtual int original_obs(int k);
	virtual vector<int> feasible_points(const Vector& vec);
	virtual double lincoeff(int i, int k);

protected:

	qintermethod meth; // The Qinter method 
	int dim;
	int kfun;
        double box_maxdiam;
	virtual void init();
	IntervalMatrix* boxes; // store boxes for each contraction
	void direct_qintercontract (int &p , IntervalVector & box);
	virtual IntervalVector boxintersection(const IntervalVector& box, int* iter);
	virtual IntervalVector pointintersection(const IntervalVector& box,int* iter);
	virtual  IntervalVector qinter_contract(int & p,int n0, int n1) ;
        virtual double max_diam_threshold(const IntervalVector &box);
	void updatepoints();
	void updateinterpoints(IntervalVector& box);
	virtual void point_contract(IntervalVector& box,int iter); 
	virtual void point_contract_exact(IntervalVector& box,int iter); 
};

 

} // end namespace ibex
#endif // __IBEX_CTC_Q_INTER_H__
