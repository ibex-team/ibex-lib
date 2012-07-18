//============================================================================
//                                  I B E X                                   
// File        : ibex_String.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 18, 2012
// Last Update : Jul 18, 2012
//============================================================================

#ifndef __IBEX_STRING_H_
#define __IBEX_STRING_H_

#include <cassert>
#include <string.h>
#include <stdio.h>

namespace ibex {

/**
 * \brief Write an index into a string, surrounded with two
 * symbols (like '['...']' or '{'..'}', etc.), including the
 * '\0' terminal character.
 *
 * For example, 12 ---> "[12]"
 *
 * \warning Up to 6 digits are allowed.
 *
 * \return the length of the string (the length does not take into
 * account the '\0' character.
 */
int index_2_string(char* buff, char lbracket, char rbracket, int index);

} // end namespace ibex
#endif // __IBEX_STRING_H__
