#ifndef _3B_CID
#define _3B_CID

#include "IbexContractor.h"
#include "IbexSystem.h"

namespace ibex {

/** \ingroup ctcgroup
 * \brief 3BCID contractor (extension of 3B with CID)
 *
 * This class is an implementation of the 3BCID algorithm defined by Gilles Trombettoni and Gilles Chabert in Proc. of CP, Constraint Programming, 2007.
 * \author Ignacio Araya, Gilles Chabert, Bertrand Neveu, Gilles Trombettoni
 * \date September 2010
 */
class _3BCID : public Contractor {
 public:
  /** Creates the 3BCID contractor for the contractor \a ctc.
   *
   * \param ctc (Sub)contractor to be applied on each slice/sub-interval. Remark: we have observed that using \a ctc=Box is generally a bad idea (except in the benchmark Yamamura1), contrarily to \a HC4 or \a Mohc.
   * \param s3b Maximum number of slices that can be eliminated by the shaving process. \a s3b is simply the inverse of the more standard main parameter of 3B. Example: for a value of s3b equal to 20, the "linear" shaving process splits the studied interval [x] in at most 20 sub-intervals, each having a width equal to 5% of \a Diam(x); the "dichotomic" shaving process handles slices of width at least 5% of \a Diam(x). Principle of the "linear" shaving (the "dichotomic" shaving is more complicated) applied to the left side and then to the right side of [x]. For the left (resp. right) side, from left to right (resp. from right to left), every sub-interval [x_i] in the left (resp. right) bound of [x] is computed; \a the subcontractor \a ctc is called on the corresponding sub-box [B_i] (i.e., the current box [B] in which [x] is replaced by [x_i], the other intervals being left unchanged); if [B_i] is empty, then it ensures that [x_i] can be safely removed from [x] (no solution in this slice) and the process continues on the next rightmost (resp. leftmost) slice; if [B_i] is not empty, with [x_i] eventually reduced to [x_i]', the left (resp. right) shaving terminates and the corresponding sub-box [B_left] (resp. [B_right]) is stored to be handled by the CID process (see below). We have observed that the \a s3b parameter has often the most significant impact on performances and thereby should be tuned in priority by the user. The best value of \a s3b generally ranges between 5 and 200, 10 being the default value. 
   * \param scid Maximum number of slices that can be contracted by the CID (constructive disjunction) contractor. Principle: when the standard shaving process in a given variable x is finished, the remaining interval is split into \a scid slices (of equal width); \a  the subcontractor ctc is then applied to these slices and the \a scid returned contracted boxes, plus [B_left], plus [B_right], are hulled, i.e., the smallest box enclosing these \a scid+2 subboxes is returned. Thus, when handling a given variable x with var3BCID, the constructive interval disjunction allows contracting the box in several dimensions. Fixing \a scid to 0 amounts in calling a "standard" shaving (contracting only [x]); the value 1 is the default value and a greater value rarely produces better performances. Therefore the user should generally \a not play with this parameter!
   * \param vhandled Number of calls to \a var3BCID, i.e., the number of variables that are handled in one call to the contract method (ALL_VARS by default). Remark: \a vhandled is used to reach a "fixpoint" in terms of contraction. \a vhandled is preferred to the precision parameter used in the standard 3B that manages the number of loops over all the variables. 
   * \param var_min_width The minimum width an interval must have to be shaved. The default value is 1e-11 but the actual default value should depend on the precision of the solutions (\a Precision::ceil): \a var_min_width should be by default one or several orders of magnitude under \a Precision::ceil. \a var_min_width is used to adjust the actual number \a s3b of slices  considered by the shaving so as to ensure that a slice has a width greater than \a var_min_width, as follows: actual_s3b = Min(s3b, floor(Diam(x)/var_min_width)). Remark: the user should generally \a not play with this parameter in priority because assigning \a var_min_width to a value greater than \a Precision::ceil is interesting in only a few structured systems, e.g., in Yamamura1 that can found in the COCONUT/COPRIN benchmarks.
  * \param cid_space The space passed by reference: /a cid_space contains \a subset of the variables to be shaved, by default all of them. Example: \a cid_space has been used by Araya et al. in conjunction with the preprocessing algorithm I-CSE  (see Proc. of CP 2008): I-CSE produces a system in which are added new auxiliary variables corresponding to common sub-expressions in the initial system; during the search of solutions that follows, the new auxiliary variables are not shaved and are thus removed from  \a cid_space.
 */
    _3BCID(Space& cid_space, Contractor& ctc, int s3b=default_s3b, int scid=default_scid,
             int vhandled=default_vhandled, REAL var_min_width=default_var_min_width) : 
    Operator(ctc.space),  cid_space(cid_space), ctc(*ctc.copy()), s3b(s3b),
      scid(scid), vhandled(vhandled<=0? Dimension(cid_space.box):vhandled),var_min_width(var_min_width),start_var(0) {ctc.contract_floor=-1;}

