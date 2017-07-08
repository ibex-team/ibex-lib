//============================================================================
//                                  I B E X
// File        : ibex_LinearRestrict.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Last Update : Jun 22, 2017
//============================================================================

#ifndef __IBEX_LINEAR_RESTRICT_H__
#define __IBEX_LINEAR_RESTRICT_H__

#include "ibex_LinearFactory.h"

namespace ibex {

class LinearRestrict : public LinearFactory {
public:
	LinearRestrict(int nb_var);
};

} /* namespace ibex */

#endif /* __IBEX_LINEAR_RESTRICT_H__ */
