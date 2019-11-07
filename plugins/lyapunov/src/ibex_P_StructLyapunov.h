//============================================================================
//                                  I B E X
// File        : ibex_P_StructLyapunov.h
// Authors     : Gilles Chabert
// Copyright   : IMT Atlantique
// License     : See the LICENSE file
// Created     : Nov 05, 2019
//============================================================================

#ifndef _IBEX_PARSER_STRUCT_LYAPUNOV_H_
#define _IBEX_PARSER_STRUCT_LYAPUNOV_H_

#include "ibex_P_Struct.h"
#include "ibex_System.h"

namespace ibex {

/**
 * \ingroup parser
 *
 * \brief Lyapunov parsing
 */
class P_StructLyapunov : public parser::P_Struct {
public:

	/**
	 * Load the structure for the Lyapunov plugin
	 *
	 */
	P_StructLyapunov(const char* filename);

	/**
	 * Initialize parsing of a package.
	 */
	virtual void begin();

	/**
	 * End parsing of a package.
	 */
	virtual void end();

	/**
	 * Function "f" of the vector field to be built.
	 */
	Function f;

	/**
	 * Lyapunov function "v"
	 */
	Function v;

	/**
	 * Quadratic minorant of v (optional)
	 */
	Function vminor;

	/*
	 * Approximation of the fixpoint of f
	 */
	Vector xhat;

	/**
	 * Uncertain parameter initial domain
	 */
	IntervalVector theta;

	/**
	 * Uncertain parameter constraints
	 */
	Array<NumConstraint> theta_ctrs;

private:
	// Declared just for allowing default constructor.
	class ParamSystem : public System {
	public:
		ParamSystem();
	};
	ParamSystem theta_sys;
};

} // end namespace ibex

#endif
