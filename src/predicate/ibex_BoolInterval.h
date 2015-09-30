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
	BoolInterval ();
	BoolInterval (bool);
	BoolInterval (IBOOLEAN );
	BoolInterval (const BoolInterval&);

	bool		 			operator== (const BoolInterval& ) const;
	bool		 			operator!= (const BoolInterval& ) const;	
	
	// And
	BoolInterval  operator&&  (const BoolInterval&) const;
	// Or
	BoolInterval  operator||  (const BoolInterval&) const;
	// Intersection
	BoolInterval  operator&  (const BoolInterval&) const;
	BoolInterval& operator&= (const BoolInterval&);
	// Union
	BoolInterval  operator|  (const BoolInterval&) const;
	BoolInterval& operator|= (const BoolInterval&);
	// Neg
	BoolInterval  operator!  () const; 
	
	
};

std::ostream& operator<< (std::ostream& os, const BoolInterval&);
BoolInterval Inter(const BoolInterval&,const BoolInterval&);
BoolInterval Not(const BoolInterval& x);
BoolInterval Union(const BoolInterval&, const BoolInterval&);
BoolInterval And(const BoolInterval&,const BoolInterval&);
BoolInterval Or(const BoolInterval&,const BoolInterval&);
BoolInterval leq(const BoolInterval& x, const BoolInterval& y);
BoolInterval geq(const BoolInterval& x, const BoolInterval& y);
BoolInterval Restrict(const BoolInterval& x, const BoolInterval& y);      // return x and !y
BoolInterval Xor(const BoolInterval& x, const BoolInterval& y);

//======================================================================
//============== INLINE IMPLEMENTATION =================================
//======================================================================


inline BoolInterval::BoolInterval ()
{value = MAYBE;}

inline BoolInterval::BoolInterval (bool b)
{if (b) value=YES; else value=NO;}

inline BoolInterval::BoolInterval (IBOOLEAN tt) {value = tt;}

inline BoolInterval::BoolInterval (const BoolInterval& t)
{*this = t;}

inline BoolInterval BoolInterval::operator&& (const BoolInterval& y) const          
{    return And(*this,y);  }

inline BoolInterval BoolInterval::operator|| (const BoolInterval& y) const          
{    return Or(*this,y); }

inline BoolInterval BoolInterval::operator& (const BoolInterval& y) const                        
{    return Inter(*this,y);  }

inline BoolInterval BoolInterval::operator| (const BoolInterval& y) const        
{    return Union(*this,y); }

inline BoolInterval& BoolInterval::operator&= (const BoolInterval& y)
{    this->value = (*this&y).value; return  *this; }

inline BoolInterval& BoolInterval::operator|= (const BoolInterval& y)
{    this->value = (*this|y).value; return  *this; }

inline bool BoolInterval::operator== (const BoolInterval& y) const
{	return (this->value==y.value);}

inline bool BoolInterval::operator!= (const BoolInterval& y) const
{	return (this->value!=y.value);}

inline BoolInterval BoolInterval::operator! () const {
	if (this->value == YES) return BoolInterval(NO);
	if (this->value == NO) return BoolInterval(YES);
	return this;
}


inline std::ostream& operator<< (std::ostream& os, const BoolInterval& a)
{
	if      (a.value==YES)    os<<"  YES";
	if      (a.value==NO)     os<<"   NO";
	if      (a.value==MAYBE)  os<<"MAYBE";
	return os;
}

inline BoolInterval Not(const BoolInterval& x){
	if (x == MAYBE)   return MAYBE;
	if (x == YES)      return NO;
	if (x == NO)     return YES;
	return EMPTY;
}

inline BoolInterval And(const BoolInterval& x, const BoolInterval& y){
	if ((x==EMPTY)||(y==EMPTY))   return BoolInterval(EMPTY);
	if ((x==NO)||(y==NO)) return BoolInterval(NO);
	if ((x==MAYBE)||(y==MAYBE)) return BoolInterval(MAYBE);
	return BoolInterval(YES);
}

inline BoolInterval Or(const BoolInterval& x, const BoolInterval& y){
	if ((x==EMPTY)||(y==EMPTY))   return BoolInterval(EMPTY);
	if ( (x==YES) || (y==YES) ) return BoolInterval(YES);
	if ( (x==MAYBE) || (y==MAYBE) ) return BoolInterval(MAYBE);
	return BoolInterval(NO);
}

inline BoolInterval Inter(const BoolInterval& x,const BoolInterval& y){
	if (x == y)   return x;
	if (x == MAYBE)   return y;
	if (y == MAYBE)   return x;
	return BoolInterval(EMPTY);
}

inline BoolInterval Union(const BoolInterval& x,const BoolInterval& y){
	if (x == EMPTY)   return y;
 	if (y == EMPTY)   return x;
	if (x == y)   return x;
	return BoolInterval(MAYBE);
}

inline BoolInterval Xor(const BoolInterval& x,const BoolInterval& y){
	if (x == EMPTY)   return EMPTY;
	if (y == EMPTY)   return EMPTY;
	if (x == MAYBE)   return MAYBE;
	if (y == MAYBE)   return MAYBE;
	if (x == y)   return NO;
	return BoolInterval(YES);
}

inline BoolInterval geq(const BoolInterval& x, const BoolInterval& y){     // Project the constraint x>=y with respect to x
	BoolInterval r;
        r=BoolInterval(MAYBE);
        if (y==EMPTY)  r=BoolInterval(EMPTY);
	if (y==YES) r=BoolInterval(YES);
        return Inter(x,r);
}

inline BoolInterval leq(const BoolInterval& x, const BoolInterval& y){      // Project the constraint x<=y with respect to x
	BoolInterval r;
        r=BoolInterval(MAYBE);
        if (y==EMPTY)  r=BoolInterval(EMPTY);
	if (y==NO) r=BoolInterval(NO);
        return Inter(x,r);
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

inline BoolInterval Restrict(const BoolInterval& x, const BoolInterval& y)      // return x and !y
{ return And(x,!y);}


} // namespace ibex

#endif // __IBEX_BOOL_INTERVAL_H__
