//============================================================================
//                                  I B E X                                   
// File        : ibex_Ctc3BCID.h
// Author      : Ignacio Araya, Gilles Chabert,
//               Bertrand Neveu, Gilles Trombettoni
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Jul 20, 2018
//============================================================================

#ifndef __IBEX_CTC_3B_CID_H__
#define __IBEX_CTC_3B_CID_H__

#include "ibex_Ctc.h"
#include "ibex_BitSet.h"

namespace ibex {


/**
 * \ingroup contractor
 * \brief 3B+CID contractor (extension of 3B with CID)
 *
 * This class is an implementation of the 3BCID algorithm defined by Gilles
 * Trombettoni and Gilles Chabert in Proc. of CP, Constraint Programming, 2007.
 */
class Ctc3BCid : public Ctc {
public:

	/**
	 * \brief Creates the 3BCID contractor for the contractor \a ctc.
	 *
	 * \param ctc           (Sub)contractor to be applied on each slice/sub-interval. Remark: we have
	 *                      observed that using \a ctc=Box is generally a bad idea (except in the
	 *                      benchmark Yamamura1), contrary to \a HC4 or \a Mohc.
	 * \param s3b           Maximum number of slices that can be eliminated by the shaving process.
	 *                      \a s3b is simply the inverse of the more standard main parameter of 3B.
	 *                      Example: for a value of s3b equal to 20, the "linear" shaving process
	 *                      splits the studied interval [x] in at most 20 sub-intervals, each having
	 *                      a width equal to 5% of \a x.diam(); the "dichotomic" shaving process
	 *                      handles slices of width at least 5% of x.diam(). Principle of the
	 *                      "linear" shaving (the "dichotomic" shaving is more complicated) applied
	 *                      to the left side and then to the right side of [x]. For the left (resp.
	 *                      right) side, from left to right (resp. from right to left), every sub-
	 *                      interval [x_i] in the left (resp. right) bound of [x] is computed; \a the
	 *                      subcontractor \a ctc is called on the corresponding sub-box [B_i] (i.e.,
	 *                      the current box [B] in which [x] is replaced by [x_i], the other intervals
	 *                      being left unchanged); if [B_i] is empty, then it ensures that [x_i] can
	 *                      be safely removed from [x] (no solution in this slice) and the process
	 *                      continues on the next rightmost (resp. leftmost) slice; if [B_i] is not
	 *                      empty, with [x_i] eventually reduced to [x_i]', the left (resp. right)
	 *                      shaving terminates and the corresponding sub-box [B_left] (resp. [B_right])
	 *                      is stored to be handled by the CID process (see below). We have observed
	 *                      that the \a s3b parameter has often the most significant impact on performances
	 *                      and thereby should be tuned in priority by the user. The best value of \a s3b
	 *                      generally ranges between 5 and 200, 10 being the default value.
	 * \param scid          Maximum number of slices that can be contracted by the CID (constructive
	 *                      disjunction) contractor. Principle: when the standard shaving process in a
	 *                      given variable x is finished, the remaining interval is split into \a scid
	 *                      slices (of equal width); the sub-contractor \a ctc is then applied to these
	 *                      slices and the \a scid returned contracted boxes, plus [B_left], plus
	 *                      [B_right], are hulled, i.e., the smallest box enclosing these \a scid+2
	 *                      sub-boxes is returned. Thus, when handling a given variable x with var3BCID,
	 *                      the constructive interval disjunction allows contracting the box in several
	 *                      dimensions. Fixing \a scid to 0 amounts in calling a "standard" shaving
	 *                      (contracting only [x]); the value 1 is the default value and a greater value
	 *                      rarely produces better performances. Therefore the user should generally not
	 *                      play with this parameter!
	 * \param vhandled      Number of calls to \a var3BCID, i.e., the number of variables that are handled
	 *                      in one call to the contract method (all variables by default). Remark:
	 *                      \a vhandled is used to reach a "fixpoint" in terms of contraction. \a vhandled
	 *                      is preferred to the precision parameter used in the standard 3B that manages
	 *                      the number of loops over all the variables.
	 * \param var_min_width The minimum width an interval must have to be shaved. The default value is 1e-11
	 *                      but the actual default value should depend on the precision of the solutions
	 *                      (#ibex::CtcPrecision::ceil): \a var_min_width should be by default one or
	 *                      several orders of magnitude under #ibex::CtcPrecision::ceil. \a var_min_width
	 *                      is used to adjust the actual number \a s3b of slices considered by the shaving
	 *                      so as to ensure that a slice has a width greater than \a var_min_width, as follows:
	 *                      actual_s3b = Min(s3b, floor(x.diam()/var_min_width)). Remark: the user should
	 *                      generally <b>not</b> play with this parameter in priority because assigning
	 *                      \a var_min_width to a value greater than #ibex::CtcPrecision::ceil is interesting
	 *                      in only a few structured systems, e.g., in Yamamura1 that can found in the
	 *                      COCONUT/COPRIN benchmarks.
	 * \param cid_vars       The mask indicating the subset of variables to be shaved.
	 *                       Example: this parameter has been used by Araya et al. in conjunction with the
	 *                       preprocessing algorithm I-CSE (see Proc. of CP 2008): I-CSE produces a system
	 *                       in which are added new auxiliary variables corresponding to common sub-
	 *                       expressions in the initial system; during the search of solutions that follows,
	 *                       the new auxiliary variables are not shaved and are thus removed from \a cid_space.
	 */
	Ctc3BCid(const BitSet& cid_vars, Ctc& ctc, int s3b=default_s3b, int scid=default_scid,
			int vhandled=-1, double var_min_width=default_var_min_width);

