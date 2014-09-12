#include "ibex_IntInterval.h"

namespace ibex{
//-------------------------------------------------------------------------------------------------------------
IntInterval::IntInterval():_lb(std::numeric_limits<int>::min()),
    _ub(std::numeric_limits<int>::min()), empty(true)
{
}
//-------------------------------------------------------------------------------------------------------------
IntInterval::IntInterval(int lb, int ub, bool empty):_lb(lb), _ub(ub), empty(empty)
{
    if(_ub < _lb){
        std::swap(_ub, _lb);
    }
}
//-------------------------------------------------------------------------------------------------------------
void IntInterval::inflate(int rad){
    _ub+= rad;
    _lb-= rad;
}
//-------------------------------------------------------------------------------------------------------------
IntInterval IntInterval::operator&=(const IntInterval &a)
{
    if(this->is_empty() || a.is_empty())
        this->setEmpty();
    else {
        this->_lb = std::max(_lb,a.lb());
        this->_ub = std::min(_ub,a.ub());
    }
}
//-------------------------------------------------------------------------------------------------------------
bool IntInterval::is_empty() const{
    return empty;
}

//-------------------------------------------------------------------------------------------------------------
void IntInterval::setEmpty(){
    empty = true;
    _lb = _ub =std::numeric_limits<int>::min();
}

//-------------------------------------------------------------------------------------------------------------
int IntInterval::lb() const {
    return _lb;
}
//-------------------------------------------------------------------------------------------------------------
int& IntInterval::lb() {
    return _lb;
}

//-------------------------------------------------------------------------------------------------------------
int IntInterval::ub() const{
    return _ub;
}

//-------------------------------------------------------------------------------------------------------------
int& IntInterval::ub() {
    return _ub;
}
//-------------------------------------------------------------------------------------------------------------
std::ostream& operator <<(std::ostream& Stream, const IntInterval& obj)
{
    Stream << "[ " << obj._lb << " , " << obj._ub << "]";
    return Stream; // N'oubliez pas de renvoyer le flux, afin de pouvoir chaîner les appels
}

}
