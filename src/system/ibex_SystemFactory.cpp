//============================================================================
//                                  I B E X                                   
// File        : ibex_SystemFactory.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Apr 25, 2018
//============================================================================

#include "ibex_SystemFactory.h"
#include "ibex_Exception.h"
#include "ibex_ExprCtr.h"
#include "ibex_ExprCopy.h"

using std::vector;

namespace ibex {

SystemFactory::SystemFactory() : nb_arg(0), nb_var(0), input_args(0), sys_args(0), goal(NULL), system_built(false) { }


SystemFactory::~SystemFactory() {
	if (!system_built) {
		if (goal) delete goal;

		for (unsigned int i=0; i<ctrs.size(); i++)
			delete ctrs[i];

		if (!f_ctrs.empty())
			cleanup(ExprVector::new_col(f_ctrs),false);

		for (int i=0; i<sys_args.size(); i++) {
			delete &sys_args[i];
		}
	}
}

void SystemFactory::add_var(const ExprSymbol& v) {
	add_var(v,IntervalVector(v.dim.size()));
}

void SystemFactory::add_var(const ExprSymbol& v, const Interval& init_box) {
	add_var(v,IntervalVector(v.dim.size(),init_box));
}

void SystemFactory::add_var(const ExprSymbol& v, const IntervalVector& init_box) {
	assert(v.dim.size()==init_box.size());
	if (goal || !ctrs.empty()) ibex_error("cannot add a variable to a system after a constraint (or the goal function)");

	tmp_input_args.push_back(&v);
	nb_arg++;
	nb_var+= v.dim.size();

	boxes.push_back(init_box);
}

void SystemFactory::add_var(const Array<const ExprSymbol>& a) {
	if (system_built) ibex_error("only one system can be built with a factory");
	if (goal || !ctrs.empty()) ibex_error("cannot add a variable to a system after a constraint (or the goal function)");

	for (int i=0; i<a.size(); i++)
		add_var(a[i]);
}

void SystemFactory::add_var(const Array<const ExprSymbol>& a, const IntervalVector& box) {
	if (system_built) ibex_error("only one system can be built with a factory");
	if (goal || !ctrs.empty()) ibex_error("cannot add a variable to a system after a constraint (or the goal function)");

	for (int i=0; i<a.size(); i++) {
		tmp_input_args.push_back(&a[i]);
		nb_arg++;
		nb_var+= a[i].dim.size();
	}
	boxes.push_back(box);
}

/*
void SystemFactory::add_var(const Array<const ExprSymbol>& a, const Array<const IntervalVector>& init_boxes) {
	if (goal || !ctrs.empty()) ibex_error("cannot add a variable to a system after a constraint (or the goal function)");

	for (int i=0; i<a.size(); i++) {
		tmp_args.push_back(&a[i]);
		nb_arg++;
		nb_var+= a[i].dim.size();
		tmp_bound.push_back(init_boxes[i]);
	}
}
*/

void SystemFactory::init_args() {

	if (input_args.size()>0) return; // already done.

	input_args.add(tmp_input_args);

	sys_args.resize(input_args.size());

	varcopy(input_args, sys_args);
}

void SystemFactory::add_goal(const ExprNode& goal) {
	init_args();

	Array<const ExprSymbol> goal_vars(input_args.size());
	varcopy(input_args,goal_vars);
	const ExprNode& goal_expr=ExprCopy().copy(input_args, goal_vars, goal);
	this->goal = new Function(goal_vars, goal_expr);
}

void SystemFactory::add_goal(const Function& goal) {
	init_args();

	// check that the arguments of the goal
	// matches the arguments entered
	assert(varequals(goal.args(), input_args));

	this->goal = new Function(goal);
}

void SystemFactory::add_ctr(const ExprCtr& ctr) {
	init_args();

	Array<const ExprSymbol> ctr_args(input_args.size());
	varcopy(input_args,ctr_args);
	const ExprNode& ctr_expr=ExprCopy().copy(input_args, ctr_args, ctr.e).simplify();

	ctrs.push_back(new NumConstraint(*new Function(ctr_args, ctr_expr), ctr.op, true));

	f_ctrs.push_back(& f_ctrs_copy.copy(input_args, sys_args, ctr.e, true));
}

void SystemFactory::add_ctr(const NumConstraint& ctr) {
	init_args();

	// check that the arguments of the constraint
	// matches the arguments entered
	assert(varequals(ctr.f.args(),input_args));

	ctrs.push_back(new NumConstraint(*new Function(ctr.f), ctr.op, true));

	f_ctrs.push_back(& f_ctrs_copy.copy(ctr.f.args(), sys_args, ctr.f.expr(), true));
}

// precondition: nb_ctr > 0
void System::init_f_ctrs(const std::vector<const ExprNode*>& fac_f_ctrs) {

	if (fac_f_ctrs.empty()) {
		// don't delete the symbols now because
		// one may read the "args" field of System even
		// if there is no constraint
		return; // <=> m>0
	}

	int total_output_size=0;

	for (vector<const ExprNode*>::const_iterator it=fac_f_ctrs.begin(); it!=fac_f_ctrs.end(); it++) {
		total_output_size += (*it)->dim.size();
	}

	Array<const ExprNode> image(total_output_size);
	if (total_output_size>0) ops = new CmpOp[total_output_size];

	// concatenate all the components of all the constraints function
	int i=0;
	int c=0;
	for (vector<const ExprNode*>::const_iterator it=fac_f_ctrs.begin(); it!=fac_f_ctrs.end(); it++, c++) {

		/*========= 1st variant ===============
		 * will have the disadvantage that the DAG structure
		 * of a vector constraint may be lost, e.g., a 2D constraint x+y=0
		 * will result in two components (x+y)[0] and (x+y)[1] where (x+y)
		 * is for each a separate subnode.
		 */
//		const Function& fj=ctrs[j].f;
//		for (int k=0; k<fj.image_dim(); k++) {
//			const ExprNode& e=ExprCopy().copy(fj[k].args(), args, fj[k].expr());
//			image.set_ref(i++,e);
//		}
		/*======================================================================*/


		/*========= 2nd variant ===============*/
		/* copy the DAG directly of the jth constraint and create
		 * indexed expression after.
		 *
		 * Note : a vector constraint like
		 *   (x[0],x[1])=(0,1)
		 * will be first transformed into
		 *   (x[0],x[1])[0]=(0,1)[0] and (x[0],x[1])[1]=(0,1)[1]
		 * and then, thanks to simplify(), transformed back into:
		 *    x[0]=0 and x[1]=1.
		 */
		const ExprNode& e=**it;

		const Dim& dim=e.dim;
		switch (dim.type()) {
		case Dim::SCALAR :
			ops[i]=ctrs[c].op;
			image.set_ref(i++,e);
			break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:
			for (int k=0; k<dim.vec_size(); k++) {
				ops[i]=ctrs[c].op;
				image.set_ref(i++,e[k]);
			}
			break;
		case Dim::MATRIX:
			for (int k=0; k<dim.nb_rows(); k++)
				for (int l=0; l<dim.nb_cols(); l++) {
					ops[i]=ctrs[c].op;
					image.set_ref(i++,e[k][l]);
				}
			break;
		default:
			assert(false);
			break;
		}
		/*======================================================================*/

	}
	assert(i==total_output_size);

	f_ctrs.init(args, total_output_size>1? ExprVector::new_col(image).simplify() : image[0].simplify());
}


System::System(const SystemFactory& fac) : id(next_id()), nb_var(0), nb_ctr(0), ops(NULL), box(1) {
	init(fac);
}

void System::init(const SystemFactory& fac) {
	if (fac.system_built)
		ibex_error("only one system can be built with a factory");

	// the field fac.args is initialized upon addition of an objective
	// function or a constraint.
	if (fac.input_args.is_empty()) {
		// an empty system is not an error
		// (and may happen with automatic generation)
		// warning: set fac.system_built=true *after* this line
		((SystemFactory&) fac).init_args();
	}

	fac.system_built = true;

	(int&) nb_var = fac.nb_var;
	(int&) nb_ctr = fac.ctrs.size();

	// =========== init goal ==============
	goal = fac.goal;

	// f is not initialized here --> see init_f_from_ctrs() below

	// =========== init args ==============
	args.resize(fac.nb_arg);
	for (int i=0; i<fac.nb_arg; i++) {
		args.set_ref(i,fac.sys_args[i]);
	}

	// =========== init box ==============
	box.resize(nb_var);
	int i=0;
	for (std::vector<IntervalVector>::const_iterator it=fac.boxes.begin(); it!=fac.boxes.end(); it++) {
		box.put(i,*it);
		i+=(*it).size();
	}

	// =========== init ctrs ==============
	ctrs.resize(nb_ctr);
	for (int i=0; i<nb_ctr; i++)
		ctrs.set_ref(i,*(fac.ctrs[i]));

	// =========== init main function
	// we cannot generate first the global function f_ctrs and
	// then each constraint with (f[i] op 0) because
	// a constraint can be vector or matrix valued.
	// so we do the contrary: we generate first the constraints,
	// and build f with the components of all constraints' functions.

	init_f_ctrs(fac.f_ctrs);
}

} // end namespace

