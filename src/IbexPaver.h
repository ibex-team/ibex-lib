/*---------------------------------------------------------------------------------
 * 
 * Default  Paver
 * ---------------------------
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

#ifndef _IBEX_PAVER_H
#define _IBEX_PAVER_H

#include <iostream>
#include <vector>
#include <stack>
#include "IbexException.h"
#include "IbexEvaluator.h" // just for EmptyBoxException (may be removed then)
#include "IbexCell.h"
#include "IbexContractor.h"
#include "IbexContractorList.h"
#include "IbexBisector.h"
/* #include "IbexInnerContractor.h" */
#include "IbexTimer.h"
#include "IbexPaving.h"
#include "IbexCellStack.h"

namespace ibex {

/** \ingroup ctcgroup
 *
 * \brief Precision contractor.
 *
 * Stop criterion for \link #ibex::Paver pavers \endlink. This contractor usually controls the precision 
 * of solutions of a solver: when the interval diameter of all the variables in the current box is 
 * lower than \link Precision::ceil ceil \endlink, the box is entirely contracted (unfeasibility exception). 
 * Solutions are therefore usually associated to this contractor in a solving process.
 * Precision of solutions is directly related to the parameter \link Precision::ceil ceil \endlink.
 *
 * \author Gilles Chabert
 * \date January 2008
 */
class Precision : public Contractor {
 public:
  /** Create a bisection stopping contractor of ceilision \a ceil. */
  Precision(Space& space, REAL ceil) : Operator(space), ceil(ceil) { }

  /** Copy constructor. */
  Precision(const Precision& p) : Operator(p.space), Contractor(p), ceil(p.ceil) { }

  /** Implement the generic contractor copy. */
  virtual Contractor* copy() const {
    return new Precision(*this);
  }

  /* Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this); 
  }

  /** Contract. */
  void contract();

  /** The precision. */
  const REAL ceil;
};

/** 
 * \brief Paver.
 * 
 * This class implements a classical branch & bound algorithm (with depth-first search). 
 * It allows to apply alternatively a list of contractors and a bisector until the stack 
 * of boxes gets empty.
 * See the description of this algorithm in <a href="www.references.html#cha09">[cha09]</a>
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class Paver {
  public:

  /** Create a "standard" depth-first paver with a contractor and a bisector. 
   * The space of the paver is a reference to the space of the contractor in argument.
   * The precision is \a prec. */
  Paver(const Contractor& _ctc, const Bisector& bsc, REAL prec=Bisector::default_prec);
  
  /** Create a depth-first search paver on a given space with (a copy of) a list of contractor, 
   * a bisector and (optionnaly) a cell buffer. By default, the cell buffer is a stack. */
  Paver(Space& space, const ContractorList& ctc, const Bisector& bsc, CellBuffer& cells=(default_buffer_assign=true, *new CellStack()));

  /** Deletes this instance. */
  ~Paver();

  /** (Re)initializes the exploration. 
   * \see #next_box() and #explore(). */
  void restart();

  /** Continues exploration until another box gets entirely contracted.
   * The method returns the number of the last contractor being called before the box
   * gets empty.
   *
   * \return the contractor number or -1 if the exploration is over.
   *
   * \throw #ibex::OverflowException - if the total paving size exceeds #capacity.
   * \throw #ibex::TimeOutException - if the number of cells created exceeds #cell_limit.
   * \sa #restart(). */
  virtual int next_box();

  /** Calls all the contractors until the fixpoint is reached
   * \return the contractor number that entirely emptied the box, if any. Otherwise, return
   *         -1 (the box is non empty and the fixpoint is reached). */
  int contract(Cell& c);

  /** Performs a complete exploration of the search space (with the list of contractors given in argument
   * of the constructors and calculates the time spent.
   *
   * \note This methods automatically calls #restart(): if an exploration is in hand, it is
   * abandoned.
   * \return the total time spent in seconds.
   * \see #next_box() for details about exceptions. */
  double explore();

  /** Displays on standard output a report of the last exploration.
   *
   * Information provided: 
   * <ul><li>number of boxes successfully contracted by each contractor.
   *     <li>total number of cells created during the exploration
   *     <li>total running time
   * </ul> */
  virtual void report();

  /** Return the paving of the last exploration (NULL if #solver_mode was true) */
  const PavingNode& result() { return *paving; }

  /*----------------------------------------------------------------------------------*/
  /*                                        PARAMETERS                                */
  /*----------------------------------------------------------------------------------*/

  /** Capacity of the solver, i.e., the total number of boxes that can be stored.
   * This parameter allows to bound space complexity.
   * The value can be fixed by the user. By default, it is -1 (no limit). 
   * Combined with \a solver_mode, this field allows to bound the number of solutions. */
  long capacity;
   

  /** Maximal number of cells created by the solver.
   * This parameter allows to bound time complexity.
   * The value can be fixed by the user. By default, it is -1 (no limit). */
  long cell_limit;

/** Maximum cpu time used by the solver.
   * This parameter allows to bound time complexity.
   * The value can be fixed by the user. By default, it is -1 (no limit). */

