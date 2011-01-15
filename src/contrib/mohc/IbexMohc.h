#ifndef _IBEX_MOHC_REVISE_H
#define _IBEX_MOHC_REVISE_H

#include "IbexEnv.h"
#include "IbexSystem.h"
#include "IbexConstraint.h"
#include "mohc/IbexConstraintOG.h"
#include "IbexContractor.h"
#include "Ibex3BCID.h"
#include "IbexPropagation.h"

namespace ibex {

  
 /** \ingroup ctcgroup
 * \brief MohcRevise algorithm.
 *
 * This class is an implementation of the <i>Mohc-Revise</i> algorithm.
 * See description in <a href="http://www-sop.inria.fr/coprin/trombe/publis/mohc_aaai_2010.pdf">[ara10]</a>.
 *
 * \author Ignacio Araya, Bertrand Neveu, Gilles Trombettoni
 * \date September 2010
 */ 
class MohcRevise : public Contractor {
 public:

 /** Creates the Mohc-Revise contractor for handling/revising a single constraint.
   *
   * \param ctr_mohc The revised constraint rewritten by Occurrence Grouping (see \link ibex::ConstraintOG \endlink)
   * \param space The contracted variables.
   * \param tau_mohc Ratio to rather set between 0.5 and 0.9999. See class \link ibex::Mohc \endlink.
   * \param epsilon Precision ratio (percentage of interval width) to rather set between 0.01 and 0.1. See class \link ibex::Mohc \endlink.
   * \param univ_newton_min_width Minimum width of a slice in the <i>MonotonicBoxNarrow</i> procedure. The default value is 1e-8 but the actual default value should depend on the precision of the solutions.
   */
  MohcRevise(ConstraintOG& ctr_mohc, Space& space, REAL tau_mohc, REAL epsilon, REAL min_width);

    /** Duplicates this instance (space is passed by reference). */
  MohcRevise (const MohcRevise & mhrv);
  
  /** Implement the contractor abstract copy (space is passed by reference).*/
  virtual MohcRevise* copy() const {
    return new MohcRevise(*this);
  }

  /* Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    p.print(*this);
  }

  /** Contract the box using <i>Mohc-Revise</i> in the constraint \a ctr_mohc. */
  virtual void contract();

  /** Return true if the entity ent is involved in the expression of this equation. */
  virtual bool involves(int ent) const {
    return ctr_mohc.adj.find(ent)!=ctr_mohc.adj.end();
  }
  
  /** The constraint. */
  ConstraintOG& ctr_mohc;
 
  /* Perform \a HC4-Revise*/
  void hc4Revise();
  
  /* Perform \a MinRevise*/  
  void minRevise();
  /* Perform \a MaxRevise*/
  void maxRevise();
  void minRevise2();
  void maxRevise2();
  void LazyMonoBoxNarrow(bool minEval);
  
  /* Perform \a MonotonicBoxNarrow*/
  void MonoBoxNarrow();
  
  /* Perform \a MonotonicBoxNarrow to the i-th variable of \a ctr_mohc
  *
  * \param x The interval been contracted
  */
  void MonoBoxNarrow(INTERVAL& x, int i);
  
 private:
  /* Temporary box (see contract()) */


  void initLeftNarrowFmin(INTERVAL x, int var);
  void initLeftNarrowFmax(INTERVAL x, int var);
  void initRightNarrowFmin(INTERVAL x, int var);
  void initRightNarrowFmax(INTERVAL x, int var);

  void LeftNarrowFmin(INTERVAL& x, int var);
  void LeftNarrowFmax(INTERVAL& x, int var);
  void RightNarrowFmin(INTERVAL& x, int var);
  void RightNarrowFmax(INTERVAL& x, int var);  
  
  void initialize_apply();
  void apply_fmax_to_false_except(int var);
  void apply_fmin_to_false_except(int var);

  INTERVAL_VECTOR tmpbox;
  
  int* ApplyFmin;
  int* ApplyFmax;
  
  INTERVAL *LB, *RB;
  
  REAL tau_mohc;
  REAL epsilon;
  REAL univ_newton_min_width;

  static const int YES = 1;
  static const int NO = 2;
  static const int MAYBE =3;
  
};

 /** \ingroup ctcgroup
 * \brief Mohc algorithm
 *
 * This class implements the preprocessing method used by the <i>Mohc</i> algorithm described in <a href="http://www-sop.inria.fr/coprin/trombe/publis/mohc_aaai_2010.pdf">[ara10]</a>. (See \link ibex::Mohc_All \endlink for an easy-to-use Mohc-based strategy.)
 * \author Ignacio Araya, Bertrand Neveu, Gilles Trombettoni
 * \date September 2010
 */

class MohcPreproc : public Contractor {
   public:

