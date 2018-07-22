//============================================================================
//                                  I B E X                                   
// File        : ibex_CellDoubleHeap.h
// Author      : Gilles Chabert, Jordan Ninin
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Sep 12, 2014
//============================================================================

#ifndef __IBEX_CELL_DOUBLE_HEAP_H__
#define __IBEX_CELL_DOUBLE_HEAP_H__

#include "ibex_DoubleHeap.h"
#include "ibex_CellCostFunc.h"
#include "ibex_CellBufferOptim.h"
#include "ibex_ExtendedSystem.h"

namespace ibex {

/**
 * \ingroup optim
 *
 * \brief Double-heap buffer (for global optimization)
 *
 * This is a double-heap buffer where the first heap criterion
 * is LB (lower bound of the objective domain) and the second
 * heap criterion is set by the user (default is UB).
 *
 * The second one is chosen at each node with a probability
 * crit2_pr/100 (default value is crit2_pr=50).
 *
 * \see "A new multi-selection technique in interval methods
 *       for global optimization", L.G. Casado, Computing, 2000
 */
class CellDoubleHeap : public DoubleHeap<Cell>, public CellBufferOptim {

public:

	/**
	 * \brief Create the buffer.
	 *
	 * \param sys    - the extended system to optimize
	 * \param critpr - probability to choose the second criterion in node selection;
	 *                 integer in [0,100]. By default 50. The value 0 corresponds to
	 *                 use a single criterion for node selection (the classical one :
	 *                 minimizing the lower bound of the estimate of the objective).
	 *                 The value 100 corresponds to use a single criterion for node
	 *                 selection (the second one used, crit2).
	 * \param crit   - second criterion in node selection (the first criterion is the
	 *                 minimum of the objective estimate). default value CellHeapOPtim::UB.
	 */
	CellDoubleHeap(const ExtendedSystem& sys, int crit2_pr=50,
			CellCostFunc::criterion crit2=CellCostFunc::UB);

    /**
	 * \brief Delete *this.
	 */
	~CellDoubleHeap();

	/**
	 * \brief Add properties required by this buffer.
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& map);

	/**
	 * \brief Flush the buffer.
	 *
	 * All the remaining cells will be *deleted*
	 */
	void flush();

	/** \brief Return the size of the buffer. */
	unsigned int size() const;

	/** \brief Return true if the buffer is empty. */
	bool empty() const;

	/** \brief Push a new cell on the stack. */
	void push(Cell* cell);

	/** \brief Pop a cell from the stack and return it.*/
	Cell* pop();

	/** \brief Return the next box (but does not pop it).*/
	Cell* top() const;


	std::ostream& print(std::ostream& os) const;

	/**
	 * \brief Return the minimum value of the heap
	 *
	 */
	virtual double minimum() const;

	/**
	 * \brief Contract the heap
	 *
	 * Removes (and deletes) from the heap all the cells
	 * with a cost (according to the cost function of the
	 * first heap) greater than \a loup.
	 */
	virtual void contract(double loup);

	/**
	 * \brief Cost function of the first heap
	 */
	CellCostFunc& cost1();

	/**
	 * \brief Cost function of the second heap
	 */
	CellCostFunc& cost2();

protected:

	/**
	 * The system
	 */
	const ExtendedSystem& sys;
};

/*================================== inline implementations ========================================*/

inline CellDoubleHeap::CellDoubleHeap(const ExtendedSystem& sys, int crit2_pr, CellCostFunc::criterion crit2) :
		DoubleHeap<Cell>(*new CellCostVarLB(sys, sys.goal_var()), false,
				*CellCostFunc::get_cost(sys, crit2, sys.goal_var()), true /* TODO: give right value */, crit2_pr),
		sys(sys) {
}

inline CellDoubleHeap::~CellDoubleHeap() {
	flush();
	delete &cost1();
	delete &cost2();
}

inline void CellDoubleHeap::contract(double new_loup) {

	// DoubleHeap::contract requires the costs of
	// the first heap to be up-to-date.
	if (cost1().depends_on_loup) {
		cost1().set_loup(new_loup);
		heap1->sort();
	}

	cost2().set_loup(new_loup);
	DoubleHeap<Cell>::contract(new_loup);
}

inline CellCostFunc& CellDoubleHeap::cost1()      { return (CellCostFunc&) heap1->costf; }

inline CellCostFunc& CellDoubleHeap::cost2()      { return (CellCostFunc&) heap2->costf; }

inline void CellDoubleHeap::add_property(const IntervalVector& init_box, BoxProperties& map) {
      // add data "pu" and "pf" (if required)
       cost2().add_property(map);
}

inline void CellDoubleHeap::flush()               { DoubleHeap<Cell>::flush(); }

inline unsigned int CellDoubleHeap::size() const  { return DoubleHeap<Cell>::size(); }

inline bool CellDoubleHeap::empty() const         { return DoubleHeap<Cell>::empty(); }

inline void CellDoubleHeap::push(Cell* cell) {
       // we know cost1() does not require OptimData
       cost2().set_optim_data(*cell);

       // the cell is put into the 2 heaps
       DoubleHeap<Cell>::push(cell);


}

inline Cell* CellDoubleHeap::pop()                { return DoubleHeap<Cell>::pop(); }
inline Cell* CellDoubleHeap::top() const          { return DoubleHeap<Cell>::top(); }

inline double CellDoubleHeap::minimum() const     { return DoubleHeap<Cell>::minimum(); }

inline std::ostream& CellDoubleHeap::print(std::ostream& os) const {
	os << "==============================================================================\n";
	if (empty()) {
		return os << " EMPTY heap" << std::endl;
	} else {
		os << " first heap " << " size " << heap1->size() << " top " << heap1->top()->box << std::endl;
		os << " second heap " << " size " << heap2->size() << " top " << heap2->top()->box ;
		return  os << std::endl;
	}
}


} // namespace ibex

#endif // __IBEX_CELL_DOUBLE_HEAP_H__
