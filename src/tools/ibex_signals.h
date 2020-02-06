//============================================================================
//                                  I B E X
// File        : ibex_signals.h
// Author      : Antoine Marendet
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Feb 5, 2020
// Last Update : Feb 5, 2020
//============================================================================

#ifndef __IBEX_SIGNALS_H__
#define __IBEX_SIGNALS_H__

#include "ibex_Exception.h"

#ifdef HAVE_UNISTD_H
    #include <unistd.h>
#endif
// if _POSIX_VERSION is defined in unistd.h, we can use the POSIX-1 specification
#if defined(HAVE_SIGNAL_H) && defined(_POSIX_VERSION)
    #include <signal.h>
    #define IBEX_POSIX_SIGNALS
#endif

namespace ibex {

/**
 * \brief Helper functions for OS signals handling
 */
namespace signals {

class SignalSIGINT: Exception { };

/**
 * \brief Return true if the current version of Ibex supports signals.
 */
constexpr bool support_signals() {
#ifdef IBEX_POSIX_SIGNALS
    return true;
#else
    return false;
#endif
}

/**
 * \brief Activate or deactivate SIGINT catching on POSIX compliant systems.
 *
 * Do not use in a multithreaded context.
 */
void set_catch_SIGINT(bool turn_on);

/**
 * \brief Throw SignalSIGINT if a SIGINT has been received since the last call to this function.
 */
void check_SIGINT();

} // namespace signals
} // namespace ibex

#endif  // __IBEX_SIGNALS_H__