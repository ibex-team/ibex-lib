//============================================================================
//                                  I B E X                                   
// File        : Bisector
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 8, 2012
// Last Update : May 8, 2012
//============================================================================

#ifndef __IBEX_BISECTOR_H__
#define __IBEX_BISECTOR_H__

#include "ibex_Cell.h"
#include <utility>

namespace ibex {

/**
 * \defgroup bisector Bisectors
 */

/**
 * \ingroup bisector
 *
 * \brief Generic bisector
 *
 * Base class of all bisectors. A bisector is an operator used by a strategy
 * (like the \link ibex::Solver solver \endlink) to make a choice point.
 * An implementation of this class must provide a #bisect(const IntervalVector& box) function.
 *
 * Bisectors also handle a precision: when the domain of a variable is less than
 * its corresponding precision, the bisector skips this variable.
 * When all the variables are skipped, the bisector throws a NoBisectableVariableException.
 * *
 * This precision allows to control the depth of the search (paver/solver/optimizer).
 * The boundary of the set to be described by the search is usually associated to the boxes
 * that were not bisectable. So, the precision of the boundary is directly related to the
 * \link ibex::Bsc::prec precision \endlink parameter of this class.
 *
 */
class Bsc {

public:

	/**
	 * \brief Build a bisection with the given precision
	 *
	 * The precision is the same for all the variables. Therefore the bisector
	 * throws a NoBisectableVariableException iff the maximal diameter of the
	 * box is less than \a prec.
	 */
	Bsc(double prec);

	/**
	 * \brief Build a bisection with heterogeneous precision
	 *
	 * This predicates works for ceil.size() variables.
	 *
	 * The precision vector gives the threshold precision for each variable.
	 * The bisector throws a NoBisectableVariableException if, for every i,
	 * the diameter of box[i] is less than prec[i].
	 */
	Bsc(const Vector& prec);

	/**
	 * \brief Delete *this.
	 */
	virtual ~Bsc() { }

	/**
	 * \brief Bisect the current cell and return the result.
	 *
	 * The information in a cell is e.g., used to get the last bisected variable in case
	 * the bisector is called by a solver/paver (e.g., RoundRobin).
	 * Implementation is <b>optional</b>. By default, this function call bisect(cell.box).
	 * See #bisect(const IntervalVector&).
	 */
	std::pair<Cell*,Cell*> bisect_cell(const Cell& cell);

	/**
	 * \brief Bisect the current box and return the result.
	 */
	virtual std::pair<IntervalVector,IntervalVector> bisect(const IntervalVector& box)=0;

	/**
	 * Allows to add the backtrackable data required
	 * by this bisector to the root cell before a
	 * strategy is executed.<br>
	 * By default: add information on the last bisected variable. See #ibex::BisectedVar.
	 */
	virtual void add_backtrackable(Cell& root);

	/**
	 * \brief Default ratio (0.45)
	 */
	static double default_ratio();

	/**
	 * \brief Return true if the precision is the same for all variables.
	 */
	bool uniform_prec() const;

	/**
	 * \brief Precision for the ith variable
	 */
	double prec(int i) const;

	/**
	 * \brief True iff box[i] is greater than the required precision
	 *        and bisectable.
	 */
	bool too_small(const IntervalVector& box, int i) const;


private:
	/*
	 * The precision values
	 *
	 * If the size of _prec is 1, this means the same precision prec[0]
	 * applies for every variable.
	 */
	const Vector _prec;
};


/** \ingroup bisector
 *
 * \brief Last bisected variable (used by RoundRobin, CtcPropag, etc.)
 */
class BisectedVar : public Backtrackable {
public:
	BisectedVar() : var(-1) { }

	BisectedVar(int x) : var(x) { }

	std::pair<Backtrackable*,Backtrackable*> down() {
		return std::pair<Backtrackable*,Backtrackable*>(new BisectedVar(var),new BisectedVar(var));
	}

	int var;

protected:

	explicit BisectedVar(const BisectedVar& e) : var(e.var) { }
	Backtrackable* copy() const { return new BisectedVar(*this);};

};


/*============================================ inline implementation ============================================ */


inline std::pair<Cell*,Cell*> Bsc::bisect_cell(const Cell& cell) {
	std::pair<IntervalVector,IntervalVector> boxes=this->bisect(cell.box);
	return cell.bisect(boxes.first,boxes.second);
}

inline bool Bsc::uniform_prec() const {
	return _prec.size()==1;
}

inline double Bsc::prec(int i) const {
	return uniform_prec() ? _prec[0]  // same precision for all variables
	                      : _prec[i]; // specific precision
}

inline bool Bsc::too_small(const IntervalVector& box, int i) const {
	return (box[i].diam()<prec(i)
		|| !box[i].is_bisectable() // test for avoiding to bisect infinite intervals BNE
		   );
}

} // end namespace ibex
#endif // __IBEX_BISECTOR_H__
