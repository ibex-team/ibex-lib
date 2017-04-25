/* ============================================================================
 * I B E X - Set-membership operations
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 13, 2014
 * ---------------------------------------------------------------------------- */

#include "ibex_IntervalMatrix.h"

namespace ibex {

// just for defining a hierarchy of types
class __Real {
public:
	typedef double SCALAR;
	typedef Vector VECTOR;
	typedef Matrix MATRIX;
};

inline bool basic_is_subset(const Interval& x, const Interval& y) {
	return (y.lb()<=x.lb() && y.ub()>=x.ub());
}

// 0= not subset
// 1=subset
// 2=strict subset
inline int basic_is_strict_subset(const Interval& x, const Interval& y) {
	if (y.lb()<x.lb())
		if (y.ub()>=x.ub()) return 2;
		else return 0;
	else if (y.ub()>x.ub())
		if (y.lb()<=x.lb()) return 2;
		else return 0;
	else if (basic_is_subset(x,y)) return 1;
	else return 0;
}

inline bool basic_is_interior_subset(const Interval& x, const Interval& y) {
	return (y.lb()==NEG_INFINITY || y.lb()<x.lb()) &&
		   (y.ub()==POS_INFINITY || y.ub()>x.ub());
}

inline bool basic_is_relative_interior_subset(const Interval& x, const Interval& y) {
	return (y.is_degenerated() && x==y) || basic_is_interior_subset(x,y);
}

// 0= not interior subset
// 1=interior subset
// 2=strict interior subset
inline int basic_is_strict_interior_subset(const Interval& x, const Interval& y) {
	if (y.lb()<x.lb())
		if (y.ub()==POS_INFINITY || y.ub()>x.ub()) return 2;
		else return 0;
	else if (y.ub()>x.ub())
		if (y.lb()==NEG_INFINITY || y.lb()<x.lb()) return 2;
		else return 0;
	else if (basic_is_interior_subset(x,y)) return 1;
	else return 0;
}

inline bool basic_is_superset(const Interval& x, const Interval& y) {
	return basic_is_subset(y,x);
}

inline bool basic_is_strict_superset(const Interval& x, const Interval& y) {
	return basic_is_strict_subset(y,x);
}

inline bool basic_contains(const Interval& x, double d) {
	return d>=x.lb() && d<=x.ub();
}

inline bool basic_interior_contains(const Interval& x, double d) {
	return d>x.lb() && d<x.ub();
}

inline bool basic_intersects(const Interval& x, const Interval& y) {
	return x.lb()<=y.ub() && x.ub()>=y.lb();
}

// 0=not intersect
// 1=intersect
// 2=overlap
inline int basic_overlaps(const Interval& x, const Interval& y) {
	if (x.ub()>y.lb())
		if (y.ub()>x.lb()) return 2;
		else if (y.ub()==x.lb()) return 1;
		else return 0;
	else if (x.ub()==y.lb()) return 1;
	else return 0;
}

inline bool basic_is_disjoint(const Interval& x, const Interval& y) {
	return x.lb()>y.ub() || x.ub()<y.lb();
}

//inline bool basic_is_degenerated(const Interval& x) {
//	return x.lb()==x.ub();
//}
//
//inline bool basic_is_unbounded(const Interval& x) {
//	return x.lb()==NEG_INFINITY || x.ub()==POS_INFINITY;
//}

#define __IBEX_GENERATE_BASIC_SET_OP_AND__(S1,S2,BASIC_COND) \
  \
  inline bool BASIC_COND(const S1::VECTOR& x, const S2::VECTOR& y) { \
	assert(x.size()==y.size()); \
    for (int i=0; i<x.size(); i++) \
      if (!BASIC_COND(x[i],y[i])) return false; \
    return true; \
  } \
  \
  inline bool BASIC_COND(const S1::MATRIX& x, const S2::MATRIX& y) { \
	  assert(x.nb_rows()==y.nb_rows()); \
	  assert(x.nb_cols()==y.nb_cols()); \
	  for (int i=0; i<x.nb_rows(); i++) \
        for (int j=0; j<x.nb_cols(); j++) \
          if (!BASIC_COND(x[i][j],y[i][j])) return false; \
    return true; \
  }

#define __IBEX_GENERATE_BASIC_SET_OP_OR__(S1,S2,BASIC_COND) \
  \
  inline bool BASIC_COND(const S1::VECTOR& x, const S2::VECTOR& y) { \
	assert(x.size()==y.size()); \
    for (int i=0; i<x.size(); i++) \
      if (BASIC_COND(x[i],y[i])) return true; \
    return false; \
  } \
  \
  inline bool BASIC_COND(const S1::MATRIX& x, const S2::MATRIX& y) { \
	assert(x.nb_rows()==y.nb_rows()); \
	assert(x.nb_cols()==y.nb_cols()); \
	for (int i=0; i<x.nb_rows(); i++) \
      for (int j=0; j<x.nb_cols(); j++) \
        if (BASIC_COND(x[i][j],y[i][j])) return true; \
    return false; \
  }

#define __IBEX_GENERATE_BASIC_SET_OP_AND_OR__(S1,S2,BASIC_COND) \
  \
  inline int BASIC_COND(const S1::VECTOR& x, const S2::VECTOR& y) { \
	assert(x.size()==y.size()); \
	int cond=1; \
    for (int i=0; i<x.size(); i++) { \
      cond *= BASIC_COND(x[i],y[i]); \
      if (cond==0) return 0; \
    } \
    return cond; \
  } \
  \
  inline int BASIC_COND(const S1::MATRIX& x, const S2::MATRIX& y) { \
	assert(x.nb_rows()==y.nb_rows()); \
	assert(x.nb_cols()==y.nb_cols()); \
	int cond=1; \
	for (int i=0; i<x.nb_rows(); i++) \
      for (int j=0; j<x.nb_cols(); j++) { \
          cond *= BASIC_COND(x[i][j],y[i][j]); \
          if (cond==0) return 0; \
      } \
    return cond; \
  }

__IBEX_GENERATE_BASIC_SET_OP_AND__   (Interval,Interval, basic_is_subset)
__IBEX_GENERATE_BASIC_SET_OP_AND_OR__(Interval,Interval, basic_is_strict_subset)
__IBEX_GENERATE_BASIC_SET_OP_AND__   (Interval,Interval, basic_is_interior_subset)
__IBEX_GENERATE_BASIC_SET_OP_AND__   (Interval,Interval, basic_is_relative_interior_subset)
__IBEX_GENERATE_BASIC_SET_OP_AND_OR__(Interval,Interval, basic_is_strict_interior_subset)
__IBEX_GENERATE_BASIC_SET_OP_AND__   (Interval,__Real,   basic_contains)
__IBEX_GENERATE_BASIC_SET_OP_AND__   (Interval,__Real,   basic_interior_contains)
__IBEX_GENERATE_BASIC_SET_OP_AND__   (Interval,Interval, basic_intersects)
__IBEX_GENERATE_BASIC_SET_OP_AND_OR__(Interval,Interval, basic_overlaps)
__IBEX_GENERATE_BASIC_SET_OP_OR__    (Interval,Interval, basic_is_disjoint)

template<typename T>
inline bool is_subset(const T& x, const T& y) {
	return x.is_empty() || (!y.is_empty() && basic_is_subset(x,y));
}

template<typename T>
inline bool is_strict_subset(const T& x, const T& y) {
	// basic_is_strict_subset(x,y)=2^n where n is the number of components
	// satisfying strict inclusion
	return !y.is_empty() && (x.is_empty() || (basic_is_strict_subset(x,y)>=2));
}

template<typename T>
inline bool is_interior_subset(const T& x, const T& y) {
	return x.is_empty() || (!y.is_empty() && basic_is_interior_subset(x,y));
}

template<typename T>
inline bool is_relative_interior_subset(const T& x, const T& y) {
	return x.is_empty() || (!y.is_empty() && basic_is_relative_interior_subset(x,y));
}

template<typename T>
inline bool is_strict_interior_subset(const T& x, const T& y) {
	return !y.is_empty() && (x.is_empty() || (basic_is_strict_interior_subset(x,y)>=2));
}

template<typename T>
inline bool is_superset(const T& x, const T& y) {
	return is_subset(y,x);
}

template<typename T>
inline bool is_strict_superset(const T& x, const T& y) {
	return is_strict_subset(y,x);
}

template<typename T,typename S>
inline bool contains(const T& x, const S& y) {
	return !x.is_empty() && basic_contains(x,y);
}

template<typename T,typename S>
inline bool interior_contains(const T& x, const S& y) {
	return !x.is_empty() && basic_interior_contains(x,y);
}

template<typename T,typename S>
inline bool intersects(const T& x, const S& y) {
	return !x.is_empty() && !y.is_empty() && basic_intersects(x,y);
}

template<typename T,typename S>
inline bool overlaps(const T& x, const S& y) {
	return !x.is_empty() && !y.is_empty() && (basic_overlaps(x,y)>=2);
}

template<typename T,typename S>
inline bool is_disjoint(const T& x, const S& y) {
	return x.is_empty() || y.is_empty() || basic_is_disjoint(x,y);
}

#define __IBEX_GENERATE_SET_OP__(S2,COND) \
  \
  bool Interval::COND(const S2::SCALAR& x) const                  { return ibex::COND(*this,x); } \
  bool IntervalVector::COND(const S2::VECTOR& x) const            { return ibex::COND(*this,x); } \
  bool IntervalMatrix::COND(const S2::MATRIX& x) const            { return ibex::COND(*this,x); }

__IBEX_GENERATE_SET_OP__(Interval,is_subset)
__IBEX_GENERATE_SET_OP__(Interval,is_strict_subset)
__IBEX_GENERATE_SET_OP__(Interval,is_interior_subset)
__IBEX_GENERATE_SET_OP__(Interval,is_relative_interior_subset)
__IBEX_GENERATE_SET_OP__(Interval,is_strict_interior_subset)
__IBEX_GENERATE_SET_OP__(Interval,is_superset)
__IBEX_GENERATE_SET_OP__(Interval,is_strict_superset)
__IBEX_GENERATE_SET_OP__(__Real,contains)
__IBEX_GENERATE_SET_OP__(__Real,interior_contains)
__IBEX_GENERATE_SET_OP__(Interval,intersects)
__IBEX_GENERATE_SET_OP__(Interval,overlaps)
__IBEX_GENERATE_SET_OP__(Interval,is_disjoint)

} // namespace ibex

