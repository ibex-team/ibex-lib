//============================================================================
//                                  I B E X
// File        : ibex_BoxProperties.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 01, 2018
// Last Update : Jul 09, 2018
//============================================================================

#include "ibex_BoxProperties.h"

#include <algorithm>

using namespace std;

namespace ibex {

namespace {

struct DepComparator {

	DepComparator(const Map<int,false>& level) : level(level) { }

	bool operator()(Bxp* x, Bxp* y) {
		return level[x->id] < level[y->id];
	}

	const Map<int,false>& level;
};

} // end anonymous namespace

int BoxProperties::topo_sort_rec(const Bxp& el, Map<int,false>& level) const {
	int l;
	try {
		l = level[el.id];
		if (l==-1) // means: father node
			throw CircualDependency();
	} catch(Map<int,false>::NotFound&) {
		l=0;
		level.insert_new(el.id, -1); // -1 means: in visit
		for (std::vector<long>::const_iterator it=el.dependencies.begin(); it!=el.dependencies.end(); it++) {
			try {
				int l2=topo_sort_rec(map[*it], level);
				if (l2>=l) l=l2+1;
			} catch(Map<Bxp>::NotFound&) {
				throw PropertyNotFound();
			}
		}
		level[el.id] = l;
	}
	return l;
}

void BoxProperties::topo_sort() const {
	dep.clear();

	Map<int,false> level;

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

void BoxProperties::add(Bxp* prop) {

	if (map.found(prop->id)) return;

	// forced to create a pointer to a pair because this is the way Map works

	map.insert_new(prop->id, *prop);

	_dep_up2date=false;
}

const Bxp* BoxProperties::operator[](long id) const {
	try {
		return &map[id];
	} catch(Map<Bxp>::NotFound&) {
		return NULL; //throw PropertyNotFound();
	}
}

void BoxProperties::update(const BoxEvent& e) {

	if (!_dep_up2date) topo_sort();

	// Update properties respecting dependencies
	// We could also create each time a new property map with
	// the required properties only. But we have shared
	// memory to avoid copies -> not very safe
	for (vector<Bxp*>::iterator it=dep.begin(); it!=dep.end(); ++it) {
		(*it)->update(e,*this);
	}
}

void BoxProperties::propagate(const Bxp& p) {
//	// TODO: something more clever of course.. !!
//
//	vector<Bxp*>::iterator it;
//	for (it=dep.begin(); it!=dep.end() && (*it)->id != p.id; ++it) { }
//	if (it==dep.end())
//		ibex_error("[BoxProperties]: unknown property in propagate");
//	for (; it!=dep.end(); ++it) {
//		(*it)->update(e,*this);
//	}
}

void BoxProperties::update_bisect(const Bisection& b, BoxProperties& lprop, BoxProperties& rprop) const {

	if (!_dep_up2date) topo_sort();

	// Duplicate properties respecting dependencies
	for (vector<Bxp*>::iterator it=dep.begin(); it!=dep.end(); it++) {
		Bxp* p1 = (*it)->copy(b.left, lprop);
		p1->update(BoxEvent(b.left,BoxEvent::CONTRACT,BitSet::singleton(b.box.size(), b.pt.var)), lprop);

		Bxp* p2 = (*it)->copy(b.right, rprop);
		p2->update(BoxEvent(b.right,BoxEvent::CONTRACT,BitSet::singleton(b.box.size(), b.pt.var)), rprop);

		lprop.add(p1);
		lprop.dep.push_back(p1);
		rprop.add(p2);
		rprop.dep.push_back(p2);
	}

	lprop._dep_up2date = true; // avoid a call to topo_sort()
	rprop._dep_up2date = true; // avoid a call to topo_sort()
}

BoxProperties::BoxProperties(const IntervalVector& box) : box(box), _dep_up2date(true) {

}

BoxProperties::BoxProperties(const BoxProperties& p) : BoxProperties(p.box, p) {

}

BoxProperties::BoxProperties(const IntervalVector& box, const BoxProperties& p) : box(box), _dep_up2date(p._dep_up2date) {

	if (!p._dep_up2date) p.topo_sort();

	// Duplicate properties respecting dependencies
	for (vector<Bxp*>::iterator it=p.dep.begin(); it!=p.dep.end(); it++) {
		Bxp* bxp=(*it)->copy(box, *this);
		add(bxp);
		dep.push_back(bxp);
	}

	_dep_up2date = true; // avoid a call to topo_sort()
}

BoxProperties::~BoxProperties() {
	for (Map<Bxp>::iterator it=map.begin(); it!=map.end(); it++)
		delete it->second;
}

ostream& operator<<(ostream& os, const BoxProperties& p) {
	os << "{\n";
	for (Map<Bxp>::const_iterator it=p.map.begin(); it!=p.map.end(); it++) {
		os << "  " << it->second->to_string() << endl;
	}
	os << "}";
	return os;
}
} /* namespace ibex */
