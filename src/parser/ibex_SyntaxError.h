//============================================================================
//                                  I B E X                                   
// File        : ibex_SyntaxError.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#ifndef __IBEX_SYNTAX_ERROR_H__
#define __IBEX_SYNTAX_ERROR_H__

#include <string.h>
#include <stdlib.h>
#include "ibex_Exception.h"

namespace ibex {

/**
 * \brief Syntax error exception.
 *
 * Throw when an input file contains a syntax error.
 */
class SyntaxError : public Exception {
 public:
  SyntaxError(const std::string& msg1, const char* token1=NULL, int line1=-1) : msg(msg1), token(token1? strdup(token1) : NULL), line(line1) { }

  SyntaxError(const SyntaxError& e) : msg(e.msg), token(e.token? strdup(e.token) : NULL), line(e.line) { }

  ~SyntaxError() { if (token) free((char*) token); }

  /** Error message */
  std::string msg;
  /** Nearest token */
  const char *token;
  /** Line number */
  int line;
};


std::ostream& operator<< (std::ostream& os, const SyntaxError& e);

} // end namespace ibex
#endif // __IBEX_SYNTAX_ERROR_H__