  /** Creates the preprocessing
    * \param ctrs The list of constraints.
    * \param space The space passed by reference
    */     
      MohcPreproc(vector<ConstraintOG*> ctrs, Space& space): Operator(space), ctrs(ctrs){
      }

  /** Duplicates this instance (space is passed by reference). */
     MohcPreproc (const MohcPreproc& mopr);
    
      /** Implement the contractor abstract copy (space is passed by reference).*/
      virtual MohcPreproc* copy() const {
         return new MohcPreproc(*this);
      }

      /* Accept the pretty printer. */
      virtual void accept_printer(const OperatorPrinter& p) const { 
         p.print(*this);
      }

   /** Perform the preprocessing mehtod. It computes rho_mohc for each constraint.*/
      virtual void contract();

      /** The list of constraints. */
      vector<ConstraintOG*> ctrs;
};

  
 /** \ingroup ctcgroup
 * \brief Mohc algorithm
 *
 * This class is an implementation of the <i>Mohc</i> algorithm described in <a href="http://www-sop.inria.fr/coprin/trombe/publis/mohc_aaai_2010.pdf">[ara10]</a>. (See \link ibex::Mohc_All \endlink for an easy-to-use Mohc-based strategy.)
 * Before performing a contraction using <i>Mohc</i>, it is necessary  to perform a preprocessing in the current box. The preprocessing is performed by the method get_preproc()->contract(). 
 * The following code shows how we can generate a contractor Shaving(Mohc):
 * \code
 * #include "IbexMohc.h"
 * #include "IbexSequence.h"
 * #include "IbexShaving.h"
 * ...
 * Mohc mohc(sys);
 * Sequence shav_mohc(*mohc.get_preproc(),Shaving(mohc));
 * ...
 * \endcode
 * \author Ignacio Araya, Bertrand Neveu, Gilles Trombettoni
 * \date September 2010
 */
class Mohc : public Propagation {
 public:

  /** Creates the <i>Mohc</i> contractor.
  
    * \param sys The handled system.
    * \param ratio The precision ratio (percentage of interval width) in the propagation loop. See \a IbexPropagation.
    * \param incremental A boolean indicating whether the propagation is incrementally run from the last variable that has been bisected in the search tree. To be generally set to true when \a Mohc is the only contractor called between two bisections. See \a IbexPropagation.
    * \param tau_mohc tau_mohc Ratio to rather set between 0.5 and 0.9999. \a tau_mohc is the most significant user-defined parameter of \a Mohc. If \c tau_mohc=1, this means that the <i>MohcRevise</i> procedure will always call the "monotonic" contractors, namely <i>MinRevise</i>, <i>Maxrevise</i> and <i>MonoBoxNarrow</i>; If \c tau_mohc=0.6, this means that, for a given constraint ctr, <i>MohcRevise</i> will call the monotonic contractors only if rho_mohc(ctr) is inferior to 0.6, i.e., if the width of the interval image of ctr evaluated by monotonicity is less than 60% of the image width obtained by natural evaluation. 
    * Remark 1: \a rho_mohc(ctr) is computed in a preprocessing phase called before the propagation: it is computed for all the constraints in the system by the constructor of the class \link ibex::MohcPreproc \endlink that is called inside the constructor of the class \link ibex::Mohc \endlink. 
    * Remark 2: In practice, the default value (0.6 or 0.5) would ensure that <i>Mohc</i> show performances either significantly better than HC4 (see \link ibex::HC4 \endlink) or similar, but hopefully never worse. Please contact the authors if it occurs.
    * \param epsilon The precision ratio (percentage of interval width) of the shaving process performed by the monotonic BoxNarrow procedure (see the method \a MonoBoxNarrow). Remark: when called inside a solving strategy (i.e., a tree search), <i>Mohc</i> is not very sensitive to this parameter, contrarily to the standard <i>Box</i> contractor. Experiments on zero-dimensional n*n benchmarks (see Araya et al. in Proc. CP 2010 and details in the PhD thesis of Ignacio Araya) suggest that epsilon=3% should be relevant for any system when <i>Mohc</i> is called "alone" between two choice points in the search tree, whereas epsilon=10% should be "always" adequate (and sufficient) when <i>Mohc</i> is a sub-contractor of a shaving contractor, such as <i>3B</i> or <i>3BCID</i> (see \link ibex::_3BCID \endlink).
    * \param univ_newton_min_width Minimum width of a slice in the <i>MonotonicBoxNarrow</i> procedure. The default value is 1e-8 but the actual default value should depend on the precision of the solutions
    */

