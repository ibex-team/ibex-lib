/*---------------------------------------------------------------------------------
 * 
 * Bisectors: base class and some standard implementations 
 *--------------------------------------------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#ifndef _IBEX_BISECTOR_H
#define _IBEX_BISECTOR_H

#include "IbexOperator.h"
#include "IbexBisection.h"
#include "IbexCell.h"
#include "IbexBlockSet.h"
#include "IbexSystem.h"

namespace ibex {

/** \ingroup solvergroup
 * \brief Generic bisector
 *
 * Base class of all bisectors. A bisector is an operator used by a \link ibex::Paver paver \endlink
 * to make a choice point.
 * An implementation of this class must provide a #bisect() function.
 *
 * \author Gilles Chabert
 * \date November 2007 
 */
class Bisector : public Operator {
public:
  
  /** Create the bisector on a given space.
   *  \param space - The space, passed by reference. 
   *  \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   *  bisected interval. Default value is 0.45,  */
  Bisector(Space& space, REAL ratio=default_ratio) : Operator(space), ratio(ratio), cell(NULL) { }

  /** Duplicate this instance (space is passed by reference). */
  Bisector(const Bisector& b) : Operator(b.space), ratio(b.ratio), cell(b.cell) { }

  /** Override the interface of the generic operator copy. */
  virtual Bisector* copy() const=0;

  /** Return the current cell (NULL pointer if none). 
   * This information can be typically used to get the last bisected variable in case 
   * the bisector is called by a paver (e.g., for RoundRobin). */
  Cell* current_cell() { return cell; }

  /** Bisect the current box (in a solving process). 
   * \warning *this will modify the space associated to the cell*. */
  Bisection bisect(Cell& cell);

  /** Bisect the current box and return the result. */
  virtual Bisection bisect()=0;

  /** Return the last bisected variable if this bisector
   * is called from a paver, -1 otheriwse.
   * If this is the first time this bisector is called by
   * the paver, return also -1. */
  int last_bisected_var() {
    Cell* cell=current_cell();
    if (cell && !cell->root())
      return cell->last_split_var;
    else 
      return -1;
  }

  /** Set to 1e-07 (__IBEX_DEFAULT_PRECISION). */
  static const REAL default_prec;

  /** Set to 0.45 (__IBEX_DEFAULT_RATIO_BISS). */
  static const REAL default_ratio;
  
  /** Ratio between the diameters of the left and right parts of a bisected
   *  interval. */
  const REAL ratio;

  //  protected:
  //  Bisection bisection;

  private:
  Cell* cell;

};

/** \ingroup solvergroup
 * \brief Round-robin bisector
 *
 * \author Gilles Chabert
 * \date October 2007 
 */
class RoundRobin : public Bisector {
 public:
  /** Create a bisector with round-robin heuristic.
   * \param space - The space, passed by reference. 
   * \param w - the minimum width (diameter) an interval must have to be bisected (see #w).
   * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   * bisected interval. Default value is 0.45
   */
  RoundRobin(Space& space, REAL w=Bisector::default_prec, REAL ratio=Bisector::default_ratio) : Bisector(space, ratio), w(w) { }

  /** Duplicate this instance (space is passed by reference). */  
  RoundRobin(const RoundRobin& rr) : Bisector(rr), w(rr.w) { }

  /** Implement the bisector abstract copy. */
  virtual RoundRobin* copy() const {
    return new RoundRobin(*this);
  }
 
  /** Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this);
  }

  Bisection bisect() { return bisect(last_bisected_var()); }

  /** Bisect the box. 
   *  \param last_var - the last component that has been bisected (hence, if the domain of the 
   *  \c last_var+1 component is wider than \a w, it will be bisected) or -1 if this is the first call (in this case,
   *  the variable examinated first is the first one). In case all the variables have domain diameter less than \a w,
   *  the first variable (0) will be bisected. */
  Bisection bisect(int last_var) const;

  /** Allow to skip variables whose domain is too small in the round-robin heuristic. This parameter usually
   *  matches the \link Precision::ceil ceil \endlink paramter of the precision contractor
   *  since boxes that are too small are usually both not bisected and considered as precise enough. */
  const REAL w;
  
};

/** \ingroup solvergroup
 * \brief Largest-domain first bisector
 *
 * \author Gilles Chabert
 * \date October 2007 
 */
class LargestFirst : public Bisector {
 public:
  /** Create a bisector with largest-domain-first heuristic.
   * \param space - The space, passed by reference. 
   * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   * bisected interval. Default value is 0.45*/
  LargestFirst(Space& space, REAL ratio=Bisector::default_ratio) : Bisector(space, ratio) { }
 
