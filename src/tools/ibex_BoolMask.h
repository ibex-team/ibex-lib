/* ============================================================================
 * I B E X - Boolean mask
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_BOOL_MASK_H__
#define __IBEX_BOOL_MASK_H__

#include <vector>

namespace ibex {

/**
 * Boolean mask.
 */
class BoolMask {

public:
	BoolMask(int n);
	void set_all();
	void unset_all();
	bool all_set() const;
	bool all_unset() const;
	bool operator[](int i);
	const bool operator[](int i) const;
	void set(int i);
	void unset(int i);
	~BoolMask();
private:
	bool* mask;
};

} // namespace ibex
#endif // __IBEX_BOOL_MASK_H__
