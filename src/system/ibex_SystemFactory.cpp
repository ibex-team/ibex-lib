//============================================================================
//                                  I B E X                                   
// File        : ibex_SystemFactory.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : May 26, 2013
//============================================================================

#include "ibex_SystemFactory.h"
#include "ibex_Exception.h"
#include "ibex_ExprCtr.h"
#include "ibex_ExprCopy.h"
#include "ibex_EmptySystemException.h"

using std::vector;

namespace ibex {

SystemFactory::SystemFactory() : nb_arg(0), nb_var(0), goal(NULL), args(NULL) { }


SystemFactory::~SystemFactory() {
	if (args) delete args;
}

void SystemFactory::add_var(const ExprSymbol& v) {
	if (goal || !ctrs.empty()) ibex_error("cannot add a variable to a system after a constraint (or the goal function)");

	tmp_args.push_back(&v);
	nb_arg++;
	nb_var+= v.dim.size();
}

void SystemFactory::add_var(const Array<const ExprSymbol>& a) {
	if (goal || !ctrs.empty()) ibex_error("cannot add a variable to a system after a constraint (or the goal function)");

	for (int i=0; i<a.size(); i++) {
		tmp_args.push_back(&a[i]);
		nb_arg++;
		nb_var+= a[i].dim.size();
	}
}

void SystemFactory::add_goal(const ExprNode& goal) {
	if (!args) args = new Array<const ExprSymbol>(tmp_args);

	Array<const ExprSymbol> goal_vars(args->size());
	varcopy(*args,goal_vars);
	const ExprNode& goal_expr=ExprCopy().copy(*args, goal_vars, goal); //, true);
	this->goal = new Function(goal_vars, goal_expr);
}

void SystemFactory::add_goal(const Function& goal) {
	if (!args) args = new Array<const ExprSymbol>(tmp_args);

	// check that the arguments of the goal
	// matches the arguments entered
	assert(varequals(goal.args(), *args));

	this->goal = new Function(goal);
}

void SystemFactory::add_ctr(const ExprCtr& ctr) {
	if (!args) args = new Array<const ExprSymbol>(tmp_args);

	Array<const ExprSymbol> ctr_vars(args->size());
	varcopy(*args,ctr_vars);
	const ExprNode& ctr_expr=ExprCopy().copy(*args, ctr_vars, ctr.e); //, true);

	ctrs.push_back(new NumConstraint(*new Function(ctr_vars, ctr_expr), ctr.op, true));
}

void SystemFactory::add_ctr2(const ExprCtr& ctr) {
	if (!args) args = new Array<const ExprSymbol>(tmp_args);

	Array<const ExprSymbol> ctr_vars(args->size());
	varcopy(*args,ctr_vars);
	const ExprNode& ctr_expr=ExprCopy().copy(*args, ctr_vars, ctr.e); //, true);

	ctrs.push_back(new NumConstraint(*new Function(ctr_vars, ctr_expr), ctr.op, true));
}

void SystemFactory::add_ctr(const NumConstraint& ctr) {
	if (!args) args = new Array<const ExprSymbol>(tmp_args);

	// check that the arguments of the constraint
	// matches the arguments entered
	assert(varequals(ctr.f.args(),*args));

	ctrs.push_back(new NumConstraint(*new Function(ctr.f), ctr.op, true));
}

// precondition: nb_ctr > 0
void System::init_f_from_ctrs() {

	if (ctrs.is_empty()) return; // <=> m>0

	int total_output_size=0;
	for (int j=0; j<ctrs.size(); j++)
		total_output_size += ctrs[j].f.image_dim();

	Array<const ExprNode> image(total_output_size);
	int i=0;

	// concatenate all the components of all the constraints function
	for (int j=0; j<ctrs.size(); j++) {
		Function& fj=ctrs[j].f;

		/*========= 1st variant ===============
		 * will have the disadvantage that the DAG structure
		 * of a vector constraint may be lost, e.g., a 2D constraint x+y=0
		 * will result in two components (x+y)[0] and (x+y)[1] where (x+y)
		 * is for each a separate subnode.
		 */
//		for (int k=0; k<fj.image_dim(); k++) {
//			const ExprNode& e=ExprCopy().copy(fj[k].args(), args, fj[k].expr());
//			image.set_ref(i++,e);
//		}
		/*======================================================================*/


		/*========= 2nd variant ===============*/
		/* copy the DAG directly of the jth constraint and create
		 * indexed expression after.
		 *
		 * TODO: Disadvantage : a vector constraint like
		 *   (x[0],x[1])=(0,1)
		 * will be transformed into
		 *   (x[0],x[1])[0]=(0,1)[0] and (x[0],x[1])[1]=(0,1)[1]
		 * instead of
		 *    x[0]=0 and x[1]=1.
		 */
		const ExprNode& e=ExprCopy().copy(fj.args(), args, fj.expr());

		const Dim& fjd=fj.expr().dim;
		switch (fjd.type()) {
		case Dim::SCALAR :
			image.set_ref(i++,e);
			break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:
			for (int k=0; k<fjd.vec_size(); k++)
				image.set_ref(i++,e[k]);
			break;
		case Dim::MATRIX:
			for (int k=0; k<fjd.dim2; k++)
				for (int l=0; l<fjd.dim3; l++)
					image.set_ref(i++,e[k][l]);
			break;
		default:
			assert(false);
			break;
		}
		/*======================================================================*/

	}
	assert(i==total_output_size);

	// TODO: we should probably homgenize; in the case of a scalar function
	// a 1-sized vector should be created.
	f.init(args, total_output_size>1? ExprVector::new_(image,false) : image[0]);
}


System::System(const SystemFactory& fac) : nb_var(0), nb_ctr(0), box(1) {
	init(fac);
}

void System::init(const SystemFactory& fac) {

	// the field fac.args is initialized upon addition of an objective
	// function or a constraint.
	if (!fac.args) throw EmptySystemException();

	(int&) nb_var = fac.nb_var;
	(int&) nb_ctr = fac.ctrs.size();

	// =========== init goal ==============
	goal = fac.goal;

	// f is not initialized here --> see init_f_from_ctrs() below

	// =========== init args ==============
	args.resize(fac.nb_arg);
	varcopy(*fac.args, args);

	box.resize(nb_var);

	// =========== init ctrs ==============
	ctrs.resize(nb_ctr);
	for (int i=0; i<nb_ctr; i++)
		ctrs.set_ref(i,*(fac.ctrs[i]));

	// =========== init main function
	init_f_from_ctrs();
}

} // end namespace

