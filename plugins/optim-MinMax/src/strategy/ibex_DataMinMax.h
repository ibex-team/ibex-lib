//============================================================================
//                                  I B E X                                   
// File        : ibex_DataMinMax.h
// Author      : Dominique Monnet, Jordan Ninin
// License     : See the LICENSE file
// Created     : Oct 1, 2016
//============================================================================

#ifndef __IBEX_DATA_MINMAX__
#define __IBEX_DATA_MINMAX__

#include "ibex_Backtrackable.h"
#include "ibex_Interval.h"
#include "ibex_DoubleHeap.h"
#include "ibex_CellCostFunc.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Data required for the Optimizer
 */
class DataMinMax : public Backtrackable {

public:
	/**
	 * \brief Constructor for the root node (followed by a call to init_root).
	 */
	DataMinMax();

	/**
	 * \brief Delete *this.
	 */
	~DataMinMax();

	/**
	 * \brief Duplicate the structure into the left/right nodes
	 */
	std::pair<Backtrackable*,Backtrackable*> down();


    /**
     * Enclosure of maximum of the objective function
     */
    Interval fmax;

	/**
	 * \brief Casado criterion
	 *
	 * Constraint factor of the current box : between 0 infeasible and 1 for all constraints satisfied.
	 */
	double pu;

	/**
	 * y_heap inherited from father of box
	 */
    DoubleHeap<Cell>* y_heap;


	/**
	 * Cost function of the heap to store the element of the light solver
	 */
	static CellCostMaxPFub y_heap_costf1;
	static CellCostPFlb y_heap_costf2;

protected:


	/**
	 * \brief Constructor by copy.
	 */
	explicit DataMinMax(const DataMinMax& e);

	/**
	 * \brief Create a copy
	 */
	Backtrackable* copy() const { return new DataMinMax(*this);};

};


class CellCostFmaxlb : public CellCostFunc { // element are sorted from the lowest lb of the evaluation of the objective function to the greatest
public:

	CellCostFmaxlb();

	/**
	 * \brief Return the cost associated to "data"
	 */
	virtual double cost(const Cell& elem) const;

	/**
	 * \brief Add backtrackable data required by this cost function
	 *
	 * This function is called for the root cell (before a
	 * strategy is executed).
	 *
	 * Does nothing by default.
	 */
     virtual void add_backtrackable(Cell& root);

};

class CellCostmaxFmaxub : public CellCostFunc { // element are sorted from the lowest lb of the evaluation of the objective function to the greatest
public:

	CellCostmaxFmaxub();
	/**
	 * \brief Return the cost associated to "data"
	 */
	virtual double cost(const Cell& elem) const;

	/**
	 * \brief Add backtrackable data required by this cost function
	 *
	 * This function is called for the root cell (before a
	 * strategy is executed).
	 *
	 * Does nothing by default.
	 */
     virtual void add_backtrackable(Cell& root);

};

class CellCostFmaxub : public CellCostFunc { // element are sorted from the lowest lb of the evaluation of the objective function to the greatest
public:

	CellCostFmaxub();
	/**
	 * \brief Return the cost associated to "data"
	 */
	virtual double cost(const Cell& elem) const;

	/**
	 * \brief Add backtrackable data required by this cost function
	 *
	 * This function is called for the root cell (before a
	 * strategy is executed).
	 *
	 * Does nothing by default.
	 */
     virtual void add_backtrackable(Cell& root);

};

} // end namespace ibex
#endif // __IBEX_DATA_MINMAX_H__
