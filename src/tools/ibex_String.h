//============================================================================
//                                  I B E X                                   
// File        : ibex_String.h
// Author      : Gilles Chabert, Simon Rohou
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 18, 2012
// Last Update : April 18, 2016
//============================================================================

#ifndef __IBEX_STRING_H_
#define __IBEX_STRING_H_

#include <cassert>
#include <string.h>
#include <stdio.h>
#include "ibex_Interval.h"

namespace ibex {

/**
 * \brief Write an index at the end of a string, surrounded with two
 * symbols (like '['...']' or '{'..'}', etc.), including the
 * '\0' terminal character.
 *
 * For example, append_index("foo",12) ---> "foo[12]"
 *
 * \warning Up to 6 digits are allowed.
 *
 * \return the new string. It has to be freed by the caller.
 */
char* append_index(const char* base, char lbracket, char rbracket, int index);

/**
 * \brief Generate a variable name (_x_i).
 */
char* next_generated_var_name();

/**
 * \brief Generate a function name (_f_i).
 */
char* next_generated_func_name();

/**
 * \brief Casts an Interval object into a C string.
 *
 * \param the Interval object to parse
 * \param precision number of digits to display, default: use of operator<< precision
 * 
 * \return the corresponding C string, e.g.: "[-0.215,53.2]" or "[ empty ]"
 */
const char* intv2str(const Interval& intv, int precision = -1);

/**
 * \brief Parses the C string str, interpreting its content as
 * an interval and returns the corresponding object.
 *
 * Note: unwanted spaces are removed before cast.
 *
 * \param str C string, e.g.: "[-0.215,53.2]" or "[ empty ]"
 * 
 * \return the corresponding Interval object
 */
const Interval str2intv(const char* str);

} // end namespace ibex
#endif // __IBEX_STRING_H__
