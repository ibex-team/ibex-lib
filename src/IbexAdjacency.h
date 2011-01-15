/*---------------------------------------------------------------------------------
 * 
 * Adjacency of contractors
 * ------------------------
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

#ifndef _IBEX_ADJACENCY_H
#define _IBEX_ADJACENCY_H

#include "IbexSpace.h"
#include "IbexQueue.h"
#include "IbexContractorList.h"

namespace ibex {

/** \ingroup ctcgroup
 * \brief Adjacency of a contractor list (CSP).
 * 
 * This class contains the adjacency (incidence graph) of contractors (e.g., CSP)
 * typically used by propagation-based algorithms.
 * An Adjacency allows to know whether an entity is impacted by a contractor or not. As an example,
 * it allows to know whether an equation (with any associated contractor) contains a variable.
 * 
 * The space complexity is in O(n*m) where n is the number of entities and m the number of
 * contractors. For efficiency reasons, both an adjacency table and adjacency lists are built.
 *
 * Since one may rather think in terms of "CSP" and "constraints" rather than
 * "contractors, we shall switch to the former more intuitive terminology
 * in the documentation of this class.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class Adjacency {

 public:

  /** Build the adjacency of a list of contractors.
   *
   * In case one of the components of \a csp has a different space than \a space: if an entity belongs to 
   * the component's space but not to \a space, it will be ignored. This means that no contractor will be 
   * considered to impact this entity (so that, e.g., the value returned by #nb_var() will match the number 
   * of variables in \a space). */
  Adjacency(const ContractorList& csp, const Space& space);

  /** Duplicate the adjacency. */
  Adjacency(const Adjacency&); 

  /** Delete the adjacency. */
  ~Adjacency();

  /** Return the number of constraints. */
  inline int nb_ctr() const { return _nb_ctr; }

  /** Return the number of variables in the CSP. */
  inline int nb_var() const { return nb_ent[IBEX_VAR]; }

  /** Return the number of univ-params in the CSP. */
  inline int nb_upr() const { return nb_ent[IBEX_UPR]; }

  /** Return the number of exist-params in the CSP. */
  inline int nb_epr() const { return nb_ent[IBEX_EPR]; }

  /** Return the number of symbolic constants in the CSP. */
  inline int nb_syb() const { return nb_ent[IBEX_SYB]; }

  /*--------------------------------------------------------------------------------*/
  /** Return the number of variables in a constraint \a ctr.
   *  \retval n - a value that satisfies 0 <= \a n <= #nb_var().
   *  \pre 0 <= \a ctr < #nb_ctr(). */
  inline int ctr_nb_vars(int ctr) const { return ctr_adj[IBEX_VAR][ctr][0]; }

  /** Return the \a i th variable in a constraint \a ctr. 
   *  \retval var - a value that satisfies 0 <= \a var < #nb_var().
   *  \pre 0 <= \a var < #nb_var(). <br> 0 <= \a i < #ctr_nb_vars (\a ctr). */
  inline int ctr_ith_var(int ctr, int i) const { return ctr_adj[IBEX_VAR][ctr][i+1]; }

  /** Return the number of constraints containing a variable \a var.
   *  \pre 0 <= \a var < #nb_var().
   *  \retval n - a value that satisfies 0 <= \a n <= #nb_ctr(). */
  inline int var_nb_ctrs(int var) const { return ent_adj[IBEX_VAR][var][0]; }

  /** Return the \a i th constraint containing a variable \a var.
   *  \retval ctr - a value that satisfies 0 <= \a ctr < #nb_ctr().
   *  \pre 0 <= \a var < #nb_var(). <br> 0 <= \a i < #var_nb_ctrs (\a var). */
  inline int var_ith_ctr(int var, int i) const { return ent_adj[IBEX_VAR][var][i+1]; }

  /*--------------------------------------------------------------------------------*/
  /** Return the number of univ parameters in a constraint \a ctr.
   *  \retval n - a value that satisfies 0 <= \a n <= #nb_upr().
   *  \pre 0 <= \a ctr < #nb_ctr(). */
  inline int ctr_nb_uprs(int ctr) const { return ctr_adj[IBEX_UPR][ctr][0]; }

  /** Return the \a i th univ parameter in a constraint \a ctr. 
   *  \retval upr - a value that satisfies 0 <= \a upr < #nb_upr().
   *  \pre 0 <= \a upr < #nb_upr(). <br> 0 <= \a i < #ctr_nb_uprs (\a ctr). */
  inline int ctr_ith_upr(int ctr, int i) const { return ctr_adj[IBEX_UPR][ctr][i+1]; }

  /** Return the number of constraints containing a univ parameter \a upr.
   *  \pre 0 <= \a upr <  #nb_upr().
   *  \retval n - a value that satisfies 0 <= \a n <= #nb_ctr(). */
  inline int upr_nb_ctrs(int upr) const { return ent_adj[IBEX_UPR][upr][0]; }
  
  /** Return the \a i th constraint containing a univ parameter \a upr.
   *  \retval ctr - a value that satisfies 0 <= \a ctr < #nb_ctr().
   *  \pre 0 <= \a upr < #nb_upr(). <br> 0 <= \a i < #upr_nb_ctrs (\a upr). */
  inline int upr_ith_ctr(int upr, int i) const { return ent_adj[IBEX_UPR][upr][i+1]; }    

  /*--------------------------------------------------------------------------------*/
  /** Return the number of exist parameters in a constraint \a ctr.
   *  \retval n - a value that satisfies 0 <= \a n <= #nb_epr().
   *  \pre 0 <= \a ctr < #nb_ctr(). */
  inline int ctr_nb_eprs(int ctr) const { return ctr_adj[IBEX_EPR][ctr][0]; }

  /** Return the \a i th exist parameter in a constraint \a ctr. 
   *  \retval epr - a value that satisfies 0 <= \a epr < #nb_epr().
   *  \pre 0 <= \a epr < #nb_epr(). <br> 0 <= \a i < #ctr_nb_eprs (\a ctr). */
  inline int ctr_ith_epr(int ctr, int i) const { return ctr_adj[IBEX_EPR][ctr][i+1]; }

  /** Return the number of constraints containing an exist parameter \a epr.
   *  \pre 0 <= \a epr <  #nb_epr().
   *  \retval n - a value that satisfies 0 <= \a n <= #nb_ctr(). */
  inline int epr_nb_ctrs(int epr) const { return ent_adj[IBEX_EPR][epr][0]; }
  
  /** Return the \a i th constraint containing an exist parameter \a epr.
   *  \retval ctr - a value that satisfies 0 <= \a ctr < #nb_ctr().
   *  \pre 0 <= \a epr < #nb_epr(). <br> 0 <= \a i < #epr_nb_ctrs (\a epr). */
  inline int epr_ith_ctr(int epr, int i) const { return ent_adj[IBEX_EPR][epr][i+1]; }    

  /*--------------------------------------------------------------------------------*/
  /** Return the number of symbolic constants in a constraint \a ctr.
   *  \retval n - a value that satisfies 0 <= \a n <= #nb_syb().
   *  \pre 0 <= \a ctr < #nb_ctr(). */
  inline int ctr_nb_sybs(int ctr) const { return ctr_adj[IBEX_SYB][ctr][0]; }

  /** Return the \a i th symbolic constant in a constraint \a ctr. 
   *  \retval syb - a value that satisfies 0 <= \a syb < #nb_syb().
   *  \pre 0 <= \a syb < #nb_syb(). <br> 0 <= \a i < #ctr_nb_sybs (\a ctr). */
  inline int ctr_ith_syb(int ctr, int i) const { return ctr_adj[IBEX_SYB][ctr][i+1]; }

  /** Return the number of constraints containing a symbolic constant \a syb.
   *  \pre 0 <= \a syb <  #nb_syb().
   *  \retval n - a value that satisfies 0 <= \a n <= #nb_ctr(). */
  inline int syb_nb_ctrs(int syb) const { return ent_adj[IBEX_SYB][syb][0]; }
  
  /** Return the \a i th constraint containing a symbolic constant \a syb.
   *  \retval ctr - a value that satisfies 0 <= \a ctr < #nb_ctr().
   *  \pre 0 <= \a syb < #nb_syb(). <br> 0 <= \a i < #syb_nb_ctrs (\a syb). */
  inline int syb_ith_ctr(int syb, int i) const { return ent_adj[IBEX_SYB][syb][i+1]; }

  
  /*--------------------------------------------------------------------------------*/

  /** Initialize a propagation queue by inserting all the 
   *  possible pairs constraint/variable. */
  void init(CoupleQueue& q) const;

  /** Push in a propagation queue all the pairs constraint/variable impacted by the reduction
   *  of one given variable \a var. 
   *  \see #ibex::Propagation. */
  inline void propagate(CoupleQueue& q, int var) const { propagate(q,-1,var); }

  /** Push in a propagation queue all the pairs constraint/variable impacted by the reduction
   *  of a variable \a var by contractor \a ctr.
   *  \pre The variable \a var must appear in \a ctr. Otherwise the behaviour is undefined. 
   *  \see #ibex::Propagation. */
  void propagate(CoupleQueue& q, int ctr, int var) const;

  /** Display the internal structure (matrix & tables). 
   *  For debug purposes only. */
  friend std::ostream& operator<<(std::ostream& os, const Adjacency& a);

 private:

  int _nb_ctr;

  int nb_ent[4];

  int **adj_matrix[4];
  int **ctr_adj[4];
  int **ent_adj[4];
};

} // end namespace

#endif