  Mohc(const System& sys, REAL ratio=default_ratio, bool incremental=false, 
        REAL tau_mohc=default_tau_mohc, REAL epsilon=default_epsilon, 
        REAL univ_newton_min_width=default_univ_newton_min_width);

  Mohc(MohcPreproc* preproc, const System& sys, REAL ratio=default_ratio, bool incremental=false, 
        REAL tau_mohc=default_tau_mohc, REAL epsilon=default_epsilon, 
        REAL univ_newton_min_width=default_univ_newton_min_width);




  /** Duplicates this instance (space is passed by reference). */
  Mohc (const Mohc &);

  /** Implement the contractor abstract copy (space is passed by reference).*/
  virtual Mohc* copy() const {
     return new Mohc(*this);
  }
  
  /* Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    p.print(*this);
  }
    
  /** Deletes this instance. */  
  virtual ~Mohc() {delete preproc; }

  /** Returns the preprocessing instance. The preprocessing
  computes rho_mohc for each constraint before 
  calling the <i>Mohc</i> contractor.*/
  MohcPreproc* get_preproc(){
     return preproc;
  }
  
  
  /** Default \a tau_mohc value, set to 0.5  **/
  static const REAL default_tau_mohc;

  /** Default \a epsilon value, set to 0.1  **/
  static const REAL default_epsilon;
  
  /** Default \a min_width value, set to 1e-8 **/
  static const REAL default_univ_newton_min_width;

  /** The preprocessing. The method preproc.contract()
  computes rho_mohc for all the constraints. 
  It should be performed once after every bisection
  followed by <i>Mohc</i> or <i>Shaving(Mohc)</i>. */
  MohcPreproc* preproc;
 
};

 /** \ingroup ctcgroup
 * \brief Mohc-based contractor
 *
 * This class is an implementation of a Mohc-based contractor. The contractor performs the following procedures:
 * - Preprocessing (compute the \a ratio_mohc value to be used in the Mohc algorithm)
 * - Mohc (described in Araya, Trombettoni, Neveu, Proc. AAAI 2010)
 * - 3BCID(Mohc) (performed only if \a with3BCID is set to true)
 * \author Ignacio Araya, Bertrand Neveu, Gilles Trombettoni
 * \date September 2010
 */
class Mohc_All : public Contractor {
  public:
    
