//============================================================================
//                                  I B E X                                   
// File        : ibex_ACID.h
// Author      : Bertrand Neveu , Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Jul 1, 2012
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
 *      where 3BCID is called with the  number of variables determined during the last tunig phase.
 */

class CtcAcid : public Ctc3BCid {

public :
	/**
	 * \brief ACID constructor
	 *
	 * The parameters are the same as for 3BCID: ctc, s3b, scid, var_min_width  except vhandled
	 * which is now computed and no more a parameter
	 * \param  ct_ratio (for contracting ratio), a new parameter, at the kernel of the ACID algorithm:
	 *         it indicates that variables are useful to be shaved until all variable shavings yield
	 *         average gains smaller than ct-ratio. the default value is 0.005,
	 * \param  sys The constraint system useful to compute the order of the variables to be shaved.using
	 *         a smearsumrel like (cf bisector) criterion
	 *
	 */
	CtcAcid(const System& sys, const BoolMask& cid_vars, Ctc& ctc, int s3b=default_s3b, int scid=default_scid,
			   double var_min_width=default_var_min_width, double ct_ratio=default_ctratio);

	/**
	 * \brief the contraction function
	 *
	 * TODO : translate comment below in english.
	 *
	 * CID adaptatif :  calcul du nombre de variables nbcidvar sur lesquelles on applique cid
	 * au début   pour initialiser on essaye pendant nbinitcalls    nbcidvar = nbvar
	 * cette 1re phase de réglage permet de calculer nbcidvar pour la phase suivante
	 * ensuite, on refait un réglage qui recalcule le nb de variables tous les factor * nbinitcalls
	 * en faisant des stats pendant  nbinitcalls  nouuds.
	 *  pendant cette periode de réglage, on essaye avec  nbcidvar =  min (max (2, 2 * nbcidvar)  , 5 * nbvar)
	 *
	 *  Pour les stats, on determine apres combien de variables, on gagne moins en moyenne (sur les dimensions)que ctratio par domaine .
	 *  ce nb de variables moyen (sur les noeuds de la phase de réglage) deviendra alors nbcidvar
	 */
	virtual void contract(IntervalVector& box);

	double nbvar_stat();

	/** the handled constraint system */
	System system;

	/** the average (on all tunings) of the  number of variables to be shaved  : result given at the end of the search*/
	static double nbvarstat;

	/** default ctratio value, set to 0.005 */
	static const double default_ctratio;

protected :
	/**
	 * TODO: translate comment in english.
	 *
	 * Calcul de l'ordre dans lequel les variables seront traitées.
	 * variante smear d'Ignacio Araya   (cf smearsumrel dans IbexBisector.cpp)
	 */
	void compute_smearorder(IntervalVector& box);

	vector<int> smearorder;
	int nbcalls;
	double nbctvar;
	double ctratio;
	int nbcidvar;
	int nbtuning;
};

} // end namespace ibex
#endif // __IBEX_CTC_ACID_H__
