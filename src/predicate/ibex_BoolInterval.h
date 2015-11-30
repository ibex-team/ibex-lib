//============================================================================
//                                  I B E X                                   
// File        : ibex_BoolInterval.h
// Author      : Gilles Chabert, Luc Jaulin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 27, 2012
// Last Update : Sep 29, 2015
//============================================================================

#ifndef __IBEX_BOOL_INTERVAL_H__
#define __IBEX_BOOL_INTERVAL_H__
#include <iostream>

namespace ibex {

/**
 * \defgroup predicate Predicates
 */

/** \ingroup predicate */
/*@{*/

/**
 * \brief Boolean interval.
 */
typedef enum { EMPTY_BOOL, NO, YES, MAYBE } BoolInterval;

/**
 * \brief Set x to the intersection with y
 */
BoolInterval& operator&=(BoolInterval& x, const BoolInterval& y);

/**
 * \brief Set x to the union with y
 */
BoolInterval& operator|=(BoolInterval& x, const BoolInterval& y);

/**
 * \brief Negation
 */
BoolInterval operator!(const BoolInterval& x);

/**
 * \brief Logical AND
 *
 * \warning Not to be confused with intersection
 */
BoolInterval operator&&(const BoolInterval&, const BoolInterval&);

/**
 * \brief Logical OR
 *
 * \warning Not to be confused with union
 */
BoolInterval operator||(const BoolInterval&, const BoolInterval&);

/**
 * \brief Logical exclusive OR (XOR)
 */
BoolInterval operator^(const BoolInterval&, const BoolInterval&);

/**
 * \brief Intersection
 *
 * \warning Not to be confused with logical AND (&&)
 */
BoolInterval operator&(const BoolInterval&, const BoolInterval&);

/**
 * \brief Union
 *
 * \warning Not to be confused with logical OR (||)
 */
BoolInterval operator|(const BoolInterval&, const BoolInterval&);

///**
// * \brief Project the constraint x<=y with respect to x
//
// [gch]: I don't exactly understand what these functions do
// */
//BoolInterval leq(const BoolInterval& x, const BoolInterval& y);
//
///**
// * \brief Project the constraint x>=y with respect to x
// */
//BoolInterval geq(const BoolInterval& x, const BoolInterval& y);

/**
 * \brief Return x and !y
 */
BoolInterval restrict(const BoolInterval& x, const BoolInterval& y);

/**
 * \brief Steams out a boolean interval
 */
std::ostream& operator<< (std::ostream& os, const BoolInterval&);

/*@}*/

/*================================== inline implementations ========================================*/

inline BoolInterval& operator&=(BoolInterval& x, const BoolInterval& y) {
	return (x = (x & y));
}

inline BoolInterval& operator|=(BoolInterval& x, const BoolInterval& y) {
	return (x = (x | y));
}

inline BoolInterval operator!(const BoolInterval& x) {
	if (x == YES) return NO;
	if (x == NO) return YES;
	return x;
}

inline BoolInterval operator&&(const BoolInterval& x, const BoolInterval& y) {
	if ((x == EMPTY_BOOL) || (y == EMPTY_BOOL)) return EMPTY_BOOL;
	if ((x == NO)    || (y == NO))    return NO;
	if ((x == MAYBE) || (y == MAYBE)) return MAYBE;
	return YES;
}

inline BoolInterval operator||(const BoolInterval& x, const BoolInterval& y) {
	if ((x == EMPTY_BOOL) || (y == EMPTY_BOOL)) return EMPTY_BOOL;
	if ((x == YES)   || (y == YES))   return YES;
	if ((x == MAYBE) || (y == MAYBE)) return MAYBE;
	return NO;
}

inline BoolInterval operator^(const BoolInterval& x, const BoolInterval& y) {
	if (x == EMPTY_BOOL)   return EMPTY_BOOL;
	if (y == EMPTY_BOOL)   return EMPTY_BOOL;
	if (x == MAYBE)   return MAYBE;
	if (y == MAYBE)   return MAYBE;
	if (x == y)   return NO;
	return YES;
}

inline BoolInterval operator&(const BoolInterval& x, const BoolInterval& y) {
	if (x == y)       return x;
	if (x == MAYBE)   return y;
	if (y == MAYBE)   return x;
	return EMPTY_BOOL;
}

inline BoolInterval operator|(const BoolInterval& x, const BoolInterval& y) {
	if (x == EMPTY_BOOL)   return y;
 	if (y == EMPTY_BOOL)   return x;
	if (x == y)       return x;
	return MAYBE;
}

//inline BoolInterval geq(const BoolInterval& x, const BoolInterval& y) {
//	BoolInterval     r=MAYBE;
//	if (y == EMPTY_BOOL)  r=EMPTY_BOOL;
//	if (y == YES)    r=YES;
//	return x & r;
//}
//
//inline BoolInterval leq(const BoolInterval& x, const BoolInterval& y) {
//	BoolInterval    r=MAYBE;
//	if (y == EMPTY_BOOL) r=EMPTY_BOOL;
//	if (y == NO)    r=NO;
//	return x & r;

/*                               a     &&    (implique b)
1*0=\EMPTY_BOOL                       1     &&      0
1*1=1                            1     &&     [0,1]
1*[0,1]=1                        1     &&     [0,1]
0*0=0                            0     &&      0
0*1=0                            0     &&      [0,1]
0*[0,1]=0                        0     &&      [0,1]
[0,1]*0=0                        [0,1] &&       0
[0,1]*1=[0,1]                    [0,1] &&      [0,1]
[0,1]*[0,1]=[0,1]                [0,1] &&     [0,1]
*/
//}

inline BoolInterval restrict(const BoolInterval& x, const BoolInterval& y) {
	return x && (!y);
}

inline std::ostream& operator<<(std::ostream& os, const BoolInterval& x) {
	if      (x==YES)    os << "  YES";
	if      (x==NO)     os << "   NO";
	if      (x==MAYBE)  os << "MAYBE";
	return os;
}

} // namespace ibex

#endif // __IBEX_BOOL_INTERVAL_H__
