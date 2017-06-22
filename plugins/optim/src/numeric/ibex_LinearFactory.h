/*
 * ibex_LinearFactory.h
 *
 *  Created on: Jun 22, 2017
 *      Author: gilles
 */

#ifndef __IBEX_LINEAR_FACTORY_H__
#define __IBEX_LINEAR_FACTORY_H__

#include "ibex_IntervalVector.h"
#include "ibex_LinearSolver.h"

namespace ibex {

class LinearFactory {
public:
	LinearFactory(int nb_var);

	/**
	 * \brief The linearization technique.
	 *
	 * It must be implemented in the subclasses.
	 *
	 * \return the number of constraints (possibly 0) or -1 if the system is
	 *         infeasible.
	 */
	virtual int linearization(const IntervalVector& box, LinearSolver& lp_solver)=0;

	virtual ~LinearFactory();

	/**
	 * \brief Number of variables
	 */
	int nb_var() const;

private:
//	int _nb_ctr;
	int _nb_var;
//	int _goal_var;
};

/*================================== inline implementations ========================================*/

inline int LinearFactory::nb_var() const {
	return _nb_var;
}

} /* namespace ibex */

#endif /* __IBEX_LINEAR_FACTORY_H__ */
