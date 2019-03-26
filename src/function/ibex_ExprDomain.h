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

	~ExprTemplateDomain();

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

private:
	ExprTemplateDomain(const ExprTemplateDomain&); // forbidden
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
	TemplateDomain<D> d(d_expr[e.index]); // Depending on the type of index, can be a reference or a copy.
	return new TemplateDomain<D>(d,d.is_reference);
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
inline ExprTemplateDomain<D>::~ExprTemplateDomain() {
	for (int i=0; i<ExprData<TemplateDomain<D> >::data.size(); i++) {
		delete &ExprData<TemplateDomain<D> >::data[i];
	}
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
	load(ExprData<TemplateDomain<D> >::args, d, ExprData<TemplateDomain<D> >::f.used_vars);
}

template<class D>
inline void ExprTemplateDomain<D>::write_arg_domains(const Array<const TemplateDomain<D> >& d) {
	load(ExprData<TemplateDomain<D> >::args, d, ExprData<TemplateDomain<D> >::f.used_vars);
}

template<class D>
inline void ExprTemplateDomain<D>::read_arg_domains(Array<TemplateDomain<D> >& d) const {
	load(d, ExprData<TemplateDomain<D> >::args, ExprData<TemplateDomain<D> >::f.used_vars);
}


template<class D>
inline void ExprTemplateDomain<D>::write_arg_domains(const typename D::VECTOR& box) {

	if (ExprData<TemplateDomain<D> >::f.all_args_scalar()) {
		for (std::vector<int>::const_iterator j=ExprData<TemplateDomain<D> >::f.used_vars.begin();
				j!=ExprData<TemplateDomain<D> >::f.used_vars.end(); ++j) {
			ExprData<TemplateDomain<D> >::args[*j].i()=box[*j];
		}
	}
	else {
		load(ExprData<TemplateDomain<D> >::args, box, ExprData<TemplateDomain<D> >::f.used_vars);
	}
}

template<class D>
inline void ExprTemplateDomain<D>::read_arg_domains(typename D::VECTOR& box) const {
	if (ExprData<TemplateDomain<D> >::f.all_args_scalar()) {

		for (std::vector<int>::const_iterator  j=ExprData<TemplateDomain<D> >::f.used_vars.begin();
				j!=ExprData<TemplateDomain<D> >::f.used_vars.end(); ++j) {
			box[*j]=ExprData<TemplateDomain<D> >::args[*j].i();
		}
	}
	else {
		load(box, ExprData<TemplateDomain<D> >::args, ExprData<TemplateDomain<D> >::f.used_vars);
	}
}


} // namespace ibex

#endif /* __IBEX_EXPR_DOMAIN__ */
