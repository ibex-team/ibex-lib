//============================================================================
//                                  I B E X                                   
// File        : ibex_CellCostFunc.h
// Author      : Jordan Ninin, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Last Update : Mar 04, 2015
//============================================================================

#ifndef __IBEX_CELL_COST_FUNC__
#define __IBEX_CELL_COST_FUNC__

#include "ibex_System.h"
#include "ibex_SharedHeap.h"
#include "ibex_Cell.h"

namespace ibex {

// -----------------------------------------------------------------------------------------------------------------------------------

/**
 * \ingroup strategy
 *
 * \brief Root class of cell cost functions
 */
class CellCostFunc : public CostFunc<Cell> {

public:

	CellCostFunc(bool depends_on_loup);

	/**
	 * \brief Criteria implemented for a heap in optimization
	 *
	 * - LB for the first one
	 * - another for the second one among UB, C3, C5, C7, PU, PF, PF_LB, PF_UB.
	 */
	typedef enum {LB,UB,C3,C5,C7,PU,PF_LB, PF_UB} criterion;

	/**
	 * \brief Get the cost function from a criterion.
	 */
	static CellCostFunc* get_cost(criterion crit, int goal_var);

	/**
	 * \brief Store the new "loup" obtained by the optimizer, if used.
	 *
	 * Does nothing (by default).
	 */
       virtual void set_loup(double /*lb*/) { }

	/**
	 * \brief Add backtrackable data required by this cost function
	 *
	 * This function is called for the root cell (before a
	 * strategy is executed).
	 *
	 * Does nothing by default.
	 */
       virtual void add_backtrackable(Cell& /*root*/) { }

	/**
	 * \brief Set data in OptimData in the cell
	 *
	 * This function is called right after "contract_and_bound" in the Optimizer.
	 *
	 * The data required depends on the cost function.
	 *
	 * Does nothing (by default).
	 */
       virtual void set_optim_data(Cell& /*c*/, System& /*sys*/) { }

	/**
	 * If the cost depends on the loup.
	 */
	bool depends_on_loup;

};

/**
 * \ingroup strategy
 *
 * \brief Cell Heap based on lower bound criterion
 */
class CellCostVarLB: public CellCostFunc  {
public:

	CellCostVarLB(int goal_var) ;

	/** The "cost" of a element. */
	virtual double cost(const Cell& c) const;


private:
	/** Index of the objective variable. */
	const int goal_var;
};
// -----------------------------------------------------------------------------------------------------------------------------------

/**
 * \ingroup strategy
 *
 * \brief Cell Heap based on upper bound criterion
 */
class CellCostVarUB: public CellCostFunc  {
public:

	CellCostVarUB(int ind_var) ;

	/** The "cost" of a element. */
	virtual double cost(const Cell& c) const;


private:
	/** Index of the objective variable. */
	const int goal_var;
};
// -----------------------------------------------------------------------------------------------------------------------------------

/**
 * \ingroup strategy
 *
 * \brief Cost-based Cell Heap
 */
class CellCostC3: public CellCostFunc {
public:

	/**
	 * TODO: what is this "lb" for?
	 */
	CellCostC3(double lb= NEG_INFINITY) ;

	/**
	 * \brief Contracts the heap.
	 *
	 * Removes (and deletes) from the heap all the elements
	 * with a cost greater than \a lb.
	 */
	virtual void set_loup(double lb) { loup = lb; }

	/** The "cost" of a element. */
	virtual double cost(const Cell& c) const;

	/**
	 * \brief Add OptimData
	 */
	virtual void add_backtrackable(Cell& root);

	/**
	 * \brief Set "pf" in OptimData in the cell
	 */
	virtual void set_optim_data(Cell& c, System& sys);
private:

	/** the lower upper bound. */
	double loup;

};
// -----------------------------------------------------------------------------------------------------------------------------------


class CellCostC5: public CellCostFunc {
public:

	/**
	 * TODO: what is this "lb" for?
	 */
	CellCostC5(double lb= NEG_INFINITY) ;

	/**
	 * \brief Contracts the heap.
	 *
	 * Removes (and deletes) from the heap all the elements
	 * with a cost greater than \a lb.
	 */
	virtual void set_loup(double lb) { loup = lb; }

	/** The "cost" of a element. */
	virtual double cost(const Cell& c) const;

	/**
	 * \brief Add OptimData
	 */
	virtual void add_backtrackable(Cell& root);

	/**
	 * \brief Set "pf" and "pu" in OptimData in the cell
	 */
	virtual void set_optim_data(Cell& c, System& sys);
private:

	/** the lower upper bound. */
	double loup;
};
// -----------------------------------------------------------------------------------------------------------------------------------

class CellCostC7: public CellCostFunc {
public:

	/**
	 * TODO: what is this "lb" for?
	 */
	CellCostC7(int ind_var, double lb=NEG_INFINITY) ;

	/**
	 * \brief Contracts the heap.
	 *
	 * Removes (and deletes) from the heap all the elements
	 * with a cost greater than \a lb.
	 */
	virtual void set_loup(double lb) { loup = lb; }

	/** The "cost" of a element. */
	virtual	double cost(const Cell& c) const;

	/**
	 * \brief Add OptimData
	 */
	virtual void add_backtrackable(Cell& root);

	/**
	 * \brief Set "pf" and "pu" in OptimData in the cell
	 */
	virtual void set_optim_data(Cell& c, System& sys);
private:

	/** the lower upper bound. */
	double loup;

	/** Index of the objective variable. */
	const int goal_var;
};

// -----------------------------------------------------------------------------------------------------------------------------------

class CellCostPU: public CellCostFunc {
public:
	CellCostPU();

	/** The "cost" of a element. */
	virtual	double cost(const Cell& c) const;

	/**
	 * \brief Add OptimData
	 */
	virtual void add_backtrackable(Cell& root);

	/**
	 * \brief Set "pf" in OptimData in the cell
	 */
	virtual void set_optim_data(Cell& c, System& sys);
};


class CellCostPFlb: public CellCostFunc {
public:
	CellCostPFlb();

	/**
	 * \brief Add OptimData
	 */
	virtual void add_backtrackable(Cell& root);

	/**
	 * \brief Set "pf" in OptimData in the cell
	 */
	virtual void set_optim_data(Cell& c, System& sys);

	/** The "cost" of a element. */
	virtual	double cost(const Cell& c) const;

};

// -----------------------------------------------------------------------------------------------------------------------------------


class CellCostPFub: public CellCostFunc {
public:
	CellCostPFub();

	/**
	 * \brief Add OptimData
	 */
	virtual void add_backtrackable(Cell& root);

	/**
	 * \brief Set "pf" in OptimData in the cell
	 */
	virtual void set_optim_data(Cell& c, System& sys);

	/** The "cost" of a element. */
	virtual	double cost(const Cell& c) const;

};

} // end namespace ibex
#endif // __IBEX_CELL_COST_FUNC__
