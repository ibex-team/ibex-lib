#ifndef __IBEX_EXPR_DOMAIN__
#define __IBEX_EXPR_DOMAIN__

#include <iostream>

#include "ibex_ExprData.h"

namespace ibex {

template<class D>
class ExprDomainFactory : public ExprDataFactory<TemplateDomain<D> > {
public:
	/** Delete this. */
	virtual ~ExprDomainFactory();
	/** Visit an indexed expression. */
	virtual TemplateDomain<D>* init(const ExprIndex& e, TemplateDomain<D>& expr_deco);
	/** Visit a leaf.*/
	virtual TemplateDomain<D>* init(const ExprLeaf& e);
	/** Visit a n-ary operator. */
	virtual TemplateDomain<D>* init(const ExprNAryOp& e, Array<TemplateDomain<D> >& args_deco);
	/** Visit a binary operator. */
	virtual TemplateDomain<D>* init(const ExprBinaryOp& e, TemplateDomain<D>& left_deco, TemplateDomain<D>& right_deco);
	/** Visit an unary operator. */
	virtual TemplateDomain<D>* init(const ExprUnaryOp& e, TemplateDomain<D>& expr_deco);
	/** Visit a transpose. */
	virtual TemplateDomain<D>* init(const ExprTrans& e, TemplateDomain<D>& expr_deco);
};

/**
 * \brief Domain associated to nodes of a function.
 *
 * These data are used by all forward/backward algorithms
 * (Eval, Gradient, HC4Revise, etc.).
 *
 */
template<class D>
class ExprTemplateDomain : public ExprData<TemplateDomain<D> > {
public:

	ExprTemplateDomain(const Function& f);

	// Why my compiler forces me to redeclare these functions?
	// ------------------------------------------------------
	const TemplateDomain<D>& operator[](int i) const;
	TemplateDomain<D>& operator[](int i);
	// ------------------------------------------------------

	/**
	 * \brief Initialize symbols domains from d
	 *
	 * \param grad - true<=>update "g" (gradient) false <=>update "d" (domain)
	 * \see #ibex::ExprLabel
	 */
	void write_arg_domains(const Array<TemplateDomain<D> >& d);

	/**
	 * \brief Initialize symbols domains from d
	 *
	 * \param grad - true<=>update "g" (gradient) false <=>update "d" (domain)
	 * \see #ibex::ExprLabel
	 */
	void write_arg_domains(const Array<const TemplateDomain<D> >& d);

	/**
	 * \brief Initialize symbols domains from a box
	 *
	 * \param grad - true<=>update "g" (gradient) false <=>update "d" (domain)
	 * \see #ibex::ExprLabel
	 */
	void write_arg_domains(const typename D::VECTOR& box);

	/**
	 * \brief Initialize d from symbols domains
	 *
	 * \param grad - true<=>read "g" (gradient) false <=>read "d" (domain)
	 * \see #ibex::ExprLabel
	 */
	void read_arg_domains(Array<TemplateDomain<D> >& d) const;

