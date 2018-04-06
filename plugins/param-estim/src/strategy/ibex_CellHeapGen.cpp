//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeapGen.cpp
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#include "ibex_CellHeapGen.h"
#include <algorithm>

using namespace std;

namespace ibex {

namespace {
/* we need this dummy class just because
 * the xxx_heap functions take the comparator
 * argument by copy (hence, we cannot give
 * "*this" since the class is abstract) */
struct CellComparator {
	bool operator()(const pair<Cell*,double>& c1, const pair<Cell*,double>& c2) {
		return c1.second >= c2.second;
	}
};
}

bool CellHeapGen::operator()(const pair<Cell*,double>& c1, const pair<Cell*,double>& c2) const {
	return c1.second >= c2.second;
}

void CellHeapGen::flush() {
	for (vector<pair<Cell*,double> >::iterator it=l.begin(); it!=l.end(); it++)
		delete it->first;

	l.clear();
}

unsigned int CellHeapGen::size() const {
	return l.size();
}

// E.g.: called by manage_buffer in Optimizer in case of a new upper bound
// on the objective ("loup"). This function then removes (and deletes) from
// the heap all the cells with a cost greater than loup.
void CellHeapGen::contract(double loup)
{
  //  cout << " before contract heap  " << l.size() << " loup " << loup << endl;
	sort_heap(l.begin(),l.end(),CellComparator());
	vector<pair<Cell*,double> >::iterator it0=l.begin();

	int k=0;
	while (it0!=l.end() && it0->second > loup) { it0++; k++; }

	for (int i=0;i<k;i++) {
		delete l[i].first;
	}

	if (k>0) l.erase(l.begin(),it0);

	make_heap(l.begin(), l.end() ,CellComparator());

	//	cout << " after contract heap " << l.size() << endl;

}


bool CellHeapGen::empty() const {
	return l.empty();
}

void CellHeapGen::push(Cell* cell) {
	if (capacity>0 && size()==capacity) throw CellBufferOverflow();

	l.push_back(pair<Cell*,double>(cell,cost(*cell)));
	push_heap(l.begin(), l.end(), CellComparator());
}

Cell* CellHeapGen::pop() {
	Cell* c = l.front().first;
	pop_heap(l.begin(),l.end(), CellComparator()); // put the "best" at the end
	l.pop_back(); // removes the "best"
	return c;     // and return it
}

Cell* CellHeapGen::top() const {
	return l.front().first;
}

ostream& operator<<(ostream& os, const CellHeapGen& heap) {
	os << "[ ";
	for (vector<pair<Cell*,double> >::const_iterator it=heap.l.begin(); it!=heap.l.end(); it++)
		os << it->first->box << " ";
	return os << "]";
}

}  // end namespace ibex
