#ifndef __IBEX_EXPR_DOMAIN__
#define __IBEX_EXPR_DOMAIN__

#include <iostream>

#include "ibex_ExprData.h"

namespace ibex {

class ExprDomainFactory : public ExprDataFactory<Domain> {
public:

	/** Visit an indexed expression. */
	virtual Domain* init(const ExprIndex& e, Domain& expr_deco);
	/** Visit a leaf.*/
	virtual Domain* init(const ExprLeaf& e);
	/** Visit a n-ary operator. */
	virtual Domain* init(const ExprNAryOp& e, Array<Domain>& args_deco);
	/** Visit a binary operator. */
	virtual Domain* init(const ExprBinaryOp& e, Domain& left_deco, Domain& right_deco);
	/** Visit an unary operator. */
	virtual Domain* init(const ExprUnaryOp& e, Domain& expr_deco);
	/** Visit a transpose. */
	virtual Domain* init(const ExprTrans& e, Domain& expr_deco);

};

/**
 * \brief Domain associated to nodes of a function.
 *
 * These data are used by all forward/backward algorithms
 * (Eval, Gradient, HC4Revise, etc.).
 *
 */
class ExprDomain : public ExprData<Domain> {
public:

	ExprDomain(Function& f);

	// Why my compiler forces me to redeclare these functions?
	// ------------------------------------------------------
	const Domain& operator[](int i) const;
	Domain& operator[](int i);
	// ------------------------------------------------------

	/**
	 * \brief Initialize symbols domains from d
	 *
	 * \param grad - true<=>update "g" (gradient) false <=>update "d" (domain)
	 * \see #ibex::ExprLabel
	 */
	void write_arg_domains(const Array<Domain>& d);

	/**
	 * \brief Initialize symbols domains from d
	 *
	 * \param grad - true<=>update "g" (gradient) false <=>update "d" (domain)
	 * \see #ibex::ExprLabel
	 */
	void write_arg_domains(const Array<const Domain>& d);

	/**
	 * \brief Initialize symbols domains from a box
	 *
	 * \param grad - true<=>update "g" (gradient) false <=>update "d" (domain)
	 * \see #ibex::ExprLabel
	 */
	void write_arg_domains(const IntervalVector& box);

	/**
	 * \brief Initialize d from symbols domains
	 *
	 * \param grad - true<=>read "g" (gradient) false <=>read "d" (domain)
	 * \see #ibex::ExprLabel
	 */
	void read_arg_domains(Array<Domain>& d) const;

	/**
	 * \brief Initialize a box from symbols domains
	 *
	 * \param grad - true<=>read "g" (gradient) false <=>read "d" (domain)
	 * \see #ibex::ExprLabel
	 */
	void read_arg_domains(IntervalVector& box) const;

};

/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline ExprDomain::ExprDomain(Function& f) : ExprData<Domain>(f, ExprDomainFactory()) {

}

inline const Domain& ExprDomain::operator[](int i) const {
	return data[i];
}

inline Domain& ExprDomain::operator[](int i) {
	return data[i];
}

inline void ExprDomain::write_arg_domains(const Array<Domain>& d) {
	load(args, d, f.nb_used_vars(), f.used_vars());
}

inline void ExprDomain::write_arg_domains(const Array<const Domain>& d) {
	load(args, d, f.nb_used_vars(), f.used_vars());
}

inline void ExprDomain::write_arg_domains(const IntervalVector& box) {

	if (f.all_args_scalar()) {
		int j;
		for (int i=0; i<f.nb_used_vars(); i++) {
			j=f.used_var(i);
			args[j].i()=box[j];
		}
	}
	else
		load(args, box, f.nb_used_vars(), f.used_vars());
}


inline void ExprDomain::read_arg_domains(Array<Domain>& d) const {
	load(d, args, f.nb_used_vars(), f.used_vars());
}

inline void ExprDomain::read_arg_domains(IntervalVector& box) const {
	if (f.all_args_scalar()) {
		int j;

		for (int i=0; i<f.nb_used_vars(); i++) {
			j=f.used_var(i);
			box[j]=args[j].i();
		}
	}
	else {
		load(box, args, f.nb_used_vars(), f.used_vars());
	}
}


} // namespace ibex

#endif /* __IBEX_EXPR_DOMAIN__ */
