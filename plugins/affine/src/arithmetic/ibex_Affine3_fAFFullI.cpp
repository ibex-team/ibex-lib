/* ============================================================================
 * D Y N I B E X - Definition of the Affine3 class based on fAFFull version 1
 * ============================================================================
 * Copyright   : ENSTA ParisTech
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Julien Alexandre dit Sandretto and Alexandre Chapoutot
 * Created     : Jul 18, 2014
 * Sponsored   : This research benefited from the support of the "Chair Complex Systems Engineering - Ecole Polytechnique, THALES, DGA, FX, DASSAULT AVIATION, DCNS Research, ENSTA ParisTech, Telecom ParisTech, Fondation ParisTech and FDO ENSTA"
 * ---------------------------------------------------------------------------- */
#include "ibex_Affine3_fAFFullI.h"
#include "ibex_Affine.h"
#include <iomanip>

namespace ibex {

double maTol = 2.5e-15;

unsigned long int AF_fAFFullI::_counter = 1;

bool noise_null (const std::pair<int,double> value) { return (value.second >= 0)&&(value.second <= 0.0); }

template<>
AffineMain<AF_fAFFullI>& AffineMain<AF_fAFFullI>::operator=(const Interval& x) {
	_elt._garbage = Interval(0.0); //
	if (x.is_empty()) {
		_n = -1;
		_elt._center = 0.0;
		// Note the empty Affine form is an empty list
		if (!_elt._rays.empty()) {
			_elt._rays.clear();
		}
	}
	else if (x.ub()>= POS_INFINITY && x.lb()<= NEG_INFINITY ) {
		_n = -2;
		_elt._center = 0.0;
		// Note the entire set in Affine form is an empty list
		if (!_elt._rays.empty()) {
			_elt._rays.clear();
		}
	}
	else if (x.ub()>= POS_INFINITY ) {
		_n = -3;
		_elt._center = x.lb();
		if (!_elt._rays.empty()) {
			_elt._rays.clear();
		}
	}
	else if (x.lb()<= NEG_INFINITY ) {
		_n = -4;
		_elt._center = x.ub();
		if (!_elt._rays.empty()) {
			_elt._rays.clear();
		}
	}
	else  {
		if (!_elt._rays.empty()) {
			_elt._rays.clear();
		}
		_n = 1;
		_elt._center = x.mid();
		std::pair<int,double> p(AF_fAFFullI::_counter++, x.rad());
		_elt._rays.push_back(p);

	}
	return *this;
}



template<>
AffineMain<AF_fAFFullI>::AffineMain() :
_n		(-2		),
_elt	(0.0, std::list<std::pair<int,double> >(), Interval(0.0))	{
}

template<>
AffineMain<AF_fAFFullI>::AffineMain(int n, int m, const Interval& itv) :
_n 		(1),
_elt	(0.0, std::list<std::pair<int,double> >(), Interval(0.0))
{
	assert((n>=0) && (m>=0));
	// std::cout << "not implemented" << std::endl;
	*this = itv;
}


template<>
AffineMain<AF_fAFFullI>::AffineMain(const double d) :
_n 		(1),
_elt	(0.0, std::list<std::pair<int,double> >(), Interval(0.0)) {
	if (fabs(d)<POS_INFINITY) {
		_elt._center = d;
	} else {
		if (d>0) {
			_n = -3;
		} else {
			_n = -4;
		}
	}
}


template<>
AffineMain<AF_fAFFullI>::AffineMain(const Interval & itv):
_n 		(1),
_elt	(0.0,std::list<std::pair<int,double> >(),Interval(0.0)) {
	*this = itv;
}


template<>
AffineMain<AF_fAFFullI>::AffineMain(const AffineMain<AF_fAFFullI>& x) :
_n		(x._n),
_elt	(x._elt._center, std::list<std::pair<int,double> >(),x._elt._garbage) {
	if (!x._elt._rays.empty())
	{
		std::list<std::pair<int,double> >::const_iterator it = x._elt._rays.begin();
		for (; it != x._elt._rays.end(); ++it) {
			_elt._rays.push_back(std::pair<int,double>(it->first,it->second));
		}
	}

}


template<>
double AffineMain<AF_fAFFullI>::val(int i) const{
	assert((0<=i) && (((unsigned int)i)<=AF_fAFFullI::_counter));
	if (i == 0) return _elt._center;
	if (!_elt._rays.empty()) {
		std::list<std::pair<int,double> >::const_iterator iter = _elt._rays.begin();
		for (; iter != _elt._rays.end(); ++iter) {
			if (iter -> first == i) { return iter -> second; }
			if (iter -> first > i) { return 0.0; }
		}
	}
	return 0.0;
}

template<>
double AffineMain<AF_fAFFullI>::err() const{
	return _elt._garbage.rad();
}



template<>
int AffineMain<AF_fAFFullI>::size() const {
	if (!_elt._rays.empty())
	{
		std::pair<int,double> p = _elt._rays.back();
		return p.first;
	}
	else
		return 0;

}

template<>
const Interval AffineMain<AF_fAFFullI>::itv() const {

	if (is_actif()) {
		Interval res(_elt._center);
		Interval pmOne(-1.0, 1.0);
		if (!_elt._rays.empty()) {

			std::list<std::pair<int,double> >::const_iterator it = _elt._rays.begin();

			for (; it != _elt._rays.end(); ++it) {
				res += (it -> second * pmOne);

			}
		}
		res += _elt._garbage;
		return res;
	} else if (_n==-1) {
		return Interval::EMPTY_SET;
	} else if (_n==-2) {
		return Interval::ALL_REALS;
	} else if (_n==-3) {
		return Interval(_elt._center,POS_INFINITY);
	} else  {  //if (_n==-4)
		return Interval(NEG_INFINITY,_elt._center);
	}
}

template<>
double AffineMain<AF_fAFFullI>::mid() const{
	return (is_actif()) ? _elt._center : itv().mid();
}


template<>
std::ostream& operator<<(std::ostream& os, const AffineMain<AF_fAFFullI>& x) {
	os << std::setprecision(15) << x.itv() << " : ";
	if (x.is_actif()) {
		os << x.val(0);
		for (int i = 1; i <= x.size(); i++) {
			double v = x.val(i);
			if (v!=0)
			{
				os << std::setprecision(15) <<" + " << v << " eps_" << i;
			}
		}
		// Check that err() is a centered intervall
		os << " + " << x.err() << "[-1,1]";
	} else {
		os << "Affine3 form not Activate ";
	}
	return os;
}


template<>
AffineMain<AF_fAFFullI>& AffineMain<AF_fAFFullI>::operator=(const AffineMain<AF_fAFFullI>& x) {
	if (this != &x) {
		_n = x._n;
		_elt._center = x._elt._center;
		_elt._garbage = x._elt._garbage;
		_elt._rays.clear();

		if (!x._elt._rays.empty())
		{
			std::list<std::pair<int,double> >::const_iterator it = x._elt._rays.begin();
			for (; it != x._elt._rays.end(); ++it) {
				_elt._rays.push_back(std::pair<int,double>(it->first,it->second));
			}
		}
	}
	return *this;
}

template<>
AffineMain<AF_fAFFullI>& AffineMain<AF_fAFFullI>::operator=(double d) {
	_elt._garbage = Interval(0.0);
	if (!_elt._rays.empty()) {
		_elt._rays.clear();
	}

	if (fabs(d)<POS_INFINITY) {
		_n = 0;
		_elt._center = d;

	} else {
		if (d>0) {
			_n = -3;
		} else {
			_n = -4;
		}
	}

	return *this;
}



/**
 * Code for the particular case:
 * if the affine form is actif, _n>1  and _n is the size of the affine form
 * if the set is degenerate, _n = 0 or itv().diam()< AF_EC
 * if the set is empty, _n = -1
 * if the set is ]-oo,+oo[, _n = -2 and _ err=]-oo,+oo[
 * if the set is [a, +oo[ , _n = -3 and _err = [a, +oo[
 * if the set is ]-oo, a] , _n = -4 and _err = ]-oo, a]
 *
 */

template<>
AffineMain<AF_fAFFullI>& AffineMain<AF_fAFFullI>::Aneg() {
	if (is_actif()) {
		_elt._center = -_elt._center;
		_elt._garbage = -_elt._garbage;
		if (!_elt._rays.empty()) {
			std::list<std::pair<int,double> >::iterator it = _elt._rays.begin();
			for (; it != _elt._rays.end(); ++it) {
				it->second = -(it->second);
			}
		}
	} else {
		switch(_n) {
		case -3 : {
			_elt._center=-_elt._center;
			_n = -4;
			break;
		}
		case -4 : {
			_elt._center= -_elt._center;
			_n = -3;
			break;
		}
		}
	}
	return *this;
}




template<>
AffineMain<AF_fAFFullI>& AffineMain<AF_fAFFullI>::operator*=(double alpha) {
	Interval roundoff_error(0.,0.);
	Interval intermediate(0.,0.);

	if (is_actif()) {
		//std::cout << "B1" << std::endl;
		if (alpha >= 0.0 && alpha <= 0) {
			_n = 0;
			_elt._center = 0.0;
			_elt._rays.clear();
			_elt._garbage = Interval(0.0);
		}
		else if ((fabs(alpha)) < POS_INFINITY) {
			// Computation step for the center
			intermediate = Interval(_elt._center) * alpha;
			_elt._center = intermediate.mid();
			roundoff_error += intermediate.rad();

			_elt._garbage *= alpha;

			// Computation step for the rays
			if (!_elt._rays.empty()) {
				std::list<std::pair<int,double> >::iterator it =  _elt._rays.begin();
				for (; it != _elt._rays.end(); ++it) {
					intermediate = Interval (it -> second) * alpha;
					it -> second = intermediate.ub(); // Check if it true
				}
			}

			_elt._garbage += roundoff_error * Interval(-1,1);
			if (_elt._garbage.rad() > maTol) {
				std::pair<int,double> pcumul(AF_fAFFullI::_counter++, _elt._garbage.rad());
				_elt._rays.push_back(pcumul);
				_elt._garbage = Interval(0.0);
			}
		} else {
			*this = itv()*alpha;
		}
	} else {
		*this = itv()* alpha;
	}
	_elt._rays.remove_if(noise_null);

	return *this;
}




template<>
AffineMain<AF_fAFFullI>& AffineMain<AF_fAFFullI>::operator+=( const AffineMain<AF_fAFFullI>& y) {
	Interval roundoff_error(0.,0.);
	Interval intermediate(0.,0.);

	if (is_actif() && y.is_actif()) {
		// Computation step for the center
		intermediate = Interval(_elt._center) + y._elt._center;
		_elt._center = intermediate.mid();
		roundoff_error += intermediate.rad();

		_elt._garbage += y._elt._garbage;

		// Computation step for the rays
		if (_elt._rays.empty())	{
			if (!y._elt._rays.empty())	{
				std::list<std::pair<int,double> >::const_iterator ity = y._elt._rays.begin();
				for (; ity != y._elt._rays.end(); ++ity)	{
					_elt._rays.push_back(std::pair<int,double>(ity->first,ity->second));
				}
			}
		} else if (/*!_elt._rays.empty() &&*/ !y._elt._rays.empty()) {
			std::list<std::pair<int,double> >::iterator it =  _elt._rays.begin();
			std::list<std::pair<int,double> >::const_iterator ity =  y._elt._rays.begin();

			while ((ity != y._elt._rays.end()) || (it != _elt._rays.end()))		{
				if (ity == y._elt._rays.end()) { //y is finished : stop
					break;
				}
				else if (it == _elt._rays.end()) { //x is finished : we push y
					std::pair<int,double> py(ity->first,ity->second );
					_elt._rays.insert(it, py);
					ity++;
				}
				else if (it -> first == ity -> first) { //same noise term : add
					intermediate = Interval(it -> second) + ity -> second;
					it -> second = intermediate.ub();
					it++;
					ity++;
				}
				else if (it -> first < ity -> first) { //noise of y after current x noise : x++
					it++;
				}
				else  { //noise of y before current x noise : add y before x
					std::pair<int,double> py(ity->first,ity->second );
					_elt._rays.insert(it, py);
					ity++;
				}
			}
		}

		_elt._garbage += roundoff_error * Interval(-1,1);
		if (_elt._garbage.rad() > maTol) {
			std::pair<int,double> pcumul(AF_fAFFullI::_counter++, _elt._garbage.rad());
			_elt._rays.push_back(pcumul);
			_elt._garbage = Interval(0.0);
		}

	} else if (is_actif()) { // y is not a valid affine2 form. So we add y.itv() such as an interval
		*this += y.itv();
	} else if (y.is_actif()) {
		Interval tmp = itv();
		*this = y;
		*this += tmp;
	} else {
		*this = itv() + y.itv();
	}

	_elt._rays.remove_if(noise_null);

	return *this;
}


template<>
AffineMain<AF_fAFFullI>& AffineMain<AF_fAFFullI>::operator+=(double beta) {
	Interval roundoff_error(0.,0.);
	Interval intermediate(0.,0.);

	if (is_actif() && (fabs(beta))<POS_INFINITY) {

		intermediate = Interval(_elt._center) + beta;
		_elt._center = intermediate.mid();
		roundoff_error += intermediate.rad();

		_elt._garbage += roundoff_error * Interval(-1,1);
		if (_elt._garbage.rad() > maTol) {
			std::pair<int,double> pcumul(AF_fAFFullI::_counter++, _elt._garbage.rad());
			_elt._rays.push_back(pcumul);
			_elt._garbage = Interval(0.0);
		}

	} else {
		*this = itv()+ beta;
	}

	_elt._rays.remove_if(noise_null);

	return *this;
}


template<>
AffineMain<AF_fAFFullI>& AffineMain<AF_fAFFullI>::inflate(double ddelta) {
	Interval roundoff_error(0.,0.);

	if (is_actif()) {
		//std::cout << "B4" << std::endl;
		if ((fabs(ddelta))<POS_INFINITY) {
			//double error = ddelta;
			std::pair<int,double> pdelta(AF_fAFFullI::_counter++, ddelta);
			_elt._rays.push_back(pdelta);
		}
		else {
			Interval temp = itv()+Interval(-1,1)*ddelta;
			_elt._center = temp.mid();
			std::pair<int,double> p(AF_fAFFullI::_counter++, temp.rad());
			_elt._rays.push_back(p);
		}

		_elt._garbage += roundoff_error * Interval(-1,1);
		if (_elt._garbage.rad() > maTol) {
			std::pair<int,double> pcumul(AF_fAFFullI::_counter++, _elt._garbage.rad());
			_elt._rays.push_back(pcumul);
			_elt._garbage = Interval(0.0);
		}

	} else {
		*this = itv()+Interval(-1,1)*ddelta;
	}

	_elt._rays.remove_if(noise_null);

	return *this;
}


template<>
AffineMain<AF_fAFFullI>& AffineMain<AF_fAFFullI>::operator*=(const AffineMain<AF_fAFFullI>& y) {
	// std::cout << "in *= "<< *this <<std::endl;
	// std::cout << "in *= "<< y <<std::endl;

	if (is_actif() && (y.is_actif())) {

		AffineMain<AF_fAFFullI> ax(*this);
		AffineMain<AF_fAFFullI> ay(y);
		Interval ity = y.itv() - y._elt._center;
		//std::cout << "ity : " << ity << std::endl;

		double y0 = y._elt._center;

		AffineMain<AF_fAFFullI> temp1 = y*(this->_elt._center);
		ax._elt._center = 0.0;
		Interval itx = ax.itv();
		//std::cout << "itx : " << itx << std::endl;

		AffineMain<AF_fAFFullI> temp2 = ax*(y0);

		itx *= ity;
		//std::cout << "itx*ity : " << itx << std::endl;

		*this = temp1+temp2+itx;

	} else { // y is not a valid affine2 form. So we add y.itv() such as an interval
		*this = itv() * y.itv();
	}

	return *this;
}


template<>
AffineMain<AF_fAFFullI>& AffineMain<AF_fAFFullI>::operator*=(const Interval& y) {

	//*this *= Affine2Main<AF_fAFFullI>(y);
	if (	(!is_actif())||
			y.is_empty()||
			y.is_unbounded() ) {
		*this = itv()*y;

	} else {
		*this *= AffineMain<AF_fAFFullI>(y);
	}
	return *this;
}

template<>
AffineMain<AF_fAFFullI>& AffineMain<AF_fAFFullI>::operator+=(const Interval& y) {

	if (	(!is_actif())||
			y.is_empty()||
			y.is_unbounded() ) {
		*this = itv()+y;

	} else {
		//std::cout << "+=(y)" << y << std::endl;
		*this += AffineMain<AF_fAFFullI>(y);

	}


	return *this;
}

template<>
AffineMain<AF_fAFFullI>& AffineMain<AF_fAFFullI>::Asqr(const Interval& itv) {

	if (	(!is_actif())||
			itv.is_empty()||
			itv.is_unbounded()||
			(itv.diam() < AF_EC)  ) {
		*this = pow(itv,2);

	} else  {
		this->Apow(Interval(2),itv);
	}

	//	std::cout << "out sqr "<<std::endl;
	return *this;
}




template<>
void AffineMain<AF_fAFFullI>::compact(double tol){
	Interval pmOne(-1.0, 1.0);

	if (!_elt._rays.empty()) {
		std::list<std::pair<int,double> >::iterator iter = _elt._rays.begin();
		Interval cumul(0.0);

		int list_size = _elt._rays.size();

		double seuil = tol * list_size * 0.0001;

		while (list_size > 10)
		{

			for (iter = _elt._rays.begin(); iter != _elt._rays.end(); ++iter) {
				if (std::abs(iter -> second) < seuil) {
					cumul += (iter->second * pmOne);

					iter = _elt._rays.erase(iter);
					iter--;
				}
			}
			seuil*=10;
			list_size = _elt._rays.size();
		}

		_elt._garbage += cumul;

		if (_elt._garbage.rad() > maTol)
		{
			std::pair<int,double> pcumul(AF_fAFFullI::_counter++, _elt._garbage.rad());
			_elt._rays.push_back(pcumul);
			_elt._garbage = Interval(0.0);
		}
	}
	return;
}


}// end namespace ibex
