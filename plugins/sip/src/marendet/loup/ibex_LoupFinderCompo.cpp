#include "ibex_LoupFinderCompo.h"
#include <utility>
namespace ibex {

LoupFinderCompo::LoupFinderCompo(const Array<LoupFinder>& loup_finders) 
: loup_finders_(loup_finders) {

}

LoupFinderCompo::~LoupFinderCompo() {

}

void LoupFinderCompo::add_property(const IntervalVector& init_box, BoxProperties& prop) {
    for(int i = 0; i < loup_finders_.size(); ++i) {
        loup_finders_[i].add_property(init_box, prop);
    }
}

std::pair<IntervalVector, double> LoupFinderCompo::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {
    auto pair = std::make_pair(loup_point, loup);
    for(int i = 0; i < loup_finders_.size(); ++i) {
        pair = loup_finders_[i].find(box, pair.first, pair.second);
    }
    return pair;
}

std::pair<IntervalVector, double> LoupFinderCompo::find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop) {
    auto pair = std::make_pair(loup_point, loup);
    for(int i = 0; i < loup_finders_.size(); ++i) {
        pair = loup_finders_[i].find(box, pair.first, pair.second, prop);
    }
    return pair;
}

}