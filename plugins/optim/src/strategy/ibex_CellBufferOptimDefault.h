//============================================================================
//                                  I B E X
// File        : ibex_CellBufferOptim.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 24, 2017
//============================================================================

//#ifndef __IBEX_CELL_BUFFER_OPTIM_H__
//#define __IBEX_CELL_BUFFER_OPTIM_H__
//
//#include "ibex_CellDoubleHeap.h"
//
//namespace ibex {
//
///**
// * \ingroup optim
// *
// * \brief Default cell buffer used for optimization.
// *
// * This is a double-heap buffer where the first heap criterion
// * is LB (lower bound of the objective domain) and the second
// * heap criterion is set by the user (default is UB).
// *
// * The second one is chosen at each node with a probability
// * crit2_pr/100 (default value is crit2_pr=50).
// */
//class CellBufferOptimDefault : public CellDoubleHeap {
//public:
//	/**
//	 * \brief Create the buffer.
//	 *
//	 * \param sys    - the original (not extended) system to optimize
//	 * \param critpr - probability to choose the second criterion in node selection; integer in [0,100]. By default 50
//	 * \param crit   - second criterion in node selection (the first criterion is the minimum of the objective estimate). default value CellHeapOPtim::UB
//	 */
//	CellBufferOptimDefault(const System& sys, int crit2_pr=50,
//			CellCostFunc::criterion crit2=CellCostFunc::UB);
//
//	/**
//	 * \brief Add backtrackable data required by this buffer.
//	 */
//	virtual void add_backtrackable(Cell& root);
//
//	/**
//	 * \brief Delete this.
//	 */
//	virtual ~CellBufferOptim();
//
//	/** \see CellBuffer. */
//	virtual void push(Cell* cell);
//
////	/** \see CellBuffer. */
////	virtual void flush();
////
////	/** \see CellBuffer. */
////	virtual unsigned int size() const=0;
////
////	/** \see CellBuffer. */
////	virtual bool empty() const=0;
////
////	/** \see CellBuffer. */
////	virtual void push(Cell* cell)=0;
////
////	/** \see CellBuffer. */
////	virtual Cell* pop()=0;
////
////	/** \see CellBuffer. */
////	virtual Cell* top() const;
//protected:
//
//	/**
//	 * The system
//	 */
//	const System& sys;
//};
//
//} /* namespace ibex */
//
//#endif /* __IBEX_CELL_BUFFER_OPTIM_H__ */