	/**
	 * \brief Initialize a box from symbols domains
	 *
	 * \param grad - true<=>read "g" (gradient) false <=>read "d" (domain)
	 * \see #ibex::ExprLabel
	 */
	void read_arg_domains(typename D::VECTOR& box) const;

};

typedef ExprTemplateDomain<Interval> ExprDomain;

/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/
template<class D>
ExprDomainFactory<D>::~ExprDomainFactory() {

}

template<class D>
TemplateDomain<D>* ExprDomainFactory<D>::init(const ExprIndex& e, TemplateDomain<D>& d_expr) {
	switch (e.expr.type()) {
	case Dim::SCALAR:
		return new TemplateDomain<D>(d_expr.i());
		break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:
		return new TemplateDomain<D>(d_expr.v()[e.index]);
		break;
	case Dim::MATRIX:
		return new TemplateDomain<D>(d_expr.m()[e.index],true);
		break;
	default: // Dim::MATRIX_ARRAY:
		return new TemplateDomain<D>(d_expr.ma()[e.index]);
		break;
	}
}

template<class D>
TemplateDomain<D>* ExprDomainFactory<D>::init(const ExprLeaf& e) {
	return new TemplateDomain<D>(e.dim);
}

template<class D>
TemplateDomain<D>* ExprDomainFactory<D>::init(const ExprNAryOp& e, Array<TemplateDomain<D> >&) {
	return new TemplateDomain<D>(e.dim);
}

template<class D>
TemplateDomain<D>* ExprDomainFactory<D>::init(const ExprBinaryOp& e, TemplateDomain<D>&, TemplateDomain<D>&) {
	return new TemplateDomain<D>(e.dim);
}

template<class D>
TemplateDomain<D>* ExprDomainFactory<D>::init(const ExprUnaryOp& e, TemplateDomain<D>&) {
	return new TemplateDomain<D>(e.dim);
}

template<class D>
TemplateDomain<D>* ExprDomainFactory<D>::init(const ExprTrans& e, TemplateDomain<D>& expr_deco) {

	if (e.dim.is_vector()) {
		// share references
		return new TemplateDomain<D>(expr_deco, true);
	} else {
		// TODO: seems impossible to have references
		// in case of matrices...
		return new TemplateDomain<D>(e.dim);
	}
}

template<class D>
inline ExprTemplateDomain<D>::ExprTemplateDomain(const Function& f) : ExprData<TemplateDomain<D> >(f, ExprDomainFactory<D>()) {

}

template<class D>
inline const TemplateDomain<D>& ExprTemplateDomain<D>::operator[](int i) const {
	return ExprData<TemplateDomain<D> >::data[i];
}

template<class D>
inline TemplateDomain<D>& ExprTemplateDomain<D>::operator[](int i) {
	return ExprData<TemplateDomain<D> >::data[i];
}

template<class D>
inline void ExprTemplateDomain<D>::write_arg_domains(const Array<TemplateDomain<D> >& d) {
	load(ExprData<TemplateDomain<D> >::args, d, ExprData<TemplateDomain<D> >::f.nb_used_vars(), ExprData<TemplateDomain<D> >::f.used_vars());
}

template<class D>
inline void ExprTemplateDomain<D>::write_arg_domains(const Array<const TemplateDomain<D> >& d) {
	load(ExprData<TemplateDomain<D> >::args, d, ExprData<TemplateDomain<D> >::f.nb_used_vars(), ExprData<TemplateDomain<D> >::f.used_vars());
}

template<class D>
inline void ExprTemplateDomain<D>::read_arg_domains(Array<TemplateDomain<D> >& d) const {
	load(d, ExprData<TemplateDomain<D> >::args, ExprData<TemplateDomain<D> >::f.nb_used_vars(), ExprData<TemplateDomain<D> >::f.used_vars());
}


template<class D>
inline void ExprTemplateDomain<D>::write_arg_domains(const typename D::VECTOR& box) {

	if (ExprData<TemplateDomain<D> >::f.all_args_scalar()) {
		int j;
		for (int i=0; i<ExprData<TemplateDomain<D> >::f.nb_used_vars(); i++) {
			j=ExprData<TemplateDomain<D> >::f.used_var(i);
			ExprData<TemplateDomain<D> >::args[j].i()=box[j];
		}
	}
	else
		load(ExprData<TemplateDomain<D> >::args, box, ExprData<TemplateDomain<D> >::f.nb_used_vars(), ExprData<TemplateDomain<D> >::f.used_vars());
}


template<class D>
inline void ExprTemplateDomain<D>::read_arg_domains(typename D::VECTOR& box) const {
	if (ExprData<TemplateDomain<D> >::f.all_args_scalar()) {
		int j;

		for (int i=0; i<ExprData<TemplateDomain<D> >::f.nb_used_vars(); i++) {
			j=ExprData<TemplateDomain<D> >::f.used_var(i);
			box[j]=ExprData<TemplateDomain<D> >::args[j].i();
		}
	}
	else {
		load(box, ExprData<TemplateDomain<D> >::args, ExprData<TemplateDomain<D> >::f.nb_used_vars(), ExprData<TemplateDomain<D> >::f.used_vars());
	}
}


} // namespace ibex

#endif /* __IBEX_EXPR_DOMAIN__ */