  /** Duplicate this instance (space is passed by reference). */
  LargestFirst(const LargestFirst& lf) : Bisector(lf) { }

  /** Implement the bisector abstract copy. */
  virtual LargestFirst* copy() const {
    return new LargestFirst(*this);
  }

  /** Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this);
  }

  /** Bisect the box. */
  Bisection bisect(); 
};


/** \ingroup solvergroup
 * \brief Bisector based on the Smear function
 *
 * \author Gilles Chabert and Bertrand Neveu
 * \date October 2007 
 * \date updated: September 2010 (BNE)
 */
class SmearFunction : public RoundRobin {

protected :
   int nbvars;

 public:
  /** Create a bisector with Smear function heuristic,i.e using the impacts based on the Jacobian matrix.
      Virtual class with 4 implementations SmearMax, SmearSum, SmearMaxRelative, SmearSumRelative corresponding to
      different variants for computing the variable to be bisected, all variants using the Jacobian matrix.
   *  \param sys - The system of equations. 
   *  \param w (optional) - - the minimum width (diameter) an interval must have to be bisected (see #w).
   *  \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   *  bisected interval. Default value is 0.45*/
 SmearFunction(const System& sys,REAL w=Bisector::default_prec, REAL ratio=Bisector::default_ratio) : 
   RoundRobin(sys.space, w, ratio), sys(sys) {nbvars=sys.nb_var(); }

  
 
  /** Duplicate this instance (space is passed by reference). */
  SmearFunction(const SmearFunction& smf) : RoundRobin(smf), sys(smf.sys) {nbvars=smf.nbvars; }

 
  /* Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this);
  }

  /** Bisect the box. 
   * \param last_var - the last component that has been bisected.
   * In case the jacobian matrix could not be computed correctly, the box is split
   * with the \link ibex::RoundRobin::bisect(int) const round-robin strategy \endlink. */
  Bisection bisect(int last_var) const ;

  /** Bisect the current box */
  Bisection bisect() { return bisect(last_bisected_var()); }

  /** returns the variable to bisect : the variable i with the greatest  maximum impact  Abs(Dfj/Dxi) * Diam(xi)
   * \param J the jacobian matrix J */
  virtual int var_to_bisect(INTERVAL_MATRIX& J) const =0;
 protected:
  System sys;
};

class SmearMax : public SmearFunction
 {

  public :
  /** Create a bisector using the Smear function heuristic (defined by Kearfott in 1990),
      where the variable with the maximum impact is chosen to be bisected
   *  \param sys - The system of equations. 
   *  \param w (optional) - - the minimum width (diameter) an interval must have to be bisected (see #w).
   *  \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   *  bisected interval. Default value is 0.45 */


    SmearMax (const System& sys, REAL w=Bisector::default_prec, REAL ratio=Bisector::default_ratio)
      : SmearFunction (sys,w,ratio) {;};
 /** implement the bisector abstract copy. */
  virtual SmearMax* copy() const {
    return new SmearMax(*this);
  }

  ~SmearMax() {;};

  
  /** returns the variable to bisect : the variable i with the greatest  impact over the constraints fj : max(j) Abs(Dfj/Dxi) * Diam (xi)   
   * \param J the jacobian matrix J */ 
  int var_to_bisect(INTERVAL_MATRIX& J) const;
};

class SmearSum : public SmearFunction
 {

  public :
  /** Create a bisector with a variant of the Smear function heuristic defined by Hansen, 
      where the sum of the impacts is used instead of the maximum
   *  \param sys - The system of equations. 
   *  \param w (optional) - - the minimum width (diameter) an interval must have to be bisected (see #w).
   *  \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   *  bisected interval. Default value is 0.45 */


    SmearSum (const System& sys, REAL w=Bisector::default_prec, REAL ratio=Bisector::default_ratio)
      : SmearFunction (sys,w,ratio) {;};

   virtual SmearSum* copy() const {
    return new SmearSum(*this);
  }
  ~SmearSum() {;};
  
  /** returns the variable to bisect : the variable i with the greatest  sum of  impacts over the constraints fj : sum(j) Abs(Dfj/Dxi) * Diam (xi)   
   * \param J the jacobian matrix J */ 
  int var_to_bisect(INTERVAL_MATRIX& J) const;
};


