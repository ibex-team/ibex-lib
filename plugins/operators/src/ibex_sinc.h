//============================================================================
//                                  I B E X
// File        : ibex_sinc.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 02, 2018
//============================================================================

#ifndef __IBEX_SINC_H__
#define __IBEX_SINC_H__

#include "ibex_Expr.h"
#include "ibex_Domain.h"

namespace ibex {

//DECLARE_UNARY_OPERATOR("sinc",sinc,sinc_bwd,dsinc,dsinc);

/** Forward evaluation. */
Domain sinc(const Domain& x);

/** Backward evaluation. */
void sinc_bwd(Domain& x, const Domain& y);

/** Numerical derivative. */
Domain dsinc(const Domain& x);

/** Symbolic derivative. */
const ExprNode& dsinc_symb(const ExprNode& expr);

} // end namespace

#endif /* __IBEX_SINC_H__ */
