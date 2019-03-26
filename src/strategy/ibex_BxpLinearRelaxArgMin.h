//============================================================================
//                                  I B E X
// File        : ibex_BxpLinearRelaxArgMin.h
// Authors     : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 30, 2018
// Last Update : Aug 01, 2018
//============================================================================

#ifndef __IBEX_BXP_LINEAR_RELAX_ARGMIN_H__
#define __IBEX_BXP_LINEAR_RELAX_ARGMIN_H__

#include "ibex_Bxp.h"
#include "ibex_System.h"
#include "ibex_Map.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Store the argmin of a system linear relaxation.
 */
class BxpLinearRelaxArgMin : public Bxp {
public:

	/**
	 * \brief Build the property.
	 *
	 * If sys is a normalized/extended system, the property is associated to the original
	 * system id.
	 */
	BxpLinearRelaxArgMin(const System& sys);

	/**
	 * \brief Copy the property
	 */
	virtual BxpLinearRelaxArgMin* copy(const IntervalVector& box, const BoxProperties& prop) const;

	/**
	 * \brief Update the property after box modification.
	 */
	virtual void update(const BoxEvent& event, const BoxProperties& prop);

	/**
	 * \brief To string
	 */
	virtual std::string to_string() const;

	/**
	 * \brief The system
	 */
	const System& sys;

	/**
	 * \brief Return the argmin (NULL if none).
	 *
	 * The size of the returned vector is the number of variables of the original system
	 * (even if an extended system was given to the constructor).
	 */
	const Vector* argmin();

	/**
	 * \brief Get the BxpLinearRelaxArgMin id of a system.
	 *
	 * If sys is a normalized/extended system, return the id of the original system.
	 */
	static long get_id(const System& sys);

protected:
	friend class CtcLinearRelax;

	Vector _argmin;
	bool inside;
	static Map<long,false>& ids();
};


/*================================== inline implementations ========================================*/

inline const Vector* BxpLinearRelaxArgMin::argmin() {
	return inside ? &_argmin : NULL;
}

} /* namespace ibex */

#endif /* __IBEX_BXP_LINEAR_RELAX_ARGMIN_H__ */
