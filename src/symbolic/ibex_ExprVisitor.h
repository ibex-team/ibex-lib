/* ============================================================================
 * I B E X - Interface for expression visitors
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 06, 2012
 * Last Update : May 04, 2020
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_EXPR_VISITOR_H__
#define __IBEX_EXPR_VISITOR_H__

#include "ibex_Expr.h"
#include "ibex_ExprOperators.h"
#include "ibex_NodeMap.h"

#include <type_traits>

namespace ibex {

/*
 * \ingroup symbolic
 * \brief Interface for expression visitors.
 *
 * Template arguments:
 *  - T: the return type of visit(..)
 * -  ArgTypes: the argument types of visit(...) (optional, can be any)
 *
 */

template<typename T, typename ... ArgTypes>
class ExprVisitor {

public:

	virtual ~ExprVisitor() { }

protected:

	template<class Q = T>
	typename std::enable_if<std::is_same<Q,void>::value, Q>::type visit_with_cache(std::true_type _is_void, const ExprNode& e, ArgTypes... args) {
		if (!cache.found(e)) {
			e.accept_visitor(*this,args...);
			cache.insert(e,true);
		}
	}

	template<class Q = T>
	typename std::enable_if<!std::is_same<Q,void>::value, Q>::type visit_with_cache(std::false_type _is_void, const ExprNode& e, ArgTypes... args) {
		typename IBEX_NODE_MAP(T)::iterator it = cache.find(e);
		if (it==cache.end()) {
			T t=e.accept_visitor(*this,args...);
			cache.insert(e, t);
			return t;
		} else {
			return it->second;
		}
	}

	/** Visit an expression. */
	virtual T visit(const ExprNode& e, ArgTypes... args) {
		return visit_with_cache(std::is_same<T,void>(), e, args...);
	}

  /** Visit an indexed expression. */
   virtual T visit(const ExprIndex& e, ArgTypes... args) {
	  return visit((const ExprNode&) e, args...);
  }

  /** Visit a leaf. */
   virtual T visit(const ExprLeaf& e, ArgTypes... args) {
	  return visit((const ExprNode&) e, args...);
  }

  /** Visit a n-ary operator. */
   virtual T visit(const ExprNAryOp& e, ArgTypes... args) {
	  return visit((const ExprNode&) e, args...);
  }

  /** Visit a binary operator. */
   virtual T visit(const ExprBinaryOp& e, ArgTypes... args) {
	  return visit((const ExprNode&) e, args...);
  }

  /** Visit an unary operator. */
   virtual T visit(const ExprUnaryOp& e, ArgTypes... args) {
	  return visit((const ExprNode&) e, args...);
  }

  /*===============================================================*/
  /*                optional functions                             */
  /*===============================================================*/

  /*==================== 0-ary operators =========================*/
  /** Visit a symbol. */
   virtual T visit(const ExprSymbol& e, ArgTypes... args) {
	   return visit((const ExprLeaf&) e, args...);
  }

  /** Visit a constant. */
   virtual T visit(const ExprConstant& e, ArgTypes... args) {
	   return visit((const ExprLeaf&) e, args...);
  }

  /*==================== n-ary operators =========================*/
  /** Visit a vector of expressions.
   * By default: call visit(const ExprNAryOp& e). */
    virtual T visit(const ExprVector& e, ArgTypes... args) {
	   return visit((const ExprNAryOp&) e, args...);
   }

   /** Visit a function application.
   * By default: call visit(const ExprNAryOp& e). */
    virtual T visit(const ExprApply& e, ArgTypes... args) {
	   return visit((const ExprNAryOp&) e, args...);
   }

   /** Visit a function Chi.
   * By default: call visit(const ExprNAryOp& e). */
    virtual T visit(const ExprChi& e, ArgTypes... args) {
	   return visit((const ExprNAryOp&) e, args...);
   }

  /*==================== binary operators =========================*/
   /** Visit a generic binary operator (Implementation is not mandatory).
    * By default: call visit(const ExprBinaryOp& e). */
    virtual T visit(const ExprGenericBinaryOp& e, ArgTypes... args) {
 	  return visit((const ExprBinaryOp&) e, args...);
   }

   /** Visit an addition (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
   virtual T visit(const ExprAdd& e, ArgTypes... args) {
	  return visit((const ExprBinaryOp&) e, args...);
  }

  /** Visit an multiplication (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
   virtual T visit(const ExprMul& e, ArgTypes... args) {
	  return visit((const ExprBinaryOp&) e, args...);
  }

  /** Visit a subtraction (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
   virtual T visit(const ExprSub& e, ArgTypes... args) {
	  return visit((const ExprBinaryOp&) e, args...);
  }

  /** Visit a division (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
   virtual T visit(const ExprDiv& e, ArgTypes... args) {
	  return visit((const ExprBinaryOp&) e, args...);
  }

  /** Visit a max  (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
   virtual T visit(const ExprMax& e, ArgTypes... args) {
	  return visit((const ExprBinaryOp&) e, args...);
  }

  /** Visit a min (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
   virtual T visit(const ExprMin& e, ArgTypes... args) {
	  return visit((const ExprBinaryOp&) e, args...);
  }

  /** Visit a arctan2 (Implementation is not mandatory).
   * By default: call visit(const ExprBinaryOp& e). */
   virtual T visit(const ExprAtan2& e, ArgTypes... args) {
	  return visit((const ExprBinaryOp&) e, args...);
  }


