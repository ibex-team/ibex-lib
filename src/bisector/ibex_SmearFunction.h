//============================================================================
//                                  I B E X                                   
// File        : Smear Function bisectors
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : July 19, 2012
// Last Update : July 19, 2012
//============================================================================

#ifndef __IBEX_SMEAR_FUNCTION_H__
#define __IBEX_SMEAR_FUNCTION_H__

#include "ibex_Bsc.h"
#include "ibex_Backtrackable.h"
#include "ibex_RoundRobin.h"
#include "ibex_System.h"

namespace ibex {
/**
 * \ingroup bisector
 *
 * \brief bisector with Smear function heuristic  abstract class
 *
 */

class SmearFunction : public RoundRobin {

protected :
   int nbvars;

 public:
  /** Create a bisector with Smear function heuristic,i.e using the impacts based on the Jacobian matrix.
      Abstract  class with 4 implementations SmearMax, SmearSum, SmearMaxRelative, SmearSumRelative corresponding to
      different variants for computing the variable to be bisected, all variants using the Jacobian matrix.
   *  \param sys - The system of equations. 
   *  \param w (optional) - - the minimum width (diameter) an interval must have to be bisected (see #w).
   *  \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   *  bisected interval. Default value is 0.45*/
 SmearFunction( System& sys,double w=Bsc::default_prec, double ratio=Bsc::default_ratio) : 
   RoundRobin( w, ratio), sys(sys) {nbvars=sys.nb_var; }

  
 

  /** Bisect the box. 
   * \param last_var - the last component that has been bisected.
   * In case the jacobian matrix could not be computed correctly, the box is split
   * with the \link ibex::RoundRobin::bisect(int) const round-robin strategy \endlink. */
   virtual  std::pair<IntervalVector,IntervalVector> bisect(const IntervalVector& box, int& last_var);


  /** returns the variable to bisect : the variable i with the greatest  maximum impact  Abs(Dfj/Dxi) * Diam(xi)
   * \param J the jacobian matrix J */
   virtual int var_to_bisect(IntervalMatrix& J, const IntervalVector& box ) const =0;
 protected:
  System& sys;
};

/**
 * \ingroup bisector
 *
 * \brief bisector with Smear function heuristic (maximum impact)
 *
 */
class SmearMax : public SmearFunction
 {

  public :
  /** Create a bisector using the Smear function heuristic (defined by Kearfott in 1990),
      where the variable with the maximum impact is chosen to be bisected
   *  \param sys - The system of equations. 
   *  \param w (optional) - - the minimum width (diameter) an interval must have to be bisected (see #w).
   *  \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   *  bisected interval. Default value is 0.45 */


    SmearMax (System& sys, double w=Bsc::default_prec, double ratio=Bsc::default_ratio)
      : SmearFunction (sys,w,ratio) {;};

  

  ~SmearMax() {;};

  
  /** returns the variable to bisect : the variable i with the greatest  impact over the constraints fj : max(j) Abs(Dfj/Dxi) * Diam (xi)   
   * \param J the jacobian matrix J */ 
  int var_to_bisect(IntervalMatrix& J,const IntervalVector& box  ) const;
};

/**
 * \ingroup bisector
 *
 * \brief bisector with Smear function heuristic (maximum sum of impacts)
 *
 */


class SmearSum : public SmearFunction
 {

  public :
  /** Create a bisector with a variant of the Smear function heuristic defined by Hansen, 
      where the sum of the impacts is used instead of the maximum
   *  \param sys - The system of equations. 
   *  \param w (optional) - - the minimum width (diameter) an interval must have to be bisected (see #w).
   *  \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   *  bisected interval. Default value is 0.45 */


    SmearSum (System& sys, double  w=Bsc::default_prec, double ratio=Bsc::default_ratio)
      : SmearFunction (sys,w,ratio) {;};

  
  ~SmearSum() {;};
  
  /** returns the variable to bisect : the variable i with the greatest  sum of  impacts over the constraints fj : sum(j) Abs(Dfj/Dxi) * Diam (xi)   
   * \param J the jacobian matrix J */ 
  int var_to_bisect(IntervalMatrix& J, const IntervalVector& box ) const;
};


/**
 * \ingroup bisector
 *
 * \brief bisector with Smear function heuristic (maximum sum of normalized impacts)
 *
 */

class SmearSumRelative : public SmearFunction
 {
  public :
/** Create a bisector with a variant of the Smear function heuristic, where we use a normalized sum of the impacts instead the maximum used by SmearMax or the sum used by SmearSum
   *  \param sys - The system of equations. 
   *  \param w (optional) - - the minimum width (diameter) an interval must have to be bisected (see #w).
   *  \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   *  bisected interval. Default value is 0.45 */


    SmearSumRelative (System& sys, double w=Bsc::default_prec, double ratio=Bsc::default_ratio)
      : SmearFunction (sys,w,ratio) {;};
  ~SmearSumRelative() {;};


  
/** returns the variable to bisect : the variable i with the greatest  normalized sum of  impacts over the constraints fj : sum(j) Dfj/Dxi * Diam (xi) / NC(fj) , where NC(fj) = sum(i) Abs(Dfj/Dxi) * Diam(xi)
   * \param J the jacobian matrix J */ 
  int var_to_bisect(IntervalMatrix & J, const IntervalVector& box ) const ;
};


/**
 * \ingroup bisector
 *
 * \brief bisector with Smear function heuristic (maximum  normalized impact)
 *
 */



class SmearMaxRelative : public SmearFunction
 {
  public :
   /** Create a bisector with a variant of the Smear function heuristic, where we use a normalized maximimum impact instead of the max used by SmearMax or the sum used by SmearSum
   *  \param sys - The system of equations. 
   *  \param w (optional) - - the minimum width (diameter) an interval must have to be bisected (see #w).
   *  \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   *  bisected interval. Default value is 0.45 */


    SmearMaxRelative (System& sys, double w=Bsc::default_prec, double ratio=Bsc::default_ratio)
      : SmearFunction (sys,w,ratio) {;};
  ~SmearMaxRelative() {;};


  
/** returns the variable to bisect : the variable i with the greatest normalized  impact over the constraints fj :  Dfj/Dxi * Diam (xi) / NC(fj) , where NC(fj) = sum(i) Abs(Dfj/Dxi) * Diam(xi)
   * \param J the jacobian matrix J */ 

  int var_to_bisect(IntervalMatrix & J, const IntervalVector& box ) const ;
};



} // end namespace ibex

#endif // __IBEX_SMEAR_FUNCTION_H__
