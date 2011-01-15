/*---------------------------------------------------------------------------------
 * 
 * Exceptions
 * ---------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#ifndef _IBEX_EXCEPTION_H
#define _IBEX_EXCEPTION_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>

namespace ibex {

/**\defgroup exceptgroup IbexExceptions*/
/*@{*/

/** \ingroup exceptgroup
 *
 * \brief Base class for all exceptions raised by ibex. 
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class IbexException { };


/**
 * \brief Unfeasibility exception.
 *
 * Throw when unfeasibility is detected.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class EmptyBoxException : public IbexException { };

/** 
 * \brief Non recoverable exception
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class NonRecoverableException : public IbexException {
 public:
  NonRecoverableException(const std::string& message) : message(message) { }

  NonRecoverableException(const NonRecoverableException& error) : message(error.message) { }

  std::string message;  
};

/** 
 * \brief Evaluation exception
 *
 * IbexException related to evaluations (i.e., projection, derivation, etc...) of expressions.
 * All these exceptions may be raised from an environment or a space.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class EvalException : public virtual IbexException { };


/*================================================================================*/
/*                                 EVALUATION                                     */
/*================================================================================*/

/** 
 * \brief Differentiability exception
 *
 * Throw when the function is not differentiable.
 * This happens in the very particular cases where, e.g., the function to derivate is 
 * \f$ x \mapsto \sqrt{x}\f$ and [x]=[0,0].
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class NotDifferentiableException : public EvalException { };


/** 
 * \brief Unbounded result exception
 *
 * Throw when an unbounded interval is computed. This allows to save computation
 * time in situations where unbounded intervals leads anyway to abort the calling
 * procedure (such as the computation of a jacobian matrix).
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class UnboundedResultException : public EvalException { };

/*================================================================================*/
/*                                LINEAR ALGEBRA                                    */
/*================================================================================*/

/** 
 * \brief IbexExceptions related to linear systems
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class LinearException : public IbexException { };

/** 
 * \brief Not square matrix exception
 *
 * Thrown when a rectangular matrix is found instead of a square one as expected.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class NotSquareMatrixException : public LinearException { };

/**
 * \brief Singularity exception
 *
 * Throw in case of singularity.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class SingularMatrixException : public LinearException { };

/**
 * \brief Null Gauss pivot exception
 *
 * Throw in case of null pivot is Gauss elimination techniques.
 * \author Gilles Chabert
 * \date March 2007
 */
class NullPivotException : public LinearException { };


/**
 * \brief Not an inverse-positive matrix exception
 *
 * Throw by methods such as Hansen-Bliek.
 *
 * \author Gilles Chabert
 * \date Sept 2009
 */
class NotInversePositivieMatrixException : public LinearException { };

/*================================================================================*/
/*                                    SOLVER                                      */
/*================================================================================*/

/** 
 * \brief Paver exception
 * \author Gilles Chabert
 * \date October 2007 
 */
class PaverException : public IbexException { };

/** 
 * \brief Overflow exception.
 *
 * Thrown when the number of boxes stored by a solver exceeds
 * the limit. See \link ibex::Paver::capacity capacity \endlink.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class OverflowException : public PaverException { };

/**
 * \brief Paver timeout exception.
 *
 * Thrown when the number of cells created by the solver exceeds
 * the limit. See \link ibex::Paver::cell_limit \endlink.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class TimeOutException : public PaverException { };


/*================================================================================*/
/*                                    PARSER                                      */
/*================================================================================*/

/** 
 * \brief Unknown file exception.
 *
 * Thrown when a system is trying to be loaded from an unknown file.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class UnknownFileException : public IbexException { 
 public:
  UnknownFileException(const char* name) : filename(strdup(name)) { }

  UnknownFileException(const UnknownFileException& e) : filename(strdup(e.filename)) { }

  ~UnknownFileException() { free((char*) filename); }

  const char* filename;
};

/** 
 * \brief Syntax error exception.
 *
 * Throw when an input file contains a syntax error.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class SyntaxErrorException : public IbexException {
 public:
  SyntaxErrorException(const std::string& msg, const char* token=NULL, int line=-1) : msg(msg), token(token? strdup(token) : NULL), line(line) { }

  SyntaxErrorException(const SyntaxErrorException& e) : msg(e.msg), token(e.token? strdup(e.token) : NULL), line(e.line) { }

  ~SyntaxErrorException() { if (token) free((char*) token); }

  /** Error message */
  std::string msg;
  /** Nearest token */
  const char *token;
  /** Line number */
  int line;  
};

inline std::ostream& operator<< (std::ostream& os, const SyntaxErrorException& e) {
  os << e.msg;
  if (e.token) os << " near \"" << e.token << "\"";
  if (e.line!=-1) os << " line " << e.line;
  return os;
}

inline std::ostream& operator<< (std::ostream& os, const UnknownFileException& e) {
  return os << "could not open file " << e.filename;
}
inline std::ostream& operator<< (std::ostream& os, const NonRecoverableException& e) {
  return os << e.message;
}

/*@}*/

} // end namespace ibex

#endif
