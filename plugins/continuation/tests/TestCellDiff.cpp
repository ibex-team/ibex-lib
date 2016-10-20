/* ============================================================================
 * I B E X - Continuation Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes and CNRS
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Alexandre Goldsztejn
 * Created     : Sep 06, 2016
 * ---------------------------------------------------------------------------- */

#include "TestCellDiff.h"
#include "ibex_ContCell.h"

using namespace std;

namespace ibex {

void TestCellDiff::test01() {

	Variable x,y,z;
    Function f(x,y,z,sqr(x)+sqr(y)+sqr(z)-1);
    IntervalVector domain(3);

    VarSet vars(f,z);
	IntervalVector box(3,Interval(-1,1));
	ContCell cell(box,box,domain,vars);
	IntervalVector box2(3,Interval(0,2));

//	cout << "Try to make a \"diff\" between the cell:\n";
//	cout << cell;
//	cout << "\nand the box " << box2 << endl;

	// note: set everything as parameter
	// so that the "diff" operates in all
	// dimensions
	cell.diff(box2,f,VarSet(3,BitSet::all(3),false));

//	cout << "\n\nResult:" << endl;
//	cout << cell;

	CPPUNIT_ASSERT(cell.nb_facets()==6);
	list<ContCell::Facet>::const_iterator it=cell.facets.begin();

	double _facet42_in[][2]={{0,1},{1,1},{-0,0}}; // contracted (but not very tight: unicity box)
	double _facet42_out[][2]={{0,1},{1,1},{-1,0}};
	CPPUNIT_ASSERT(it->facet.is_superset(IntervalVector(3,_facet42_in)));
	CPPUNIT_ASSERT(it->facet.is_subset(IntervalVector(3,_facet42_out)));

	it++;
	double _facet41[][2]={{-1,0},{1,1},{-1,1}};
	CPPUNIT_ASSERT(it->facet==IntervalVector(3,_facet41));

	it++;
	double _facet22_in[][2]={{1,1},{0,1},{0,0}};  // contracted (but not very tight: unicity box)
	double _facet22_out[][2]={{1,1},{0,1},{-1,0}};
	CPPUNIT_ASSERT(it->facet.is_superset(IntervalVector(3,_facet22_in)));
	CPPUNIT_ASSERT(it->facet.is_subset(IntervalVector(3,_facet22_out)));

	it++;
	double _facet21[][2]={{1,1},{-1,0},{-1,1}};
	CPPUNIT_ASSERT(it->facet==IntervalVector(3,_facet21));

	it++;
	double _facet1[][2]={{-1,-1},{-1,1},{-1,1}};
	CPPUNIT_ASSERT(it->facet==IntervalVector(3,_facet1));

	it++;
	double _facet3[][2]={{-1,1},{-1,-1},{-1,1}};
	CPPUNIT_ASSERT(it->facet==IntervalVector(3,_facet3));

}

} // end namespace
