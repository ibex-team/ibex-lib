//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcACID.h
// Author      : Bertrand Neveu , Gilles Trombettoni
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Jul 20, 2018
//============================================================================

#ifndef __IBEX_CTC_ACID_H__
#define __IBEX_CTC_ACID_H__

#include "ibex_Ctc3BCid.h"
#include "ibex_System.h"

namespace ibex {

/**
 * \ingroup contractor
 * \brief ACID contractor (Adpative version of 3BCID)
 *
 * This class is an implementation of the ACID algorithm, an adaptive version of the 3BCID  contractor
 * defined by Gilles Trombettoni and Gilles Chabert in Proc. of CP, Constraint Programming, 2007.
 * The "handled" number of variables for which a shaving will be performed is adaptively tuned.
 * <ul> The ACID algorithm alternates:
 * <li> small tuning phases (during e.g 50 nodes)  where the shaving is called on a number of variables
 *      double of the last tuned value (all variables in the first tuning phase);  Statistics are made in
 *      order to determine an average number of interesting calls,  The number of variables to be handled
 *      in the next running phase is computed at the end of the tuning phase.
 *
 * <li> large running phases (during e.g. 950 nodes)
 *      where 3BCID is called with the  number of variables determined during the last tuning phase.
 */

class CtcAcid : public Ctc3BCid {

public :
	/**
	 * \brief ACID constructor
	 *
	 * The parameters are the same as for 3BCID: cid_vars, ctc, s3b, scid, var_min_width  except vhandled
	 * which is now computed and no more a parameter
	 * \param  ct_ratio (for contracting ratio), a new parameter, at the kernel of the ACID algorithm:
	 *         it indicates that variables are useful to be shaved until all variable shavings yield
	 *         average gains smaller than ct_ratio. the default value is 0.005,
	 * \param optim : boolean parameter : if true, the last variable (the objective in optimization) is put at the first place 
	 * in the vector of variables to be shaved : useful for optimization only
	 *
	 * \param  sys The constraint system useful to compute the order of the variables to be shaved.using a smearsumrel 
	 * like (cf bisector) criterion : in case of optimization, it should be the extended constraint system (see ibex_Optimizer)
	 *
	 */
    CtcAcid(const System& sys, const BitSet& cid_vars, Ctc& ctc, bool optim=false, int s3b=default_s3b, int scid=default_scid,
	    double var_min_width=default_var_min_width, double ct_ratio=default_ctratio);
    /**
    * \brief ACID constructor
	 *
         *  ACID constructor on all variables : no need to give the cid_vars parameter 
	 */

    CtcAcid(const System& sys, Ctc& ctc, bool optim=false, int s3b=default_s3b, int scid=default_scid,
	    double var_min_width=default_var_min_width, double ct_ratio=default_ctratio);

	/**
	 * \brief the contraction function
	 *
	 * Adaptive CID : the number nbcidbar of variables on which 3BCID is applied is adaptively determined
	 * At the beginning, during the first nbinitcalls nodes,there isthe first tuning phase where nbcidvar = nbvar
	 * This tuning phase is repeated every "factor * nbinitcalls" node during "nbinitcalls" nodes
	 * During these tuning phases, we call 3BCID with  nbcidvar =  min (max (2, 2 * nbcidvar)  , 5 * nbvar)
	 * Between two tuning phases,  we use nbcidvar computed in the preceding tuning phase and
	 * 3BCID is applied on the first nbcidvar variables, the variables being 
	 * sorted by the smearsumrel criterion
	 * In case of optimization (optim=true), the last variable corresponding to the objective is placed first in the variables to be shaved.
	 *
	 *  For computing nbcidvar during a tuning phase, one determines after how many variables, 
	 *  the average gain (on all the dimensions of the current box) is less  than ct_ratio:
	 *  this average number of variables (during the tuning phase) will become nbcidvar.
	 */
	virtual void contract(IntervalVector& box, ContractContext& context);

	virtual void contract(IntervalVector& box);

	double nbvar_stat();

	/** the handled constraint system */
	const System& system;

	/** the average (on all tunings) of the  number of variables to be shaved  : result given at the end of the search*/
	static double nbvarstat;

	/** default ctratio value, set to 0.002. */
	static constexpr double default_ctratio = 0.002;

protected :
	/**
	 * Order for sorting the variables to be shaved : smear variant by Ignacio Araya
	 *  (cf SmearSumRelative dans bisector/ibex_SmearFunction.cpp)
	 */
	void compute_smearorder(IntervalVector& box);
	/** in case of optimization (optim = true) , the variable corresponding to the objective is put as first variable */
	void putobjfirst();
	std::vector<int> smearorder;
	int nbcalls;
	double nbctvar;
	double ctratio;
	int nbcidvar;
	int nbtuning;
	bool optim;
};

} // end namespace ibex

#endif // __IBEX_CTC_ACID_H__
