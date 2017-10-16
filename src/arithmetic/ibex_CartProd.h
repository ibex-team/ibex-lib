/*
 * ibex_CartProd.h
 *
 *  Created on: Oct 12, 2017
 *      Author: gilles
 */

#ifndef __IBEX_CART_PROD_H__
#define __IBEX_CART_PROD_H__

#include "ibex_IntervalVector.h"
#include "ibex_Vector.h"

namespace ibex {



/**
 *
 * \brief Cartesian product of x and y.
 */
IntervalVector cart_prod(const IntervalVector& x, const IntervalVector& y);



}


#endif /* IBEX_CARTPROD_H_ */
