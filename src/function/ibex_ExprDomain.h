#ifndef __IBEX_EXPR_DOMAIN__
#define __IBEX_EXPR_DOMAIN__

#include <iostream>

#include "ibex_ExprData.h"

namespace ibex {

class ExprDomain : public ExprData<Domain> {

public:

	ExprDomain(Function& f);

	// Why my compiler asks me to redeclare these functions?
	inline const Domain& operator[](int i) const { return data[i]; }

	inline Domain& operator[](int i)             { return data[i]; }

	/**
	 * \brief Initialize symbols domains from d
	 *
	 * \param grad - true<=>update "g" (gradient) false <=>update "d" (domain)
	 * \see #ibex::ExprLabel
	 */
	void write_arg_domains(const Array<Domain>& d) const;

	/**
	 * \brief Initialize symbols domains from d
	 *
	 * \param grad - true<=>update "g" (gradient) false <=>update "d" (domain)
	 * \see #ibex::ExprLabel
	 */
	void write_arg_domains(const Array<const Domain>& d) const;

	/**
	 * \brief Initialize symbols domains from a box
	 *
	 * \param grad - true<=>update "g" (gradient) false <=>update "d" (domain)
	 * \see #ibex::ExprLabel
	 */
	void write_arg_domains(const IntervalVector& box) const;

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

protected:

	/** Visit an indexed expression. */
	virtual Domain* init(const ExprIndex& e, const Domain& expr_deco);
	/** Visit a leaf.*/
	virtual Domain* init(const ExprLeaf& e);
	/** Visit a n-ary operator. */
	virtual Domain* init(const ExprNAryOp& e, const Array<Domain>& args_deco);
	/** Visit a binary operator. */
	virtual Domain* init(const ExprBinaryOp& e, const Domain& left_deco, const Domain& right_deco);
	/** Visit an unary operator. */
	virtual Domain* init(const ExprUnaryOp& e, const Domain& expr_deco);
	/** Visit a transpose. */
	virtual Domain* init(const ExprTrans& e, const Domain& expr_deco);

};

/* ============================================================================
 	 	 	 	 	 	 	 implementation
  ============================================================================*/
inline ExprDomain::ExprDomain(Function& f) : FunctionData<Domain>(f) {

}

inline void ExprDomain::write_arg_domains(const Array<Domain>& d) const {
	load(args, d, f.nb_used_vars(), f._used_var);
}

inline void ExprDomain::write_arg_domains(const Array<const Domain>& d) const {
	load(args, d, f.nb_used_vars(), f._used_var);
}

inline void ExprDomain::write_arg_domains(const IntervalVector& box) const {

	if (f.all_args_scalar()) {
		int j;
		for (int i=0; i<f.nb_used_vars(); i++) {
			j=f.used_var(i);
			args[j].i()=box[j];
		}
	}
	else
		load(args, box, f.nb_used_vars(), f._used_var);
}


inline void ExprDomain::read_arg_domains(Array<Domain>& d) const {
	load(d, args, f.nb_used_vars(), f._used_var);
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
		load(box, args, f.nb_used_vars(), f._used_var);
	}
}


} // namespace ibex

#endif /* __IBEX_EXPR_DOMAIN__ */