  /** Duplicates this instance (space is passed by reference). */
  _3BCID(const _3BCID& cid) : Operator(cid.space), Contractor(cid), cid_space(cid.cid_space), ctc(*cid.ctc.copy()), s3b(cid.s3b),
      scid(cid.scid), vhandled(cid.vhandled),var_min_width(cid.var_min_width),start_var(cid.start_var) {ctc.contract_floor=-1;}

  /** Implements the contractor abstract copy. */
  virtual _3BCID* copy() const {
    return new _3BCID(*this);
  }

  /* Accepts the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this);
  }

  /** Deletes this instance. */  
  ~_3BCID() { 
    delete &ctc; 
  }

  /** Apply contraction. 
   * If the \link ibex::Contractor::Indicators::scope scope \endlink field is a
   * specific variable, then #var3BCID is called with the specified variable.
   * Otherwise, 3BCID will be performed.*/

  virtual void contract();

  /** Contracts the \a box slicing the variable \a var.
   *
   * \return false iff the width of the domain of \a var is less than \a var_min_width (var3BCID is not performed).
   * \throw ibex::EmptyBoxException - if inconsistency is detected. */ 
  

  /** This space contains the variables to which var3BCID is applied **/
  Space& cid_space; 

  /** The sub-contractor */
  Contractor& ctc;

  /** Default s3b value, set to 10 (__IBEX_DEFAULT_S3B) **/
  static const int default_s3b;

  /** Default scid value, set to 1 (__IBEX_DEFAULT_SCID) **/
  static const int default_scid;

  /** Default vhandled value, set to ALL_VARS (__IBEX_DEFAULT_VHANDLED) **/
  static const int default_vhandled;

  /** Default var_min_width value, set to 1e-11 (__IBEX_DEFAULT_VAR_MIN_WIDTH) **/
  static const REAL default_var_min_width;

  /** Dichotomy limit , set to 16 : if the number of slices is greater than 
      LimitCIDDichotomy, the shaving process is achieved in a dichotomic way, 
      otherwise it is achieved in a linear way. **/
  static  const int LimitCIDDichotomy;

  protected:
 /** Applies 3BCID contraction on the variable var  */
  bool var3BCID(int var);

  /** Applies 3BCID contraction on the variable var with a dichotomic 3B algorithm */
  bool var3BCID_dicho(int var, REAL wv);

  /** Applies 3BCID contraction on the variable var with a linear 3B algorithm */
  bool var3BCID_slices(int var, int locs3b, REAL w_DC, INTERVAL & dom);

  /** Left or right shaving in a dichotomic way */
  bool shave_bound_dicho(int var,  REAL wv, bool left);

  /** Contracts with CID \a savebox slicing the variable \a var.
    *
    * \param var The current variable.
    * \param savebox The box to be contracted.
    * \param newbox The resulting contrated box
    * \return true if varCID was useful */
  bool varCID(int var, INTERVAL_VECTOR &savebox, INTERVAL_VECTOR &newbox);

  /** Returns true iff \a box1 and \a box2 are equal, excepting the current interval (\a var ) **/
  bool equalBoxes (int var, INTERVAL_VECTOR &box1, INTERVAL_VECTOR &box2);

  /** The maximum number of slices that the contractor will try to refute **/
  int s3b;

  /** The number of slices that the contractor will try to contract after the refutation phase **/
  int scid;

  /** Number of calls to \a var3BCID in one call to the #contract() method. **/
  int vhandled;

  /** The minimum width a interval must have to be varcided **/
  REAL var_min_width;

  /** First variable to varcid in the next call to the #contract() method **/
  int start_var;

};

}

#endif

