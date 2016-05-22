/* ============================================================================
 * I B E X - DoubleIndex of an expression
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 1, 2016
 * Last Update : May 20, 2016
 * ---------------------------------------------------------------------------- */

#include "ibex_DoubleIndex.h"

namespace ibex {

std::ostream& operator<<(std::ostream& os, const DoubleIndex idx) {
	if (idx.all()) return os;

	os << "(";
	switch (idx.dim.type()) {
	case Dim::ROW_VECTOR:
		if (idx.one_col()) return os << idx.first_col();
		else return os << idx.first_col() << ":" << idx.last_col();
	case Dim::COL_VECTOR:
		if (idx.one_row()) return os << idx.first_row();
		else return os << idx.first_row() << ":" << idx.last_row();
	default:
		assert(idx.dim.is_matrix());
		if (idx.all_rows()) os << ":";
		else if (idx.one_row()) os << idx.first_row();
		else os << idx.first_row() << ":" << idx.last_row();
		os << ",";
		if (idx.all_cols()) os << ":";
		else if (idx.one_col()) os << idx.first_col();
		else os << idx.first_col() << ":" << idx.last_col();
	}
	return os << ")";
}

} /* namespace ibex */

