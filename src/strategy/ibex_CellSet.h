//============================================================================
//                                  I B E X                                   
// File        : ibex_CellSet.h
// Author      : Ignacio Araya
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 28, 2014
// Last Update : Oct 28, 2014
//============================================================================

#ifndef __IBEX_CELL_SET_H__
#define __IBEX_CELL_SET_H__

#include "ibex_CellBuffer.h"
#include <set>

namespace ibex {

/** \ingroup strategy
 *
 * \brief Data required for the Optimizer
 */
class CellBS : public Backtrackable {
public:
	/**
	 * \brief Constructor for the root node (followed by a call to init_root).
	 */
	CellBS();

	/**
	 * \brief Delete *this.
	 */
	~CellBS();

	/**
	 * \brief Duplicate the structure into the left/right nodes
	 */
	std::pair<Backtrackable*,Backtrackable*> down();

    /**unique identifier for comparisons*/
    int id;
	/** lower bound for the box */
	double lb;
	/** lower bound for the box */
	double ub;
	/** depth of the node **/
	int depth;

protected:

	CellBS(const CellBS&);
};


struct minLB {
  bool operator() (const Cell* c1, const Cell* c2) const
  {  
	  if(c1->get<CellBS>().lb < c2->get<CellBS>().lb) return true;
	  else if(c1->get<CellBS>().lb == c2->get<CellBS>().lb && c1->get<CellBS>().depth < c2->get<CellBS>().depth) return true;
	  else return(c1->get<CellBS>().lb == c2->get<CellBS>().lb && c1->get<CellBS>().depth == c2->get<CellBS>().depth && c1->get<CellBS>().id > c2->get<CellBS>().id);
  }
};

struct minLB_lbub {
  bool operator() (const Cell* c1, const Cell* c2) const
  {  
	  if(c1->get<CellBS>().lb < c2->get<CellBS>().lb) return true;
	  else if(c1->get<CellBS>().ub < c2->get<CellBS>().ub && c1->get<CellBS>().lb == c2->get<CellBS>().lb) return true;
	  else if(c1->get<CellBS>().ub == c2->get<CellBS>().ub && c1->get<CellBS>().lb == c2->get<CellBS>().lb && c1->get<CellBS>().depth > c2->get<CellBS>().depth) return true;
	  else return(c1->get<CellBS>().lb == c2->get<CellBS>().lb && c1->get<CellBS>().ub == c2->get<CellBS>().ub && c1->get<CellBS>().depth == c2->get<CellBS>().depth && c1->get<CellBS>().id > c2->get<CellBS>().id);
  }
};

struct minUB {
  bool operator() (const Cell* c1, const Cell* c2) const
  {  
	  if(c1->get<CellBS>().ub < c2->get<CellBS>().ub) return true;
	  else if(c1->get<CellBS>().lb < c2->get<CellBS>().lb && c1->get<CellBS>().ub == c2->get<CellBS>().ub) return true;
	  else if(c1->get<CellBS>().ub == c2->get<CellBS>().ub && c1->get<CellBS>().lb == c2->get<CellBS>().lb && c1->get<CellBS>().depth > c2->get<CellBS>().depth) return true;
	  else return(c1->get<CellBS>().lb == c2->get<CellBS>().lb && c1->get<CellBS>().ub == c2->get<CellBS>().ub && c1->get<CellBS>().depth == c2->get<CellBS>().depth && c1->get<CellBS>().id > c2->get<CellBS>().id);
  }
};

//same as a stack
struct maxID {
  bool operator() (const Cell* c1, const Cell* c2) const
  {  
	  return (c1->get<CellBS>().id > c2->get<CellBS>().id);
  }
};

//same as a stack
struct maxDepth {
  bool operator() (const Cell* c1, const Cell* c2) const
  {  
	  if (c1->get<CellBS>().depth > c2->get<CellBS>().depth) return true;
	  else if(c1->get<CellBS>().lb < c2->get<CellBS>().lb && c1->get<CellBS>().depth == c2->get<CellBS>().depth) return true;
	  else return(c1->get<CellBS>().lb == c2->get<CellBS>().lb && c1->get<CellBS>().depth == c2->get<CellBS>().depth && c1->get<CellBS>().id > c2->get<CellBS>().id);
  }
};

struct deepFirst {
  bool operator() (const Cell* c1, const Cell* c2) const
  {  
	  if (c1->get<CellBS>().depth > c2->get<CellBS>().depth) return true;
	  else return(c1->get<CellBS>().depth == c2->get<CellBS>().depth && c1->get<CellBS>().id > c2->get<CellBS>().id);
  }
};

struct minLB4ibf {
  bool operator() (const Cell* c1, const Cell* c2) const
  {  
	  if(c1->get<CellBS>().lb < c2->get<CellBS>().lb) return true;
	  else if(c1->get<CellBS>().lb == c2->get<CellBS>().lb && c1->get<CellBS>().depth > c2->get<CellBS>().depth) return true;
	  else return(c1->get<CellBS>().lb == c2->get<CellBS>().lb && c1->get<CellBS>().depth == c2->get<CellBS>().depth && c1->get<CellBS>().id > c2->get<CellBS>().id);
  }
};

/** \ingroup strategy
 *
 * \brief Cell Set.
 *
 * For depth-first search. \see #CellBuffer
 */
template <class T>
class CellSet : public CellBuffer {
 public:
  /** Flush the buffer.
   * All the remaining cells will be *deleted* */
  void flush();

  /** Return the size of the buffer. */
  int size() const;

  /** Return true if the buffer is empty. */
  bool empty() const;

  /** push a new cell on the stack. */
  void push(Cell* cell);

  /** Pop a cell from the stack and return it.*/
  Cell* pop();

  /** Return the next box (but does not pop it).*/
  Cell* top() const;
  
  /** Return a random box with lb<=max_lb */
  Cell* random_pop(double max_lb);

  Cell* erase(Cell* c);

/**
   * Removes (and deletes) from the heap all the cells
   * with a cost greater than \a loup. y is the variable related to the objective function
   */
  void contract(double loup, int y, bool remove=false);

 private:
  /* Set of cells */
  std::set<Cell*,T> cset;
  
};


/** \ingroup strategy
 *
 * \brief Cell BiCriteria.
 *
 * For best-first switching among two criteria (minLB and other one). \see #CellBuffer
 */
template <class T2>
class CellBiCriteria : public CellBuffer {
 public:
 
  CellBiCriteria(double prob_maxlb=0.5);
 
  /** Flush the buffer.
   * All the remaining cells will be *deleted* */
  void flush();

  /** Return the size of the buffer. */
  int size() const;

  /** Return true if the buffer is empty. */
  bool empty() const;

  /** push a new cell on the stack. */
  void push(Cell* cell);

  /** Pop a cell from the stack and return it.*/
  Cell* pop();

  /** Return the next box (but does not pop it).*/
  Cell* top() const;
  
  //~ Cell* erase(Cell* c);

/**
   * Removes (and deletes) from the heap all the cells
   * with a cost greater than \a loup. y is the variable related to the objective function
   */
  void contract(double loup, int y, bool remove=false);

 private:
  /* Set of cells */
  std::set<Cell*,minLB_lbub> cset;
  std::set<Cell*,T2> cset2;
  double prob_maxlb;
};




} // end namespace ibex
#endif // __IBEX_CELLSET_H__
