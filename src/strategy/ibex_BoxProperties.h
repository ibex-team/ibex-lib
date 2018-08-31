//============================================================================
//                                  I B E X
// File        : ibex_BoxProperties.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 01, 2018
// Last Update : Jul 09, 2018
//============================================================================

#ifndef __IBEX_BOX_PROPERTIES_H__
#define __IBEX_BOX_PROPERTIES_H__

#include "ibex_Map.h"
#include "ibex_Bxp.h"
#include "ibex_Id.h"

#include <utility>

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Box properties
 *
 * This class allows to store a set of potentially inter-dependent
 * box properties, in a map structure.
 */
class BoxProperties {
public:
	/**
	 * \brief Thrown if there is a circular dependency between 2 properties.
	 */
	class CircualDependency : Exception { };

	/**
	 * \brief Buid a new property map.
	 */
	BoxProperties(const IntervalVector& box);

	/**
	 * \brief Copy constructor.
	 *
	 * Duplicate all properties for the new box.
	 */
	BoxProperties(const IntervalVector& box, const BoxProperties&);

	/**
	 * \brief Copy constructor.
	 *
	 * Duplicate all properties for the same box.
	 */
	BoxProperties(const BoxProperties&);

	/**
	 * \brief Delete this.
	 */
	virtual ~BoxProperties();

	/**
	 * \brief Add a new property.
	 *
	 * Does nothing if the property is already in.
	 * Automatically deleted by the destructor of BoxProperties.
	 *
	 * The dependencies, i.e., the other properties required by prop
	 * have to be set in prop->dependencies before calling update(...).
	 */
	void add(Bxp* prop);

	/**
	 * \brief Return the property of identifier \a id.
	 *
	 * \return NULL if this property does not exist in the map.
	 */
	const Bxp* operator[](long id) const;

	/**
	 * \brief Return the property of identifier \a id.
	 *
	 * \return NULL if this property does not exist in the map.
	 */
	Bxp* operator[](long id) {
		return (Bxp*) ((const BoxProperties*) this)->operator[](id);
	}

	/**
	 * \brief Update all the properties after box modification.
	 *
	 * The order properties are updated with respect dependencies.
	 *
	 * \param event  - The box modification
	 */
	void update(const BoxEvent& event);

	/*
	 * \brief Incremental update.
	 *
	 * When a box has not been changed but a property "p" has
	 * been updated, this function allows to call update(...)
	 * only on properties that depend on "p".
	 */
	void propagate(const Bxp& p);

	/**
	 * \brief Update all the properties after box bisection.
	 *
	 * The order properties are updated with respect dependencies.
	 *
	 * \param b     - The bisection
	 * \param lprop - The properties associated to the left box after bisection
	 * \param rprop - The properties associated to the right box after bisection
	 */
	void update_bisect(const Bisection& b, BoxProperties& lprop, BoxProperties& rprop) const;

	/**
	 * \brief The box.
	 */
	const IntervalVector& box;

protected:
	/*
	 * internal (so far).
	 */
	class PropertyNotFound  : Exception { };

	/*
	 * Recursively called by topo_sort()
	 */
	int topo_sort_rec(const Bxp& el, Map<int,false>& level) const;

	/*
	 * Update #dep with a topological sort of the map
	 */
	void topo_sort() const;

	/*
	 * The map that allows to retrieve a property by its id.
	 */
	Map<Bxp> map;

	/*
	 * Array of properties sorted by dependency level
	 * (the first element is at the lowest level =
	 *  depends on nothing).
	 */
	mutable std::vector<Bxp*> dep;

	/*
	 * Whether the topological sort is up to date.
	 */
	mutable bool _dep_up2date;

	friend std::ostream& operator<<(std::ostream& os, const BoxProperties&);
};

/*
 * \brief Print the property ids in the map
 */
std::ostream& operator<<(std::ostream& os, const BoxProperties&);

} /* namespace ibex */

#endif /* __IBEX_BOX_PROPERTIES_H__ */
