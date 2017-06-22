/*
 * ibex_LinearRestrict.h
 *
 *  Created on: Jun 22, 2017
 *      Author: gilles
 */

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