	/**     3BCID constructor
	 *
	 *  ACID constructor on all variables : no need to give the cid_vars parameter ; all variables are the variables of the ctc contractor
	 */
	Ctc3BCid(Ctc& ctc, int s3b=default_s3b, int scid=default_scid,
			int vhandled=-1, double var_min_width=default_var_min_width);

	/**
	 * \brief Delete this.
	 */
	~Ctc3BCid();

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

	/**
	 * \brief Add sub-contractor properties to the map
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& map);

	/**
	 * \brief Apply contraction.
	 *
	 * Applies 3BCID contraction on #vhandled variables of #cid_vars, starting
	 * by #start_var.
	 * #cid_vars contains the variables to be shaved,
	 * If the \link ibex::Contractor::Indicators::scope scope \endlink field is a
	 * specific variable, then #var3BCID is called with the specified variable.
	 * Otherwise, 3BCID will be performed.
	 *
	 */
	virtual void contract(IntervalVector& box, ContractContext& context);

	/** The variables to which var3BCID is applied **/
	BitSet cid_vars;

	/** The sub-contractor */
	Ctc& ctc;

	/** Default s3b value, set to 10 **/
	static constexpr int default_s3b = 10;

	/** Default scid value, set to 1 **/
	static constexpr int default_scid = 1;

	/** Default var_min_width value, set to 1e-11 **/
	static constexpr double default_var_min_width = 1.e-11;

	/** Dichotomy limit , set to 16 : if the number of slices is greater than
      LimitCIDDichotomy, the shaving process is achieved in a dichotomic way,
      otherwise it is achieved in a linear way. **/
	static constexpr int LimitCIDDichotomy = 16;

protected:
	/**
	 * Applies 3BCID contraction on the variable var.
	 * Computes the actual number of slices and the size of a slice,
	 * taking into account the size limit #var_min_width.
	 * Decides the shaving mode : dichotomy or slices
	 */
	bool var3BCID(IntervalVector& box, int var);

