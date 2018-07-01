/*
 * ibex_BoxProperties.h
 *
 *  Created on: Jul 1, 2018
 *      Author: gilles
 */

#ifndef __IBEX_BOX_PROPERTIES_H__
#define __IBEX_BOX_PROPERTIES_H__

#include "ibex_Map.h"
#include "ibex_BoxProp.h"
#include "ibex_Id.h"

#include <utility>

namespace ibex {

class BoxProperties {
public:
	class CircualDependency : Exception { };
	class PropertyNotFound : Exception { };

	/**
	 *
	 */
	BoxProperties();

	virtual ~BoxProperties();

	class NotFound : public Exception { };

	/**
	 * \brief Add a new property.
	 *
	 * Does nothing if the property is already in.
	 * Automatically deleted by the destructor of BoxProperties.
	 *
	 * \param dependencies - the other properties required by prop.
	 */
	void add(const Bxp* prop);

	/**
	 * \brief Return the property of identifier \a id.
	 *
	 * \throw a NotFound exception if this property does not exist.
	 */
	const Bxp* operator[](long id) const;

	/**
	 * \brief Return the property of identifier \a id.
	 *
	 * \throw a NotFound exception if this property does not exist.
	 */
	Bxp* operator[](long id) {
		return (Bxp*) ((const BoxProperties*) this)->operator[](id);
	}

	void update(const IntervalVector& new_box, bool contract, const BitSet& impact);

	void update_copy(BoxProperties& copy) const;

	void update_bisect(const BisectionPoint&, BoxProperties& left, BoxProperties& right) const;

protected:
	int topo_sort_rec(const Bxp& el, Map<int,false>& level) const;

	void topo_sort() const;

	Map<Bxp> map;

	/**
	 * Array of properties sorted by dependency level
	 * (the first the lowest level = depends on nothing).
	 */
	mutable std::vector<Bxp*> dep;

	mutable bool _dep_up2date;
};

} /* namespace ibex */

#endif /* __IBEX_BOX_PROPERTIES_H__ */
