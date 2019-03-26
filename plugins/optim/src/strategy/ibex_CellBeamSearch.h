//============================================================================
//                                  I B E X                                   
// File        : ibex_CellBeamSearch.h
// Author      : Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Aug 29, 2017
// Modified    : Sep 07, 2017
//============================================================================

#ifndef __IBEX_CELL_BEAM_SEARCH_H__
#define __IBEX_CELL_BEAM_SEARCH_H__

#include "ibex_CellHeap.h"
#include "ibex_CellCostFunc.h"
#include "ibex_CellBufferOptim.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_Cell.h"

namespace ibex {

/**
 * \ingroup optim
 *
 * \brief Beamsearch buffer (for global optimization)
 *
 * This class implements a beamsearch strategy : the open nodes are distributed in three buffers , each of 
 * them implemented by a heap (CellHeap)
 * - The global heap (used when the two others are empty to choose a new branch to explore) implemented by the Beamsearch object itself
 * - The current heap (of size limited by the  beamsize parameter) where the nodes are selected to be bisected (member currentheap)
 * - The future heap , where the handled nodes are stored (after contraction , search for loup )  (member futureheap)
 * 
 *  When beamsize=1, we have FeasibleDiving strategy ("best strategy in JOGO 14 
 *  "Node selection strategies in interval Branch and Bound algorithms"  article by B. Neveu, G. Trombettoni and I. Araya)
 * 
 */
class CellBeamSearch: public CellHeap {
public :

	/**
	 * \brief Create the global buffer from two heaps.
	 */
	CellBeamSearch (CellHeap& currentbuffer, CellHeap& futurebuffer, const ExtendedSystem & sys, unsigned int beamsize =default_beamsize) ;

	/**
	 * \brief Delete this.
	 */
	virtual ~CellBeamSearch();

	/** \brief Size of the global buffer. */
	virtual unsigned int size() const;

	/** \brief Flush the buffer. */
	virtual void flush();

	/** \brief Return true if the buffer is empty. */
	virtual bool empty() const;

	/** \brief Push a new cell on the futurebuffer heap. */
	virtual void push(Cell* cell);

	/**
	 * \brief Pop a cell from the BeamSearch buffer.
	 *
	 * \note if the currentbuffer is not empty, pop a cell from the currentbuffer
	 * else pop a cell from the future buffer, empties the future buffer
	 * (beamsize-1 cells put into the current buffer 	and  the remaining into the global buffer
	 */
	virtual Cell* pop();

	/** \brief Return the next cell (but does not pop it).*/
	virtual Cell* top() const;

	/** \brief Returns the minimum LB of all 3 buffers (global , current and future). */
	virtual double minimum() const;

	/** \brief Remove the cells with a LB greater than new_loup */
	virtual void contract (double new_loup);

	/** \brief The default value for the maximum beam size */
	static constexpr unsigned int default_beamsize = 1;

protected :
	/**
	 * \brief Return the cost of a cell
	 *
	 * LB criterion.
	 */
	double cell_cost (const Cell& cell) const;

	/** \brief The maximum size of "currentbuffer " (the beam) */
	unsigned  int   beamsize;

	/** \brief Empties the future buffer and distribute the cells between the current buffer and the global buffer*/
	void move_buffers();

	/** \brief The current buffer, actual beam of limited size */
	CellHeap&  currentbuffer;

	/** \brief The nodes sons of a node from the current buffer */
	CellHeap&  futurebuffer;

};

} // namespace ibex

#endif // __IBEX_CELL_BEAM_SEARCH_H__

