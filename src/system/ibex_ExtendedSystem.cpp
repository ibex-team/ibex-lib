//============================================================================
//                                  I B E X                                   
// File        : ibex_ExtendedSystem.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2013
// Last update : Aug 01, 2018
//============================================================================

#include "ibex_ExtendedSystem.h"

namespace ibex {

const char* ExtendedSystem::goal_name() {
	static const char* _goal_name = "__goal__"; // construct-on-first-use idiom (safe: no de-initialization dependency between static objects)
	return _goal_name;
}

ExtendedSystem::ExtendedSystem(const System& sys, double eps) :
	NormalizedSystem(sys,eps,true) {
}

void ExtendedSystem::write_ext_box(const IntervalVector& box, IntervalVector& ext_box) const {
	int i2=0;
	for (int i=0; i<nb_var-1; i++,i2++) {
		if (i2==goal_var()) i2++; // skip goal variable
		ext_box[i2]=box[i];
	}
}

void ExtendedSystem::read_ext_box(const IntervalVector& ext_box, IntervalVector& box) const {
	int i2=0;
	for (int i=0; i<nb_var-1; i++,i2++) {
		if (i2==goal_var()) i2++; // skip goal variable
		box[i]=ext_box[i2];
	}
}

void ExtendedSystem::write_ext_vec(const Vector& x, Vector& xy) const {
	int i2=0;
	for (int i=0; i<nb_var-1; i++,i2++) {
		if (i2==goal_var()) i2++; // skip goal variable
		xy[i2]=x[i];
	}
}

void ExtendedSystem::read_ext_vec(const Vector& xy, Vector& x) const {
	int i2=0;
	for (int i=0; i<nb_var-1; i++,i2++) {
		if (i2==goal_var()) i2++; // skip goal variable
		x[i]=xy[i2];
	}
}

} // end namespace ibex
