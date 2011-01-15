/*---------------------------------------------------------------------------------
 * 
 * Q-Intersection contractor 
 * -----------------------------------------------------------------
 *
 * Copyright (C) 2007-2010 Gilles Chabert
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

#ifndef _IBEX_Q_INTER_H
#define _IBEX_Q_INTER_H

#include "IbexContractor.h"

namespace ibex {

/** \ingroup ctcgroup
 *
 * \brief Q-Intersection.
 *
 * This contractors reduces the domain of a vector x to the
 * hull of all the intersections of q boxes among p.
 *
 * The implementation is based on an (slightly optimized) version of Jaulin-Goldsztejn' algorithm.
 *
 * This contractor is typically used for intersecting p boxes corresponding to 
 * measurements of the same object, in presence of outliers. In this case, 
 * q/p is a safe bound on the frequency of outliers.
 * 
 * \author Gilles Chabert
 * \date September 2010
 */
class QInter : public Contractor {
  public:    
   /** 
    * Q-intersection of p boxes with Jaulin-Goldsztejn' algorithm
    *
    * Contracts x to the q-intersection of some boxes
    * 
    * \pre all the vectors must be bounded (but this constraint
    * could be removed easily...)
    * 
    * \param space the space to work on
    * \param x the variable to contract
    * \param y an array of variables (the "measurements")
    * \param q the number of boxes in y that must interrsect x */
   QInter(Space& space, const ExtendedSymbol& x, const ExtendedSymbol& y, int q);
   
   /** Duplicate this instance. */
   QInter(const QInter& q);
   
   /** Implement the contractor abstract copy.*/
   QInter* copy() const { return new QInter(*this); }
   
   /** Accept the pretty printer. */
   void accept_printer(const OperatorPrinter& op) const;

   /** Contract the domain of x */
   void contract();

   const ExtendedSymbol& x;
   const ExtendedSymbol& _y;
   const int n;
   const int p;
   const int q;

  private:
   /* return the domain of the symbol "y[j+1][i+1]" */
   const INTERVAL& y(int j, int i);

   /* return true iff "y[j+1]" contains m */
   bool y_contains(int j, const VECTOR& m);

   /* intersects the domain of x with the box b.
    *
    * throw EmptyBoxException if the result is 
    * empty */
   void set_x(const INTERVAL_VECTOR& b);

};

} // end namespace

#endif
