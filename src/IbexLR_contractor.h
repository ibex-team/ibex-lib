#ifndef _LR_CONTRACTOR
#define _LR_CONTRACTOR

#include "IbexContractor.h"
#include "IbexSystem.h"
#include "IbexEvaluatorFactory.h"
#include "soplex.h"

using namespace soplex;

namespace ibex {

/** \ingroup ctcgroup
 * \brief 3BCID contractor (extension of 3B with CID)
 *
 * This in an abstract class for linear relaxation-based contractors (AF-based, X-Newton)
 * \author Ignacio Araya
 * \date April 2012
 */
 
 class LR_contractor : public Contractor {
 
  public:
  enum ctc_mode {  ONLY_Y, ALL_BOX };
 
  LR_contractor(const System& sys, Contractor* ctc, int goal_ctr=-1,
     REAL ratio_fp=default_ratio_fp, REAL ratio_fp2=default_ratio_fp2, REAL var_min_width=default_var_min_width, 
		ctc_mode cmode=ALL_BOX, int max_iter_soplex=100, REAL max_diam_box=default_max_diam_box);
	     
  LR_contractor(const LR_contractor& lrc);
  
   /** Implements the contractor abstract copy. */
  virtual LR_contractor* copy() const = 0;

  /* Accepts the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const = 0; 

  /** Basic iteration of the LR-based contractor. Linearize the system and performs calls to Simplex **/
  virtual void iter( );
 
  /** Apply contraction. **/
  virtual void contract();

  /** The pre-contractor */
  Contractor* ctc;

  /** The system */
  const System sys;

  /** Default ratio_fp value, set to 0.1  **/
  static const REAL default_ratio_fp;

  /** Default ratio_fp value, set to 0.01  **/
  static const REAL default_ratio_fp2;
  
  /** Default var_min_width value, set to 1e-11  **/
  static const REAL default_var_min_width;

 /** Default max_diam_box value, set to 1e4  **/
  static const REAL default_max_diam_box;
 
  /** The contraint related to the objective function **/
  int goal_ctr;

  Evaluator* goal;          // evaluator for the goal function
  
  protected:
    
  /** The fixpoint ratio **/
  REAL ratio_fp;

  /* The fixpoint2 ratio (see implementation of void X_Newton::contract() to understand) */
  REAL ratio_fp2;

  /** Min size of a variable to be contracted **/
  REAL var_min_width;
  
  REAL max_diam_box;

  /** Indicates if only y is contracted (cmode=ONLY_Y) or all the box (ALL_BOX) **/
  ctc_mode cmode;
  
  /** isvar[i][j] indicates if the j^th is related to the i^th variable **/
  bool** isvar;

  /** indicates if the constraint is linear **/
  bool* linear;
  
  /** the linearization technique. It must be implemented in the subclasses **/
  virtual int Linearization(SoPlex& mysoplex) = 0; 
  virtual void Map(INTERVAL_VECTOR& box, INTERVAL_VECTOR& tbox) { box&=tbox; }

  //Soplex related functions and variables
  int max_iter_soplex;
  SPxSolver::Status run_simplex(SoPlex& mysoplex, SPxLP::SPxSense sense, int var, int n, INTERVAL& obj, REAL bound);    
  void choose_next_variable ( SoPlex& mysoplex , int & nexti, int & infnexti, int* inf_bound, int* sup_bound);
  void optimizer(SoPlex& mysoplex, int n, int nb_ctrs);


  bool isInner(const System& sys, int j); /* redoundant method? */
  
  private:
  REAL ratio_maxreduction(INTERVAL_VECTOR& box1, INTERVAL_VECTOR& box2);

};
}

#endif
