/*
 * ibex_BoxProperties.cpp
 *
 *  Created on: Jul 1, 2018
 *      Author: gilles
 */

#include "ibex_BoxProperties.h"

#include <algorithm>

using namespace std;

namespace ibex {

namespace {

struct DepComparator {

	DepComparator(const Map<int,false>& level) : level(level) { }

	bool operator()(Bxp* x, Bxp* y) {
		return level[x->id] > level[y->id];
	}

	const Map<int,false>& level;
};

} // end anonymous namespace

int BoxProperties::topo_sort_rec(const Bxp& el, Map<int,false>& level) {
	int l;
	try {
		l = level[el.id];
		if (l==-1) // means: father node
			throw CircualDependency();
	} catch(Map<int>::NotFound&) {
		l=0;
		level.insert_new(el.id, -1); // -1 means: in visit
		for (std::vector<long>::const_iterator it=el.dependencies.begin(); it!=el.dependencies.end(); it++) {
			try {
				int l2=topo_sort_rec(map[*it], level);
				if (l2>=l) l=l2+1;
			} catch(Map<std::pair<int, Bxp> >::NotFound&) {
				throw PropertyNotFound();
			}
		}
		level[el.id] = l;
	}
	return l;
}

void BoxProperties::topo_sort() {
	dep.clear();

	Map<int> level;

	for (Map<Bxp>::const_iterator it=map.begin(); it!=map.end(); it++) {
		// push the property in the dependency array
		dep.push_back(it->second);
		// and determine its level
		topo_sort_rec(*it->second, level);
	}

	// sort the dependency array w.r.t. level
	std::stable_sort(dep.begin(),dep.end(),DepComparator(level));

	_dep_up2date = true;
}

void BoxProperties::add(const Bxp* prop) {

	if (map.found(prop->id)) return;

	// forced to create a pointer to a pair because this is the way Map works	map.insert_new(prop.id, *new std::pair<Bxp*,std::vector<long> >(&prop,dependencies));
	_dep_up2date=false;
}

const Bxp* BoxProperties::operator[](long id) const {
	try {
		return &map[id];
	} catch(Map<std::pair<int, Bxp> >::NotFound&) {
		throw NotFound();
	}
}

void BoxProperties::update(const IntervalVector& new_box, bool contract, const BitSet& impact) {

	if (!_dep_up2date) topo_sort();

	// Update properties respecting dependencies
	// We could also create each time a new property map with
	// the required properties only. But we have shared
	// memory to avoid copies -> not very safe
	for (vector<std::pair<Bxp*, int> >::iterator it=dep.begin(); it!=dep.end(); it++) {
		it->first->update(new_box,contract,impact,*this);
	}
}

void BoxProperties::update_copy(BoxProperties& copy) const {
	if (!_dep_up2date) topo_sort();

	// Duplicate properties respecting dependencies
	for (vector<std::pair<Bxp*, int> >::iterator it=dep.begin(); it!=dep.end(); it++) {
		copy.add(it->first->copy());
	}
}

void BoxProperties::update_bisect(const BisectionPoint& pt, BoxProperties& left, BoxProperties& right) const {

	if (!_dep_up2date) topo_sort();

	// Duplicate properties respecting dependencies
	for (vector<std::pair<Bxp*, int> >::iterator it=dep.begin(); it!=dep.end(); it++) {
		pair<Bxp*,Bxp*> p = it->first->update_bisect(pt,*this);
		left.add(p.first);
		right.add(p.second);
	}
}

BoxProperties::BoxProperties() : _dep_up2date(true) {

}

BoxProperties::~BoxProperties() {
	for (Map<Bxp>::iterator it=map.begin(); it!=map.end(); it++)
		delete &it->second;
}

} /* namespace ibex */