class SmearSumRelative : public SmearFunction
 {
  public :
/** Create a bisector with a variant of the Smear function heuristic, where we use a normalized sum of the impacts instead the maximum used by SmearMax or the sum used by SmearSum
   *  \param sys - The system of equations. 
   *  \param w (optional) - - the minimum width (diameter) an interval must have to be bisected (see #w).
   *  \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   *  bisected interval. Default value is 0.45 */


    SmearSumRelative (const System& sys, REAL w=Bisector::default_prec, REAL ratio=Bisector::default_ratio)
      : SmearFunction (sys,w,ratio) {;};
  ~SmearSumRelative() {;};

  SmearSumRelative* copy() const {
    return new SmearSumRelative(*this);
  }
/** returns the variable to bisect : the variable i with the greatest  normalized sum of  impacts over the constraints fj : sum(j) Dfj/Dxi * Diam (xi) / NC(fj) , where NC(fj) = sum(i) Abs(Dfj/Dxi) * Diam(xi)
   * \param J the jacobian matrix J */ 
  int var_to_bisect(INTERVAL_MATRIX & J) const ;
};


/** Create a bisector with a variant of the Smear function heuristic, where we use a normalized maximimum impact instead of the max used by SmearMax or the sum used by SmearSum
   *  \param sys - The system of equations. 
   *  \param w (optional) - - the minimum width (diameter) an interval must have to be bisected (see #w).
   *  \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   *  bisected interval. Default value is 0.45 */


class SmearMaxRelative : public SmearFunction
 {
  public :
    SmearMaxRelative (const System& sys, REAL w=Bisector::default_prec, REAL ratio=Bisector::default_ratio)
      : SmearFunction (sys,w,ratio) {;};
  ~SmearMaxRelative() {;};

  SmearMaxRelative* copy() const {
    return new SmearMaxRelative(*this);
  }
/** returns the variable to bisect : the variable i with the greatest normalized  impact over the constraints fj :  Dfj/Dxi * Diam (xi) / NC(fj) , where NC(fj) = sum(i) Abs(Dfj/Dxi) * Diam(xi)
   * \param J the jacobian matrix J */ 

  int var_to_bisect(INTERVAL_MATRIX & J) const ;
};


/** \ingroup solvergroup
 * \brief Blockwise round-robin bisector
 *
 * The strategy of this bisector is as follows. Variables of the first block are elected in a round-robin strategy until they all have their diameter
 * lower than #w. Then, the variables of the second block are elected in a round-robin strategy, and so on.
 * The procedure can skip several blocks until a variable with a large enough domain is found.
 * 
 * If none, the first variable of the first non-empty block is then elected.
 *
 * When the bisection is called with last_var set to a particular variable, the process starts at the block to which last_var belong and does not
 * check (backward) the previous blocks.
 *
 * \author Gilles Chabert
 * \date created: November 2007 
 * \date last update: September 2009
 */
class BlockRoundRobin : public Bisector {
 public:
  /** Create a bisector with blockwise round-robin heuristic.
   * \param space - The space, passed by reference. 
   * \param bs - The variable blocks
   * \param w - the minimum width (diameter) an interval must have to be bisected.
   * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the 
   * bisected interval. Default value is 0.45 */

  BlockRoundRobin(Space& space, const BlockSet& bs, REAL w=Bisector::default_prec, REAL ratio=Bisector::default_ratio) : 
    Bisector(space, ratio), w(w), bs(bs) { }
 
  /** Duplicate this instance (space is passed by reference). */
  BlockRoundRobin(const BlockRoundRobin& brr) : Bisector(brr), w(brr.w), bs(brr.bs) { }

  /** Implement the bisector abstract copy. */
  virtual BlockRoundRobin* copy() const {
    return new BlockRoundRobin(*this);
  }

  /** Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this);
  }

  /** Bisect the box. 
   * \param last_var - the last component that has been bisected (hence, if the domain of the 
   * \c last_var+1 component is wider than \a w, it will be bisected) or -1 if this is the first call (in this case,
   * the variable examinated first is the first one). */
  Bisection bisect(int last_var) const throw(BlockSetException) ;

  /** Bisect the current box (in a paving/solving process). 
   * \warning - may also throw BlockSetException. */
  Bisection bisect() { return bisect(last_bisected_var()); }

  /** Allows to skip variables whose domain is too small in the blockwise round-robin heuristic. This parameter 
   * usually matches the \link Precision::ceil ceil \endlink paramter of the precision contractor
   * since boxes that are too small are usually both not bisected and considered as precise enough. */
  const REAL w;

 private:
  BlockSet bs;
};

} // end namespace

#endif
