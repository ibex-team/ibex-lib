//============================================================================
//                                  I B E X
// File        : ibex_signals.cpp
// Author      : Antoine Marendet
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Feb 5, 2020
// Last Update : Feb 5, 2020
//============================================================================

#include "ibex_signals.h"
#include <iostream>

namespace ibex {

#ifdef IBEX_POSIX_SIGNALS
namespace {
    // 0 if no SIGINT received, 1 otherwise
    sig_atomic_t sigint = 0;
    // sigaction handler
    struct sigaction sigint_handler;
    /* Store the old sigaction handler, to reset the manager to its previous behaviour
     * when deactivating signals
     */
    struct sigaction* old_sigaction = NULL;

    // sigaction action to perform when receiving SIGINT
    void sigint_action(int signal_value) {
        pid_t pid = getpid();
        std::cout << " SIGINT received on PID " << pid << std::endl;
        sigint = 1;
    }

} // anonymous namespace
#endif // _POSIX_VERSION

namespace signals {

void set_catch_SIGINT(bool turn_on) {
#ifdef IBEX_POSIX_SIGNALS
    if(turn_on) {
        sigint_handler.sa_handler = sigint_action;
	    sigemptyset(&sigint_handler.sa_mask);
	    sigint_handler.sa_flags = 0;
        sigaction(SIGINT, &sigint_handler, old_sigaction);
    } else {
        sigaction(SIGINT, old_sigaction, NULL);
    }
#else
    if(turn_on) {
        ibex_warning("Signals not supported on this system.");
    }
#endif
}

void check_SIGINT() {
#ifdef IBEX_POSIX_SIGNALS
    if(sigint) {
        // Clear sigint
        sigint = 0;
        throw SignalSIGINT();
    }
#endif
}

} // namespace signals

} // namespace ibex