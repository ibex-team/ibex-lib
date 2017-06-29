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
#include <cassert>

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
 * \brief Agenda.
 *
 * An agenda is a fixed-size list of positive integers where each element can only appear once.
 * In particular:
 * - The "push" operation adds an element at the end of the list, if not present
 * - The "pop" operation retrieves and element at the beginning of the list
 */
class Agenda {

public:

	/**
	 * \brief Create the agenda.
	 *
	 * All elements will be inside the range [0,size-1].
	 */
	Agenda(int size) : size(size), _first(-1), last(-1) {
		table = new int[size];
		for (int i=0; i<size; i++) {
			table[i]=-1;
		}
	}

	/**
	 * \brief Duplicate the agenda.
	 * \param swap - If true: reverse the list.
	 */
	Agenda(const Agenda& a, bool swap=false) : size(a.size), _first(swap? a.last : a._first), last(swap? a._first : a.last) {
		table = new int[size];

		for (int i=0; i<size; i++) {
			if (a.table[i]==-1) table[i]=-1;
			else
				if (swap) {
					// note: -2 means "past-the-end"
					if (a.table[i]!=-2) table[a.table[i]]=i;
				} else table[i]=a.table[i];
		}
		if (swap && _first!=-1) table[last]=-2;
	}

	/**
	 * \brief Delete this.
	 */
	virtual ~Agenda() {
		delete[] table;
	}

	/**
	 * \brief Push an integer at the end.
	 */
	inline void push(int p) {
		assert(p>=0 && p<size);
		if (table[p]!=-1) return;

		if (_first==-1) _first = p;
		else table[last] = p;

		last = p;
		table[last] = -2; // past-the-end
	}

	/**
	 * \brief Push all integers of another agenda
	 *
	 * The ordering of a common subsequence of integers
	 * is the one of "this" agenda.
	 *
	 * \pre The \"size\" of the agenda must be less or equal
	 *      than the \"size\" of this.
	 */
	inline void push(const Agenda& a) {
		assert(a.size<=size);
		if (a.empty()) return;
		for (int i=a.first(); i!=a.end(); i=a.next(i)) {
			push(i);
		}
	}

	/**
	 * \brief Pop an integer from the beginning.
	 *
	 * \throw EmptyAgendaException if the agenda is empty.
	 */
	inline void pop(int& p)  {
		if (_first == -1) throw EmptyAgendaException();

		p = _first;

		int tmp = table[_first];
		table[_first] = -1;

		if (tmp == -2) {
			_first = -1;
		} else {
			_first = tmp;
		}
	}

	/**
	 * \brief First element
	 */
	inline int first() const {
		if (_first == -1) throw EmptyAgendaException();
		return _first;
	}

	/**
	 * \brief Element following x.
	 * \prec x must be in the list.
	 */
	inline int next(int x) const {
		assert(x>=0 && x<size && table[x]!=-1);
		return table[x];
	}

	/**
	 * \brief The past-the-end iterator
	 */
	inline int end() const {
		return -2;
	}

	/**
	 * \brief Remove all integers
	 */
	inline void flush() {
		int ignore;
		while (!empty()) pop(ignore);
	}

	/**
	 * \brief True iff the agenda is empty.
	 */
	inline bool empty() const {
		return _first==-1;
	}

	/**
	 * \brief The size defining the range of the agenda.
	 *
	 * All elements must be inside [0,size-1].
	 */
	const int size;

protected:
	int _first, last;
	int *table;
};


/**
 * \brief Streams out an agenda.
 */
std::ostream& operator<<(std::ostream& os, const Agenda& a);

/**
 * \ingroup tools
 * \brief Propagation agenda (agenda)
 *
 * This class has to be used instead of some other STL container to avoid
 * pushing twice the same element.
 * \sa #ibex::HyperGraph where propagation algorithms are defined.
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
