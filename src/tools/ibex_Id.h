//============================================================================
//                                  I B E X
// File        : ibex_Id.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 18, 2018
//============================================================================

#ifndef __IBEX_ID_H__
#define __IBEX_ID_H__

namespace ibex {

/**
 * \ingroup tools
 *
 * \brief Generate an identifier.
 *
 * This function returns a unique number in the
 * whole execution of the program.
 */
long next_id();

}

#endif /* __IBEX_ID_H__ */
