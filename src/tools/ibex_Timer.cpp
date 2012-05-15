//============================================================================
//                                  I B E X                                   
// File        : ibex_Timer.cpp
// Author      : ???? and Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : May 13, 2012
//============================================================================

#include "ibex_Timer.h"

namespace ibex {

Timer::Time Timer::real_lapse;
Timer::Time Timer::virtual_ulapse;
Timer::Time Timer::virtual_slapse;
Timer::Time Timer::real_time;
Timer::Time Timer::virtual_utime;
Timer::Time Timer::virtual_stime;
long Timer::resident_memory;

#ifndef __WIN32__
//  std::clock_t Timer::res;
struct rusage Timer::res;
#endif

struct timeval Timer::tp;

/*
 *  The virtual time of day and the real time of day are calculated and
 *  stored for future use.  The future use consists of subtracting these
 *  values from similar values obtained at a later time to allow the user
 *  to get the amount of time used by the backtracking routine.
 */
void Timer::start()
{
#ifndef __WIN32__
	//    res = std::clock();
	getrusage( RUSAGE_SELF, &res );

	virtual_utime = (Time) res.ru_utime.tv_sec +
			(Time) res.ru_utime.tv_usec / 1000000.0;
	virtual_stime = (Time) res.ru_stime.tv_sec +
			(Time) res.ru_stime.tv_usec / 1000000.0;
#endif

	gettimeofday( &tp, NULL );
	real_time =    (Time) tp.tv_sec +
			(Time) tp.tv_usec / 1000000.0;
}


/*
 *  Stop the stopwatch and return the time used in seconds (either
 *  REAL or VIRTUAL time, depending on ``type'').
 */
void Timer::stop( TimerType type )
{
	if (type == __REAL) {
		gettimeofday( &tp, NULL );
		real_lapse = (Time) tp.tv_sec +
				(Time) tp.tv_usec / 1000000.0
				-

				real_time;
	}
	else {

		//      res = std::clock();
#ifndef __WIN32__
		getrusage( RUSAGE_SELF, &res );
		virtual_ulapse = (Time) res.ru_utime.tv_sec +
				(Time) res.ru_utime.tv_usec / 1000000.0
				- virtual_utime;
		virtual_slapse = (Time) res.ru_stime.tv_sec +
				(Time) res.ru_stime.tv_usec / 1000000.0
				- virtual_stime;
		resident_memory = res.ru_ixrss;
#endif
	}
}

void Timer::check(double timeout) {
	if (VIRTUAL_TIMELAPSE()>timeout) throw TimeOutException();
	//Timer::stop();
	//time += Timer::VIRTUAL_TIMELAPSE();
	//if (time >=time_limit) throw TimeOutException();
	/*
if (Timer::RESIDENT_MEMORY() > 100000)
  {cout << "memory limit " << Timer::RESIDENT_MEMORY() << endl;
    throw MemoryException();
  }
	 */
	//Timer::start();
}

} // end namespace ibex
