/* ============================================================================
 * I B E X - Tube implementation
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Oct 7, 2013
 * ---------------------------------------------------------------------------- */


#include "ibex_Tube.h"


namespace ibex {




Tube::Tube(double t0, double tf, double pas, const Interval x) :
			 _t0(t0), _tf(tf), _deltaT(pas), _vec((int)(((tf - t0) / pas)), x) {
}

Tube::Tube(double t0, double tf, double pas, const IntervalVector x) :
				_t0(t0), _tf(tf), _deltaT(pas), _vec(x) {
}

Tube::Tube(double t0, double tf, double pas, double bounds[][2]) :
				_t0(t0), _tf(tf), _deltaT(pas), _vec((int) (((tf - t0) / pas)), bounds) {
}

Tube::Tube(double t0, double tf, double pas, const Vector& x) :
				_t0(t0), _tf(tf), _deltaT(pas), _vec(x) {
}

void Tube::resize(double new_deltaT) {
	//TODO
}

Tube Tube::subTube(double t0, double tf) const {
	//TODO
	return Tube(*this);
}

Tube& Tube::operator =(const Tube& x) {
	if (this != &x) {
		assert(size() == x.size());
		_t0 = x._t0;
		_tf = x._tf;
		_deltaT = x._deltaT;
		_vec = x._vec;
	}
	return *this;
}

Tube& Tube::operator =(const IntervalVector& x) {
	assert(size() == x.size());
	_vec = x;
	return *this;
}

Tube& Tube::operator &=(const Tube& x) {
	//TODO
	return *this;
}

Tube& Tube::operator |=(const Tube& x) {
	//TODO
	return *this;
}

std::pair<Tube, Tube> Tube::bisect(int i, double ratio) const {
	//TODO
	Tube left(*this);
	Tube right(*this);
	//TODO
	return std::pair<Tube,Tube>(left,right);
}

Tube& Tube::operator /=(const Tube& x2) {
	assert((_t0 == x2._t0) && (_tf == x2._tf) && (_deltaT == x2._deltaT));
	for (int i = 0; i < size(); i++) {
		_vec[i] /= x2._vec[i];
	}
	return *this;
}

Tube& Tube::ctcFwd(const Function& f) {
	//TODO
	return *this;
}

Tube& Tube::ctcBwd(const Function& f) {
	//TODO pour utiliser f, il faut faire : f.eval(IntervalVector(1,_vec[i]))
	return *this;
}

Tube& Tube::ctcFwdBwd(const Function& f) {
	//TODO
	return *this;
}

Tube& Tube::ctcPeriodic(double t) {
	//TODO
	return *this;
}

Tube& Tube::ctcPeriodic(const Interval& t) {
	//TODO
	return *this;
}

Tube& Tube::ctcSym(double pivot) {
	//TODO
	return *this;
}

Tube& Tube::ctcSym(const Interval& pivot) {
	//TODO
	return *this;
}

Tube& Tube::ctcASym(double pivot) {
	//TODO
	return *this;
}

Tube& Tube::ctcASym(const Interval& pivot) {
	//TODO
	return *this;
}

Tube& Tube::ctcDelay(double t) {
	//TODO
	return *this;
}

Tube& Tube::ctcDelay(const Interval& t) {
	//TODO
	return *this;
}

Tube& Tube::ctcScale(double alpha) {
	//TODO
	return *this;
}

Tube& Tube::ctcScale(const Interval& alpha) {
	//TODO
	return *this;
}


Tube compose(const Function& f, const Tube& x) {
	IntervalVector vec(x.size());
	for (int i=0;i<x.size();i++) {vec[i]=f.eval(IntervalVector(1,x[i]));}
	return Tube(x.getT0(),x.getTf(),x.getDeltaT(),vec);
	//TODO to check
}


std::ostream& operator <<(std::ostream& os, const Tube& x) {
	//TODO
	os << " it is a Tube ";
	return os;
}









} // end namespace













