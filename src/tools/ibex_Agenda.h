/* ============================================================================
 * I B E X - Propagation Agenda
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_AGENDA_H__
#define __IBEX_AGENDA_H__

#include "ibex_Exception.h"
#include "ibex_HyperGraph.h"
#include "ibex_BitSet.h"

namespace ibex {

/**
 * \ingroup tools
 * \brief Empty agenda exception.
 *
 * Thrown when an element is retrieved from an empty agenda.
 */
class EmptyAgendaException : public Exception { };


/*
 * \ingroup tools
 * \brief Propagation agenda.
 */
class Agenda {

 public:

  Agenda(int size) : size(size), first(-1), last(-1) {
    table = new int[size];
    for (int i=0; i<size; i++) {
      table[i]=-1;
    }
  }

  virtual ~Agenda() {
    delete[] table;
  }

  inline void push(int p) {
    if (table[p]!=-1) return;

    if (first==-1) first = p;
    else table[last] = p;

    last = p;
    table[last] = first; // just to remember that (i,j) is set
  }

  inline void pop(int& p)  {
    if (first == -1) throw EmptyAgendaException();

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

  friend std::ostream& operator<<(std::ostream& os, const Agenda& q);

 protected:
  int size;
  int first, last;
  int *table;

  private:
  bool delete_it; // optim info
};

/**
 * \ingroup tools
 * \brief Propagation agenda (agenda)
 *
 * This class has to be used instead of the STL agenda class to avoid
 * pushing twice the same element.
 * \sa #ibex::Adjacency where propagation algorithms are defined.
*/
class ArcAgenda : Agenda {

 public:
  /** Create a propagation agenda for variable/constraint pairs.
   * \param nb_ctr - The number of constraints.
   * \param nb_var - The number of variables. */
  ArcAgenda(int nb_ctr, int nb_var) : Agenda(nb_ctr*nb_var), nb_ctr(nb_ctr), nb_var(nb_var) {
  }

  /** Number of constraints. */
  const int nb_ctr;

  /** Number of variables. */
  const int nb_var;

  /** Push a pair variable/constraint into the propagation agenda.
   *  \pre 0 <= \a ctr < #nb_ctr. <br> 0 <= \a var < #nb_var. */
  inline void push(int ctr, int var) {
    Agenda::push(ctr*nb_var+var);
  }

  /** Pop a pair variable/constraint from the propagation agenda.
   * \param ctr - (out) the popped constraint
   * \param var - (out) the popped variable */
  inline void pop(int& ctr, int& var) {
    int p;
    Agenda::pop(p);
    ctr = p / nb_var;
    var = p % nb_var;
  }

  /** Flush the propagation agenda. */
  inline void flush() {
    Agenda::flush();
  }

  /** Return true if the propagation agenda is empty. */
  inline bool empty() const {
    return Agenda::empty();
  }

  /**
   * \brief Initialize the agenda from an hypergraph.
   *
   * Initialize the agenda by inserting all the
   * arcs of \a g.
   */
  void init(const HyperGraph& g);

  /**
   * \brief Push in the agenda all the arcs of a graph \a g impacted by
   *  the reduction of one given variable \a var.
   *  \see #ibex::Propagation.
   */
  inline void propagate(const HyperGraph& g, int var) { propagate(g,-1,var); }

  /**
   * \brief Push in the agenda all the arcs of a graph \a g impacted by
   *  the reduction of variables set in the mask.
   *
   *  \see #ibex::Propagation.
   */
  void propagate(const HyperGraph& g, const BitSet& m);


  /**
   * \brief Push in the agenda all the arcs of a graph \a g impacted by the reduction
   *  of a given arc (\a var, \a ctr).
   *
   *  \pre The variable \a var must appear in \a ctr. Otherwise the behaviour is undefined.
   *  \see #ibex::Propagation.
   */
  void propagate(const HyperGraph& g, int ctr, int var);

  /**
   * \brief Stream out the agenda.
   */
  friend std::ostream& operator<<(std::ostream& os, const ArcAgenda& q);
};

} // namespace ibex
#endif // __IBEX_AGENDA_H__
