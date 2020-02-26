#ifndef __IBEX_SPARSE_VECTOR_H__
#define __IBEX_SPARSE_VECTOR_H__

#include <map>

namespace ibex {

class SparseVector {
public:
    typedef std::map<int, double>::iterator iterator;
    typedef std::map<int, double>::const_iterator const_iterator;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    void set(int index, double value);

    double operator[](int index) const;
    double& operator[](int index);

    int size() const;
private:
    std::map<int, double> data_;
};

} // namespace ibex

#endif  // __IBEX_SPARSE_VECTOR_H__