  /*==================== unary operators =========================*/

  /** Visit a generic unary operator (Implementation is not mandatory).
     * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprGenericUnaryOp& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a minus sign (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprMinus& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a transpose (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprTrans& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a sign (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprSign& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit an abs (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprAbs& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a power expression.
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprPower& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a sqr (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprSqr& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a sqrt (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprSqrt& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit an exp (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprExp& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a log (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprLog& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a cos (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprCos& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a sin (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprSin& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a tan (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprTan& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a cosh (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprCosh& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a sinh (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprSinh& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a tanh (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprTanh& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a acos (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprAcos& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a asin (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprAsin& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a atan (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprAtan& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a acosh (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprAcosh& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a asinh (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprAsinh& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a atanh (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprAtanh& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a floor (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprFloor& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a ceil (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprCeil& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

  /** Visit a saw (Implementation is not mandatory).
   * By default call visit(const ExprUnaryOp&). */
   virtual T visit(const ExprSaw& e, ArgTypes... args) {
	  return visit((const ExprUnaryOp&) e, args...);
  }

 protected:

   friend class ExprNode;

   /**
    * Cache for data associated to nodes.
    * The data is a simple "bool" in case of T==void
    * (used to mark visited nodes).
    */
  NodeMap<typename std::conditional<std::is_same<T,void>::value,bool,T>::type> cache;
};

template<typename T, typename ... ArgTypes>
T ExprNode::accept_visitor(ExprVisitor<T,ArgTypes...>& v, ArgTypes&&... args) const {
	switch(type_id()) {
	case NumExprIndex: 			return v.visit((const ExprIndex&) *this, args...); break;
	case NumExprSymbol: 		return v.visit((const ExprSymbol&) *this, args...); break;
	case NumExprConstant: 		return v.visit((const ExprConstant&) *this, args...); break;
	case NumExprVector: 		return v.visit((const ExprVector&) *this, args...); break;
	case NumExprApply: 			return v.visit((const ExprApply&) *this, args...); break;
	case NumExprChi: 			return v.visit((const ExprChi&) *this, args...); break;
	case NumExprGenericBinaryOp:return v.visit((const ExprGenericBinaryOp&) *this, args...); break;
	case NumExprAdd: 			return v.visit((const ExprAdd&) *this, args...); break;
	case NumExprMul: 			return v.visit((const ExprMul&) *this, args...); break;
	case NumExprSub: 			return v.visit((const ExprSub&) *this, args...); break;
	case NumExprDiv: 			return v.visit((const ExprDiv&) *this, args...); break;
	case NumExprMax: 			return v.visit((const ExprMax&) *this, args...); break;
	case NumExprMin: 			return v.visit((const ExprMin&) *this, args...); break;
	case NumExprAtan2: 			return v.visit((const ExprAtan2&) *this, args...); break;
	case NumExprGenericUnaryOp: return v.visit((const ExprGenericUnaryOp&) *this, args...); break;
	case NumExprMinus: 			return v.visit((const ExprMinus&) *this, args...); break;
	case NumExprTrans: 			return v.visit((const ExprTrans&) *this, args...); break;
	case NumExprSign: 			return v.visit((const ExprSign&) *this, args...); break;
	case NumExprAbs: 			return v.visit((const ExprAbs&) *this, args...); break;
	case NumExprPower: 			return v.visit((const ExprPower&) *this, args...); break;
	case NumExprSqr: 			return v.visit((const ExprSqr&) *this, args...); break;
	case NumExprSqrt: 			return v.visit((const ExprSqrt&) *this, args...); break;
	case NumExprExp: 			return v.visit((const ExprExp&) *this, args...); break;
	case NumExprLog: 			return v.visit((const ExprLog&) *this, args...); break;
	case NumExprCos: 			return v.visit((const ExprCos&) *this, args...); break;
	case NumExprSin: 			return v.visit((const ExprSin&) *this, args...); break;
	case NumExprTan: 			return v.visit((const ExprTan&) *this, args...); break;
	case NumExprCosh: 			return v.visit((const ExprCosh&) *this, args...); break;
	case NumExprSinh: 			return v.visit((const ExprSinh&) *this, args...); break;
	case NumExprTanh: 			return v.visit((const ExprTanh&) *this, args...); break;
	case NumExprAcos: 			return v.visit((const ExprAcos&) *this, args...); break;
	case NumExprAsin: 			return v.visit((const ExprAsin&) *this, args...); break;
	case NumExprAtan: 			return v.visit((const ExprAtan&) *this, args...); break;
	case NumExprAcosh:			return v.visit((const ExprAcosh&) *this, args...); break;
	case NumExprAsinh: 			return v.visit((const ExprAsinh&) *this, args...); break;
	case NumExprAtanh:			return v.visit((const ExprAtanh&) *this, args...); break;
	case NumExprFloor:			return v.visit((const ExprFloor&) *this, args...); break;
	case NumExprCeil:			return v.visit((const ExprCeil&) *this, args...); break;
	case NumExprSaw:			return v.visit((const ExprSaw&) *this, args...); break;
	default:					assert(false);
	}
}

} // end namespace ibex

#endif // __IBEX_EXPR_VISITOR_H__
