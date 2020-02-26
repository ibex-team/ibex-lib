#include "ibex_SparseVector.h"

namespace ibex {

void SparseVector::set(int index, double value) {
    data_[index] = value;
}

int SparseVector::size() const {
    return data_.size();
}

double SparseVector::operator[](int index) const {
    return data_.at(index);
}

double& SparseVector::operator[](int index) {
    return data_[index];
}

SparseVector::iterator SparseVector::begin() {
    return data_.begin();
}

SparseVector::const_iterator SparseVector::begin() const {
    return data_.begin();
}

SparseVector::const_iterator SparseVector::cbegin() const {
    return data_.cbegin();
}

SparseVector::iterator SparseVector::end() {
    return data_.end();
}

SparseVector::const_iterator SparseVector::end() const {
    return data_.end();
}

SparseVector::const_iterator SparseVector::cend() const {
    return data_.cend();
}

} // namespace ibex