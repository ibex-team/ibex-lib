/* ============================================================================
 * I B E X - Implementation of the Affine2Main<AF_iAF> class based on fAF version 2
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 16, 2013
 * ---------------------------------------------------------------------------- */
#include "ibex_Affine2_iAF.h"
#include "ibex_Affine.h"

namespace ibex {


template<>
AffineMain<AF_iAF>& AffineMain<AF_iAF>::operator=(const Interval& x) {

	if (x.is_empty()) {
		_n = -1;
		_elt._err = x;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else if (x.ub()>= POS_INFINITY && x.lb()<= NEG_INFINITY ) {
		_n = -2;
		_elt._err = x;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else if (x.ub()>= POS_INFINITY ) {
		_n = -3;
		_elt._err = x;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else if (x.lb()<= NEG_INFINITY ) {
		_n = -4;
		_elt._err = x;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else  {
		if (_elt._val!=NULL) { delete[] _elt._val; }
		_n = 0;
		_elt._val	= new Interval[1];
		_elt._val[0] = x.mid();
		_elt._err	= x.rad();
	}
	return *this;
}




template<>
AffineMain<AF_iAF>::AffineMain() :
		 _n		(-2		),
		 _elt	(NULL	,Interval::ALL_REALS)	{
 }

template<>
AffineMain<AF_iAF>::AffineMain(int n, int m, const Interval& itv) :
			_n 		(n),
			_elt	(NULL,0.0)
{
	assert((n>=0) && (m>=0) && (m<=n));
	if (!(itv.is_unbounded()||itv.is_empty())) {
		_elt._val	=new Interval[n + 1];
		_elt._val[0] = itv.mid();
		for (int i = 1; i <= n; i++){
			_elt._val[i] = 0.0;
		}

		if (m == 0) {
			_elt._err = itv.rad();
		} else {
			_elt._val[m] = itv.rad();
		}
	} else {
		*this = itv;
	}
}


template<>
AffineMain<AF_iAF>::AffineMain(const double d) :
			_n 		(0),
			_elt	(NULL,0.0) {
	if (fabs(d)<POS_INFINITY) {
		_elt._val = new Interval[1];
		_elt._err = 0.0; //abs(d)*AF_EE();
		_elt._val[0] = d;
	} else {
		_n=-1;
		_elt._err = Interval(d);
	}
}


template<>
AffineMain<AF_iAF>::AffineMain(const Interval & itv):
			_n 		(0),
			_elt	(NULL,0.0) {

	if (itv.is_empty()) {
		_n = -1;
		_elt._err = itv;
	} else if (itv.ub()>= POS_INFINITY && itv.lb()<= NEG_INFINITY ) {
		_n = -2;
		_elt._err = itv;
	} else if (itv.ub()>= POS_INFINITY ) {
		_n = -3;
		_elt._err = itv;
	} else if (itv.lb()<= NEG_INFINITY ) {
		_n = -4;
		_elt._err = itv;
	} else  {
		_n = 0;
		_elt._val	= new Interval[1];
		_elt._val[0] = itv.mid();
		_elt._err	= itv.rad();
	}
}


template<>
AffineMain<AF_iAF>::AffineMain(const AffineMain<AF_iAF>& x) :
		_n		(x._n),
		_elt	(NULL	,x._elt._err ) {
	if (is_actif()) {
		_elt._val =new Interval[x.size() + 1];
		for (int i = 0; i <= x.size(); i++){
			_elt._val[i] = x._elt._val[i];
		}
	}
}



template<>
double AffineMain<AF_iAF>::val(int i) const{
	assert((0<=i) && (i<=_n));
	return (_elt._val[i]).mid();
}


template<>
double AffineMain<AF_iAF>::err() const{
	Interval tmp(_elt._err);
	for (int i=0;i<=_n; i++) {
		tmp += _elt._val[i].rad();
	}
	return tmp.ub();
}


template<>
const Interval AffineMain<AF_iAF>::itv() const {

	if (is_actif()) {
		Interval res(_elt._val[0]);
		Interval pmOne(-1.0, 1.0);
		for (int i = 1; i <= _n; i++){
			res += (_elt._val[i] * pmOne);
		}
		res += _elt._err * pmOne;
		return res;
	} else {
		return _elt._err;
	}
}


template<>
double AffineMain<AF_iAF>::mid() const{
	return itv().mid();
}


template<>
AffineMain<AF_iAF>& AffineMain<AF_iAF>::operator=(const AffineMain<AF_iAF>& x) {
	if (this != &x) {
		_elt._err = x._elt._err;
		if (x.is_actif()) {
			if (_n!=x._n) {
				_n =x._n;
				if (_elt._val!=NULL) { delete[] _elt._val; }
				_elt._val = new Interval[_n+1];
			}
			if (_elt._val==NULL) _elt._val = new Interval[_n+1];

			for (int i = 0; i <= x.size(); i++) {
				_elt._val[i] = x._elt._val[i];
			}
		} else {
			_n = x._n;
			if (_elt._val != NULL) {
				delete[] _elt._val;
				_elt._val = NULL;
			}
		}
	}
	return *this;

}

template<>
AffineMain<AF_iAF>& AffineMain<AF_iAF>::operator=(double d) {

	if (fabs(d)<POS_INFINITY) {
		if (_elt._val!=NULL) { delete[] _elt._val; }
		_n = 0;
		_elt._val = new Interval[1];
		_elt._err = 0.0; //abs(d)*AF_EE();
		_elt._val[0] = d;
	} else {
		_elt._err = d;
		_n = -1;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	}
	return *this;
}


/** \brief Return (-x) */
template<>
AffineMain<AF_iAF>& AffineMain<AF_iAF>::Aneg() {
	if (is_actif()) {
		for (int i = 0; i <= _n; i++) {
			_elt._val[i] = (-_elt._val[i]);
		}
	} else {
		switch(_n) {
		case -3 : {
			_elt._err=-_elt._err;
			_n = -4;
			break;
		}
		case -4 : {
			_elt._err= -_elt._err;
			_n = -3;
			break;
		}
		}
	}
	return *this;
}




template<>
AffineMain<AF_iAF>& AffineMain<AF_iAF>::operator*=(double alpha) {
	if (is_actif()) {
		if (alpha==0.0) {
			for (int i=0; i<=_n;i++) {
				_elt._val[i]=0;
			}
			_elt._err = 0;
		} else if ((fabs(alpha)) < POS_INFINITY) {
			for (int i=0; i<=_n;i++) {
				_elt._val[i] *= alpha;
			}
			_elt._err *= fabs(alpha);

			bool b = (_elt._err.ub()<POS_INFINITY);
			for (int i=0;i<=_n;i++) {
				b &= (abs(_elt._val[i]).ub()<POS_INFINITY);
			}
			if (!b) *this = Interval::ALL_REALS;
		} else {
			*this = itv()*alpha;
		}
	} else {
		*this = itv()* alpha;
	}
	return *this;
}


template<>
AffineMain<AF_iAF>& AffineMain<AF_iAF>::operator+=(const AffineMain<AF_iAF>& y) {

	if (is_actif() && y.is_actif()) {
		if (_n==y._n) {
			for(int i=0;i<=_n;i++) {
				_elt._val[i] +=y._elt._val[i];
			}
			_elt._err += y._elt._err;

			bool b = (_elt._err.ub()<POS_INFINITY);
			for (int i=0;i<=_n;i++) {
				b &= (abs(_elt._val[i]).ub()<POS_INFINITY);
			}
			if (!b) 	*this = Interval::ALL_REALS;

		} else  {
			if (_n>y.size()) {
				*this += y.itv();
			} else {
				Interval tmp1 = itv();
				*this = y;
				*this += tmp1;
			}
		}
	} else if (is_actif()) { // y is not a valid affine2 form. So we add y.itv() such as an interval
		*this += y.itv();
	} else if (y.is_actif()) {
		Interval tmp = itv();
		*this = y;
		*this += tmp;
	} else {
		*this = itv() +y.itv();
	}

	return *this;

}


template<>
AffineMain<AF_iAF>& AffineMain<AF_iAF>::operator+=( double beta) {
	if (is_actif() && (fabs(beta))<POS_INFINITY) {
		_elt._val[0] += beta;

		if ((abs(_elt._val[0]).ub()==POS_INFINITY)) { *this = Interval::ALL_REALS; }

	} else {
		*this = itv()+ beta;
	}
	return *this;
}



template<>
AffineMain<AF_iAF>& AffineMain<AF_iAF>::inflate(double ddelta) {
	if (is_actif() && (fabs(ddelta))<POS_INFINITY) {
		_elt._err += fabs(ddelta);

		if (_elt._err.ub()==POS_INFINITY ) { *this = Interval::ALL_REALS; }
	} else {
		*this = itv()+Interval(-1,1)*ddelta;;
	}
	return *this;
}



template<>
AffineMain<AF_iAF>& AffineMain<AF_iAF>::operator*=(const AffineMain<AF_iAF>& y) {
//	std::cout << "in *= "<< *this <<std::endl;

	if (is_actif() && (y.is_actif())) {

		if (_n==y.size()) {
			Interval Sx, Sy, Sxy, Sz, xVal0,tmp;
			int i;
			Interval * xTmp;

			xTmp = new Interval[_n + 1];
			Sx=0.0; Sy=0.0; Sxy=0.0; Sz=0.0; xVal0=0.0; tmp =0.0;

			for (i = 1; i <= _n; i++) {

				tmp =_elt._val[i]*y._elt._val[i];
				Sz += tmp;
				Sxy += abs(tmp);
				Sx += abs(_elt._val[i]);
				Sy += abs(y._elt._val[i]);

			}

			xVal0 = _elt._val[0];
			// RES = X%T(0) * res
			for (i = 0; i <= _n; i++) {
				_elt._val[i] *= y._elt._val[0];
			}

			// Xtmp = X%T(0) * Y
			xTmp[0] = 0.0;
			for (i = 1; i <= _n; i++) {
				xTmp[i] = xVal0*y._elt._val[i];
			}

			//RES =  RES + Xtmp = ( Y%(0) * X ) + ( X%T(0) * Y - X%T(0)*Y%(0) )
			for (i = 0; i <= _n; i++) {
				_elt._val[i] += xTmp[i];
			}

			_elt._val[0] += 0.5 *Sz ;
			_elt._err = abs(y._elt._val[0]) * _elt._err + abs(xVal0) * y._elt._err +(_elt._err+Sx)*(y._elt._err+Sy)- 0.5 *  Sxy;


			{
				bool b = (_elt._err.ub()<POS_INFINITY);
				for (i=0;i<=_n;i++) {
					b &= (abs(_elt._val[i]).ub()<POS_INFINITY);
				}
				if (!b) {
					*this = Interval::ALL_REALS;
				}
			}
			delete[] xTmp;

		} else {
			if (_n>y.size()) {
				*this *= AffineMain<AF_iAF>(size(),0,y.itv());
			} else {
				Interval tmp1 = this->itv();
				*this = y;
				*this *= AffineMain<AF_iAF>(size(),0,tmp1);
			}
		}


	} else { // y is not a valid affine2 form. So we add y.itv() such as an interval
		*this = itv() * y.itv();
	}
//	std::cout << "out *= "<< *this<<std::endl;

	return *this;
}


template<>
AffineMain<AF_iAF>& AffineMain<AF_iAF>::operator*=(const Interval& y) {
	if (	(!is_actif())||
			y.is_empty()||
			y.is_unbounded() ) {
		*this = itv()*y;
	} else {
		*this *= AffineMain<AF_iAF>(size(),0,y);

	}
	return *this;
}

template<>
AffineMain<AF_iAF>& AffineMain<AF_iAF>::Asqr(const Interval& itv) {
//	std::cout << "in sqr "<<std::endl;

	if (	(!is_actif())||
			itv.is_empty()||
			itv.is_unbounded()||
			(itv.diam() < AF_EC)  ) {
		*this = pow(itv,2);

	} else  {

		Interval Sx, Sx2, x0;
		Sx = 0; Sx2 = 0;  x0 = 0;

		// compute the error
		for (int i = 1; i <= _n; i++) {
			Sx2 += _elt._val[i]*_elt._val[i];
			Sx += abs(_elt._val[i]);
		}
		// compute 2*_elt._val[0]*(*this)
		x0 = _elt._val[0];
		_elt._val[0] = x0*x0;

		// compute 2*_elt._val[0]*(*this)
		for (int i = 1; i <= _n; i++) {
			_elt._val[i] *= (2*x0);
		}
		_elt._val[0] += 0.5 *Sx2;

		_elt._err = (2*abs(x0) * _elt._err - 0.5 * Sx2) + pow((_elt._err+Sx),2);


		{
			bool b = (_elt._err.ub()<POS_INFINITY);
			for (int i=0;i<=_n;i++) {
				b &= (abs(_elt._val[i]).ub()<POS_INFINITY);
			}
			if (!b) {
				*this = Interval::ALL_REALS;
			}
		}

	}

//	std::cout << "out sqr "<<std::endl;
	return *this;
}

template<>
void AffineMain<AF_iAF>::compact(double tol){
	for (int i=1;i<=_n;i++) {
		if (abs(_elt._val[i]).ub()<tol) {
			_elt._err += abs(_elt._val[i]).ub();
			_elt._val[i] =0;
		}
	}
}



}// end namespace ibex




