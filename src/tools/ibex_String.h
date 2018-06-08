//============================================================================
//                                  I B E X                                   
// File        : ibex_String.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 18, 2012
// Last Update : Jan 17, 2018
//============================================================================

#ifndef __IBEX_STRING_H__
#define __IBEX_STRING_H__

#include "ibex_SymbolMap.h"
#include "ibex_Expr.h"

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
 *
 * Note: needs to be disallocated.
 */
char* next_generated_var_name();

/**
 * \brief Generate a function name (_f_i).
 *
 * Note: needs to be disallocated.
 *
 */
char* next_generated_func_name();

/**
 * \brief Convert a string to int or throw a SyntaxError exception.
 */
int parse_integer(const std::string& str);

/**
 * \brief Parse an indexed symbol.
 *
 * Example: "x[1]" (string) ---> x[1] (node).
 *
 * \warning Not all possible Minibex expressions for indexed symbols
 *          are allowed!  Examples:
 *          x(1:2)  ok
 *          x(:)    ok
 *          x(1+1)  *refused*  --> use x(2)
 *          x(1)(2) *refused*  --> use x(1,2)
 *
 * \param symbols - The symbols in a map structure
 * \param str     - The string to parse.
 *
 * \note If the resulting node is an indexed symbol, it has
 *       to be freed (call "cleanup(...,false)").
 *
 * Throw SyntaxError if "str" is invalid.
 */
const ExprNode& parse_indexed_symbol(const SymbolMap<const ExprSymbol*>& symbols, const std::string& str);


/**
 * \brief Alpha-numeric random string
 *
 * \param len - length of the string. A null
 *              character is added after the last.
 */
char* random_alphanum_string(int len);

} // end namespace ibex

#endif // __IBEX_STRING_H__
