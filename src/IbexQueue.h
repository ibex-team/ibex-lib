/*---------------------------------------------------------------------------------
 * 
 * Propagation queue
 * -----------------
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

#ifndef _IBEX_QUEUE_H
#define _IBEX_QUEUE_H

#include "IbexException.h"
#include <iostream>

namespace ibex {

/**\defgroup miscgroup Other structures*/
/*@{*/

/** 
 * \brief Empty queue exception.
 *
 * Thrown when an element is retreived from an empty queue.
 * \author Gilles Chabert
 * \date March 2007
 */
class EmptyQueueException : public IbexException { };


/*
 * \brief Propagation queue.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class Queue {
  
 public:
  
  Queue(int size) : size(size), first(-1), last(-1) {
    table = new int[size];
    for (int i=0; i<size; i++) {
      table[i]=-1;
    }
  }
  
  virtual ~Queue() { 
    delete[] table;
  }

  inline void push(int p) {
    if (table[p]!=-1) return; 

    if (first==-1) first = p;
    else table[last] = p;

    last = p;
    table[last] = first; // just to remember that (i,j) is set
  }
  
  inline void pop(int& p) throw(EmptyQueueException) {
    if (first == -1) throw EmptyQueueException();

    p = first;

    int tmp = table[first];
    table[first] = -1;

    if (first == tmp) {
      first = -1;
    } else {
      first = tmp;
      table[last] = first; // just to remember that table[last] is set
    }
  }

  inline void flush() {
    int ignore;
    while (!empty()) pop(ignore);
  }

  inline bool empty() const { 
    return first==-1; 
  }

  friend std::ostream& operator<<(std::ostream& os, const Queue& q);

 protected:
  int size;
  int first, last;
  int *table;  

  private:
  bool delete_it; // optim info 
};

/** 
 * \brief Propagation queue (agenda)
 *
 * This class has to be used instead of the STL queue class to avoid 
 * pushing twice the same element. 
 * \sa #ibex::Adjacency where propagation algorithms are defined. 
 *
 * \author Gilles Chabert
 * \date March 2007
*/
class CoupleQueue : Queue {
  
 public:
  /** Create a propagation queue for variable/constraint pairs.
   * \param nb_ctr - The number of constraints. 
   * \param nb_var - The number of variables. */
  CoupleQueue(int nb_ctr, int nb_var) : Queue(nb_ctr*nb_var), nb_ctr(nb_ctr), nb_var(nb_var) { 
  }
    
  /** Number of constraints. */
  const int nb_ctr;

  /** Number of variables. */
  const int nb_var;

  /** Push a pair variable/constraint into the propagation queue. 
   *  \pre 0 <= \a ctr < #nb_ctr. <br> 0 <= \a var < #nb_var. */
  inline void push(int ctr, int var) { 
    Queue::push(ctr*nb_var+var); 
  }

  /** Pop a pair variable/constraint from the propagation queue.
   * \param ctr - (out) the popped constraint 
   * \param var - (out) the popped variable */
  inline void pop(int& ctr, int& var) { 
    int p;
    Queue::pop(p);    
    ctr = p / nb_var;
    var = p % nb_var;
  }

  /** Flush the propagation queue. */
  inline void flush() { 
    Queue::flush(); 
  }

  /** Return true if the propagation queue is empty. */
  inline bool empty() const { 
    return Queue::empty(); 
  }
  
  /** Stream out the queue. */
  friend std::ostream& operator<<(std::ostream& os, const CoupleQueue& q);
};

/*@}*/

} // end namespace

#endif
