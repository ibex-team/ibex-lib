//============================================================================
//                                  I B E X
// File        : ibex_P_StructLyapunov.h
// Authors     : Gilles Chabert
// Copyright   : IMT Atlantique
// License     : See the LICENSE file
// Created     : Nov 05, 2019
//============================================================================

#include "ibex_P_StructLyapunov.h"
#include "ibex_P_SysGenerator.h"
#include "ibex_SyntaxError.h"
#include "ibex_UnknownFileException.h"

#include <string.h>

#ifndef _WIN32 // MinGW does not support mutex
#include <mutex>
namespace {
std::mutex mtx;
}
#define LOCK mtx.lock()
#define UNLOCK mtx.unlock()
#else
#define LOCK
#define UNLOCK
#endif

extern int ibexparse();
extern void ibexrestart(FILE *);
extern FILE* ibexin;

using namespace std;

namespace ibex {

P_StructLyapunov::ParamSystem::ParamSystem() { }

P_StructLyapunov::P_StructLyapunov(const char* filename) : xhat(1 /*tmp*/) {
	FILE* fd = fopen(filename, "r");

	if (fd == NULL) throw UnknownFileException(filename);

	LOCK;

	ibexin = fd;

	try {
		parser::pstruct = this;
		ibexparse();
		parser::pstruct = NULL;
	}

	catch(SyntaxError& e) {
		parser::pstruct = NULL;
		fclose(fd);
		ibexrestart(ibexin);
		UNLOCK;
		throw e;
	}

	fclose(fd);

	UNLOCK;
}

void P_StructLyapunov::begin() {
	P_Struct::begin();
}

void P_StructLyapunov::end() {

	for (vector<Function*>::iterator it = source.func.begin(); it!=source.func.end(); ++it) {
		if (strcmp((*it)->name,"f")==0)
			parser::init_function_by_copy(f,**it);
		else if (strcmp((*it)->name,"v")==0)
			parser::init_function_by_copy(v,**it);
		else if (strcmp((*it)->name,"vminor")==0)
			parser::init_function_by_copy(vminor,**it);
	}

	if (f.name==NULL)
		throw SyntaxError("input file must contain a function named \"f\"");
	if (v.name==NULL)
		throw SyntaxError("input file must contain a function named \"v\"");
	if (vminor.name==NULL)
		parser::init_function_by_copy(vminor,v);

	/* if x was declared as a constant... */
	//	bool x_cst_found=false;
	//	for (vector<const char*>::const_iterator it = scopes.top().cst.begin(); it !=scopes.top().cst.end(); ++it) {
	//
	//		if (strcmp(*it,"x")) {
	//			Domain* d= new Domain(scopes.top().get_cst(*it).get());
	//			switch (d->dim.type()) {
	//			case Dim::SCALAR :
	//				xhat.resize(1);
	//				xhat[0]=d->i().lb();
	//				break;
	//			case Dim::ROW_VECTOR:
	//			case Dim::COL_VECTOR:
	//				xhat.resize(d->dim.vec_size());
	//				xhat = d->v().lb();
	//				break;
	//			case Dim::MATRIX:
	//				throw SyntaxError("unexpected matrix constant \"x\"");
	//			}
	//			x_cst_found = true;
	//		}
	//	}
	//	if (!x_cst_found)
	//		throw SyntaxError("input file must contain a constant named \"x\"");

	parser::P_SysGenerator(scopes).generate(source,theta_sys);

	/* x declared as a variable */
	// size of x
	int n=theta_sys.args[0].dim.vec_size();

	// size of theta
	int p=theta_sys.args.size()==2? theta_sys.args[1].dim.vec_size(): 0;

	xhat.resize(n);
	xhat=theta_sys.box.subvector(0,n-1).lb();

	theta=p>0? theta_sys.box.subvector(n,n+p-1) : /* ignored */ IntervalVector::empty(1);

	theta_ctrs.add(theta_sys.ctrs);

	P_Struct::end();
}

} // end namespace ibex

