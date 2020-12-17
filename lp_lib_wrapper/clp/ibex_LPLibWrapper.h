#ifndef _IBEX_LPLIBWRAPPER_H_
#define _IBEX_LPLIBWRAPPER_H_

#ifdef DEBUG
	#undef DEBUG
	#ifndef DEBUGGING
		#define DEBUGGING
	#endif
	#include "ClpSimplex.hpp"
	#define DEBUG
#else
    #include "ClpSimplex.hpp"
#endif

#define IBEX_LPSOLVER_WRAPPER_ATTRIBUTES ClpSimplex *myclp

#endif /* _IBEX_LPLIBWRAPPER_H_ */
