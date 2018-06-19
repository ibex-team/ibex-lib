/*
 * ibex_Property.h
 *
 *  Created on: Jun 6, 2018
 *      Author: gilles
 */

#ifndef IBEX_PROPERTY_H_
#define IBEX_PROPERTY_H_

namespace ibex {

class Property {
public:
	Property() { }

	virtual ~Property() { }

	/**
	 * \brief Create a copy.
	 *
	 * The copy must have the same id.
	 */
	virtual Property* copy() const=0;

};

} /* namespace ibex */

#endif /* IBEX_PROPERTY_H_ */
