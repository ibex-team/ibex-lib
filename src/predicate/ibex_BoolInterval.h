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

/**
 * \defgroup predicate Predicates
 */
#include <iostream>

namespace ibex {

/**
 * \ingroup predicate
 *
 * \brief Boolean interval.
 */
typedef enum { NO, MAYBE, YES, EMPTY} IBOOLEAN;

class BoolInterval {
public:
	IBOOLEAN value;

	/**
	 * \brief Create the interval MAYBE
	 */
	BoolInterval();

	/**
	 * \brief Create the interval YES (true) or NO (false)
	 */
	BoolInterval(bool);

	/*
	 * \brief Create an interval from a IBOOLEAN value
	 */
	BoolInterval(IBOOLEAN);

	/**
	 * \brief Copy a boolean interval
	 */
	BoolInterval(const BoolInterval&);

	/**
	 * \brief Equality test
	 */
	bool operator==(const BoolInterval&) const;

	/**
	 * \brief Non-equality test
	 */
	bool operator!=(const BoolInterval&) const;

	/**
	 * \brief Set this interval to the intersection with another
	 */
	BoolInterval& operator&=(const BoolInterval&);
	
	/**
	 * \brief Set this interval to the union with another
	 */
	BoolInterval& operator|= (const BoolInterval&);
	
	/**
	 * \brief Negation
	 */
	BoolInterval operator!() const;
	
};


/**
 * \brief Logical AND
 *
 * \warning Not to be confused with intersection
 */
BoolInterval operator&&(const BoolInterval&,const BoolInterval&);

/**
 * \brief Logical OR
 *
 * \warning Not to be confused with union
 */
BoolInterval operator||(const BoolInterval&,const BoolInterval&);

/**
 * \brief Intersection
 *
 * \warning Not to be confused with logical AND
 */
BoolInterval operator&(const BoolInterval&, const BoolInterval&);

/**
 * \brief Union
 *
 * \warning Not to be confused with logical OR
 */
BoolInterval operator|(const BoolInterval&, const BoolInterval&);

BoolInterval Not(const BoolInterval& x);

BoolInterval leq(const BoolInterval& x, const BoolInterval& y);

/**
 * \brief Project the constraint x>=y with respect to x
 */
BoolInterval geq(const BoolInterval& x, const BoolInterval& y);

/**
 * \brief Return x and !y
 */
BoolInterval restrict(const BoolInterval& x, const BoolInterval& y);

BoolInterval Xor(const BoolInterval& x, const BoolInterval& y);

std::ostream& operator<< (std::ostream& os, const BoolInterval&);

//======================================================================
//============== INLINE IMPLEMENTATION =================================
//======================================================================


inline BoolInterval::BoolInterval() {
	value = MAYBE;
}

inline BoolInterval::BoolInterval(bool b) {
	if (b) value=YES; else value=NO;
}

inline BoolInterval::BoolInterval(IBOOLEAN tt) {
	value = tt;
}

inline BoolInterval::BoolInterval(const BoolInterval& t) {
	*this = t;
}

inline BoolInterval& BoolInterval::operator&= (const BoolInterval& y) {
	this->value = (*this & y).value;
	return *this;
}

inline BoolInterval& BoolInterval::operator|= (const BoolInterval& y) {
	this->value = (*this | y).value;
	return *this;
}

inline bool BoolInterval::operator== (const BoolInterval& y) const {
	return (this->value==y.value);
}

inline bool BoolInterval::operator!= (const BoolInterval& y) const {
	return (this->value!=y.value);
}

inline BoolInterval BoolInterval::operator! () const {
	if (this->value == YES) return BoolInterval(NO);
	if (this->value == NO) return BoolInterval(YES);
	return this;
}

inline std::ostream& operator<< (std::ostream& os, const BoolInterval& a) {
	if      (a.value==YES)    os<<"  YES";
	if      (a.value==NO)     os<<"   NO";
	if      (a.value==MAYBE)  os<<"MAYBE";
	return os;
}

inline BoolInterval Not(const BoolInterval& x) {
	if (x == MAYBE)   return MAYBE;
	if (x == YES)      return NO;
	if (x == NO)     return YES;
	return EMPTY;
}

inline BoolInterval operator&&(const BoolInterval& x, const BoolInterval& y) {
	if ((x==EMPTY)||(y==EMPTY)) return BoolInterval(EMPTY);
	if ((x==NO)||(y==NO)) return BoolInterval(NO);
	if ((x==MAYBE)||(y==MAYBE)) return BoolInterval(MAYBE);
	return BoolInterval(YES);
}

inline BoolInterval operator||(const BoolInterval& x, const BoolInterval& y) {
	if ((x==EMPTY)||(y==EMPTY))     return BoolInterval(EMPTY);
	if ( (x==YES) || (y==YES) )     return BoolInterval(YES);
	if ( (x==MAYBE) || (y==MAYBE) ) return BoolInterval(MAYBE);
	return BoolInterval(NO);
}

inline BoolInterval operator&(const BoolInterval& x, const BoolInterval& y) {
	if (x == y)       return x;
	if (x == MAYBE)   return y;
	if (y == MAYBE)   return x;
	return BoolInterval(EMPTY);
}

inline BoolInterval operator|(const BoolInterval& x, const BoolInterval& y) {
	if (x == EMPTY)   return y;
 	if (y == EMPTY)   return x;
	if (x == y)       return x;
	return BoolInterval(MAYBE);
}

inline BoolInterval Xor(const BoolInterval& x,const BoolInterval& y) {
	if (x == EMPTY)   return EMPTY;
	if (y == EMPTY)   return EMPTY;
	if (x == MAYBE)   return MAYBE;
	if (y == MAYBE)   return MAYBE;
	if (x == y)   return NO;
	return BoolInterval(YES);
}

inline BoolInterval geq(const BoolInterval& x, const BoolInterval& y) {
	BoolInterval r;
        r=BoolInterval(MAYBE);
        if (y==EMPTY)  r=BoolInterval(EMPTY);
	if (y==YES) r=BoolInterval(YES);
        return inter(x,r);
}

inline BoolInterval leq(const BoolInterval& x, const BoolInterval& y) {
	BoolInterval r;
        r=BoolInterval(MAYBE);
        if (y==EMPTY)  r=BoolInterval(EMPTY);
	if (y==NO) r=BoolInterval(NO);
        return inter(x,r);
/*                               a     &&    (implique b)
1*0=\EMPTY                       1     &&      0
1*1=1                            1     &&     [0,1]
1*[0,1]=1                        1     &&     [0,1]
0*0=0                            0     &&      0
0*1=0                            0     &&      [0,1]
0*[0,1]=0                        0     &&      [0,1]
[0,1]*0=0                        [0,1] &&       0
[0,1]*1=[0,1]                    [0,1] &&      [0,1]
[0,1]*[0,1]=[0,1]                [0,1] &&     [0,1]
*/
}

inline BoolInterval restrict(const BoolInterval& x, const BoolInterval& y) {
	return x && (!y);
}

} // namespace ibex

#endif // __IBEX_BOOL_INTERVAL_H__