	/**
	 * Applies 3BCID contraction on the variable var with a dichotomic 3B algorithm
	 *
	 * Call the right, left (3B) and central (CID) contractions, the right and left
	 * shavings being done in a dichotomic way : returns true if a contraction is done
	 */
	virtual bool var3BCID_dicho(IntervalVector& box, int var, double wv);

	/**
	 * Applies 3BCID contraction on the variable var with a linear 3B algorithm
	 *
	 * Achieves the shaving in a "linear way" (left and right) and calls
	 * the CID central contraction.
	 *
	 * Principle: The "linear" shaving is applied to the left side and
	 * then to the right side of [var]. For the left (resp. right) side,
	 * from left to right (resp. from right to left), every sub-interval
	 * [x_i] (dom in the code below) in the left (resp. right) bound of
	 * [var] is computed (all [x_i] have the same size depending on
	 * locs3b); the sub-contractor \a ctc is called on the corresponding
	 * sub-box [B_i] (i.e., the current box box in which [var] is
	 * replaced by [x_i], the other intervals being left unchanged); if
	 * [B_i] is empty, then it ensures that [x_i] can be safely removed
	 * from [x] (no solution in this slice) and the process continues
	 * (in a linear way) on the next rightmost (resp. leftmost) slice;
	 * if [B_i] is not empty, with [x_i] eventually reduced to [x_i]',
	 * the left (resp. right) shaving terminates and the corresponding
	 * sub-box [B_left] (resp. [B_right]) is stored to be handled by the
	 * CID process. In var3BCID_slices, newbox=([B_left] | [B_right])
	 * is transmitted to the method varCID (var3Bcid_box=newbox).
	 */
	virtual bool var3BCID_slices(IntervalVector& box, int var, int locs3b, double w_DC, Interval& dom);

	/**
	 * Left or right shaving in a dichotomic way.
	 *
	 * 3B dicho applies 3B left or right contraction
	 * returns in box  the left or right non empty slide.
	 *
	 * \return what?? TODO
	 */
	bool shave_bound_dicho(IntervalVector& box, int var, double wv, bool left);

	/**
	 * Contracts with CID \a box slicing the variable \a var.
	 *
	 * central CID contraction : applies a constructive disjunction, i.e., divides the varcid_box into scid slices,
	 * contracts each slice and builds the hull of the contracted slices.
	 * This hull is made in var3Bcid_box that contains at the beginning the hull of the right and left slices that
	 * have been already treated by a shaving algorithm.
	 * During the CID algorithm, one contracts the scid central slices : for each slice, one calls the subcontractor
	 * with var as impact indicator for incrementality.
	 *
	 * returns true if one other variable, different from var, is contracted
	 *
	 * \param var     The current variable.
	 * \param savebox The box to be contracted.
	 * \param newbox  The resulting contracted box
	 * \return        true iff varCID was useful
	 */
	bool varCID(int var, IntervalVector& box, IntervalVector& newbox);

	void update_and_contract(IntervalVector& box, int var);

	/**
	 * Returns true iff \a box1 and \a box2 are equal, excepting the current interval (\a var )
	 */
	bool equalBoxes (int var, IntervalVector& box1, IntervalVector& box2);

	/** The maximum number of slices that the contractor will try to refute **/
	int s3b;

	/** The number of slices that the contractor will try to contract after the refutation phase **/
	int scid;

	/** Number of calls to \a var3BCID in one call to the #contract() method. **/
	int vhandled;

	/** The minimum width a interval must have to be varCIDed **/
	double var_min_width;

	/** First variable to varCID in the next call to the #contract() method **/
	int start_var;

	/**
	 * Current context
	 * Used to pass the impact (+ the shaved variable) to the sub-contractor.
	 * Allow to benefit from the incrementality of the sub-contractor.
	 * Also use to transmit the properties.
	 */
	ContractContext* context;

	virtual int limitCIDDichotomy();
};

} // end namespace ibex
#endif // __IBEX_CTC_3B_CID_H__