    /** Creates the \a Mohc_All contractor.
    * \param sys The handled system.
    * \param ratio The precision ratio (percentage of interval width) in the propagation loop. See \a IbexPropagation.
    * \param incremental A boolean indicating whether the propagation is incrementally run from the last variable that has been bisected in the search tree. To be generally set to true when \a Mohc is the only contractor called between two bisections  \link ibex::Propagation \endlink. The Mohc contractor inside 3Bcid (when with3BCID is true) is always incremental.
    * \param tau_mohc tau_mohc Ratio to rather set between 0.5 and 0.9999. \a tau_mohc is the most significant user-defined parameter of <i>Mohc</i>. If \c tau_mohc=1, this means that the <i>MohcRevise</i> procedure will always call the "monotonic" contractors, namely <i>MinRevise</i>, <i>Maxrevise</i> and <i>MonoBoxNarrow</i>; If \c tau_mohc=0.6, this means that, for a given constraint ctr, <i>MohcRevise</i> will call the monotonic contractors only if rho_mohc(ctr) is inferior to 0.6, i.e., if the width of the interval image of ctr evaluated by monotonicity is less than 60% of the image width obtained by natural evaluation. 
    * Remark 1: \a rho_mohc(ctr) is computed in a preprocessing phase called before the propagation: it is computed for all the constraints in the system by the constructor of the class \link ibex::MohcPreproc \endlink that is called inside the constructor of the class \link ibex::Mohc \endlink. 
    * Remark 2: In practice, the default value (0.6 or 0.5) would ensure that <i>Mohc</i> show performances either significantly better than HC4 (see \link ibex::HC4 \endlink) or similar, but hopefully never worse. Please contact the authors if it occurs.
    * \param epsilon The precision ratio (percentage of interval width) of the shaving process performed by the monotonic BoxNarrow procedure (see the method \a MonoBoxNarrow). 
    * Remark: when called inside a solving strategy (i.e., a tree search), <i>Mohc</i> is not very sensitive to this parameter, contrarily to the standard <i>Box</i> contractor. Experiments on zero-dimensional n*n benchmarks (see Araya et al. in Proc. CP 2010 and details in the PhD thesis of Ignacio Araya) suggest that epsilon=3% should be relevant for any system when <i>Mohc</i> is called "alone" between two choice points in the search tree, whereas epsilon=10% should be "always" adequate (and sufficient) when <i>Mohc</i> is a sub-contractor of a shaving contractor, such as 3B or 3BCID (see Ibex3BCID).
    * \param univ_newton_min_width Minimum width of a slice in the <i>MonotonicBoxNarrow</i> procedure. The default value is 1e-8 but the actual default value should depend on the precision of the solutions.
    * \param with3BCID A boolean indicating if Mohc is used as subcontractor of 3BCID.
    * \param s3b number of slices used the shaving process of 3BCID
    * \param scid Maximum number of slices that can be contracted by the CID (constructive disjunction) contractor. Principle: when the standard shaving process in a given variable x is finished, the remaining interval is split into \a scid slices (of equal width); the subcontractor ctc is then applied to these slices and the \a scid returned contracted boxes, plus [B_left], plus [B_right], are hulled, i.e., the smallest box enclosing these \c scid+2 subboxes is returned. Thus, when handling a given variable x with var3BCID, the constructive interval disjunction allows contracting the box in several dimensions. Fixing \a scid to 0 amounts in calling a "standard" shaving (contracting only [x]); the value 1 is the default value and a greater value rarely produces better performances. Therefore the user should generally \a not play with this parameter!
    * \param vhandled Number of calls to \a var3BCID, i.e., the number of variables that are handled in one call to the contract method (ALL_VARS by default). Remark: \a vhandled is used to reach a "fixpoint" in terms of contraction. \a vhandled is preferred to the precision parameter used in the standard 3B that manages the number of loops over all the variables. 
    * \param var_min_width The minimum width an interval must have to be shaved. The default value is 1e-11 but the actual default value should depend on the precision of the solutions (\link Precision::ceil \endlink): \a var_min_width should be by default one or several orders of magnitude under \link Precision::ceil \endlink. \a var_min_width is used to adjust the actual number \a s3b of slices  considered by the shaving so as to ensure that a slice has a width greater than \a var_min_width, as follows: actual_s3b = Min(s3b, floor(Diam(x)/var_min_width)). Remark: the user should generally \a not play with this parameter in priority because assigning \a var_min_width to a value greater than \link Precision::ceil \endlink is interesting in only a few structured systems, e.g., in Yamamura1 that can be found in the COCONUT/COPRIN benchmarks.
    */


  Mohc_All(const System& sys, REAL ratio=Propagation::default_ratio, bool incremental=false, 
	 REAL tau_mohc=Mohc::default_tau_mohc, REAL epsilon=Mohc::default_epsilon, 
	   REAL univ_newton_min_width=Mohc::default_univ_newton_min_width,
	  bool with3BCID=true, int s3b=_3BCID::default_s3b, int scid=_3BCID::default_scid, 
          int vhandled=_3BCID::default_vhandled, REAL var_min_width=_3BCID::default_var_min_width); 
 
  /* Added by BNE */
  /** Constructor with default values for all parameters   
   * \param sys The handled system.
   * \param with3BCID A boolean indicating if Mohc is used as subcontractor of 3BCID.
   * \param prec The precision : should correspond to the precision of the paver in case of use as a contractor inside a paver 
     this parameter is used as absolute precision for univ_newton (univ_newton_min_width) and 3BCID (var_min_width) (defaut value is 1.e-8)
   * \param tau_mohc : cf doc of constructor with all parameters (default value is tau_mohc=0.9)
   */

  Mohc_All(const System& sys, bool with3BCID,  REAL prec= Mohc::default_univ_newton_min_width, REAL tau_mohc=Mohc::default_tau_mohc);

  /** Deletes this instance. */ 	  
  virtual ~Mohc_All(){
    delete mohc;
    if(m3b) delete m3b;
  }
  
  /** Duplicates this instance (space is passed by reference). */  
  Mohc_All (const Mohc_All &);	   
  
  /** Implement the contractor abstract copy (space is passed by reference).*/
  virtual Mohc_All* copy() const {
     return new Mohc_All(*this);
  }

  /* Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    p.print(*this);
  }

  /** Apply contraction: 
  * <i>3BCID</i> with <i>Mohc</i> as subcontractor if \a with3BCID is set to true, otherwise only <i>Mohc</i>.
  */
  virtual void contract();

  /** The <i>Mohc</i> contractor */
  Mohc* mohc;
  
  /** The <i>3BCID</i> contractor */
  _3BCID* m3b;
  
  
};

} // end namespace

#endif
