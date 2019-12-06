#ifndef _IBEX_LPLIBWRAPPER_H_
#define _IBEX_LPLIBWRAPPER_H_

#ifdef DEBUG
	#undef DEBUG
	#ifndef DEBUGGING
		#define DEBUGGING
	#endif
	#include "soplex.h"
	#define DEBUG
#else
	#include "soplex.h"
#endif

#define IBEX_LPSOLVER_WRAPPER_ATTRIBUTES soplex::SoPlex *mysoplex

#endif /* _IBEX_LPLIBWRAPPER_H_ */
