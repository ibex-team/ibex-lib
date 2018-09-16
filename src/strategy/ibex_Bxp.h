//============================================================================
//                                  I B E X
// File        : ibex_Bxp.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 01, 2018
// Last update : Jul 09, 2018
//============================================================================

#include "ibex_IntervalVector.h"
#include "ibex_BitSet.h"
#include "ibex_Bisection.h"
#include "ibex_BoxEvent.h"

#include <sstream>

#ifndef __IBEX_BOX_PROPERTY_H__
#define __IBEX_BOX_PROPERTY_H__

namespace ibex {

class BoxProperties;

/**
 * \ingroup strategy
 *
 * \brief Box property.
 *
 * This interface allows to extend the simple IntervalVector data structure and to make this extension
 * being propagated through a search tree. The extended box is then visible by all operators defining
 * the search strategy (contractors, bisectors, cell buffers, etc.). See #ibex::Cell.
 *
 * A box property is a set of instances of a Bxp subclass, sharing the same identifier (#id).
 * To avoid confusion, we call "property value" an instance of the same property. Example: the
 * width of a box is a property. One can create a class BxpWidth extending Bxp. Then, BxpWidth is
 * a property and the instances of BxpWidh are property values.
 *
 * The initial value is built for the root box (typically, by an operator requiring this property)
 * and the way the property value is updated through the search tree is controlled by the two virtual
 * functions update(...) and copy(). In particular, the combination of both allows to make a node
 * inherits its property values from its father node. Note, however, that control
 * can only be made downward for the moment (e.g., there is no way of updating the properties of a
 * right branch after backtracking from the left branch).
 *
 * At each node of the tree, the value of a property required by an operator is retrieved by its
 * identifier.
 */
class Bxp {
public:

	/**
	 * \brief Create a property value.
	 *
	 * All the instances corresponding to values of the same property must have the same id.
	 */
	Bxp(long id);


	/**
	 * \brief Create a copy.
	 *
	 * The copy must have the same id.
	 *
	 * \param prop - the properties (dependencies are up to date)
	 */
	virtual Bxp* copy(const IntervalVector& box, const BoxProperties& prop) const=0;

	/**
	 * \brief Update the property value upon box modification.
	 *
	 * \param event - the box modification
	 * \param prop  - the properties (dependencies are up to date)
	 */
	virtual void update(const BoxEvent& event, const BoxProperties& prop)=0;

	/**
	 * \brief To string
	 *
	 * By default: the id.
	 */
	virtual std::string to_string() const;

	/**
	 * \brief Delete this.
	 */
	virtual ~Bxp();

	/**
	 * \brief Identifying number.
	 */
	const long id;

	/**
	 * \brief Dependencies.
	 *
	 * The identifying numbers of all the other properties required
	 * by this property.
	 */
	std::vector<long> dependencies;
};

/*================================== inline implementations ========================================*/

inline Bxp::Bxp(long id) : id(id) {
}

inline Bxp::~Bxp() {
}

inline std::string Bxp::to_string() const {
	std::stringstream ss;
	ss << '[' << id << ']';
	return ss.str();
}

} /* namespace ibex */

#endif /* __IBEX_BOX_PROPERTY_H__ */
