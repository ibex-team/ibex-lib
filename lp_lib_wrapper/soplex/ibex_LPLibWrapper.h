#ifndef _IBEX_LPLIBWRAPPER_H_
#define _IBEX_LPLIBWRAPPER_H_

#include <fstream>

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

/**
 * The following macro allows to generate an autonomous SoPlex program
 * (soplex-replay-XXX.cpp) that reproduces all calls from Ibex for this
 * instance, which may be useful for debugging in case of crash.
 *
 * #define GENERATE_SOPLEX_REPLAY
 *
 * ! CAUTION ! The size of the generated code may be HUGE. So execute 
 *             first and check the iteration number where the crash
 *             occurs. Then adjust the value of ITER_START in the .cpp file
 */

#ifdef GENERATE_SOPLEX_REPLAY
#define IBEX_LPSOLVER_WRAPPER_ATTRIBUTES soplex::SoPlex *mysoplex; \
                                         long ivec2dsvec_replay(const ibex::Vector& ivec); \
                                         long ivec2dvec_replay(const ibex::Vector& ivec); \
                                         long cmpop2type_replay(ibex::CmpOp op); \
                                         std::ofstream replay
#else
#define IBEX_LPSOLVER_WRAPPER_ATTRIBUTES soplex::SoPlex *mysoplex
#endif

#endif /* _IBEX_LPLIBWRAPPER_H_ */
