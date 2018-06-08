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
	virtual ~Property() { }

	/**
	 * \brief Create a copy.
	 */
	virtual Property* copy() const=0;
};

} /* namespace ibex */

#endif /* IBEX_PROPERTY_H_ */
