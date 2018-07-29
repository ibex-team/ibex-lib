//============================================================================
//                                  I B E X
// File        : ibex_BxpLPMinimizer.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 25, 2018
//============================================================================
//
//#ifndef __IBEX_LP_MINIMIZER_H__
//#define __IBEX_LP_MINIMIZER_H__
//
//#include "ibex_Bxp.h"
//#include "ibex_Linearizer.h"
//#include "ibex_Map.h"
//
//namespace ibex {
//
///**
// * \ingroup strategy
// *
// * \brief Whether an inequality is potentially active or not.
// *
// * The #active field is false only if the inequality #ctr is
// * inactive, i.e., satisfied for all x in the box. If #active
// * is true, the inequality may be active.
// */
//class BxpLPMinimizer : public Bxp {
//public:
//	/**
//	 * \brief Build the property value, associated to a constraint.
//	 *
//	 * \param ctr    - The constraint
//	 * \param active - Default activity value
//	 */
//	BxpLPMinimizer(const IntervalVector& box, const Linearizer& lin, bool active=true, bool up2date=false);
//
//	/**
//	 * \brief Copy the property
//	 */
//	virtual BxpLPMinimizer* copy(const IntervalVector& box, const BoxProperties& prop) const;
//
//	/**
//	 * \brief Update the property after box modification.
//	 *
//	 * Lazy variant. Does not actually calculate the image
//	 * of the function if active=true. Use check() instead.
//	 * \see Bxp::update(...).
//	 */
//	virtual void update(const BoxEvent& event, const BoxProperties& prop);
//
//	/**
//	 * \brief Check if the constraint is inactive
//	 *
//	 * Force function evaluation.
//	 */
//	void check();
//
//	/**
//	 * \brief The associated constraint.
//	 */
//	const Linearizer& lin;
//
//	/**
//	 * \brief Activity flag.
//	 *
//	 * true=possibly active, false=inactive.
//	 *
//	 * To force constraint evaluation, use check().
//	 */
//	bool active() const;
//
//	/**
//	 * \brief Mark the constraint as inactive.
//	 */
//	void set_inactive();
//
//	/**
//	 * \brie Get the BxpLPMinimizer id of a constraint.
//	 */
//	static long get_id(const Linearizer&);
//
//protected:
//	const IntervalVector& box;
//	bool _active;
//	bool up2date;
//	static Map<long,false> ids;
//};
//
///*================================== inline implementations ========================================*/
//
//inline BxpLPMinimizer::BxpLPMinimizer(const IntervalVector& box, const Linearizer& line, bool active, bool up2date) :
//		Bxp(get_id(line)), lin(line), box(box), _active(active), up2date(up2date) {
//
//}
//
//inline BxpLPMinimizer* BxpLPMinimizer::copy(const IntervalVector& box, const BoxProperties& prop) const {
//	return new BxpLPMinimizer(box, lin, _active, up2date);
//}
//
//inline bool BxpLPMinimizer::active() const {
//	return _active;
//}
//
//inline void BxpLPMinimizer::set_inactive() {
//	_active=false;
//	up2date=true;
//}
//
//} /* namespace ibex */
//
//#endif /* __IBEX_LP_MINIMIZER_H__ */
