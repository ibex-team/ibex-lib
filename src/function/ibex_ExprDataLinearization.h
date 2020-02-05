#ifndef __IBEX_EXPR_DATA_LINEARIZATION_H__
#define __IBEX_EXPR_DATA_LINEARIZATION_H__

namespace ibex {

class LinearizationVector;
class LinearizationMatrix;

class Linearization {
public:
    typedef Linearization SCALAR;
    typedef LinearizationVector VECTOR;
    typedef LinearizationMatrix MATRIX;
    typedef std::pair<Vector, double> LinearConstraint;

private:
    std::vector<LinearConstraint> data;

public:
    void add(const LinearConstraint& lc) { data.emplace_back(lc); }
    void clear() { data.clear(); }
    const LinearConstraint& operator[](int i) const { return data[i]; }
    LinearConstraint& operator[](int i) { return data[i]; }
};

class LinearizationVector {
private:
    Linearization* data;
public:

    const Linearization& operator[](int i) const { return data[i]; }
    Linearization& operator[](int i) { return data[i]; }
};



class LinearizationMatrix {
private:
    LinearizationVector* rows;
public:
    const LinearizationVector& operator[](int i) const { return rows[i]; }
    LinearizationVector& operator[](int i) { return rows[i]; }
};

typedef ExprTemplateDomain<Linearization> ExprLinearizationData;

} // namespace ibex

#endif  // __IBEX_EXPR_DATA_LINEARIZATION_H__