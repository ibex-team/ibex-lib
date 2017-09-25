/* ============================================================================
 * I B E X - ibex_BD_Factory.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 18, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_BD_FACTORY_H__
#define __IBEX_BD_FACTORY_H__

#include "ibex_SystemFactory.h"
#include "ibex_MitsosSIP.h"

namespace ibex {

/**
 * \ingroup sip
 *
 * \brief Factory for either the LBD or UBD problem
 */
class BD_Factory  : public SystemFactory {

protected:

	typedef enum { LBD, UBD, ORA } problem_type;

	/**
	 * \param sip   - The SIP problem
	 */
	BD_Factory(const MitsosSIP& sip, problem_type problem);

	virtual ~BD_Factory();

	void add_discretized_ctr(double eps_g);

	void add_gaol(double f_RES);

	/**
	 * The SIP problem
	 */
	const MitsosSIP& sip;

	/**
	 * Either LBD, UBD or ORA
	 */
	problem_type problem;

	/**
	 * Contains the variables "x" of the SIP problem
	 * and also "eta" in the case of ORA.
	 */
	Array<const ExprSymbol> new_vars;

};

class LBD_Factory : public BD_Factory {
public:
	/**
	 * \param sip   - The SIP problem
	 */
	LBD_Factory(const MitsosSIP& sip);

};

class UBD_Factory : public BD_Factory {
public:
	/**
	 * \param sip   - The SIP problem
	 * \param eps_g -
	 */
	UBD_Factory(const MitsosSIP& sip, double eps_g);
};

class ORA_Factory : public BD_Factory {
public:
	/**
	 * \param sip   - The SIP problem
	 */
	ORA_Factory(const MitsosSIP& sip, double f_RES);
};

} // namespace ibex

#endif /* __IBEX_BD_FACTORY_H__ */
