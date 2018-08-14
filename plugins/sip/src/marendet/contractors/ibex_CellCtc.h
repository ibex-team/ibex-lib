//============================================================================
//                                  I B E X                                   
// File        : ibex_CellCtc.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_CELLCTC_H__
#define __SIP_IBEX_CELLCTC_H__

#include "ibex_Array.h"
#include "ibex_BitSet.h"
#include "ibex_Cell.h"
#include "ibex_Exception.h"
#include "ibex_IntervalVector.h"

#include <cassert>
#include <exception>

namespace ibex {
/**
 * \brief Cell contractor.
 *
 * Not like the IBEX Contractor, a CellCtc does operation on a cell instead of just on a box of a cell.
 * This allows a contractor to operate on Backtrackable data of a cell. Consequently, one has to be careful to
 * enforce the locality property of the contractor when writing a CellCtc.
 */
class CellCtc {
public:
	explicit CellCtc(int nb_var);
	explicit CellCtc(const Array<CellCtc>& ctcs);
	virtual ~CellCtc();
	virtual void contractCell(Cell& cell) =0;
	virtual void contractCell(Cell& cell, const BitSet& impact) {
		assert(cell.box.size() == nb_var);
		_impact = &impact;
		try {
			contractCell(cell);
		} catch (Exception& e) { // ibex exceptions
			_impact = NULL;
			throw e;
		} catch (std::exception& e) { // other exceptions
			_impact = NULL;
			throw e;
		} catch (...) {
			ibex_error("contract: cannot handle exception");
		}

		_impact = NULL;

		contractCell(cell);
	}
	virtual void contractCell(Cell& cell, const BitSet& impact,
			BitSet& flags) {
		assert(cell.box.size() == nb_var);
		_impact = &impact;
		_output_flags = &flags;

		flags.clear();

		try {
			contractCell(cell);
		} catch (Exception& e) { // ibex exceptions
			_impact = NULL;
			_output_flags = NULL;
			throw e;
		} catch (std::exception& e) { // other exceptions
			_impact = NULL;
			_output_flags = NULL;
			throw e;
		} catch (...) {
			ibex_error("contract: cannot handle exception");
		}

		_impact = NULL;
		_output_flags = NULL;

	}
	const int nb_var;

	BitSet* input;
	BitSet* output;
	enum {
		FIXPOINT, INACTIVE, NB_OUTPUT_FLAGS
	};

protected:
	const BitSet* impact();
	void set_flag(unsigned int);
	//static bool check_nb_var_ctc_list(const Array<CellCtc>& ctcs);

private:
	const BitSet* _impact;
	BitSet* _output_flags;
};

inline CellCtc::CellCtc(int n) :
		nb_var(n), input(NULL), output(NULL), _impact(NULL), _output_flags(NULL) {
}

inline CellCtc::CellCtc(const Array<CellCtc>& l) :
		nb_var(l[0].nb_var), input(NULL), output(NULL), _impact(NULL), _output_flags(
		NULL) {
}

inline CellCtc::~CellCtc() {
}

inline const BitSet* CellCtc::impact() {
	return _impact;
}

inline void CellCtc::set_flag(unsigned int f) {
	assert(f < NB_OUTPUT_FLAGS);
	if (_output_flags)
		_output_flags->add(f);
}

} // end namespace ibex

#endif // __SIP_IBEX_CELLCTC_H__
