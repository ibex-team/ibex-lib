//============================================================================
//                                  I B E X
// File        : Separator for the constraint : Point inside a polygon
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Apr 25, 2012
//============================================================================

#include "ibex_SepPolygon.h"
#include "ibex_Ctc.h"
#include "ibex_CtcSegment.h"
#include "ibex_Array.h"
#include "ibex_BoolInterval.h"

#include <cmath>

using namespace std;

using namespace ibex;

namespace {


Array<Ctc> segment_ctc_list(vector<double>& ax, vector<double>& ay, vector<double>& bx, vector<double>& by) {
	Array<Ctc> l(ax.size());

	for(unsigned int i=0; i<ax.size(); i++) {
		l.set_ref(i, *new CtcSegment(ax[i],ay[i],bx[i],by[i]));
	}
	return l;
}

} // end anonymous namespace

namespace ibex {

SepPolygon::SepPolygon(vector<double> &_ax, vector<double> &_ay, vector<double> &_bx, vector<double> &_by) :
    		SepBoundaryCtc(*new CtcUnion(segment_ctc_list(_ax,_ay, _bx, _by)),
    				       *new PdcInPolygon(_ax,_ay,_bx,_by)) {

}

SepPolygon::~SepPolygon() {
	for(int i=0; i<((CtcUnion&) ctc_boundary).list.size(); i++) {
		delete & (((CtcUnion&) ctc_boundary).list[i]);
	}
	delete &ctc_boundary;

	delete &is_inside;
}

};
