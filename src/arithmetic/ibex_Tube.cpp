/* ============================================================================
 * I B E X - Tube implementation
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Aymeric Bethencourt,
 *               Jordan Ninin, Gilles Chabert
 * Created     : Oct 7, 2013
 * ---------------------------------------------------------------------------- */


#include "ibex_Tube.h"

namespace ibex {

Tube::Tube(double t0, double tf, double step, const Interval& x) :
						IntervalVector((int)(((tf - t0) / step)), x), _t0(t0), _tf(tf), _deltaT(step) {
}

Tube::Tube(double t0, double tf, double step, const IntervalVector& x) :
						IntervalVector(x), _t0(t0), _tf(tf), _deltaT(step) {
}

Tube::Tube(double t0, double tf, double step, double bounds[][2]) :
						IntervalVector((int) (((tf - t0) / step)), bounds), _t0(t0), _tf(tf), _deltaT(step) {
}

Tube::Tube(double t0, double tf, double step, const Vector& x) :
						IntervalVector(x), _t0(t0), _tf(tf), _deltaT(step) {
}

Tube::Tube(double t0, double tf, double step, const Function& fmin, const Function& fmax) :
                				IntervalVector((int)round(((tf - t0) / step)), Interval::ALL_REALS),_t0(t0), _tf(tf), _deltaT(step) {

	IntervalVector lx(1);
	IntervalVector ux(1);

	for(int i=0;i<size();i++) {
		lx[0]= Interval(t0+i*step,t0+(i+1)*step);
		ux[0]= Interval(t0+i*step,t0+(i+1)*step);

		(*this)[i]=Interval(fmin.eval_vector(lx)[0].lb(),fmax.eval_vector(ux)[0].ub());
	}
}

void Tube::set_t0(double t0, Interval inter) {
	IntervalVector temp = (*this);

	if(t0<get_t0()) {
		int diff = (int)round((_t0-t0)/_deltaT);
		resize((_tf-t0)/_deltaT);
		for(int i=0;i<size();i++) {
			if(i<diff) (*this)[i]=inter;
			else (*this)[i]=temp[i-diff];
		}
	}
	else {
		int diff = (int)round((t0-_t0)/_deltaT);
		resize((_tf-t0)/_deltaT);
		for(int i=0;i<size();i++) {
			(*this)[i]=temp[i+diff];
		}
	}

	_t0 = t0;
}

void Tube::set_t0(double t0) {
	this->set_t0(t0, Interval::ALL_REALS);
}

void Tube::set_tF(double tf, Interval inter) {
	IntervalVector temp = (*this);

	if(tf<this->get_tF()) {
		resize((tf-_t0)/_deltaT);
		for(int i=0;i<size();i++) {
			(*this)[i]=temp[i];
		}
	}
	else {
		//int diff = (int)round(_t0+(tf-_tf)/_deltaT);
		int orsize = temp.size();
		resize((tf-_t0)/_deltaT);
		for(int i=0;i<size();i++) {
			if(i<orsize) (*this)[i]=temp[i];
			else (*this)[i]=inter;
		}
	}

	_tf = tf;
}

void Tube::set_tF(double tf) {
	this->set_tF(tf, Interval::ALL_REALS);
}

Interval Tube::at(const Interval& time) const {
	assert(time.lb()>=_t0 && time.ub()<=_tf);
	Interval temp(Interval::EMPTY_SET);
	int first_idx=(int)round(time.lb()/_deltaT);
	int last_idx=(int)round(time.ub()/_deltaT);
	for(int i=first_idx;i<last_idx;i++) {
		temp=temp|(*this)[i];
	}
	/*
    for(double t=time.lb();t<time.ub();t+=_deltaT){
        temp=temp|(*this)[(int)round((t-_t0)/_deltaT)];
    }
	 */
	return temp;
}


void Tube::resample(double new_deltaT) {
	IntervalVector temp = (*this);
	double ratio = new_deltaT/_deltaT;

	temp.resize((int)round((1/ratio)*(*this).size()));

	if(ratio<1){
		for(int i=0;i<temp.size();i++) {
			temp[i]=(*this)[(int)round(i*ratio)];
		}
	} else {
		for(int i=0;i<temp.size();i++) {
			temp[i]=Interval::EMPTY_SET;
			for(int j=0;j<ratio;j++) {
				temp[i]=temp[i]|(*this)[(i*ratio)+j];
			}
		}
	}

	(*this).resize((int)round((1/ratio)*(*this).size()));
	(*this) = temp;
	_deltaT = new_deltaT;
}


Tube Tube::sub_tube(double t0, double tf) const {
	Tube temp = Tube(*this);
	temp.set_t0(t0,Interval::ALL_REALS);
	temp.set_tF(tf,Interval::ALL_REALS);
	return temp;
}

Tube& Tube::operator=(const Tube& x) {
	if (this != &x) {
		((IntervalVector&)(*this))=x;
		_t0 = x._t0;
		_tf = x._tf;
		_deltaT = x._deltaT;
	}
	return *this;
}

Tube& Tube::operator=(const IntervalVector& x) {
	((IntervalVector&) *this)=x;
	return *this;
}

Tube& Tube::operator &=(const Tube& x) {
	__assert_tube_time_domain__(*this,x);
	((IntervalVector&) (*this))&=x;
	return *this;
}

Tube& Tube::operator |=(const Tube& x) {
	__assert_tube_time_domain__(*this,x);
	((IntervalVector&) (*this))|=x;
	return *this;
}

double Tube::max() const {
	if(this->is_unbounded()) return POS_INFINITY;
	if(this->is_empty()) return NEG_INFINITY; // BETA
	double max = (*this)[0].ub();
	for(int i=1; i<this->size(); i++) {
		if((*this)[i].ub()>max) max = (*this)[i].ub();
	}
	return max;
}

double Tube::min() const {
	if(this->is_unbounded()) return -1000; // BETA
	if(this->is_empty()) return 0; // BETA
	double min = (*this)[0].lb();
	for(int i=1; i<this->size(); i++){
		if((*this)[i].lb()<min) min = (*this)[i].lb();
	}
	return min;
}

Tube& Tube::ctcIn(double time, const Interval& in){
	assert(time>=_t0 && time<=_tf);
	(*this)[(int)round((time-_t0)/_deltaT)]&=in;
	return *this;
}

Tube& Tube::ctcIn(const Interval& time, const Interval& in){
	assert(time.lb()>=_t0 && time.ub()<=_tf);
	for(double t=time.lb();t<time.ub();t+=_deltaT){
		(*this)[(int)round((t-_t0)/_deltaT)]&=in;
	}
	return *this;
}

Tube& Tube::ctcInter(const Tube& x){
	__assert_tube_time_domain__(*this,x);
	for(int i=0;i<size();i++){
		(*this)[i] &= x[i];
	}
	return *this;
}

Tube& Tube::ctcUnion(const Tube& x){
	__assert_tube_time_domain__(*this,x);
	for(int i=0;i<size();i++){
		(*this)[i] |= x[i];
	}
	return *this;
}

Tube& Tube::ctcEq(const Tube& x){
	__assert_tube_time_domain__(*this,x);
	for(int i=0;i<size();i++){
		(*this)[i] &= x[i];
	}
	return *this;
}

Tube& Tube::ctcSup(const Tube& x){
	__assert_tube_time_domain__(*this,x);
	Interval intt, intx;
	for(int i=0;i<size();i++){
		intt=(*this)[i]; intx=x[i];
		if(intt.lb() < intx.lb())
			(*this)[i] = Interval(intx.lb(), intt.ub());
	}
	return *this;
}

Tube& Tube::ctcInf(const Tube& x){
	__assert_tube_time_domain__(*this,x);
	Interval intt, intx;
	for(int i=0;i<size();i++){
		intt=(*this)[i]; intx=x[i];
		if(intt.ub() > intx.ub())
			(*this)[i] = Interval(intt.lb() , intx.ub());
	}
	return *this;
}


Tube& Tube::ctcFwd(const Function& f) {
	IntervalVector lx(1);
	IntervalVector ux(1);
	Interval temp;
	for(int i=0;i<(*this).size()-1;i++){
		lx[0]= Interval(_t0+i*_deltaT,_t0+(i+1)*_deltaT);
		ux[0]= Interval(_t0+i*_deltaT,_t0+(i+1)*_deltaT);

		temp=(*this)[i]+Interval(f.eval_vector(lx)[0].lb(),f.eval_vector(ux)[0].ub())*_deltaT;
		(*this)[i+1]=temp&(*this)[i+1];
	}
	return *this;
}

Tube& Tube::ctcBwd(const Function& f) {
	IntervalVector lx(1);
	IntervalVector ux(1);
	Interval temp;
	for(int i=(*this).size()-1;i>=1;i--){
		lx[0]= Interval(_t0+i*_deltaT,_t0+(i+1)*_deltaT);
		ux[0]= Interval(_t0+i*_deltaT,_t0+(i+1)*_deltaT);

		temp=(*this)[i]-Interval(f.eval_vector(lx)[0].lb(),f.eval_vector(ux)[0].ub())*_deltaT;
		(*this)[i-1]=temp&(*this)[i-1];
	}
	return *this;
}

Tube& Tube::ctcFwdBwd(const Function& f) {
	this->ctcFwd(f);
	this->ctcBwd(f);
	return *this;
}

Tube& Tube::ctcPeriodic(double period) {
	assert(period<=_tf);
	int periodi = (int)round(period/_deltaT);
	for (int i = 0; i < size(); i++) {
		(*this)[i] &= (*this)[i%periodi];
	}
	return *this;
}

Tube& Tube::ctcCSym(double pivot) {
	assert(pivot>=_t0 && pivot<=_tf);
	double dist2t0=pivot-_t0;
	double dist2tf=_tf-pivot;
	if(dist2t0>dist2tf){
		for(double t=0;t<=dist2tf;t+=_deltaT) {
			(*this)[(int)round(((pivot+t)-_t0)/_deltaT)] &= -(*this)[(int)round(((pivot-t)-_t0)/_deltaT)];
			(*this)[(int)round(((pivot-t)-_t0)/_deltaT)] &= -(*this)[(int)round(((pivot+t)-_t0)/_deltaT)];
		}
	}
	else {
		for(double t=0;t<=dist2t0;t+=_deltaT) {
			(*this)[(int)round(((pivot+t)-_t0)/_deltaT)] &= -(*this)[(int)round(((pivot-t)-_t0)/_deltaT)];
			(*this)[(int)round(((pivot-t)-_t0)/_deltaT)] &= -(*this)[(int)round(((pivot+t)-_t0)/_deltaT)];
		}
	}
	return *this;
}

Tube& Tube::ctcASym(double pivot) {
	assert(pivot>=_t0 && pivot<=_tf);
	double dist2t0=pivot-_t0;
	double dist2tf=_tf-pivot;
	if(dist2t0>dist2tf){
		for(double t=0;t<=dist2tf;t+=_deltaT) {
			(*this)[(int)round(((pivot+t)-_t0)/_deltaT)] &= (*this)[(int)round(((pivot-t)-_t0)/_deltaT)];
			(*this)[(int)round(((pivot-t)-_t0)/_deltaT)] &= (*this)[(int)round(((pivot+t)-_t0)/_deltaT)];
		}
	}
	else {
		for(double t=0;t<=dist2t0;t+=_deltaT) {
			(*this)[(int)round(((pivot+t)-_t0)/_deltaT)] &= (*this)[(int)round(((pivot-t)-_t0)/_deltaT)];
			(*this)[(int)round(((pivot-t)-_t0)/_deltaT)] &= (*this)[(int)round(((pivot+t)-_t0)/_deltaT)];
		}
	}
	return *this;
}

/*
interval ScalarTube::timeIntegration(unsigned int kmin, unsigned  int kmax)
{
  double ssup = 0.0;
  double sinf = 0.0;
  for (unsigned int k=kmin; k<=kmax; k++){
      ssup+=(*this)[k].sup*this->dt;
      sinf+=(*this)[k].inf*this->dt;
  }
  return interval(sinf,ssup);
}

ScalarTube ScalarTube::timeIntegration(void)
{
  ScalarTube u(this->name,this->tBegin,this->dt,this->tEnd);
  for (unsigned int k=0; k<this->size(); k++){
      u.push_back(this->timeIntegration(0,k));
  }
  return u;
}*/

Interval Tube::integral(unsigned int kmin, unsigned int kmax) {
	double ssup = 0.0;
	double sinf = 0.0;
	Interval temp(0,0);
	for (unsigned int k=kmin; k<=kmax; k++){
		temp += (*this)[k]*_deltaT;
	}
	return temp;
}

Tube& Tube::integral() {
	IntervalVector temp = (*this);
	for(int i=0; i < size(); i++){
		temp[i] = integral(0,i);
	}
	(*this) = temp;
	return *this;
}

Tube& Tube::shift(double delay) {
	assert(delay<_tf);
	int shifti = (int)round(delay/_deltaT);
	for (int i = 0; i < size(); i++) {
		if(i+shifti>=0 && i+shifti<size()) (*this)[i] = (*this)[i+shifti];
		else (*this)[i] = Interval::ALL_REALS; // gch: not EMPTY_SET! a vector cannot contain
		                                       // empty components if it is not empty itself.
	}

	return *this;
}

Tube& Tube::scale(double coef) {
	_tf=(_tf-_t0)*coef+_t0;
	_deltaT=_deltaT*coef;
	return *this;
}

Tube eval(const Function& f, const Tube& x) {
	IntervalVector vec(x.size());
	for (int i=0;i<x.size();i++) {vec[i]=f.eval(IntervalVector(1,x[i]));}
	return Tube(x.get_t0(),x.get_tF(),x.get_delta_t(),vec);
}

Tube inverse(const Function& f, const Tube& x) {
	IntervalVector vec(x.size());
	IntervalVector tmp(1);
	for (int i=0;i<x.size();i++) {
		tmp[1] = Interval::ALL_REALS;
		f.backward(x[i],tmp);
		vec[i] = tmp[1];
	}
	return Tube(x.get_t0(),x.get_tF(),x.get_delta_t(),vec);
}

std::ostream& operator <<(std::ostream& os, const Tube& x) {
	os << "Tube from t0="<< x.get_t0() << "s to tf="<< x.get_tF() << "s of step=" << x.get_delta_t() << "s.";
	return os;
}


} // end namespace ibex
