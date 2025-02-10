#ifndef _IBEX_LPLIBWRAPPER_H_
#define _IBEX_LPLIBWRAPPER_H_

#if defined(_WIN32)
	#pragma warning( push, 0 )
#elif defined(__GNUC__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif

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

#if defined(_WIN32)
	#pragma warning( pop )
#elif defined(__GNUC__)
	#pragma GCC diagnostic pop
#endif


#define IBEX_LPSOLVER_WRAPPER_ATTRIBUTES soplex::SoPlex *mysoplex

#endif /* _IBEX_LPLIBWRAPPER_H_ */
