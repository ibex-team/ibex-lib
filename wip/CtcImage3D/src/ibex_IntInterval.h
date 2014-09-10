#ifndef IBEX_INTINTERVAL_H
#define IBEX_INTINTERVAL_H

#include <iostream>
#include <array>
#include <limits>
namespace ibex {


class IntInterval
{
public:
    IntInterval();
    IntInterval(int lb, int ub, bool empty = false);

    int lb() const;
    int ub() const;
    int& lb();
    int& ub();
    bool is_empty() const;
    void setEmpty();
    void inflate(int rad);

    IntInterval operator &=(const IntInterval &a);
    friend std::ostream& operator <<(std::ostream& Stream, const IntInterval& obj);
private:
    int _lb;
    int _ub;
    bool empty;
};

typedef std::array<IntInterval,3> IntBox;

} // end ibex namespace

#endif // IBEX_INTINTERVAL_H
