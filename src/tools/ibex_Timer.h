//============================================================================
//                                  I B E X                                   
// File        : ibex_Timer.h
// Author      : ????
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : May 13, 2012
//============================================================================

#ifndef __IBEX_TIMER_H__
#define __IBEX_TIMER_H__

#include "ibex_Exception.h"

#include <sys/time.h>

#ifdef __WIN32__
//#include <ctime>
#else
#include <sys/resource.h>
#endif

#include <unistd.h>

namespace ibex {

/** \ingroup tools
 *
 * \brief Timeout exception.
 *
 */
class TimeOutException : public Exception { };

/** \ingroup tools
 *
 * \brief Timer.
 */
class Timer {
 public:

  typedef double Time;

  typedef enum type_timer {__REAL, VIRTUAL} TimerType;

  static void start();

  static void stop(TimerType type=VIRTUAL);

  /**
   * \brief Check the timer and throw a #ibex::TimeOutException
   * if the time elapsed exceeds #timeout.
   *
   * (add by gch).
   */
  static void check(double timeout);

  inline static Time REAL_TIMELAPSE() { return real_lapse; }
  inline static double RESIDENT_MEMORY() { return resident_memory; }
  /* not available yet under WIN32 platform */
  inline static Time VIRTUAL_TIMELAPSE() { return (virtual_ulapse + virtual_slapse); }

  static Time real_lapse;
  static Time virtual_ulapse;
  static Time virtual_slapse;

 private:
  static Time real_time;
  static Time virtual_utime;
  static Time virtual_stime;
  static long resident_memory;

#ifndef __WIN32__
  //  static std::clock_t res;
  static struct rusage res;
#endif
  static struct timeval tp;
};

} // end namespace ibex
#endif // __IBEX_TIMER_H__
