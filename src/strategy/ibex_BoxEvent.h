//============================================================================
//                                  I B E X
// File        : ibex_BoxEvent.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 08, 2018
// Last update : Jul 09, 2018
//============================================================================

#ifndef __IBEX_BOXEVENT_H__
#define __IBEX_BOXEVENT_H__

namespace ibex {

/**
 * \brief Box event.
 *
 * This class allows to encapsulate in a single object all the information
 * related to the modification of a box (when one of its dimension has changed).
 */
class BoxEvent {

public:
	 /**
	  * \brief Even type.
	  *
	  *   CONTRAT: the new box is a subset of the old box.
	  *   INFLATE: the new box is a superset of the old box.
	  *   CHANGE: otherwise
	  */
	typedef enum { CONTRACT, INFLATE, CHANGE } Type;

	/**
	 * \brief Create an event.
	 *
	 * \param box    - The box on which the event has occurred.
	 * \param type   - The type of event.
	 * \param impact - The impacted dimensions.
	 */
	BoxEvent(const IntervalVector& box, Type type, const BitSet& impact);

	/**
	 * \brief Create an event.
	 *
	 * \param box    - The box on which the event has occurred.
	 * \param type   - The type of event.
	 */
	BoxEvent(const IntervalVector& box, Type type);

	/** The box (after modification) */
	const IntervalVector& box;

	/** Event type. */
	Type type;

	/** The impact (dimensions of the box modified by the event). */
	const BitSet impact;
};


/*================================== inline implementations ========================================*/

inline BoxEvent::BoxEvent(const IntervalVector& box, Type type, const BitSet& impact) : box(box), type(type), impact(impact) {
}

inline BoxEvent::BoxEvent(const IntervalVector& box, Type type) : box(box), type(type), impact(BitSet::all(box.size())) {
}


} /* namespace ibex */

#endif /* __IBEX_BOX_EVENT_H__ */
