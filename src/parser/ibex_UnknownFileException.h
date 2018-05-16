//============================================================================
//                                  I B E X                                   
// File        : ibex_UnknownFileException.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 22, 2012
// Last Update : Jun 22, 2012
//============================================================================

#ifndef __IBEX_UNKNOWN_FILE_EXCEPTION_H__
#define __IBEX_UNKNOWN_FILE_EXCEPTION_H__

#include "ibex_Exception.h"

#include <string.h>

namespace ibex {
/**
 * \brief Unknown file exception.
 *
 * Thrown when a system is trying to be loaded from an unknown file.
 *
 */
class UnknownFileException : public Exception {
 public:
  UnknownFileException(const char* name) : filename(strdup(name)) { }

  UnknownFileException(const UnknownFileException& e) : filename(strdup(e.filename)) { }

  ~UnknownFileException() { free((char*) filename); }

  const char* filename;
};

} // end namespace ibex
#endif // __IBEX_UNKNOWN_FILE_EXCEPTION_H__
