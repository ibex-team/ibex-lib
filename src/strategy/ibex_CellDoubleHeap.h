 //============================================================================
//                                  I B E X                                   
// File        : ibex_CellDoubleHeap.h
// Author      : Gilles Chabert, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 12, 2014
//============================================================================

#ifndef __IBEX_CELL_DOUBLE_HEAP_H__
#define __IBEX_CELL_DOUBLE_HEAP_H__

#include "ibex_DoubleHeap.h"
#include "ibex_CellCostFunc.h"
#include "ibex_CellBuffer.h"

namespace ibex {

/**
 * \brief Double-heap buffer (for global optimization)
 *
 * See "A new multi-selection technique in interval methods for global optimization", L.G. Casado, Computing, 2000
 * (TODO: check ref)
 */
class CellDoubleHeap : public DoubleHeap<Cell>, public CellBuffer {

public:

	/**
	 * \brief Build a double heap for cells
	 *
	 * \param goal_var - Index of the criterion variable
	 * \param crit_2   - The second criterion used (the first is LB)
	 * \param critpr   - Probability to choose the second criterion in node selection in percentage
	 *                   integer in [0,100] (default value is 50). The value 0 corresponds to use a
	 *                   single criterion for node selection (the classical one : minimizing the lower
	 *                   bound of the estimate of the objective). The value 100 corresponds to use a
	 *                   single criterion for node selection (the second one used in buffer2)
	 */
	CellDoubleHeap(CellCostFunc& cost1, CellCostFunc& cost2, int critpr=50);

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
	 * \brief Contract the heap
	 *
	 * Removes (and deletes) from the heap all the cells
	 * with a cost (according to the cost function of the
	 * first heap) greater than \a loup.
	 */
	void contract(double loup);

	/**
	 * \brief Cost function of the first heap
	 */
	CellCostFunc& cost1();

	/**
	 * \brief Cost function of the second heap
	 */
	CellCostFunc& cost2();
};

/*================================== inline implementations ========================================*/

inline CellDoubleHeap::CellDoubleHeap(CellCostFunc& cost1, CellCostFunc& cost2, int critpr) :
		DoubleHeap<Cell>(cost1,cost1.depends_on_loup,cost2,cost2.depends_on_loup, critpr) { }

inline void CellDoubleHeap::flush()               { DoubleHeap<Cell>::flush(); }

inline unsigned int CellDoubleHeap::size() const  { return DoubleHeap<Cell>::size(); }

inline bool CellDoubleHeap::empty() const         { return DoubleHeap<Cell>::empty(); }

inline void CellDoubleHeap::push(Cell* cell)      { DoubleHeap<Cell>::push(cell); }

inline Cell* CellDoubleHeap::pop()                { return DoubleHeap<Cell>::pop(); }

inline Cell* CellDoubleHeap::top() const          { return DoubleHeap<Cell>::top(); }

 inline std::ostream& CellDoubleHeap::print(std::ostream& os) const
 {	os << "==============================================================================\n";
   os << " first heap " << " size " << heap1->size() << " top " << heap1->top()->box << std::endl;
     os << " second heap " << " size " << heap2->size() << " top " << heap2->top()->box ;
     return  os << std::endl;
 }

 //{return DoubleHeap<Cell>::print( os);}

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


} // namespace ibex

#endif // __IBEX_CELL_DOUBLE_HEAP_H__