  double time_limit;


  /** A flag for printing the trace. If set, the top of the stack is printed on the standard
   * output each time a new cell is created. Default value is \c false. */
  bool trace;

  /** If true, the paver will loop on contractors until the fixpoint
   * is reached, for each cell. Otherwise, contractors are only called once.
   * Default value is  \c true. */
  bool ctc_loop;

  /*----------------------------------------------------------------------------------*/

  /** Return the total number of cells created since the last call to #restart() 
   * (i.e., since the begining of the last exploration. Each cell corresponds to one 
   * node in the search tree. */
  long nb_cells() { return _nb_cells; }

  /** Return the number of boxes "accepted" (contracted successfully) by contractor n°\a ctc since the last call to #restart(). */
  long nb_boxes(int ctc) const { return accepted[ctc].size(); }

  /** Return the \a ith "accepted" (contracted successfully) by contractor n°\a ctc since the last call to #restart(). */
  const INTERVAL_VECTOR& box(int ctc, int i) const { return accepted[ctc][i]->box; }

  /** Return the \a ith "rejected" (i.e., uncontracted) part of the \a ith box contracted by contractor n°\a ctc.
   * If the latter is a leaf, the function simply returns a reference to it. */
  const INTERVAL_VECTOR& rej(int ctc, int i) const;

  /** This function is only effective if #solver_mode is on.
   * \c select(i) will make the subsequent calls to \c explore() storing
   * the leaves of the subpaving of contractor n°i. 
   * By default, all the contractors except instances of #ibex::Precision are not selected.
   * \see #solver_mode.*/
  void select(int ctc) { selected_ctc[ctc]=true; }

  /** This function is only effective if #solver_mode is on.
   * \c unselect(i) will make the subsequent calls to \c explore() discarding
   * the whole subpaving of contractor n°i (this will save memory).
   * By default, all the contractors except instances of #ibex::Precision are not selected.
   * \see #solver_mode.*/
  void unselect(int ctc) { selected_ctc[ctc]=false; }

  /** In this mode, the paver behabes as a standard solver:
   * this means that pavings are not generated by \c explore(): only the leaves
   * of the subpavings related to some "selected" contractors (typically,
   * the precision or certification contractor) are kept. The solver mode allows
   * to save memory.
   *
   * To select a contractor, use #select(int). 
   * 
   * If contractor n°i is selected, a call to \c nb_boxes(i) will return the number
   * of leaves, and \c box(i,j) will return the jth leaf of contractor n°i.
   * If the contractor n°i is not selected, then a call to \c nb_boxes(i) will return 0.
   *
   * You can directly set this member to \c false/\c true to switch
   * from paver mode to solver mode respectively. Default value is \c false. */
  bool solver_mode;

  /** Callback (e.g., for displaying nodes online). This callback
    * is invoked each time a new contractor node is created,
    * provided that it is not NULL and #solver_mode is \c false.
    * Default value is \c null (no external call is made). */
  void (*new_ctc_node_func)(const ContractorNode&);

  /** Interrupts the paving process. 
   * This function can be called from the callback function #new_ctc_node_func,
   * leaving the paver in a consistent state. This allows to interrupt the paving process.
  */
  void interrupt() { _interrupt = true; }


  /** Space */
  Space& space;
  
  /** Number of contractors */
  const int nb_ctc;             

  /* Remember running time of the last exploration */
  double time;               
  
 protected:
  /* Contractors. */
  //Contractor** ctc;
  ContractorList ctc;

  /* Bisector. */
  Bisector* bsc;

  /* Cell buffer. */
  CellBuffer& cells;

 protected:

  Paver(const Paver&); // forbidden (or... to be implemented?)

  /** Check the number of boxes stored in the paving */
  inline void check_capacity() throw(OverflowException) {
    if (capacity!=-1 && accepted_size>capacity) throw OverflowException();
  }
  
  /** Check the number of cells created and the time spent */
  virtual void check_limits();

  /** Prints a cell. This method is called if #trace is set. 
   * The box and the last bisected variable are output. */
  void show_cell(const Cell& c) { 
    cout << c.box << "   last-split=" << c.last_split_var; 
  }

  void show_stack();

  BacktrackableMap m;

  long _nb_cells;
  
  /* Current paving.
   * This member is only used in "paver" mode. */    
  PavingNode* paving;

  /* if selected_ctc[i]=true, leaves related to ctc n°i are stored. 
   * This member is only used in "solver" mode. */
  bool* selected_ctc;

  /* nodes accepted by contractors */
  vector<ContractorNode*> *accepted;

  int accepted_size;
 
   /* number of screens in the trace */
  int screen; 

  /* interrupt status */
  bool _interrupt;

  /** empty box (see rej() */
  INTERVAL_VECTOR emptybox;

  bool standalone_buffer;

  //static CellStack default_buffer;
  static bool default_buffer_assign;
};

/*@}*/

} // end namespace

#endif
